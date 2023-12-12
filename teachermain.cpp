#include "teachermain.h"
#include "login.h"
#include "qsqlquery.h"
#include "teacherchangepwd.h"
#include "teacherspage.h"
#include "ui_teachermain.h"

teacherMain::teacherMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::teacherMain)
{
    ui->setupUi(this);
    Login::useCSS(this);
    Login::SetCenter(this);
    this->setWindowTitle("教务系统");

    QSqlQuery query;
    QString name = QString("select name from teachers where sid = '%1' ").arg(id);
    if(query.exec(name)){
        while(query.next()){
            ui->welcom->setText("你好，" + query.value(0).toString());
        }
    }
    else qDebug() << "获取姓名失败";
}

teacherMain::~teacherMain()
{
    delete ui;
}


void teacherMain::on_viewCourse_clicked()
{
    teacherspage* page = new teacherspage;
    page->setWindowModality(Qt::ApplicationModal);
    page->show();
}


void teacherMain::on_changePwd_clicked()
{
    teacherChangePwd* page = new teacherChangePwd;
    page->setWindowModality(Qt::ApplicationModal);
    page->show();
}


void teacherMain::on_loginOut_clicked()
{
    this->close();
    Login* login = new Login();
    login->show();
}

