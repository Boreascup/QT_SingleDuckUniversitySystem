#include "courseChoose.h"
#include "login.h"
#include "studentpage.h"
#include "ui_courseChoose.h"
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QMessageBox>
#include <QScreen>
#include <QApplication>

//extern int id;

courseChoose::courseChoose(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::courseChoose)
{
    ui->setupUi(this);
    this->setWindowTitle("选课");
    Login::useCSS(this);

    //居中
    Login::SetCenter(this);


    set_all_course();
    set_my_course();

}

courseChoose::~courseChoose()
{
    delete ui;
}


void courseChoose::on_confirm_clicked(){//确认选课后一系列数据库操作

    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (currentIndex.isValid()) {
        int selectedRow = currentIndex.row();

        QModelIndex idIndex = model->index(selectedRow, 0);
        int courseID = model->data(idIndex).toInt();

        QSqlQuery query;
        QString check = QString("select * from stu_course_record "
                                     "where StudentID='%1' and CourseID='%2'").arg(id).arg(courseID);
        query.exec(check);
        if(query.next()){
            QMessageBox::warning(NULL,"选课失败","不能重复选课");
            return;
        }

        QString str = QString("insert into stu_course_record(StudentID, CourseID)"
                             "values('%1', '%2')").arg(id).arg(courseID);

        if (query.exec(str)) {
            QMessageBox::information(NULL, "提示", "选课成功！", QMessageBox::Ok);
        } else {
            qDebug() << "选课失败";
        }
        query.exec(QString("update course_info set student_num = student_num + 1 where sid = %1").arg(courseID));

    } else {
        QMessageBox::warning(NULL, "警告", "请选择要选课的行", QMessageBox::Ok);
    }

    set_all_course();
    set_my_course();
}

void courseChoose::on_dropCourse_clicked(){//退课
    QModelIndex currentIndex = ui->tableView_2->currentIndex();
    if (currentIndex.isValid()) {
        int selectedRow = currentIndex.row();

        QModelIndex idIndex = model->index(selectedRow, 0);
        int courseID = model_2->data(idIndex).toInt();

        int ok = QMessageBox::warning(this,tr("退课!"),tr("你确定退选所选课程吗？"),
                                  QMessageBox::Yes,QMessageBox::No);
        if(ok == QMessageBox::Yes){
            QSqlQuery query;
            QString drop = QString("delete from stu_course_record "
                                   "where StudentID='%1' and CourseID='%2'").arg(id).arg(courseID);

            if(query.exec(drop)){
                query.exec(QString("update course_info set student_num = student_num - 1 where sid = %1").arg(courseID));
                QMessageBox::information(NULL, "提示", "退课成功！", QMessageBox::Ok);
            }
            else{
                qDebug() << "退课失败";
            }
        }
    }
    else {
        QMessageBox::warning(NULL, "警告", "请选择要退课的行", QMessageBox::Ok);
    }
    set_all_course();
    set_my_course();
}

void courseChoose::set_all_course(){//显示全部课程

    model = new QSqlRelationalTableModel(this);
    model->setTable("course_info");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setRelation(2, QSqlRelation("teachers", "sid", "name"));
    model->setRelation(3, QSqlRelation("course_location", "sid", "location"));

    model->select();

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    QString headStr[] = {"课程编号", "课程名称", "授课教师", "课程地点", "授课学时", "课程学分", "选课人数"};
    int colWidths[] = {60, 110, 110, 100, 110, 110, 75};
    for(int i=0; i<7; i++)
    {   model->setHeaderData(i, Qt::Horizontal, headStr[i]);
        ui->tableView->setColumnWidth(i,  colWidths[i]);
    }

    ui->tableView->setColumnHidden(7,true);
    ui->tableView->setColumnHidden(8,true);
    ui->tableView->setColumnHidden(9,true);
}


void courseChoose::set_my_course() {//显示已选课程

    model_2 = new QSqlRelationalTableModel(this);
    model_2->setTable("course_info");
    model_2->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tableView_2->verticalHeader()->setVisible(false);
    ui->tableView_2->setModel(model_2);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
    }

    if(courseIDs.empty()) model_2->clear();//如果什么课都没有的话要把model清空，因为filter是写了筛选条件的sql语句，filter为空的话等于没筛选
    else model_2->setFilter(filter);

    model_2->setRelation(2, QSqlRelation("teachers", "sid", "name"));
    model_2->setRelation(3, QSqlRelation("course_location", "sid", "location"));
    model_2->select();

    QString headStr[] = {"课程编号", "课程名称", "授课教师", "课程地点", "授课学时", "课程学分", "选课人数"};
    int colWidths[] = {60, 110, 110, 110, 110, 110, 80};
    for(int i=0; i<7; i++)
    {   model_2->setHeaderData(i, Qt::Horizontal, headStr[i]);
        ui->tableView_2->setColumnWidth(i,  colWidths[i]);
    }

    ui->tableView_2->setColumnHidden(7,true);
    ui->tableView_2->setColumnHidden(8,true);
    ui->tableView_2->setColumnHidden(9,true);
}

void courseChoose::on_search_clicked(){//查询
    QString str=ui->searchText->text().trimmed();
    if(str.isEmpty()){
        if(ui->searchType->currentText()=="按课程名查询"){
            QMessageBox::warning(NULL,"查询失败","请输入课程名字");
        }
        else{
            QMessageBox::warning(NULL,"查询失败","请输入授课老师名字");
        }
    }
    else{
        set_all_course();

        QString filter;

        if(ui->searchType->currentText()=="按课程名查询"){
            model->setFilter(QString("course_info.name like '%%1%'").arg(str));
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
                if(idstr == "") model->clear();
                else model->setFilter(filter);
            }
        }

        model->select();

    }

}

void courseChoose::on_return_2_clicked(){//重置查询
    ui->searchText->clear();
    ui->searchType->setCurrentIndex(0);
    set_all_course();
}

void courseChoose::on_exit_clicked()//退出系统
{
    this->close();
}

