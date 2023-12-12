#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QtWidgets>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>


namespace Ui {
class adminpage;
}

class adminpage : public QWidget
{
    Q_OBJECT

public:
    QSqlQuery query;
    explicit adminpage(QWidget *parent = nullptr);
    void setStyle_s();
    void setStyle_t();
    void setStyle_c();
    ~adminpage();

private slots:
    void on_add_clicked();

    void on_del_clicked();

    void on_edit_clicked();

    void on_search_clicked();

    void on_back_clicked();

    void on_retreat_clicked();

    void on_exit_clicked();

    void on_comboBox_activated(int index);

private:
    Ui::adminpage *ui;
    QSqlRelationalTableModel *tm;
    int flag = 1;//1是学生，2是老师, 3是课程
};

#endif // ADMINPAGE_H
