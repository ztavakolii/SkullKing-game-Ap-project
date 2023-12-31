#include "serverorclientwindow.h"
#include "ui_serverorclientwindow.h"
#include "person.h"
#include <QSoundEffect>

extern Person* User;
extern QSoundEffect*clickSound;

ServerOrClientWindow::ServerOrClientWindow(QMainWindow*prewindow,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerOrClientWindow)
{
    ui->setupUi(this);
    setFixedSize(1280,700);

    preWindow=prewindow;

    QIcon windowsIcon(":/new/image/gamename.png");
    this->setWindowIcon(windowsIcon);
    this->setWindowTitle("Server or Client?");

    ui->background->showFullScreen();

    ui->backButton->setStyleSheet("border:none");
    ui->backButton->setIcon(QIcon(":/new/image/icons8-back-48.png"));
    ui->backButton->setIconSize(QSize(40,40));
    connect(ui->backButton,SIGNAL(clicked(bool)),this,SLOT(backButtonClicked()));

    if(User->get_gender()=="Male")
    {
        ui->guideTextEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(0, 170, 255);");
    }
    else if(User->get_gender()=="Female")
    {
        ui->guideTextEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 85, 127);");
    }
    ui->guideTextEdit->setReadOnly(true);
    QString guide="I am glad that you accepted the command of our army in this war.Commander give your first order, everyone obeys.\nServer or client?\n\nSkull King";
    ui->guideTextEdit->setText(guide);

    ui->serverButton->setStyleSheet("border:none");
    connect(ui->serverButton,SIGNAL(clicked(bool)),this,SLOT(serverButtonClicked()));

    ui->clientButton->setStyleSheet("border:none");
    connect(ui->clientButton,SIGNAL(clicked(bool)),this,SLOT(clientButtonClicked()));


}

ServerOrClientWindow::~ServerOrClientWindow()
{
    delete ui;
}

void ServerOrClientWindow::backButtonClicked()
{
    clickSound->play();
    preWindow->showMaximized();
    this->close();
}

void ServerOrClientWindow::serverButtonClicked()
{
    clickSound->play();

    serverWindow=new ServerWindow(this,preWindow);
    serverWindow->showMaximized();
    this->close();
}

void ServerOrClientWindow::clientButtonClicked()
{
    clickSound->play();

    clientWindow=new ClientWindow(preWindow,this);
    clientWindow->showMaximized();
    this->close();
}
