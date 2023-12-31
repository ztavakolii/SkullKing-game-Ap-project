#include "login.h"
#include "ui_login.h"
#include <QString>
#include <QStringList>
#include <windows.h>
#include "person.h"
#include "QMessageBox"
#include <QRegularExpression>
#include <windows.h>
#include <QSoundEffect>


extern Person* User;
extern QSoundEffect*clickSound;

login::login(QMainWindow*previousWindow,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setFixedSize(1280,700);

    preWindow=previousWindow;

    QIcon windowsIcon(":/new/image/gamename.png");
    this->setWindowIcon(windowsIcon);
    this->setWindowTitle("Login");

    ui->background->showFullScreen();

    ui->eyeButton->setStyleSheet("border:none");
    ui->loginButton->setStyleSheet("border:none");
    ui->forgetPasssWordButton->setStyleSheet("border:none");

    QString helloText="Hello my friend, the pirate war is coming. Log in, we need your command to win. Let's go, everyone is waiting for you...\n\nSkull King";
    ui->helloTextEdit->setText(helloText);

    QString forgetPasswordText="Now enter your phone number to change your password.";
    ui->forgetPassWordTextEdit->setText(forgetPasswordText);
    ui->forgetPassWordTextEdit->hide();
    ui->skullKingLabel->hide();

    QStringList countryPhoneCodes={"+98","+1","+86","+33","+49","+62","+81","+55","+61","+54","+39","+30","+34","+90","+852","+32","+964","+353","+52","+68"};
    ui->comboBox->addItems(countryPhoneCodes);
    ui->phoneLineEdit->setValidator(new QIntValidator(ui->phoneLineEdit));
    ui->comboBox->hide();
    ui->phoneLineEdit->hide();
    ui->okButton->setEnabled(false);
    ui->okButton->hide();
    ui->okButton->setStyleSheet("border:none");
    ui->ok1Label->hide();
    ui->ok2Label->hide();

    ui->newPasswordTextEdit->setText("Now enter new password.");
    ui->newPasswordTextEdit->hide();
    ui->skullKing2Label->hide();
    ui->eyeButton2->setStyleSheet("border:none");
    ui->eyeButton2->hide();
    ui->eyeLabel2->hide();
    ui->label_29->hide();

    ui->ok2Button->setStyleSheet("border:none");
    ui->ok2Button->setEnabled(false);
    ui->ok3Label->hide();
    ui->ok4Label->hide();
    ui->newPasswordLineEdit->hide();

  //  ui->statusBar->hide();

    ui->backButton->setIcon(QIcon(":/new/image/icons8-back-48.png"));
    ui->backButton->setStyleSheet("border:none");
    connect(ui->backButton,SIGNAL(clicked(bool)),this,SLOT(backButtonClicked()));

    ui->loginButton->setEnabled(false);
    connect(ui->passwordLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableLoginButton()));
    connect(ui->usernameLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableLoginButton()));

    connect(ui->eyeButton,SIGNAL(clicked(bool)),this,SLOT(changePasswordLineEditMode()));
    connect(ui->eyeButton2,SIGNAL(clicked(bool)),this,SLOT(changeNewPasswordLineEditMode()));
    connect(ui->loginButton,SIGNAL(clicked(bool)),this , SLOT(showPersonalWindow()));
    connect(ui->forgetPasssWordButton,SIGNAL(clicked(bool)),this,SLOT(forgetPassword()));
    connect(ui->phoneLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enablePhoneOkButton()));
    connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(checkMatchingUsernameandPhoneNumber()));
    connect(ui->newPasswordLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableNewPasswordOkButton()));
    connect(ui->ok2Button,SIGNAL(clicked(bool)),this,SLOT(changePasswordAndShowPersonalWindow()));
    connect(ui->usernameLineEdit,SIGNAL(textChanged(QString)),this ,SLOT(enablePhoneOkButton()));
    connect(ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(enablePhoneOkButton()));
}

login::~login()
{
    delete ui;
}

//void login::showEvent(QShowEvent *event)
//{
//  //  QMainWindow::showEvent(event);
//    QString helloText="Hello my friend, the pirate war is coming. Log in, we need your command to win. Let's go, everyone is waiting for you...\n\nSkull King";
//   // ui->helloTextEdit->setText(helloText);
//    for(int i=0;i<helloText.length();i++){
//        ui->helloTextEdit->setText(helloText.mid(0,i+1));
//        Sleep(100);
//    }
//    QMainWindow::showEvent(event);
//}

void login::changePasswordLineEditMode()
{
    clickSound->play();
    if(ui->passwordLineEdit->echoMode()==QLineEdit::Password){
        ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
        QPixmap p(":/new/image/icons8-eye-unchecked-50.png");
        ui->eyeLabel->setPixmap(p);
    }
    else if(ui->passwordLineEdit->echoMode()==QLineEdit::Normal){
        ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
        QPixmap p(":/new/image/icons8-eye-checked-50.png");
        ui->eyeLabel->setPixmap(p);
    }
}

