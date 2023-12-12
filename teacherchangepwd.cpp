#include "teacherchangepwd.h"
#include "ui_teacherchangepwd.h"
#include "login.h"
#include <QMessageBox>
#include <QTimer>
teacherChangePwd::teacherChangePwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::teacherChangePwd)
{
    ui->setupUi(this);
    this->setWindowTitle("修改密码");
    Login::useCSS(this);
}

teacherChangePwd::~teacherChangePwd()
{
    delete ui;
}

void teacherChangePwd::on_finish_clicked()
{
    ui->isOld->setText("");
    ui->isNew->setText("");
    ui->isComfirmed->setText("");
    ui->baseNote->setText("");
    QString psw=ui->oldpass->text().trimmed();
    QString newpsw=ui->newpass->text().trimmed();
    QString newpsw2=ui->confirmpass->text().trimmed();
    QTimer::singleShot(1500,this,SLOT(timeOut()));
    if(psw.isEmpty()){
        ui->oldpass->setText("");
        ui->isOld->setText("原密码不能为空");
        ui->isOld->setStyleSheet("color:red");
        ui->oldpass->setFocus();
    }
    else if(newpsw.isEmpty()){
        ui->newpass->setText("");
        ui->isNew->setText("新密码不能为空");
        ui->isNew->setStyleSheet("color:red");
        ui->newpass->setFocus();
    }
    else if(newpsw2.isEmpty()){
        ui->confirmpass->setText("");
        ui->isComfirmed->setText("确认密码不能为空");
        ui->isComfirmed->setStyleSheet("color:red");
        ui->confirmpass->setFocus();
    }
    else if(QString::compare(newpsw,newpsw2)!=0){
        QMessageBox::warning(NULL,"修改失败","两次密码输入不相同");
    }
    else{
        QSqlQuery query;
        QString searchstr = QString("select passwords from teachers where sid='%1'").arg(id);
        if(query.exec(searchstr)){
            while(query.next()){
                if(query.value(0).toString()!=psw){
                    QMessageBox::warning(NULL,"修改失败","原密码错误");
                }
                else{
                    QString str = QString("update teachers set passwords='%1' where sid='%2'").arg(ui->newpass->text()).arg(id);
                    QTimer::singleShot(1500,this,SLOT(timeOut()));
                    if (query.exec(str)) {
                        ui->baseNote->setText("修改成功");
                        ui->baseNote->setStyleSheet("color:red");
                    } else {
                        ui->baseNote->setText("修改失败");
                        ui->baseNote->setStyleSheet("color:red");
                    }
                }
            }
        }
    }
}


void teacherChangePwd::on_cancel_clicked()
{//全部清空
    ui->oldpass->setText("");
    ui->newpass->setText("");
    ui->confirmpass->setText("");
    ui->isOld->setText("");
    ui->isNew->setText("");
    ui->isComfirmed->setText("");
    ui->baseNote->setText("");
}

