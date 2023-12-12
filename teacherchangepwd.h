#ifndef TEACHANGEPASSWORD_H
#define TEACHANGEPASSWORD_H

#include <QDialog>
#include <QSqlQuery>
extern int id;
namespace Ui {
class teacherChangePwd;
}

class teacherChangePwd : public QDialog
{
    Q_OBJECT

public:
    explicit teacherChangePwd(QWidget *parent = nullptr);
    ~teacherChangePwd();

private slots:
    void on_finish_clicked();


    void on_cancel_clicked();

private:
    Ui::teacherChangePwd *ui;
};

#endif // TEACHANGEPASSWORD_H
