#ifndef TEACHERSPAGE_H
#define TEACHERSPAGE_H

#include <QWidget>
#include <QSqlRelationalTableModel>

namespace Ui {
class teacherspage;
}

class teacherspage : public QWidget
{
    Q_OBJECT

public:
    explicit teacherspage(QWidget *parent = nullptr);
    void set_my_course();
    ~teacherspage();

private slots:

    void on_show_stu_info_clicked();

    void on_loginOut_clicked();

    void on_mark_clicked();

private:
    Ui::teacherspage *ui;
    QSqlRelationalTableModel* model;
    int curCourseID;
};

#endif // TEACHERSPAGE_H
