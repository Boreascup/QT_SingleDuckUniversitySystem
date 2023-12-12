#ifndef EDIT_H
#define EDIT_H

#include <QWidget>

namespace Ui {
class edit;
}

class edit : public QWidget
{
    Q_OBJECT

public:
    explicit edit(QWidget *parent = nullptr);
    void KeyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent* event);
    ~edit();

private slots:
    void on_pushButton_clicked();
    void timeOut();
    void on_exit_clicked();

private:
    Ui::edit *ui;
};

#endif // EDIT_H
