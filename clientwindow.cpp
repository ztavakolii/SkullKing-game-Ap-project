#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "person.h"
#include <QMessageBox>

extern Person User;

ClientWindow::ClientWindow(QMainWindow*personalwindow,QMainWindow*prewindow,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    preWindow=prewindow;
    personalWindow=personalwindow;

    QIcon windowsIcon(":/new/image/gamename.png");
    this->setWindowIcon(windowsIcon);
    this->setWindowTitle("Become Client");

    ui->backButton->setStyleSheet("border:none");
    ui->backButton->setIcon(QIcon(":/new/image/icons8-back-48.png"));
    ui->backButton->setIconSize(QSize(40,40));
    connect(ui->backButton,SIGNAL(clicked(bool)),this,SLOT(backButtonClicked()));

    if(User.get_gender()=="Male"){
        ui->guideTextEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(0, 170, 255);");
        ui->serverIPLineEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(0, 170, 255);");
    }
    else if(User.get_gender()=="Female"){
        ui->guideTextEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 85, 127);");
        ui->serverIPLineEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 85, 127);");
    }

    ui->guideTextEdit->setReadOnly(true);
    ui->guideTextEdit->setText("Commander, now you need to enter the IP of the server you want to connect to it.\n\nSkull King");

    ui->connectButton->setStyleSheet("border:none");
    connect(ui->connectButton,SIGNAL(clicked(bool)),this,SLOT(connectButtonClicked()));

}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::backButtonClicked()
{
    preWindow->showMaximized();
    this->close();
}

void ClientWindow::connectButtonClicked()
{
    if(ui->serverIPLineEdit->text().length()>0){
         //IP permission must be checked with regex
        //if(IP is correct)
        {
            User.set_client(QHostAddress(ui->serverIPLineEdit->text()));
            /*if(User.get_client()->getConnectionStatus()==true)*/{
            clientWaitWindow=new ClientWaitWindow(personalWindow,this);
            clientWaitWindow->showMaximized();
            this->close();
            }
        }
        //else
//        {
//            QMessageBox*message=new QMessageBox(this);
//            message->critical(this,"Become Client","The IP is invalid, enter a valid IP.");
//            //set style sheet of this message
//        }
    }
    else{
        QMessageBox*message=new QMessageBox(this);
        message->critical(this,"Become Client","Enter your desired server IP.");
        //set style sheet of this message
    }
}