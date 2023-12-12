#include "edit.h"
#include "login.h"
#include "qevent.h"
#include "qpainter.h"
#include "ui_edit.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QByteArray>
#include <QtCore/QChar>
#include <QComboBox>
#include <QFile>
#include <QInputDialog>
#include <QTimer>



void edit::timeOut(){
    ui->label_2->setText("");
    ui->label_3->setText("");
    ui->label_4->setText("");
    ui->label_5->setText("");
}

edit::edit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::edit)
{
    ui->setupUi(this);
    Login::useCSS(this);
    setAttribute(Qt::WA_TranslucentBackground);  //设置窗口背景透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);  //去掉窗口边框

}

edit::~edit()
{
    delete ui;
}

void edit::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(QColor(255, 255, 255)));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    painter.drawRoundedRect(rect, 15, 15);  //设置窗口圆角 15px
}

void edit::on_pushButton_clicked()
{
    QString number=ui->num->text().trimmed();
    QString password=ui->psw->text().trimmed();
    QString password2=ui->psw2->text().trimmed();
    QString name=ui->name->text().trimmed();
    QSqlQuery query;
    QTimer::singleShot(1500,this,SLOT(timeOut()));
    if(name.isEmpty()){
        ui->name->setText("");
        ui->label_2->setText("姓名不能为空");
        ui->label_2->setStyleSheet("color:red");
        ui->name->setFocus();
    }
    else if(number.isEmpty()){
        ui->num->setText("");
        ui->label_3->setText("账号不能为空");
        ui->label_3->setStyleSheet("color:red");
        ui->num->setFocus();
    }
    else if(password.isEmpty()){
        ui->psw->setText("");
        ui->label_4->setText("密码不能为空");
        ui->label_4->setStyleSheet("color:red");
        ui->psw->setFocus();
    }
    else if(password2.isEmpty()){
        ui->psw2->setText("");
        ui->label_5->setText("确认密码不能为空");
        ui->label_5->setStyleSheet("color:red");
        ui->psw2->setFocus();
    }
    else if(QString::compare(password,password2)!=0){
        QMessageBox::warning(NULL,"注册失败","两次密码输入不相同");
    }
    else{
        if((!ui->stu->isChecked()&&!ui->teacher->isChecked()&&!ui->admin->isChecked()))
            QMessageBox::warning(NULL,"注册失败","请选择身份");

        else if(ui->stu->isChecked()){
            QString SelectStr=QString("SELECT COUNT(number) from students where number='%1' ").arg(number);
            if(query.exec(SelectStr)&&query.next()){
                int rows=query.value(0).toInt();
                if(rows>0){
                    QMessageBox::warning(NULL,"注册失败","账号已存在");
                }
                else{
                    QString InsertStr=QString("INSERT INTO students(number,passwords,name)"
                                                "values('%1','%2','%3')").arg(number).arg(password).arg(name);
                    if(query.exec(InsertStr)){
                        QMessageBox::information(NULL,"学生注册","注册成功！",QMessageBox::Ok);
                        this->close();
                    }
                    else qDebug() << "注册失败";
                }
            }
        }
        else if(ui->teacher->isChecked()){
            QString SelectStr=QString("SELECT COUNT(number) from teachers where number='%1' ").arg(number);
            if(query.exec(SelectStr)&&query.next()){
                int rows=query.value(0).toInt();
                if(rows>0){
                    QMessageBox::warning(NULL,"注册失败","账号已存在");
                }
                else{
                    QString InsertStr=QString("INSERT INTO teachers(number,passwords,name)"
                                                "values('%1','%2','%3')").arg(number).arg(password).arg(name);
                    if(query.exec(InsertStr)){
                        QMessageBox::information(NULL,"教师注册","注册成功！",QMessageBox::Ok);
                        this->close();
                    }
                }
            }
        }
        else if(ui->admin->isChecked()){
            //为了避免管理员泛滥，给“注册管理员”这个行为额外设置了一个密码（权限码
            QString SelectStr=QString("SELECT COUNT(number) from admin where number='%1' ").arg(number);
            if(query.exec(SelectStr)&&query.next()){
                int rows=query.value(0).toInt();
                if(rows>0){
                    QMessageBox::warning(NULL,"注册失败","账号已存在");
                }
                else{
                    QString access_code = QInputDialog::getText(this, "管理员权限码", "请输入管理员权限码:", QLineEdit::Password);
                    if (access_code == "000000") {
                        QString InsertStr=QString("INSERT INTO admin(number,passwords,name)"
                                                    "values('%1','%2','%3')").arg(number).arg(password).arg(name);
                        if(query.exec(InsertStr)){
                            QMessageBox::information(NULL,"管理员注册","注册成功！",QMessageBox::Ok);
                            this->close();
                        }
                    }
                    else {
                        QMessageBox::warning(this,"注册失败","权限码错误，请输入正确的权限码");
                    }
                }
            }
        }
    }
}


void edit::on_exit_clicked()
{
    this->close();
}

