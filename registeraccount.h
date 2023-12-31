#ifndef REGISTERACCOUNT_H
#define REGISTERACCOUNT_H

#include <QMainWindow>
#include <personalwindow.h>

namespace Ui {
class RegisterAccount;
}

class RegisterAccount : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterAccount(QMainWindow *prewindow,QWidget *parent = nullptr);
    ~RegisterAccount();

private slots:
    void on_eye_p_clicked();

    void on_back_button_clicked();

    void on_register_button_clicked(bool checked);

private:
    Ui::RegisterAccount *ui;
    QMainWindow *prewindow;
    PersonalWindow *personalwindow;
};

#endif // REGISTERACCOUNT_H
