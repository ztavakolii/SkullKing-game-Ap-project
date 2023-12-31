#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "person.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QSoundEffect>

using namespace std;
extern Person *User;
extern QSoundEffect*clickSound;

ClientWindow::ClientWindow(QMainWindow*personalwindow,QMainWindow*prewindow,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    setFixedSize(1280,700);
    preWindow=prewindow;
    personalWindow=personalwindow;

    QIcon windowsIcon(":/new/image/gamename.png");
    this->setWindowIcon(windowsIcon);
    this->setWindowTitle("Become Client");

    ui->backButton->setStyleSheet("border:none");
    ui->backButton->setIcon(QIcon(":/new/image/icons8-back-48.png"));
    ui->backButton->setIconSize(QSize(40,40));
    connect(ui->backButton,SIGNAL(clicked(bool)),this,SLOT(backButtonClicked()));

    if(User->get_gender()=="Male"){
        ui->guideTextEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(0, 170, 255);");
        ui->serverIPLineEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(0, 170, 255);");
    }
    else if(User->get_gender()=="Female"){
        ui->guideTextEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 85, 127);");
        ui->serverIPLineEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 85, 127);");
    }

    ui->guideTextEdit->setReadOnly(true);
    ui->guideTextEdit->setText("Commander, now you need to enter the IP of the server you want to connect to it.\n\nSkull King");

    ui->connectButton->setStyleSheet("border:none");
    connect(ui->connectButton,SIGNAL(clicked(bool)),this,SLOT(connectButtonClicked()));

    if(User->get_gender()=="Male"){
        QPixmap p(":/new/image/jpgtopngconverter-com (1a).png");
        p=p.scaled(QSize(131,200));
        ui->pirateLabel->setGeometry(330,400,131,200);
        ui->pirateLabel->setPixmap(p);
    }
    else if(User->get_gender()=="Female"){
        QPixmap p(":/new/image/jpgtopngconverter-com (2b).png");
        p=p.scaled(QSize(151,171));
        ui->pirateLabel->setGeometry(340,450,151,171);
        ui->pirateLabel->setPixmap(p);
    }

}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::backButtonClicked()
{
    clickSound->play();
    preWindow->showMaximized();
    this->close();
}

void ClientWindow::connectButtonClicked()
{
    clickSound->play();

    if(ui->serverIPLineEdit->text().length()>0){
         //IP permission must be checked with regex
      QRegularExpression IPRegex("^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$");
        QString IP=ui->serverIPLineEdit->text();
      QRegularExpressionMatch Match=IPRegex.match(IP);
        if(Match.hasMatch())
        {
            User->set_client(this,personalWindow,QHostAddress(ui->serverIPLineEdit->text()));
        }
        else{
            QMessageBox message;
            message.setText("The IP is invalid, enter a valid IP.");
            message.setIcon(QMessageBox::Critical);
            message.setWindowIcon(QIcon(":/new/image/gamename.png"));
            message.setStyleSheet("background-color: rgb(236, 197, 119)");
            message.exec();
        }
    }
    else{
        QMessageBox message;
        message.setText("Enter your desired server IP.");
        message.setIcon(QMessageBox::Critical);
        message.setWindowIcon(QIcon(":/new/image/gamename.png"));
        message.setStyleSheet("background-color: rgb(236, 197, 119)");
        message.exec();
    }
}


