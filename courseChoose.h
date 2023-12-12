#ifndef COURSECHOOSE_H
#define COURSECHOOSE_H

#include <QWidget>
#include <QSqlRelationalTableModel>

namespace Ui {
class courseChoose;
}

class courseChoose : public QWidget
{
    Q_OBJECT

public:
    explicit courseChoose(QWidget *parent = nullptr);
    ~courseChoose();

private slots:


    void on_confirm_clicked();

    void set_all_course();

    void set_my_course();

    void on_exit_clicked();

    void on_search_clicked();

    void on_return_2_clicked();

    void on_dropCourse_clicked();

private:
    Ui::courseChoose *ui;
    QSqlRelationalTableModel *model;
    QSqlRelationalTableModel *model_2;
};

#endif // COURSECHOOSE_H