void login::changeNewPasswordLineEditMode()
{
    clickSound->play();
    if(ui->newPasswordLineEdit->echoMode()==QLineEdit::Password){
        ui->newPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        QPixmap p(":/new/image/icons8-eye-unchecked-50.png");
        ui->eyeLabel2->setPixmap(p);
    }
    else if(ui->newPasswordLineEdit->echoMode()==QLineEdit::Normal){
        ui->newPasswordLineEdit->setEchoMode(QLineEdit::Password);
        QPixmap p(":/new/image/icons8-eye-checked-50.png");
        ui->eyeLabel2->setPixmap(p);
    }
}

void login::forgetPassword()
{
    clickSound->play();
    ui->skullKingLabel->show();
    ui->forgetPassWordTextEdit->show();
    ui->comboBox->show();
    ui->phoneLineEdit->show();
    ui->ok1Label->show();
    ui->ok2Label->show();
    ui->okButton->show();
}

void login::showPersonalWindow()
{
    clickSound->play();
    QString s=NULL;
    Person user(s,ui->usernameLineEdit->text(),s,ui->passwordLineEdit->text(),s,s,s,0,0);
    if(user.match(1))
    {
        User->set_user_name(user.get_user_name());
        User->read_information_from_file();
        personalWindow=new PersonalWindow(preWindow);
        personalWindow->showMaximized();
        this->close();
    }
    else
    {
        QMessageBox message;
        message.setText("Username and password do not match!");
        message.setIcon(QMessageBox::Critical);
        message.setWindowIcon(QIcon(":/new/image/gamename.png"));
        message.setStyleSheet("background-color: rgb(236, 197, 119)");
        message.exec();
    }
}

void login::enableLoginButton()
{
    if(ui->passwordLineEdit->text().length()>0&&ui->usernameLineEdit->text().length()>0)
        ui->loginButton->setEnabled(true);
    else ui->loginButton->setEnabled(false);
}

void login::enablePhoneOkButton()
{
    if(ui->phoneLineEdit->text().length()>0&&ui->usernameLineEdit->text().length()>0
        &&ui->comboBox->currentText().length()>0)
        ui->okButton->setEnabled(true);
    else {
        ui->okButton->setEnabled(false);
    }
}

void login::checkMatchingUsernameandPhoneNumber()
{
    clickSound->play();

    QString s=NULL;
    Person user(s,ui->usernameLineEdit->text(),s,s,ui->comboBox->currentText(),ui->phoneLineEdit->text(),s,0,0);

    if(user.match(2))
    {
        ui->skullKing2Label->show();
        ui->newPasswordTextEdit->show();
        ui->newPasswordLineEdit->show();
        ui->ok2Button->show();
        ui->ok3Label->show();
        ui->ok4Label->show();
        ui->eyeButton2->show();
        ui->label_29->show();
        ui->eyeLabel2->show();
    }
    else
    {
        QMessageBox message;
        message.setText("Username and phone number do not match!");
        message.setIcon(QMessageBox::Critical);
        message.setWindowIcon(QIcon(":/new/image/gamename.png"));
        message.setStyleSheet("background-color: rgb(236, 197, 119)");
        message.exec();
    }
}

void login::enableNewPasswordOkButton()
{
    if(ui->newPasswordLineEdit->text().length()>0)
        ui->ok2Button->setEnabled(true);
    else ui->ok2Button->setEnabled(false);
}

void login::changePasswordAndShowPersonalWindow()
{
    clickSound->play();

    QRegularExpression passwordRegex("^(?=.*[a-zA-Z])(?=.*[0-9]).{6,}$");
    QString password=ui->newPasswordLineEdit->text();
    QRegularExpressionMatch Match=passwordRegex.match(password);
    if(!Match.hasMatch())//invalid password
    {
        QMessageBox message;
        message.setText("The password must be at least 6 digits long and contain lowercase or uppercase letters and numbers.");
        message.setIcon(QMessageBox::Critical);
        message.setWindowIcon(QIcon(":/new/image/gamename.png"));
        message.setStyleSheet("background-color: rgb(236, 197, 119)");
        message.exec();
    }
    else{

        QString s=NULL;
        Person user(s,ui->usernameLineEdit->text(),s,ui->newPasswordLineEdit->text(),ui->comboBox->currentText(),ui->phoneLineEdit->text(),s,0,0);
        user.edit_password(ui->newPasswordLineEdit->text());
        User->set_user_name(user.get_user_name());
        User->read_information_from_file();
        personalWindow=new PersonalWindow(preWindow);
        personalWindow->showMaximized();
        this->close();

    }
}

void login::backButtonClicked()
{
    clickSound->play();
    preWindow->showMaximized();
    this->close();
}


