#ifndef STUDENTPAGE_H
#define STUDENTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QTableView>
namespace Ui {
class studentpage;
}

class studentpage : public QWidget
{
    Q_OBJECT

public:
    explicit studentpage(QWidget *parent = nullptr);
    ~studentpage();

private slots:

    void on_personal_info_clicked();

    void on_courseChoose_clicked();

    void on_loginOut_clicked();

    void on_allCourses_clicked();

    void on_score_clicked();

private:
    Ui::studentpage *ui;
//    QSqlRelationalTableModel *model;
//    QSqlRelationalTableModel *model_2;
};

#endif // STUDENTPAGE_H
