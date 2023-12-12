#include "scores.h"
#include "ui_scores.h"
#include "login.h"
#include <algorithm>
typedef QPair<int,double> PID;
//记录每个分数段有几个(myscore[0]对应小于60分，myscore[1]对应60-70...)

scores::scores(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scores)
{
    ui->setupUi(this);
    this->setWindowTitle("绩点");
    Login::useCSS(this);

    //居中
    Login::SetCenter(this);
    set_my_scores();
    on_pie_clicked();
}

scores::~scores()
{
    delete ui;
}

void scores::set_my_scores(){//显示绩点

    model = new QSqlRelationalTableModel(this);
    model->setTable("course_info");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QSqlQuery query;//思路找到这个学生选过的所有课的sid，存到数据结构里
    QString str = QString("select CourseID from stu_course_record where StudentID = %1").arg(id);
    QList<int> courseIDs;  // 用来存储选中的课程ID
    if (query.exec(str)) {
        while (query.next()) {
            courseIDs.append(query.value(0).toInt());
        }
    }

    QString filter;
    for (int courseID : courseIDs) {
        if (!filter.isEmpty()) {
            filter += " or ";
        }
        filter += QString("course_info.sid = %1").arg(courseID);//course_info.这个限定前缀非常非常重要否则啥都查不出来。是QSqlRelationalTableModel的一个烦人的特性
        //qDebug() << "已选的课程sid为" << courseID;
    }

    model->setRelation(2, QSqlRelation("teachers", "sid", "name"));

    if(courseIDs.empty()) model->clear();//如果什么课都没有的话要把model清空，因为filter是写了筛选条件的sql语句，filter为空的话等于没筛选
    else model->setFilter(filter);
    model->select();

    QString headStr[] = {"课程编号", "课程名称", "授课教师", "","","课程学分","","成绩","绩点","排名"};
    int colWidths[] = {60, 150, 100, 1,1,89,1,40,40,40};
    for(int i=0; i<10; i++)
    {   model->setHeaderData(i, Qt::Horizontal, headStr[i]);
        ui->tableView->setColumnWidth(i,  colWidths[i]);
    }

    //隐藏不需要的字段
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(6,true);


    double credit=0,cresco=0;//credit记录总学分，cresco记录每门课 学分*绩点 的和
    for(int i=0;i<courseIDs.size();i++){
        tem<<QString::number(courseIDs[i]);
        //查课程学分
        QString creditstr=QString("select credit from course_info where sid = %1").arg(courseIDs[i]);
        double temcredit=0;
        if (query.exec(creditstr)) {
            while (query.next()) {
                temcredit=query.value(0).toDouble();
                credit+=temcredit;
            }
        }
        //查询所选课程的成绩
        QString searchScore=QString("select Score from stu_course_record where StudentID = %1 and CourseID = %2").arg(id).arg(courseIDs[i]);
        //寻找tablemodel中courseIDs[i]在哪行
        int row;
        QModelIndexList indexes = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(courseIDs[i]), -1, Qt::MatchExactly);
        foreach (const QModelIndex &index, indexes) {
            row = index.row();
        }

        //得到tablemodel中courseIDs[i]所在行的成绩、绩点的索引
        QModelIndex idex1=model->index(row,7);
        QModelIndex idex2=model->index(row,8);
        double score=0;//用于记录成绩
        if (query.exec(searchScore)) {
            while (query.next()) {
                model->setData(idex1,query.value(0).toString());
                score=query.value(0).toDouble();
                if(score<60){
                    model->setData(idex2,"0");
                    tem<<QString::number(score)<<"0";
                }
                else{
                    model->setData(idex2,QString::number((score-50)/10));
                    cresco+=((score-50)/10)*temcredit;
                    tem<<QString::number(score)<<QString::number((score-50)/10);
                }
            }
        }

        //记录分数段
        if(score<60){
            myscore[0]++;
        }
        else if(score==60){
            myscore[1]++;
        }
        else{
            if(((int)(score-60))%10!=0){
                myscore[1+(((int)(score-60))/10)]++;
            }
            else{
                myscore[(((int)(score-60))/10)]++;
            }
        }

        //查找每门课所有学生的成绩，计算排名
        QString rankstr=QString("select StudentID,Score from stu_course_record where  CourseID = %1").arg(courseIDs[i]);
        QVector<PID> v;
        if (query.exec(rankstr)) {
            while (query.next()) {
                v.append(std::make_pair(query.value(0).toInt(),query.value(1).toDouble()));
            }
        }
        std::sort(v.begin(),v.end(),[](PID p1,PID p2){return p1.second>p2.second;});
        QString rank;
        int irank=1+v.indexOf(std::make_pair(id,score));//第几名
        int num=v.size();
        rank+=QString::number(irank)+"/"+QString::number(num);
        QModelIndex idex3=model->index(row,9);
        model->setData(idex3,rank);
        tem<<rank;
        scolist<<tem;
        tem.clear();
    }

    ui->gpa->setText("平均学分绩点："+QString::number(cresco/credit,'f',4));

}


