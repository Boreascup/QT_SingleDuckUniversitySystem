#ifndef TEACHERMAIN_H
#define TEACHERMAIN_H

#include <QWidget>

namespace Ui {
class teacherMain;
}

class teacherMain : public QWidget
{
    Q_OBJECT

public:
    explicit teacherMain(QWidget *parent = nullptr);
    ~teacherMain();

private slots:

    void on_changePwd_clicked();

    void on_viewCourse_clicked();

    void on_loginOut_clicked();

private:
    Ui::teacherMain *ui;
};

#endif // TEACHERMAIN_H
