#ifndef EDITPROFILEWINDOW_H
#define EDITPROFILEWINDOW_H

#include <QMainWindow>

namespace Ui {
class EditProfileWindow;
}

class EditProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditProfileWindow(QMainWindow*prewindow ,QWidget *parent = nullptr);
    ~EditProfileWindow();

private:
    Ui::EditProfileWindow *ui;
    QMainWindow*preWindow;

private slots:
    void backButtonClicked();
    void changePasswordLineEditMode();
};

#endif // EDITPROFILEWINDOW_H
