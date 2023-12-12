#ifndef STUINFO_H
#define STUINFO_H

#include <QWidget>
#include <QSqlQuery>
extern int id;

namespace Ui {
class stuInfo;
}

class stuInfo : public QWidget
{
    Q_OBJECT

public:
    explicit stuInfo(QWidget *parent = nullptr);

    void hide_pswpage();
    ~stuInfo();

private slots:
    void on_submit_clicked();

    void on_cancel_clicked();

    void timeOut();

    void on_change_Psw_clicked();

    void on_base_clicked();

private:
    Ui::stuInfo *ui;
};

#endif // STUINFO_H
