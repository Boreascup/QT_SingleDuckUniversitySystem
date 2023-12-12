#include "stuInfo.h"
#include "login.h"
#include "studentpage.h"
#include "ui_stuInfo.h"
#include <QMessageBox>
#include <QTimer>
stuInfo::stuInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stuInfo)
{
    ui->setupUi(this);
    this->setWindowTitle("个人信息");
    Login::useCSS(this);
    on_base_clicked();
}


stuInfo::~stuInfo()
{
    delete ui;
}

void stuInfo::timeOut(){
    ui->label_psw->setText("");
    ui->label_newpsw->setText("");
    ui->label_newpsw2->setText("");
    ui->pswNote->setText("");
    ui->baseNote->setText("");
}

void stuInfo::on_submit_clicked(){//提交修改
    if(ui->stackedWidget->currentIndex()==0){
        QString psw=ui->pwd->text().trimmed();
        QString newpsw=ui->newPwd->text().trimmed();
        QString newpsw2=ui->newPwd2->text().trimmed();
        QTimer::singleShot(1500,this,SLOT(timeOut()));
        if(psw.isEmpty()){
            ui->pwd->setText("");
            ui->label_psw->setText("原密码不能为空");
            ui->label_psw->setStyleSheet("color:red");
            ui->pwd->setFocus();
        }
        else if(newpsw.isEmpty()){
            ui->newPwd->setText("");
            ui->label_newpsw->setText("新密码不能为空");
            ui->label_newpsw->setStyleSheet("color:red");
            ui->newPwd->setFocus();
        }
        else if(newpsw2.isEmpty()){
            ui->newPwd2->setText("");
            ui->label_newpsw2->setText("确认密码不能为空");
            ui->label_newpsw2->setStyleSheet("color:red");
            ui->newPwd2->setFocus();
        }
        else if(QString::compare(newpsw,newpsw2)!=0){
            QMessageBox::warning(NULL,"修改失败","两次密码输入不相同");
        }
        else{
            QSqlQuery query;
            QString searchstr = QString("select passwords from students where sid='%1'").arg(id);
            if(query.exec(searchstr)){
                while(query.next()){
                    if(query.value(0).toString()!=psw){
                        QMessageBox::warning(NULL,"修改失败","原密码错误");
                    }
                    else{
                        QString str = QString("update students set passwords='%1' where sid='%2'").arg(ui->newPwd->text()).arg(id);
                        QTimer::singleShot(1500,this,SLOT(timeOut()));
                        if (query.exec(str)) {
                            ui->pswNote->setText("修改成功");
                            ui->pswNote->setStyleSheet("color:red");
                        } else {
                            ui->pswNote->setText("修改失败");
                            ui->pswNote->setStyleSheet("color:red");
                        }
                    }
                }
            }
        }
    }
    else{
        QString str = QString("update students set telephone='%1',address ='%2' where sid='%3'").arg(ui->lineEdit_tel->text()).arg(ui->lineEdit_address->text()).arg(id);
        QSqlQuery query;
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

void stuInfo::on_change_Psw_clicked(){//修改密码
    ui->change_Psw->setFlat(true);
    ui->base->setFlat(true);
    ui->base->setStyleSheet("background-color:transparent;border:none;""color:black;");
    ui->change_Psw->setStyleSheet("background-color:transparent;border:none;""color:#228974;""text-decoration:underline;");
    ui->change_Psw->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void stuInfo::on_base_clicked(){//个人信息
    ui->change_Psw->setStyleSheet("background-color:transparent;border:none;""color:black;");
    ui->change_Psw->setFlat(true);
    ui->base->setFlat(true);
    ui->base->setStyleSheet("background-color:transparent;border:none;""color:#228974;""text-decoration:underline;");
    ui->base->setChecked(true);
    ui->stackedWidget->setCurrentIndex(1);
    QString str = QString("select * from students  where sid='%1'").arg(id);
    QSqlQuery query;
    if (query.exec(str)) {
        while(query.next()){
            ui->lineEdit_id->setText(query.value(1).toString());
            ui->lineEdit_id->setStyleSheet("color:black;");
            ui->lineEdit_name->setText(query.value(2).toString());
            ui->lineEdit_name->setStyleSheet("color:black;");
            ui->lineEdit_major->setText(query.value(4).toString());
            ui->lineEdit_major->setStyleSheet("color:black;");
            ui->lineEdit_tel->setText(query.value(5).toString());
            ui->lineEdit_address->setText(query.value(6).toString());
        }
    }
}

void stuInfo::on_cancel_clicked(){//取消修改
    QString str = QString("select telephone, address from students  where sid='%1'").arg(id);
    QSqlQuery query;
    if (query.exec(str)) {
        while(query.next()){
            ui->lineEdit_tel->setText(query.value(0).toString());
            ui->lineEdit_address->setText(query.value(1).toString());
        }
    }
    ui->pwd->setText("");
    ui->newPwd->setText("");
    ui->newPwd2->setText("");
}
