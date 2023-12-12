#include "login.h"
#include "ui_login.h"

#include <QMEssageBox>
#include <qDebug>
#include <QSqlQuery>
#include <mainwindow.h>
#include "studentpage.h"
#include "teachermain.h"
#include "adminpage.h"
#include "edit.h"
#include <QLineEdit>
#include <QPainter>

int id;

void Login::useCSS(QWidget* w)
{
    QFile f;
    auto str = QCoreApplication::applicationDirPath();
    f.setFileName(str + "//" + "myqss.css");
    f.open(QIODevice::ReadOnly);
    QString t = f.readAll();
    w->setStyleSheet(t);
}

void Login::SetCenter(QWidget* w){//窗体居中
    QRect rect = w->frameGeometry();
    QScreen* screen = QGuiApplication::primaryScreen();
    QPoint centerPoint = screen->availableGeometry().center();
    rect.moveCenter(centerPoint);
    w->move(rect.topLeft());
}


void Login::timeOut(){
    ui->label_2->setText("");
    ui->label_4->setText("");
}

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    useCSS(this);
    ui->setupUi(this);
    ui->num->setFocus();
    //setWindowFlags(Qt::ToolTip);

    setAttribute(Qt::WA_TranslucentBackground);  //设置窗口背景透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);  //去掉窗口边框
}

void Login::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(QColor(255, 255, 255)));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    painter.drawRoundedRect(rect, 15, 15);  //设置窗口圆角 15px
}


Login::~Login()
{
    delete ui;
}

void Login::on_login_clicked()
{

    Login::number=ui->num->text().trimmed();
    QString password=ui->psw->text().trimmed();

    QSqlQuery query;

    if(Login::number.length()!=0&&password.length()!=0){

        if(ui->stu->isChecked()){//学生登录
            bool ok = true;//对不起我忘了这个东西写来干什么的了但是它一定有用。。。下次一定好好取名
            QString instruction = QString("select passwords from students where number='%1'").arg(Login::number);
            query.exec(instruction);

            while(query.next()){
                if(query.value(0).toString()==password){

                    QString getId = QString("select sid from students where number='%1'").arg(Login::number);
                    query.exec(getId);
                    while(query.next()){
                        id = query.value(0).toInt();
                    }//上面这几行用来把登录者的sid记录在全局变量id里，之后要用

                    this->close();
                    studentpage* w2=new studentpage();

                    w2->show();
                    ok = false;
                    break;
                }
                else{
                    QMessageBox::information(this, "登录失败", "用户名或者密码错误");
                    ui->num->clear();
                    ui->psw->clear();
                    ok = false;
                    break;
                }
            }
            if(ok) QMessageBox::information(this, "登录失败", "用户名或者密码错误");
        }
        else if(ui->teacher->isChecked()){//教师登录
            bool ok = true;
            QString instruction = QString("select passwords from teachers where number='%1'").arg(Login::number);
            query.exec(instruction);

            while(query.next()){
                if(query.value(0).toString()==password){

                    QString getId = QString("select sid from teachers where number='%1'").arg(Login::number);
                    query.exec(getId);
                    while(query.next()){
                        id = query.value(0).toInt();
                    }

                    this->close();
                    teacherMain *w2 = new teacherMain();
                    w2->show();
                    ok = false;
                    break;
                }
                else{
                    QMessageBox::information(this, "登录失败", "用户名或者密码错误");
                    ui->num->clear();
                    ui->psw->clear();
                    ok = false;
                    break;
                }
            }

            if(ok) QMessageBox::information(this, "登录失败", "用户名或者密码错误");
        }

        else if(ui->admin->isChecked()){//管理员登录
            bool ok = true;
            QString instruction = QString("select passwords from admin where number='%1'").arg(Login::number);
            query.exec(instruction);

            while(query.next()){
                if(query.value(0).toString()==password){
                    this->close();
                    adminpage* w2=new adminpage();
                    w2->show();
                    ok = false;
                    break;
                }
                else{
                    QMessageBox::information(this, "登录失败", "用户名或者密码错误");
                    ui->num->clear();
                    ui->psw->clear();
                    ok = false;
                    break;
                }
            }
            if(ok) QMessageBox::information(this, "登录失败", "用户名或者密码错误");
        }
        else
            QMessageBox::warning(NULL,"登录失败","请选择身份");
    }
    else{
        QTimer::singleShot(1500,this,SLOT(timeOut()));
        if(Login::number.length()==0){
            ui->num->setText("");
            ui->label_2->setText("账号不能为空");
            ui->label_2->setStyleSheet("color:red");
            ui->num->setFocus();
        }
        else if(password.length()==0){
            ui->psw->setText("");
            ui->label_4->setText("密码不能为空");
            ui->label_4->setStyleSheet("color:red");
            ui->psw->setFocus();
        }
    }

}


void Login::on_exit_clicked()
{
    exit(0);
}


void Login::on_sign_clicked()
{
    edit *w =new edit();
    w->setWindowModality(Qt::ApplicationModal);
    w->show();
}