void scores::on_search_clicked(){//查询
    QString str=ui->searchText->text().trimmed();
    if(str.isEmpty()){
        if(ui->searchType->currentText()=="课程名"){
            QMessageBox::warning(NULL,"查询失败","请输入课程名字");
        }
        else{
            QMessageBox::warning(NULL,"查询失败","请输入授课老师名字");
        }
    }
    else{
        set_my_scores();

        QString filter;

        if(ui->searchType->currentText()=="课程名"){
            filter = QString("course_info.name like '%%1%'").arg(str);
        }
        else{
            QSqlQuery query;
            int i=0;
            QString idstr = QString("select sid from teachers where name like '%%1%'").arg(str);
            if (query.exec(idstr)) {
                while (query.next()) {
                    if (!filter.isEmpty()) {
                        filter += " or ";
                    }
                    i=query.value(0).toInt();
                    filter += QString("course_info.teacher_name = %1").arg(i);
                }
            }
        }
        model->setFilter(filter);
        model->select();

        for(int i=0;i<model->rowCount();i++){
            QModelIndex index = model->index(i,0); // 获取筛选后课程id的index
            QVariant value = model->data(index); //获得课程id
            for(int j=0;j<scolist.size();j++){
                if(scolist[j].contains(value.toString())){
                    QModelIndex idex2=model->index(i,7);
                    model->setData(idex2,scolist[j][1]);
                    QModelIndex idex3=model->index(i,8);
                    model->setData(idex3,scolist[j][2]);
                    QModelIndex idex4=model->index(i,9);
                    model->setData(idex4,scolist[j][3]);
                }
            }
        }
    }
}

void scores::on_reSet_clicked(){//重置查询
    ui->searchText->clear();
    ui->searchType->setCurrentIndex(0);
    set_my_scores();
}

void scores::on_pie_clicked(){//饼状图
    ui->bar->setStyleSheet("background-color:transparent;border:none;""color:black;");
    ui->bar->setFlat(true);
    ui->pie->setFlat(true);
    ui->pie->setStyleSheet("background-color:transparent;border:none;""color:#228974;""text-decoration:underline;");
    ui->pie->setChecked(true);
    QList<QString> sco;
    sco<<"低于60"<<"60-70"<<"70-80"<<"80-90"<<"90-100";
    QPieSeries *series = new QPieSeries();
    for(int i=0;i<5;i++){
        if(myscore[i]){
            series->append(sco[i], myscore[i]);
            QPieSlice* slice=series->slices().at(series->slices().count()-1);
            slice->setLabel(sco[i]);
        }
    }
    series->setLabelsPosition(QPieSlice::LabelPosition::LabelOutside);
    series->setLabelsVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("成绩分布图");

    ui->pieChart->setChart(chart);
    ui->pieChart->setRenderHint(QPainter::Antialiasing);

    ui->stackedWidget->setCurrentIndex(0);
}

void scores::on_bar_clicked(){//柱状图
    ui->pie->setStyleSheet("background-color:transparent;border:none;""color:black;");
    ui->pie->setFlat(true);
    ui->bar->setFlat(true);
    ui->bar->setStyleSheet("background-color:transparent;border:none;""color:#228974;""text-decoration:underline;");
    ui->bar->setChecked(true);
    QList<QString> sco;
    sco<<"低于60"<<"60-70"<<"70-80"<<"80-90"<<"90-100";
    QBarSet *barset = new QBarSet("成绩分布图");
    // 添加数据
    int cnt=myscore[0];
    for(int i=0;i<5;i++){
        *barset<<myscore[i];
        cnt=cnt>myscore[i]?cnt:myscore[i];
    }
    QBarSeries *series = new QBarSeries();
    series->append(barset);
    QChart *chart = new QChart();
    chart->addSeries(series);

    // 创建轴
    QBarCategoryAxis * x = new QBarCategoryAxis();
    for(int i=0;i<5;i++)
    {
        x->append(sco[i]);
    }
    chart->addAxis(x, Qt::AlignBottom);
    series->attachAxis(x);

    QValueAxis * y = new QValueAxis();
    y->setTickCount(cnt+1); // 设置刻度数量
    y->setLabelFormat("%d"); // 设置标签格式为整数
    chart->addAxis(y, Qt::AlignLeft);
    series->attachAxis(y);
    ui->barChart->setChart(chart);
    ui->stackedWidget->setCurrentIndex(1);
}
