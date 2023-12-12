#include "studentpage.h"
#include "login.h"
#include "stuInfo.h"
#include "scores.h"
#include "courseChoose.h"
#include "ui_studentpage.h"
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QScreen>
#include <QMainWindow>
#include <QPushButton>
extern int id;

studentpage::studentpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studentpage)
{
    ui->setupUi(this);
    this->setWindowTitle("教务系统");
    Login::useCSS(this);

    //居中
    Login::SetCenter(this);

    QSqlQuery query;
    QString name = QString("select name from students where sid = '%1' ").arg(id);
    if(query.exec(name)){
        while(query.next()){
            ui->welcom->setText("你好，" + query.value(0).toString() + "!");//获取界面右上角的欢迎语
        }
    }
    else qDebug() << "获取姓名失败";

}

studentpage::~studentpage()
{
    delete ui;
}

void studentpage::on_personal_info_clicked()//个人信息的显示与修改
{

    stuInfo* info=new stuInfo();
    info->setWindowModality(Qt::ApplicationModal);
    info->show();

}

void studentpage::on_allCourses_clicked()//全校课程显示和查询
{

    courseChoose* allcourse=new courseChoose();
    allcourse->setWindowTitle("全校课程");
    allcourse->findChild<QLabel*>("label_2")->hide();
    allcourse->findChild<QLabel*>("label")->hide();
    allcourse->findChild<QLabel*>("title")->setText("当前位置：首页 > 全部开课课程");
    allcourse->findChild<QLabel*>("title")->move(670, 60);
    allcourse->findChild<QPushButton*>("confirm")->hide();
    allcourse->findChild<QToolButton*>("exit")->hide();
    allcourse->findChild<QPushButton*>("dropCourse")->hide();
    allcourse->findChild<QTableView*>("tableView_2")->hide();
    allcourse->setFixedSize(900,450);
    Login::SetCenter(allcourse);
    allcourse->setWindowModality(Qt::ApplicationModal);
    allcourse->show();
}

void studentpage::on_courseChoose_clicked()//选课
{
//    this->hide();
    courseChoose* csc=new courseChoose();
    csc->setWindowModality(Qt::ApplicationModal);
    csc->show();
}

void studentpage::on_score_clicked()//绩点
{
    scores* sco=new scores();
    sco->setWindowModality(Qt::ApplicationModal);
    sco->show();
}

void studentpage::on_loginOut_clicked()//登出
{
    this->close();
    Login* login = new Login();
    login->show();
}
