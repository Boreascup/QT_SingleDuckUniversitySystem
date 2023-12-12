#ifndef SCORES_H
#define SCORES_H

#include <QtCharts>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>
//QT_CHARTS_USE_NAMESPACE

extern int id;

namespace Ui {
class scores;
}

class scores : public QWidget
{
    Q_OBJECT

public:
    explicit scores(QWidget *parent = nullptr);

    ~scores();

private slots:
    void set_my_scores();

    void on_search_clicked();

    void on_reSet_clicked();

    void on_pie_clicked();

    void on_bar_clicked();

//    void timeOut();
private:
    Ui::scores *ui;
    QSqlRelationalTableModel *model;
    int myscore[5]={0};
    QList<QList<QString>> scolist;
    QList<QString> tem;//用于存所选课程的id、成绩、绩点、排名
};

#endif // SCORES_H
