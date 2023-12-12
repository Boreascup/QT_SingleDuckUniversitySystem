#include "login.h"
#include <QSqlDatabase>
#include <QApplication>

int main(int argc, char *argv[])
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");//固定的不用改
    db.setHostName("localhost");//固定的不用改
    db.setPort(3306);//固定的不用改
    db.setDatabaseName("MYsql");//这个是你在ODBC里创建的连接名，不是数据库名
//    db.setUserName("root");//这个是mysql的默认账号，没改的话大家都一样
//    db.setPassword("123456");//你自己设置的密码
    bool ok = db.open();
    if(ok){
        qDebug() << "Succeed to connect";
    }
    else{
        qDebug() << "Fail to connect";
    }

    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
