#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

extern int id;

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);

    static void useCSS(QWidget* w);
    static void SetCenter(QWidget* w);
    void paintEvent(QPaintEvent* event);
    ~Login();

private slots:
    void on_login_clicked();

    void on_exit_clicked();

    void on_sign_clicked();

     void timeOut();
private:
    Ui::Login *ui;
    QString number;
    //int id;
};

#endif // LOGIN_H
