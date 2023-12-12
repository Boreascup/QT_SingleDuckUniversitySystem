#include "adminpage.h"
#include "ui_adminpage.h"
#include "login.h"
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QSqlRecord>
#include <QtSql/qsqlquerymodel.h>
#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QScreen>

adminpage::adminpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminpage)
{
    ui->setupUi(this);
    this->setWindowTitle("信息管理");
    Login::useCSS(this);

    //居中
    Login::SetCenter(this);

    ui->tableView->setAlternatingRowColors(true); // 表格数据行隔行变色
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // 单个数据格
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);//居中


    ui->comboBox->addItem("学生信息管理");//comboBox是下拉栏，定义一下三个选项
    ui->comboBox->addItem("教师信息管理");
    ui->comboBox->addItem("课程信息管理");

    setStyle_s();
}

adminpage::~adminpage()
{
    delete ui;
}


void adminpage::on_comboBox_activated(int index)//根据下拉栏的不同选择，调出不同的界面
{
    if(index == 0){//学生
        flag = 1;//flag定义在头文件里了
        setStyle_s();
    }
    else if(index == 1){//教师
        flag = 2;
        setStyle_t();
    }
    else if(index == 2){//课程
        flag = 3;
        setStyle_c();
    }
}


void adminpage::setStyle_s(){//设置学生管理界面

    tm = new QSqlRelationalTableModel(this);//tm也定义在头文件里
    tm->setTable("students");//给它绑定了数据库里的students表。具体原理需要自行搜索
    tm->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    tm->select();
    tm->removeColumns(0,0);
    ui->tableView->setModel(tm);//把tm和tableView绑定了

    QString headStr[] = {"序号", "账号", "姓名", "密码", "专业","电话","住址"};//下面这些用来设置表头。否则表头会是数据库的列名
    int colWidths[] = {40, 100, 80, 120, 150,150, 150};

    for(int i=0; i<7; i++)
    {   tm->setHeaderData(i, Qt::Horizontal, headStr[i]);
        ui->tableView->setColumnWidth(i,  colWidths[i]);
    }
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setColumnHidden(6,false);
    ui->tableView->setColumnHidden(7,true);

}


void adminpage::setStyle_t(){

    tm = new QSqlRelationalTableModel(this);
    tm->setTable("teachers");
    tm->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    tm->select();
    tm->removeColumns(0,0);
    ui->tableView->setModel(tm);

    QString headStr[] = {"序号", "账号", "姓名", "密码", "职位", "电话号码"};
    int colWidths[] = {80, 150, 150, 100, 100, 100};

    for(int i=0; i<6; i++)
    {   tm->setHeaderData(i, Qt::Horizontal, headStr[i]);
        ui->tableView->setColumnWidth(i,  colWidths[i]);
    }
    ui->tableView->verticalHeader()->setVisible(false);

}

void adminpage::setStyle_c(){

    tm = new QSqlRelationalTableModel(this);
    tm->setTable("course_info");
    tm->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    tm->setRelation(2, QSqlRelation("teachers", "sid", "name"));//这里在设置外键
    tm->setRelation(3, QSqlRelation("course_location", "sid", "location"));
    tm->select();
    tm->removeColumns(0,0);

    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));//设置下拉栏
    ui->tableView->setModel(tm);
    ui->tableView->verticalHeader()->setVisible(false);

    QString headStr[] = {"课程编号", "课程名称", "授课教师", "课程地点", "授课学时", "课程学分"};
    int colWidths[] = {90, 150, 150, 150, 120, 120};
    for(int i=0; i<6; i++)
    {   tm->setHeaderData(i, Qt::Horizontal, headStr[i]);
        ui->tableView->setColumnWidth(i,  colWidths[i]);
    }
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
    ui->tableView->setColumnHidden(8,true);
    ui->tableView->setColumnHidden(9,true);
}


void adminpage::on_add_clicked()//添加一行
{
    int rowNum = tm->rowCount();
    int id = rowNum+1;
    tm->insertRow(rowNum);
    tm->setData(tm->index(rowNum,0),id);
}


void adminpage::on_del_clicked()//问题：删除某位学生之后，课程里的已选人数不减少
{

    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) return;
    int curRow = ui->tableView->currentIndex().row();

    int ok = QMessageBox::warning(this,tr("删除当前行!"),tr("你确定删除当前行吗？"),
                                  QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No){
        tm->revertAll();
    }
    else {
        tm->removeRow(curRow);
        tm->submitAll();
    }
}

void adminpage::on_edit_clicked()//提交修改
{

    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) return;

    tm->database().transaction();
    int maxCol = tm->columnCount();

    if(flag == 1) maxCol-=1;//因为有虚拟列
    else if(flag == 3) maxCol -= 4;

    bool anyColumnEmpty = false;

    for (int col = 1; col < maxCol; col++) {
        QModelIndex index = tm->index(ui->tableView->currentIndex().row(), col);
        if (tm->data(index).isNull() || tm->data(index).toString().isEmpty()) {
            anyColumnEmpty = true;
            break;
        }
    }

    QSet<QString> snumber;
    int maxRow = tm->rowCount();
    for(int row = 0; row < maxRow; row++){
        QModelIndex index = tm->index(row, 1);
        snumber.insert(tm->data(index).toString());
    }
    int n=snumber.size();


    if (anyColumnEmpty) {
        QMessageBox::warning(this, tr("警告"), tr("请输入完整信息"), QMessageBox::Yes);
        return;
    }
    if(n<maxRow){
        QMessageBox::warning(this, tr("警告"), tr("账号不能重复"), QMessageBox::Yes);
        return;
    }
    if (tm->submitAll()) {
        tm->database().commit();
        QMessageBox::information(this, "提示", "操作成功", QMessageBox::Yes);
    }
    else {
        qDebug() <<"数据库错误";
    }

}

void adminpage::on_search_clicked()//搜索
{
    QString name = ui->lineEdit->text().trimmed();

    if(name.isEmpty()){
        QMessageBox::warning(NULL,"搜索失败","您未输入任何信息");
        return;
    }

    if(flag != 3)
    tm->setFilter(QString("name like '%%1%'").arg(name));
    else
    tm->setFilter(QString("course_info.name like '%%1%'").arg(name));
    tm->select();
}

void adminpage::on_back_clicked()//搜索后的返回
{
    ui->lineEdit->clear();
    if(flag == 1) setStyle_s();
    else if(flag == 2) setStyle_t();
    else setStyle_c();
}


void adminpage::on_retreat_clicked()//撤回 感觉时不时会bug一下可以多试试看 大不了删了
{
    tm->revertAll();
}


void adminpage::on_exit_clicked()//退出
{
    this->close();
    Login* login = new Login();
    login->show();
}



