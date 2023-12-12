#include "teacherspage.h"
#include "teacherchangepwd.h"
#include "ui_teacherspage.h"
#include "login.h"
#include <QSqlQuery>
#include <QSqlRelationalDelegate>
#include <QMessageBox>
#include <QScreen>
#include <QInputDialog>

typedef QPair<int,QString> PID;

teacherspage::teacherspage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::teacherspage)
{
    ui->setupUi(this);
    Login::useCSS(this);
    //居中设置
    Login::SetCenter(this);
    this->setWindowTitle("教务系统");

    QSqlQuery query;
    QString name = QString("select name from teachers where sid = '%1' ").arg(id);
    if(query.exec(name)){
        while(query.next()){
            ui->welcome->setText("你好，" + query.value(0).toString());
        }
    }
    else qDebug() << "获取姓名失败";
    set_my_course();
    ui->mark->hide();//先藏起来
}

teacherspage::~teacherspage()
{
    delete ui;
}


void teacherspage::set_my_course(){//显示课程信息

    model = new QSqlRelationalTableModel(this);
    model->setTable("course_info");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setRelation(2, QSqlRelation("teachers", "sid", "name"));
    model->setRelation(3, QSqlRelation("course_location", "sid", "location"));

    QString filter = QString("course_info.teacher_name = '%1' ").arg(id);
    model->setFilter(filter);

    model->select();

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //表格不可更改
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自适应
    ui->tableView->setColumnHidden(3, false);
    ui->tableView->setColumnHidden(5, false);
    ui->tableView->setColumnHidden(6, false);

    ui->tableView->setColumnHidden(7, true);
    ui->tableView->setColumnHidden(8, true);
    ui->tableView->setColumnHidden(9, true);

    QString headStr[] = {"课程编号", "课程名称", "授课教师", "课程地点", "授课学时", "课程学分", "选课人数"};
    int colWidths[] = {60, 100, 100, 100, 100, 100, 80};
    for(int i=0; i<7; i++)
    {   model->setHeaderData(i, Qt::Horizontal, headStr[i]);
        ui->tableView->setColumnWidth(i,  colWidths[i]);
    }
}



void teacherspage::on_show_stu_info_clicked()//查看某门课的学生信息
{

    if(ui->show_stu_info->text() == "返回"){
        ui->show_stu_info->setText("显示学生信息");
        ui->mark->hide();
        set_my_course();
        qDebug() << "返回";
        return;
    }


    QModelIndex currentIndex = ui->tableView->currentIndex();
    qDebug() << currentIndex;
    if (currentIndex.isValid()){

        ui->show_stu_info->setText("返回");
        ui->mark->show();//打分按钮显示

        int selectedRow = ui->tableView->currentIndex().row();

        QModelIndex idIndex = model->index(selectedRow, 0);
        int courseID = model->data(idIndex).toInt();
        curCourseID = courseID;

        QSqlQuery query;
        QString str = QString("select StudentID,Score from stu_course_record where CourseID = %1").arg(courseID);
        QVector<PID> v;
        QList<int> studentIDs;
        if (query.exec(str)) {
            while (query.next()) {
                v.append(std::make_pair(query.value(0).toInt(),query.value(1).toString()));
                studentIDs.append(query.value(0).toInt());
            }
        }

        QString filter;
        for (int i : studentIDs) {
            if (!filter.isEmpty()) {
                filter += " or ";
            }
            filter += QString("sid = %1").arg(i);
        }
        qDebug() << filter;

        model = new QSqlRelationalTableModel(this);
        model->setTable("students");
        if(studentIDs.empty()) model->clear();
        else model->setFilter(filter);
        model->select();

        model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
        //model->select();
        model->removeColumns(0,0);
        ui->tableView->setModel(model);
        ui->tableView->verticalHeader()->setVisible(false);
        ui->tableView->setColumnHidden(7, false);
        ui->tableView->setColumnHidden(3, true);
        ui->tableView->setColumnHidden(5, true);
        ui->tableView->setColumnHidden(6, true);

        QString headStr[] = {"编号", "账号", "姓名", "", "专业", "", "", "成绩"};
                                                          int colWidths[] = {40, 100, 100, 1, 150, 1, 1, 100};

        for(int i=0; i<8; i++)
        {   model->setHeaderData(i, Qt::Horizontal, headStr[i]);
            ui->tableView->setColumnWidth(i,  colWidths[i]);
        }
        for(int i=0;i<v.size();i++){
            int row;
            QModelIndexList indexes = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(v[i].first), -1, Qt::MatchExactly);
            foreach (const QModelIndex &index, indexes) {
                row = index.row();
            }
            QModelIndex idex1=model->index(row,7);
            model->setData(idex1,v[i].second);
        }


    }
    else {
        ui->show_stu_info->setText("显示学生信息");
        set_my_course();
        ui->mark->hide();//藏起来
        QMessageBox::warning(NULL, "警告", "请选择要查看的行", QMessageBox::Ok);
    }
}


void teacherspage::on_loginOut_clicked()
{
    this->close();

}


void teacherspage::on_mark_clicked()//弹窗读入数据
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (currentIndex.isValid()){

        int selectedRow = ui->tableView->currentIndex().row();

        QModelIndex insertPlaceIdex=model->index(selectedRow,7);//显示成绩的地方
        int score = QInputDialog::getInt(this, "成绩录入", "请输入成绩", insertPlaceIdex.data().toInt(), 0, 100);
        model->setData(insertPlaceIdex,score);

        QModelIndex idIndex = model->index(selectedRow, 0);
        int curStudentID = model->data(idIndex).toInt();

        QSqlQuery query;
        query.prepare("UPDATE stu_course_record SET Score = :score "
                                    "WHERE StudentID = :studentId AND CourseID = :courseId");
        query.bindValue(":studentId", curStudentID);
        query.bindValue(":courseId", curCourseID);
        query.bindValue(":score", score);
        query.exec();

    }
}




