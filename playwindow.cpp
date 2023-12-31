#include "windows.h"
#include "ui_playwindow.h"
#include "QTimer"
#include "QMessageBox"
#include "QDateTime"
#include "QFile"
#include "QTextStream"
#include "person.h"
#include "player.h"
#include "thread"
#include <QLabel>
#include <QPropertyAnimation>
#include <vector>
#include <QList>
#include <string>
#include <QSoundEffect>


using namespace std;

static bool closeWindowStatus=false;
extern QSoundEffect*clickSound;
extern Person*User;
Player *player; // the information of User in game like cards set ,....
vector<Player>players;
static int count=0;
int number_of_player=4;
int Round=1;
int hand=0;
int prediction;
bool is_win=0,is_first_one=0;
QString name,main_card;

PlayWindow::PlayWindow(QMainWindow*personalwindow,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayWindow)
{
    ui->setupUi(this);
    setFixedSize(1280,700);

    personalWindow=personalwindow;

    player=new Player;

    QIcon windowsIcon(":/new/image/gamename.png");
    this->setWindowIcon(windowsIcon);
    this->setWindowTitle("Play");

    ui->back_ground->showFullScreen();
//    ui->lineEdit->setValidator(new QIntValidator(ui->lineEdit));

    ui->guideTextEdit->setReadOnly(true);
    if(User->get_gender()=="Male")
    {
        ui->guideTextEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(0, 170, 255);");
        ui->lineEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(0, 170, 255);");
    }
    else if(User->get_gender()=="Female")
    {
        ui->guideTextEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 85, 127);");
        ui->lineEdit->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(255, 85, 127);");
    }
    ui->guideTextEdit->hide();
    ui->SkullKingPicture->hide();
    ui->lineEdit->setGeometry(40,290,71,31);
    ui->lineEdit->hide();
    ui->label_3->setGeometry(120,270,51,71);
    ui->label_3->hide();
    ui->label_5->setGeometry(136,293,31,21);
    ui->label_5->hide();
    ui->pushButton->setGeometry(125,293,41,25);
    ui->pushButton->hide();

   // QPixmap picture(":/new/image/icons8-pause-button-96.png");
   // ui->stop_label->setPixmap(picture);

    ui->time_lcd->hide();
    ui->lineEdit->hide();
    ui->pushButton->hide();

    ui->groupBox->hide();
    ui->ok_button->setEnabled(false);

    ui->player1GroupBox->hide();
    ui->player2GroupBox->hide();
    ui->player3GroupBox->hide();
    ui->player4GroupBox->hide();
    ui->arrow1->hide();
    ui->arrow2->hide();
    ui->arrow3->hide();
    ui->arrow4->hide();

    ui->stop_button->setStyleSheet("border:none");
    QPixmap pixmap1(":/new/image/icons8-play-button-96.png.png");
    ui->stop_label->setPixmap(pixmap1);
    ui->exit_button->setStyleSheet("border:none");
     ui->exchange_button->setStyleSheet("border:none");
     ui->ok_button->setStyleSheet("border:none");
    savedatetime(0);//save date time in file

     // round and hand labels in begining of play must be hide and when play starts, we must show them
    //set_hand(1);
   // set_round(1);

    User->set_coin(User->get_coin()-50);//take 50 coin
    User->edit();

//    if(User->get_client()!=nullptr){
//        t=new std::thread(&PlayWindow::readInformationSentByServer,this);
//    }
//    else t=nullptr;

    ui->pushButton_1->setStyleSheet("border:none");
    ui->pushButton_2->setStyleSheet("border:none");
    ui->pushButton_3->setStyleSheet("border:none");
    ui->pushButton_4->setStyleSheet("border:none");
    ui->pushButton_5->setStyleSheet("border:none");
    ui->pushButton_6->setStyleSheet("border:none");
    ui->pushButton_7->setStyleSheet("border:none");
    ui->pushButton_8->setStyleSheet("border:none");
    ui->pushButton_9->setStyleSheet("border:none");
    ui->pushButton_10->setStyleSheet("border:none");
    ui->pushButton_11->setStyleSheet("border:none");
    ui->pushButton_12->setStyleSheet("border:none");
    ui->pushButton_13->setStyleSheet("border:none");
    ui->pushButton_14->setStyleSheet("border:none");
    ui->pushButton->setStyleSheet("border:none");
    ui->pushButton_1->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_14->setEnabled(false);
    ui->pushButton->setEnabled(false);

    ui->noRadioButton_2->hide();
    ui->yesRadioButton->hide();

    connect(this,SIGNAL(second25Signal()),this,SLOT(exitSlot()));
    connect(this,SIGNAL(second15Signal()),this,SLOT(hideSkullKingWords()));
    connect(this,SIGNAL(second20Signal()),this,SLOT(hideSkullKingWords()));
    connect(this,SIGNAL(second45Signal()),this,SLOT(hideSkullKingWords()));
    connect(this,SIGNAL(aCardWasselected(QString)),this,SLOT(check_card(QString)));
    connect(this,SIGNAL(second30Signal()),this,SLOT(enterAnAllowedCardToTheGame()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(sentNumberOfHandsSaidWon()));
    connect(this,SIGNAL(second60Signal()),this,SLOT(sentNumberOfHandsSaidWon()));
    connect(this,SIGNAL(showCardSignal(int,QString)),this ,SLOT(showCard(int,QString)));
    connect(this,SIGNAL(stopCodeReceivedSignal(bool,QString)),this,SLOT(stopCodeReceived(bool,QString)));
    connect(this,SIGNAL(youCodeReceivedSignal()),this,SLOT(youCodeReceived()));
    connect(this,SIGNAL(setCardsIconSignal()),this,SLOT(setCardsIcon()));
    connect(this,SIGNAL(exitCodeReceivedSignal(QString)),this,SLOT(exitCodeReceived(QString)));
    connect(this,SIGNAL(showExchangeRequestSignal(QString)),this,SLOT(showExchangeRequest(QString)));
    connect(this,SIGNAL(showWinnerOfCurrentHandSignal(QString)),this,SLOT(showWinnerOfCurrentHand(QString)));
    connect(this,SIGNAL(showWinnerOfWholeGameSignal(QString)),this,SLOT(showWinnerOfWholeGame(QString)));
    connect(this,SIGNAL(exchangeReplyReceivedSignal(QString)),this,SLOT(exchangeReplyReceived(QString)));
    connect(this,SIGNAL(newRoundStartedSignal()),this,SLOT(newRoundStarted()));


}

PlayWindow::~PlayWindow()
{
    delete ui;
    if(t!=nullptr)
        t->join();
}

void PlayWindow::savedatetime(int n){
    QDateTime currentDateTime=QDateTime::currentDateTime();
    QString dateTimeString=currentDateTime.toString("yyyy-MM-dd  hh:mm:ss");;
    if(n==1)
        dateTimeString=dateTimeString+" Win";

    else
      dateTimeString=dateTimeString+" Lose";
    QString file_name,arr[90];
    file_name=User->get_user_name()+"_history";
    QFile file(file_name);
    file.open(QIODevice::ReadOnly);//open the history file and read the information
    QDataStream in(&file);
    for(int i=0;i<90;i++)
        in>>arr[i];
    file.close();
    file.remove();
    QFile file2(file_name);
    file2.open(QIODevice::WriteOnly);//create new history file and write the information in it
    QDataStream out(&file2);
    for(int i=9;i<90;i++)
        out<<arr[i];
    out<<dateTimeString;
//    for(int i=0;i<8;i++);///////////////kgbjnkml,///////////////////khnkn,m //////////////
//    out<<"Lose";
    file2.close();
}

void PlayWindow::setNumberOfPlayers(int number)
{
    number_of_player=number;
}

void PlayWindow::startcountdown(int n){
    initialvalueofremainingtime=n;
    remainingtime=n;//The remaining time for the countdown
    countdowntimer=new QTimer(this);//create a new QTimer object
    connect(countdowntimer,SIGNAL(timeout()),this,SLOT(f()));
    countdowntimer->start(1000);//start the timer with a timeout of 1 second
}

void PlayWindow::on_stop_button_clicked() // activating and inactivating buttons checked
{
    clickSound->play();

    QPixmap pixmap1(":/new/image/icons8-play-button-96.png.png");
    QPixmap labelpixmap=ui->stop_label->pixmap();
    QImage image1=pixmap1.toImage();
    QImage image=labelpixmap.toImage();

    QByteArray information;
    QDataStream out(&information,QIODevice::WriteOnly);
    if(image==image1){
        if(User->get_client()!=nullptr){
            out<<'s'<<'t'<<User->get_name();
            User->get_client()->writeInformation(information);
        }
        else if(User->get_server()!=nullptr){
            User->get_server()->serverWantsToStopPlay();
        }
        QPixmap p(":/new/image/icons8-pause-button-96 (2).png.png");
        ui->stop_label->setPixmap(p);
        stopCodeReceived(true,User->get_name());
    }
    else{
        countdowntimer->stop();
        ui->time_lcd->hide();
        if(User->get_client()!=nullptr){
            out<<'r'<<'s';
            User->get_client()->writeInformation(information);
        }
        else if(User->get_server()!=nullptr){
           User->get_server()->serverWantsToResumePlay();
        }
        ::count++;
        if(::count==2)
            ui->stop_button->setEnabled(false);
        QPixmap p(":/new/image/icons8-play-button-96.png.png");
        ui->stop_label->setPixmap(p);
        ui->stop_label->show();
    }
}

void PlayWindow::on_exit_button_clicked()
{
    clickSound->play();

    QByteArray information;
    QDataStream out(&information,QIODevice::WriteOnly);

    QMessageBox message;
    message.setText("Are you sure you want to exit the game?");
    message.setIcon(QMessageBox::Warning);
    message.setWindowIcon(QIcon(":/new/image/gamename.png"));
    message.setStyleSheet("background-color: rgb(236,197,119)");
    message.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
   int result= message.exec();
    if(result==QMessageBox::Yes){
        //back to the page personalwindow
        if(User->get_client()!=nullptr){
            out<<'e'<<'t'<<User->get_name();
            User->get_client()->writeInformation(information);
        }
        else if(User->get_server()!=nullptr){
            User->get_server()->serverWantsToExit();
        }
    }
    }

void PlayWindow::f()
{
    if(remainingtime>=0){
        if(remainingtime<=10)
            ui->time_lcd->setStyleSheet("color: rgb(170,0,0);");
        else
        ui->time_lcd->setStyleSheet("color: rgb(85, 255, 0);");

        ui->time_lcd->show();//show timer
        ui->time_lcd->display(QString::number(remainingtime));
        remainingtime--;
    }
    else{
    //counttime is finish
        countdowntimer->stop();//stop the timer
        ui->time_lcd->hide();//hide timer
        if(initialvalueofremainingtime==25)
        emit second25Signal();
        if(initialvalueofremainingtime==15)
        emit second15Signal();
        if(initialvalueofremainingtime==45)
        emit second45Signal();
        if(initialvalueofremainingtime==30)
        emit second30Signal();
        if(initialvalueofremainingtime==60)
        emit second60Signal();
        if(initialvalueofremainingtime==20)
        emit second20Signal();
        QPixmap p(":/new/image/icons8-play-button-96.png");
        ui->stop_label->setPixmap(p);

    }
}

void PlayWindow::set_round(int numberofround)
{
    Round=numberofround;
    QString r="Round "+QString::number(Round)+"/7";
    ui->round_label->setText(r);
}

void PlayWindow::set_hand(int numberofhand)
{
    hand=numberofhand;
    QString h="Hand "+QString::number(hand)+"/"+QString::number(2*Round);
    ui->hand_label->setText(h);
}

void PlayWindow::showCard(int index, QString cardCode)
{
    QString cardPictureAddress=":/new/image/"+cardCode+".jpg";

    QPixmap p(cardPictureAddress);
    //p=p.scaled(39,47);
    if(index==0){
        ui->card1->setPixmap(p);
    }
    else if(index==1){
        ui->card2->setPixmap(p);
    }
    else if(index==2){
        ui->card3->setPixmap(p);
    }
    else if(index==3){
        ui->card4->setPixmap(p);
    }
}

void PlayWindow::setPlayersScore()
{
    for(int i=0;i<number_of_player;i++){
        if(i==0)
        ui->scoreLabel1->setText(QString::number(players[i].getScore()));
        else if(i==1)
        ui->scoreLabel2->setText(QString::number(players[i].getScore()));
        else if(i==2)
        ui->scoreLabel3->setText(QString::number(players[i].getScore()));
        else if(i==3)
        ui->scoreLabel4->setText(QString::number(players[i].getScore()));
    }
}

void PlayWindow::showWinnerOfCurrentHand(QString clientName)
{
    ui->guideTextEdit->setText("The winner of this hand is "+clientName+"\nSkullKing");
    ui->SkullKingPicture->show();
    ui->guideTextEdit->show();
    startcountdown(15);
}

void PlayWindow::showWinnerOfWholeGame(QString clientName)
{
    ui->guideTextEdit->setText(clientName+" won the war and took "+QString::number(number_of_player*50)+ " coins as booty. You will be transferred to your personal page in 25 seconds.\nSkullKing");
    ui->SkullKingPicture->show();
    ui->guideTextEdit->show();
    startcountdown(25);
}

void PlayWindow::stopCodeReceived(bool b,QString name)
{
    if(b==true)//the client stop the game
    {
        ui->stop_button->setEnabled(true);
    }
    else{
        ui->stop_button->setEnabled(false);
    }

    QPixmap p(":/new/image/icons8-pause-button-96 (2).png.png");
    ui->stop_label->setPixmap(p);

    ui->guideTextEdit->setText(name+" has requested to stop fighting. The war will start in less than 20 seconds.\nSkullKing");
    ui->SkullKingPicture->show();
    ui->guideTextEdit->show();
    startcountdown(20);

    ui->pushButton_1->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_14->setEnabled(false);

    ui->exchange_button->setEnabled(false);
    ui->ok_button->setEnabled(false);
    ui->checkBox1->setEnabled(false);
    ui->checkBox2->setEnabled(false);
    ui->checkBox3->setEnabled(false);

    ui->exit_button->setEnabled(false);
}

void PlayWindow::resumeCodeReceived()
{
    QPixmap p(":/new/image/icons8-play-button-96.png.png");
    ui->stop_label->setPixmap(p);

    ui->SkullKingPicture->hide();
    ui->guideTextEdit->hide();

    ui->time_lcd->hide();

    ui->pushButton_1->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_14->setEnabled(false);

    ui->exchange_button->setEnabled(true);
    ui->ok_button->setEnabled(true);
    ui->checkBox1->setEnabled(true);
    ui->checkBox2->setEnabled(true);
    ui->checkBox3->setEnabled(true);

    ui->exit_button->setEnabled(true);

    if(::count<2)
        ui->stop_button->setEnabled(true);
}

void PlayWindow::youCodeReceived()
{
    startcountdown(30);
    ui->pushButton_1->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_13->setEnabled(true);
    ui->pushButton_14->setEnabled(true);
}

void PlayWindow::showExchangeRequest(QString clientName)
{
    ui->guideTextEdit->setText(clientName+" requests to exchange the card with you.Do you accept?\nSkullKing");
    ui->SkullKingPicture->show();
    ui->guideTextEdit->show();
    startcountdown(45);
    ui->yesRadioButton->show();
    ui->noRadioButton_2->show();

}

void PlayWindow::exchangeReplyReceived(QString clientName)
{
    ui->guideTextEdit->setText(clientName+" responded positively to your card exchange request and now two of your cards will be randomly exchanged.\nSkullKing");
    ui->SkullKingPicture->show();
    ui->guideTextEdit->show();
    startcountdown(45);
}

void PlayWindow::exchangeTwoCard(QString preCard, QString newCard)
{
    std::vector<QString> cards=player->getCasrdsSet();
    for(int i=0;i<cards.size();i++){
        if(cards[i]==preCard){
        cards[i]=newCard;
        break;
        }
    }
    setCardsIcon();
}

void PlayWindow::setPlayersForserverplayer(QByteArray information)
{
    QDataStream in(&information,QIODevice::ReadOnly);
    int clientScore;
    char code;
    QString clientName;
  //  QPixmap clientProfilePicture;
    in>>code;
    for(int i=0;i<number_of_player;i++){
        Player newPlayer;
        in>>clientName;
      //  in>>clientProfilePicture;
        in>>clientScore;
        newPlayer.setName(clientName);
     //   newPlayer.setProfile(clientProfilePicture);
        newPlayer.setScore(clientScore);
        players.push_back(newPlayer);
    }
  //  placeLabelsAroundCircle(200,1); // for profile and name
   // placeLabelsAroundCircle(200,2); // for cards
    listOfplayersReceived();
}

void PlayWindow::setScoresForServerPlayer(QByteArray information)
{
    QDataStream in(&information,QIODevice::ReadOnly);
    char code;
    in>>code;
    in>>code;
    int score;
    for(int i=0;i<number_of_player;i++){
        in>>score;
        players[i].setScore(score);
    }
    setPlayersScore();
}

void PlayWindow::showTurn(int index)
{
    ui->arrow1->hide();
    ui->arrow2->hide();
    ui->arrow3->hide();
    ui->arrow4->hide();

    if(index == 0)
        ui->arrow1->show();
    else if(index == 1)
        ui->arrow2->show();
    else if(index == 2)
        ui->arrow3->show();
    else ui->arrow4->show();
}


void PlayWindow::readInformationSentByServer()
{
    QString clientName,card,cardCode,preCard,newCard;
  //  QPixmap clientProfilePicture;
    vector<QString>cards;

    int number,clientScore,numberOfRound,numberOfHand,index,score;

    char mainCode,subCode;

    // mainCode                     | Received Information
    //-----------------------------------------------------------------------------------------------
    // 'e' "exit"                   | 't' - The name of the player who want to exit
    //-----------------------------------------------------------------------------------------------
    // 's' "stop"                   | 't' - The name of player who want to stop play
    //-----------------------------------------------------------------------------------------------
    // 'r' "resume"                 | 's'
    //-----------------------------------------------------------------------------------------------
    // 'e' "exchange"               | 'c' "change" - plyer's name who requested to change the card
    //-----------------------------------------------------------------------------------------------
    // 'h' "hand"                   | number of hand
    //-----------------------------------------------------------------------------------------------
    // 'r' "round"                  | 'o' - number of round
    //-----------------------------------------------------------------------------------------------
    // 'r' "reply exchange"         | 'p' - player's name who accept the request of exchanging card
    //-----------------------------------------------------------------------------------------------
    // 'n' "number of players"      | number of players
    //-----------------------------------------------------------------------------------------------
    // 'l' "list of players"        | player name - player profile picture - player score for each player
    //-----------------------------------------------------------------------------------------------
    // 'c' "player's cards"         | codes of player's cards
    //-----------------------------------------------------------------------------------------------
    // 't' "next turn"              | the index of players vector that must choose a card
    //-----------------------------------------------------------------------------------------------
    // 'y' "you"                    | it tells the player that it is her/his turn to play a card
    //-----------------------------------------------------------------------------------------------
    // 's' "show card"              | 'w' - card code - the index of players vector that play this card
    //-----------------------------------------------------------------------------------------------
    // 'w' "winner"                 | 'h' "hand" - name or index of player that wins the hand
    //-----------------------------------------------------------------------------------------------
    // 'w' "winner"                 | 'w' "whole" - name or index of player that wins the play
    //-----------------------------------------------------------------------------------------------
    // 's' "score"                  | 'c' - the score of all players after each hand
    //-----------------------------------------------------------------------------------------------
    // 'w' "winner"                 | 'y' "you win whole game"
    //-----------------------------------------------------------------------------------------------
    // 'e' "exchange"               | 'x' - previous card code - new card code
    //-----------------------------------------------------------------------------------------------
    // 'f' "playing field card code"| card code
    //-----------------------------------------------------------------------------------------------
    // 'b' "beginner"               | 'h' "hand"
    //-----------------------------------------------------------------------------------------------
    // 's' "start"                  | 'r' "round"
    //-----------------------------------------------------------------------------------------------

    while(true){

        if(closeWindowStatus==true)
        break;

        if(User->get_client()->getReceiveStatus()==true){
            QByteArray receivedInformation="";/*=User->get_client()->readInformation()*/
            emit User->get_client()->readSignal(&receivedInformation);
            while(receivedInformation=="");
            //QDataStream in(&receivedInformation,QIODevice::ReadOnly);
            QList<QByteArray>parts=receivedInformation.split(' ');
            for(QList<QByteArray>::iterator it=parts.begin();it!=parts.end();it++){
                QDataStream in(&(*it),QIODevice::ReadOnly);
            in>>mainCode;

            switch(mainCode){
            case 'e':
                in>>subCode;
                switch(subCode){
                case 't':
                    in>>clientName;
                   emit exitCodeReceivedSignal(clientName);
                    break;

                case 'c':
                    in>>clientName;
                    name=clientName;
                    emit showExchangeRequestSignal(clientName);
                    break;

                case 'x':
                    in>>preCard>>newCard;
                    exchangeTwoCard(preCard,newCard);
                    break;
                }
                break;

            case 's':
                in>>subCode;
                switch(subCode){
                case 't':
                    in>>clientName;
                    //stopCodeReceived(false,clientName);
                    emit stopCodeReceivedSignal(false,clientName);
                    break;

                case 'w':
                    in>>cardCode>>index;
                    //showCard(index,cardCode);
                    emit showCardSignal(index,cardCode);
                    break;

                case 'c':
                    for(int i=0;i<number_of_player;i++){
                        in>>score;
                        players[i].setScore(score);
                    }
                    setPlayersScore();
                    break;
                case 'r':
                    emit newRoundStartedSignal();
                    break;
                }
                break;

            case 'r':
                in>>subCode;
                switch(subCode){
                case 's':
                    resumeCodeReceived();
                    break;

                case 'o':
                    in>>numberOfRound;
                    set_round(numberOfRound);
                    break;

                case 'p':
                    in>>clientName;
                    emit exchangeReplyReceivedSignal(clientName);
                    break;
                }
                break;

            case 'h':
                in>>numberOfHand;
                set_hand(numberOfHand);
                break;

            case 'n':
                in>>number;
                setNumberOfPlayers(number);
                break;

            case 'l':
                for(int i=0;i<number_of_player;i++){
                    Player newPlayer;
                    in>>clientName;
                  //  in>>clientProfilePicture;
                    in>>clientScore;
                    newPlayer.setName(clientName);
                  //  newPlayer.setProfile(clientProfilePicture);
                    newPlayer.setScore(clientScore);
                    players.push_back(newPlayer);
                }
                listOfplayersReceived();
              //  placeLabelsAroundCircle(200,1); // for profile and name
               // placeLabelsAroundCircle(200,2); // for cards
                break;

            case 'c':
                for(int i=0;i<2*Round;i++){
                    in>>card;
                    cards.push_back(card);
                }
                player->setCards(cards);
                //Showing that the cards go from the center of the circle to the people and
                setCardsIcon();   //matching the person cards with push button cards
                //emit setCardsIconSignal();
                break;

            case 't':
                in>>index;
                showTurn(index);
               // rotate_bottle(index);
                break;

            case 'y':
                //youCodeReceived();
                emit youCodeReceivedSignal();
                break;

            case 'w':
                in>>subCode;
                switch(subCode){
                case 'h':
                    in>>clientName;
                  emit showWinnerOfCurrentHandSignal(clientName);
                    break;

                case 'w':
                    in>>clientName;
                    emit showWinnerOfWholeGameSignal(clientName);
                    break;

                case 'y':
                    User->set_coin(User->get_coin()+(number_of_player*50));
                    User->set_cup(User->get_cup()+15);
                    is_win=1;
                    User->edit();
                    break;
                }
                break;
            case 'f':
                in>>cardCode;
                setPlayingFieldCardCode(cardCode);
                break;

            case 'b':
                in>>subCode;
                switch(subCode){
                case 'h':
                    setIsFirstOne();
                    break;
                }
                break;
            }
            }
        }
    }
}

void PlayWindow::listOfplayersReceived()
{
    for(int i=0;i<number_of_player;i++){
        if(i==0){
            ui->player1GroupBox->setTitle(players[i].getName());
            ui->scoreLabel1->setText(QString::number(players[i].getScore()));
            ui->player1GroupBox->show();
        }
        else if(i==1){
            ui->player2GroupBox->setTitle(players[i].getName());
            ui->scoreLabel2->setText(QString::number(players[i].getScore()));
            ui->player2GroupBox->show();
        }
        else if(i==2){
            ui->player3GroupBox->setTitle(players[i].getName());
            ui->scoreLabel3->setText(QString::number(players[i].getScore()));
            ui->player3GroupBox->show();
        }
        else{
            ui->player4GroupBox->setTitle(players[i].getName());
            ui->scoreLabel4->setText(QString::number(players[i].getScore()));
            ui->player4GroupBox->show();
        }
    }
}

void PlayWindow::exitCodeReceived(QString clientName)
{
    ui->guideTextEdit->setText("Commander, a person named "+clientName+" from the enemy's army, has requested to leave the war. So the war ends and you will be transferred to your personal window in 25 seconds.\nSkullKing");
    ui->SkullKingPicture->show();
    ui->guideTextEdit->show();
    is_win=1;
    startcountdown(25);
    User->set_coin(User->get_coin()+(number_of_player*50)/(number_of_player-1));
}

void PlayWindow::enterAnAllowedCardToTheGame()
{
    QByteArray information;
    QDataStream out(&information,QIODevice::WriteOnly);
    bool b,c=false;
    //   QString str="flag";
    vector<QString> cards(player->getCasrdsSet());
    QString selected_card;
    int selected_card_index;

    for(vector<QString>::iterator it=cards.begin();it!=cards.end();it++){
        if(c==false){

        selected_card=*it;

    for(vector<QString>::iterator it=cards.begin();it!=cards.end();it++){
        if(*it==selected_card){
            selected_card_index=std::distance(cards.begin(),it);
            break;
        }
    }

    if(selected_card.contains("flag")){
        if(is_first_one==false){
            for(vector<QString>::iterator it=cards.begin();it!=cards.end();it++)
                if(it->contains(main_card)){
                    b=true;
                    break;
                }
            if(b==false){
                //enter the card to the game
                c=true;
                if(User->get_client()!=nullptr){
                    out<<'c'<<selected_card;
                    User->get_client()->writeInformation(information);
                }
                else if(User->get_server()!=nullptr){
                    User->get_server()->serverSelectCard(selected_card);
                }

                player->deleteACardFromCardsList(selected_card_index);
                countdowntimer->stop();
                ui->time_lcd->hide();
                setCardsIcon();
                ui->pushButton_1->setEnabled(false);
                ui->pushButton_2->setEnabled(false);
                ui->pushButton_3->setEnabled(false);
                ui->pushButton_4->setEnabled(false);
                ui->pushButton_5->setEnabled(false);
                ui->pushButton_6->setEnabled(false);
                ui->pushButton_7->setEnabled(false);
                ui->pushButton_8->setEnabled(false);
                ui->pushButton_9->setEnabled(false);
                ui->pushButton_10->setEnabled(false);
                ui->pushButton_11->setEnabled(false);
                ui->pushButton_12->setEnabled(false);
                ui->pushButton_13->setEnabled(false);
                ui->pushButton_14->setEnabled(false);

            }
        }
        else{
            //enter the card to the game
            c=true;
            if(User->get_client()!=nullptr){
                out<<'c'<<selected_card;
                User->get_client()->writeInformation(information);
            }
            else if(User->get_server()!=nullptr){
                User->get_server()->serverSelectCard(selected_card);
            }
            player->deleteACardFromCardsList(selected_card_index);
            countdowntimer->stop();
            ui->time_lcd->hide();
            setCardsIcon();
            ui->pushButton_1->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
            ui->pushButton_5->setEnabled(false);
            ui->pushButton_6->setEnabled(false);
            ui->pushButton_7->setEnabled(false);
            ui->pushButton_8->setEnabled(false);
            ui->pushButton_9->setEnabled(false);
            ui->pushButton_10->setEnabled(false);
            ui->pushButton_11->setEnabled(false);
            ui->pushButton_12->setEnabled(false);
            ui->pushButton_13->setEnabled(false);
            ui->pushButton_14->setEnabled(false);
        }
    }
    else{
        //enter the card to the game
        c=true;
        if(User->get_client()!=nullptr){
            out<<'c'<<selected_card;
            User->get_client()->writeInformation(information);
        }
        else if(User->get_server()!=nullptr){
            User->get_server()->serverSelectCard(selected_card);
        }

        player->deleteACardFromCardsList(selected_card_index);
        countdowntimer->stop();
        ui->time_lcd->hide();
        setCardsIcon();
        ui->pushButton_1->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
        ui->pushButton_12->setEnabled(false);
        ui->pushButton_13->setEnabled(false);
        ui->pushButton_14->setEnabled(false);
    }
    }
    }
}

void PlayWindow::on_exchange_button_clicked()
{
    clickSound->play();

    ui->groupBox->show();
    ui->ok_button->setEnabled(true);
    if(number_of_player==2){
        ui->player2->hide();
        ui->player3->hide();
        ui->checkBox2->hide();
        ui->checkBox3->hide();
    }
    else if(number_of_player==3){
        ui->player3->hide();
        ui->checkBox3->hide();
    }
    vector<Player>playersListWithoutThisPlayer;
    for(vector<Player>::iterator it=players.begin();it!=players.end();it++){
        if(it->getName()!=User->get_name()){
    playersListWithoutThisPlayer.push_back(*it);
        }
    }
    for(int i=0;i<playersListWithoutThisPlayer.size();i++){
        if(i==0){
    ui->player1->setText(playersListWithoutThisPlayer[i].getName());
        }
        if(i==1){
    ui->player2->setText(playersListWithoutThisPlayer[i].getName());
        }
        if(i==2){
    ui->player3->setText(playersListWithoutThisPlayer[i].getName());
        }
    }
}


void PlayWindow::on_ok_button_clicked()
{
    clickSound->play();

    //send to server or clients
    QByteArray information;
    QDataStream out(&information,QIODevice::WriteOnly);
    int numberOfChoices=0;
    vector<QString> choices;
    if(ui->checkBox1->isChecked()==true){
        numberOfChoices++;
        choices.push_back(ui->player1->text());
    }
    if(ui->checkBox2->isChecked()==true){
        numberOfChoices++;
         choices.push_back(ui->player2->text());
    }
    if(ui->checkBox3->isChecked()==true){
        numberOfChoices++;
         choices.push_back(ui->player3->text());
    }

    if(User->get_client()!=nullptr){
         out<<'e'<<'c'<<User->get_name()<<numberOfChoices;
         for(int i=0;i<choices.size();i++){
            out<<choices[i];
         }
         User->get_client()->writeInformation(information);
    }
    else if(User->get_server()!=nullptr){
         out<<User->get_name()<<numberOfChoices;
         for(int i=0;i<choices.size();i++){
            out<<choices[i];
         }
         User->get_server()->sentExchangeRequestToClients(information);
    }
    ui->groupBox->hide();
    ui->ok_button->setEnabled(false);
    ui->checkBox1->setChecked(false);
    ui->checkBox2->setChecked(false);
    ui->checkBox3->setChecked(false);
}

void PlayWindow::exitSlot()
{
    User->get_client()=nullptr;
    User->get_server()=nullptr;
    personalWindow->showMaximized();
    end_of_play();
    this->close();
}

void PlayWindow::hideSkullKingWords()
{
    ui->SkullKingPicture->hide();
    ui->guideTextEdit->hide();
    ui->time_lcd->hide();
    ui->lineEdit->hide();
    ui->label_3->hide();
    ui->label_5->hide();
    ui->pushButton->hide();
    ui->yesRadioButton->hide();
    ui->noRadioButton_2->hide();
}

void PlayWindow::setCardsIcon()
{
    vector<QString>cards=player->getCasrdsSet();
    for(int i=0;i<cards.size();i++){
        QString cardAddress(":/new/image/"+cards[i]+".jpg");
        //QPixmap p(cardAddress);
       // QIcon icon(cardAddress);
        QIcon iicon=QIcon::fromTheme(cardAddress);
        if(i==0){
            ui->pushButton_1->setIcon(iicon);
            ui->pushButton_1->setIconSize(QSize(71,101));
            ui->pushButton_1->show();
        }
        else if(i==1){
            ui->pushButton_2->setIcon(iicon);
            ui->pushButton_2->setIconSize(QSize(71,101));
            ui->pushButton_2->show();
        }
        else if(i==2){
            ui->pushButton_3->setIcon(iicon);
            ui->pushButton_3->setIconSize(QSize(71,101));
            ui->pushButton_3->show();
        }
        else if(i==3){
            ui->pushButton_4->setIcon(iicon);
            ui->pushButton_4->setIconSize(QSize(71,101));
            ui->pushButton_4->show();
        }
        else if(i==4){
            ui->pushButton_5->setIcon(iicon);
            ui->pushButton_5->setIconSize(QSize(71,101));
            ui->pushButton_5->show();
        }
        else if(i==5){
            ui->pushButton_6->setIcon(iicon);
            ui->pushButton_6->setIconSize(QSize(71,101));
            ui->pushButton_6->show();
        }
        else if(i==6){
            ui->pushButton_7->setIcon(iicon);
            ui->pushButton_7->setIconSize(QSize(71,101));
            ui->pushButton_7->show();
        }
        else if(i==7){
            ui->pushButton_8->setIcon(iicon);
            ui->pushButton_8->setIconSize(QSize(71,101));
            ui->pushButton_8->show();
        }
        else if(i==8){
            ui->pushButton_9->setIcon(iicon);
            ui->pushButton_9->setIconSize(QSize(71,101));
            ui->pushButton_9->show();
        }
        else if(i==9){
            ui->pushButton_10->setIcon(iicon);
            ui->pushButton_10->setIconSize(QSize(71,101));
            ui->pushButton_10->show();
        }
        else if(i==10){
            ui->pushButton_11->setIcon(iicon);
            ui->pushButton_11->setIconSize(QSize(71,101));
            ui->pushButton_11->show();
        }
        else if(i==11){
            ui->pushButton_12->setIcon(iicon);
            ui->pushButton_12->setIconSize(QSize(71,101));
            ui->pushButton_12->show();
        }
        else if(i==12){
            ui->pushButton_13->setIcon(iicon);
            ui->pushButton_13->setIconSize(QSize(71,101));
            ui->pushButton_13->show();
        }
        else if(i==13){
            ui->pushButton_14->setIcon(iicon);
            ui->pushButton_14->setIconSize(QSize(71,101));
            ui->pushButton_14->show();
        }
    }

    int numberOfCards=cards.size();
    if(numberOfCards<=13)
    ui->pushButton_14->hide();
    if(numberOfCards<=12)
    ui->pushButton_13->hide();
    if(numberOfCards<=11)
    ui->pushButton_12->hide();
    if(numberOfCards<=10)
    ui->pushButton_11->hide();
    if(numberOfCards<=9)
    ui->pushButton_10->hide();
    if(numberOfCards<=8)
    ui->pushButton_9->hide();
    if(numberOfCards<=7)
    ui->pushButton_8->hide();
    if(numberOfCards<=6)
    ui->pushButton_7->hide();
    if(numberOfCards<=5)
    ui->pushButton_6->hide();
    if(numberOfCards<=4)
    ui->pushButton_5->hide();
    if(numberOfCards<=3)
    ui->pushButton_4->hide();
    if(numberOfCards<=2)
    ui->pushButton_3->hide();
    if(numberOfCards<=1)
    ui->pushButton_2->hide();
    if(numberOfCards<=0)
    ui->pushButton_1->hide();
}

void PlayWindow::on_pushButton_1_clicked()
{
    clickSound->play();
    QIcon ico=ui->pushButton_1->icon();
    QString cardCode = ui->pushButton_1->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_2_clicked()
{
    clickSound->play();
    QIcon ico=ui->pushButton_2->icon();
    QString cardCode = ui->pushButton_2->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_3_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_3->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_4_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_4->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_5_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_5->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}

void PlayWindow::on_pushButton_6_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_6->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_7_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_7->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_8_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_8->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_9_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_9->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_10_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_10->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_pushButton_11_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_11->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}



void PlayWindow::on_pushButton_12_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_12->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}



void PlayWindow::on_pushButton_13_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_13->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}



void PlayWindow::on_pushButton_14_clicked()
{
    clickSound->play();

    QString cardCode = ui->pushButton_14->icon().name();
    cardCode.remove(":/new/image/");
    cardCode.remove(".jpg");
    emit aCardWasselected(cardCode);
}


void PlayWindow::on_yesRadioButton_clicked()
{

    QByteArray information;
    QDataStream out(&information,QIODevice::WriteOnly);
    // 'r' - 'p' - name of sender of request - name of receiver of request
    if(User->get_client()!=nullptr){
        out<<'r'<<'p'<<name<<User->get_name();
        User->get_client()->writeInformation(information);
    }
    else if(User->get_server()!=nullptr){
        //call the function of server class that take to name and shuffle cards and handle exchanging cards
        User->get_server()->exchangeTwoCardRandomly(name,User->get_name());
    }
    ui->guideTextEdit->hide();
    ui->SkullKingPicture->hide();
    ui->noRadioButton_2->hide();
    ui->yesRadioButton->hide();
    ui->time_lcd->hide();
}


void PlayWindow::on_noRadioButton_2_clicked()
{
    ui->guideTextEdit->hide();
    ui->SkullKingPicture->hide();
    ui->noRadioButton_2->hide();
    ui->yesRadioButton->hide();
    ui->time_lcd->hide();
}

void PlayWindow::showEvent(QShowEvent *event)
{
    User->read_information_from_file();
    ui->back_ground->setPixmap(User->get_playWindowBackground());

    if(User->get_client()!=nullptr){
                t=new std::thread(&PlayWindow::readInformationSentByServer,this);
            }
    else t=nullptr;

    QMainWindow::showEvent(event);
}

void PlayWindow::setPlayingFieldCardCode(QString cardCode)
{
    main_card=cardCode;
}

void PlayWindow::setIsFirstOne()
{
    is_first_one=true;
}

void PlayWindow::newRoundStarted()
{
    ui->lineEdit->show();
    ui->label_3->show();
    ui->label_5->show();
    ui->pushButton->show();
    ui->pushButton->setEnabled(true);
    ui->guideTextEdit->setText("Commander, how many hands do you think we will win in this round?\nEnter your desired number below.\n\nSkullKing");
    ui->SkullKingPicture->show();
    ui->guideTextEdit->show();
    startcountdown(60);
}

void PlayWindow::serverplayerWins()
{
    User->set_coin(User->get_coin()+(number_of_player*50));
    User->set_cup(User->get_cup()+15);
    is_win=1;
    User->edit();
}

//void PlayWindow::show_line_edit()
//{
//    ui->lineEdit->show();
//    ui->pushButton->show();
//    ui->pushButton->setEnabled(true);
//}

void PlayWindow::end_of_play()
{
   // QMessageBox message;
    if(is_win==1){
        savedatetime(1);
      //  message.setText("You win");
    }
    else{
        savedatetime(0);
       // message.setText("You Lose");
    }
    //message.exec();
}

void PlayWindow::closeEvent(QCloseEvent *event)
{
    closeWindowStatus=true;
    t->join(); // ********** posibility of error ************
    QMainWindow::closeEvent(event);
}

void PlayWindow::check_card(QString selected_card)
{
    QByteArray information;
    QDataStream out(&information,QIODevice::WriteOnly);
    bool b;
 //   QString str="flag";
    vector<QString> cards(player->getCasrdsSet());
    int selected_card_index;

    for(vector<QString>::iterator it=cards.begin();it!=cards.end();it++){
        if(*it==selected_card){
        selected_card_index=std::distance(cards.begin(),it);
        break;
        }
    }

    if(selected_card.contains("flag")){
        if(is_first_one==false){
        for(vector<QString>::iterator it=cards.begin();it!=cards.end();it++)
            if(it->contains(main_card)){
                b=true;
                QMessageBox message;
                message.setText("You not allow to choose this card.");
                message.setIcon(QMessageBox::Critical);
                message.setWindowIcon(QIcon(":/new/image/gamename.png"));
                message.setStyleSheet("background-color: rgb(236, 197, 119)");
                message.exec();
                break;
            }
        if(b==false){
            //enter the card to the game
            if(User->get_client()!=nullptr){
                out<<'c'<<selected_card;
                User->get_client()->writeInformation(information);
            }
            else if(User->get_server()!=nullptr){
                User->get_server()->serverSelectCard(selected_card);
            }

            player->deleteACardFromCardsList(selected_card_index);
            countdowntimer->stop();
            ui->time_lcd->hide();
            setCardsIcon();
            ui->pushButton_1->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
            ui->pushButton_5->setEnabled(false);
            ui->pushButton_6->setEnabled(false);
            ui->pushButton_7->setEnabled(false);
            ui->pushButton_8->setEnabled(false);
            ui->pushButton_9->setEnabled(false);
            ui->pushButton_10->setEnabled(false);
            ui->pushButton_11->setEnabled(false);
            ui->pushButton_12->setEnabled(false);
            ui->pushButton_13->setEnabled(false);
            ui->pushButton_14->setEnabled(false);

        }
        }
        else{
        //enter the card to the game
        if(User->get_client()!=nullptr){
            out<<'c'<<selected_card;
            User->get_client()->writeInformation(information);
        }
        else if(User->get_server()!=nullptr){
            User->get_server()->serverSelectCard(selected_card);
        }
        player->deleteACardFromCardsList(selected_card_index);
        countdowntimer->stop();
        ui->time_lcd->hide();
        setCardsIcon();
        ui->pushButton_1->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
        ui->pushButton_12->setEnabled(false);
        ui->pushButton_13->setEnabled(false);
        ui->pushButton_14->setEnabled(false);
        }
    }
    else{
        //enter the card to the game
        if(User->get_client()!=nullptr){
        out<<'c'<<selected_card;
        User->get_client()->writeInformation(information);
        }
        else if(User->get_server()!=nullptr){
        User->get_server()->serverSelectCard(selected_card);
        }

        player->deleteACardFromCardsList(selected_card_index);
        countdowntimer->stop();
        ui->time_lcd->hide();
        setCardsIcon();
        ui->pushButton_1->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_5->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(false);
        ui->pushButton_12->setEnabled(false);
        ui->pushButton_13->setEnabled(false);
        ui->pushButton_14->setEnabled(false);
    }
}

void PlayWindow::sentNumberOfHandsSaidWon()
{

    ui->time_lcd->hide();
    ui->lineEdit->hide();
    ui->label_3->hide();
    ui->label_5->hide();
    ui->pushButton->hide();
    ui->guideTextEdit->hide();
    ui->SkullKingPicture->hide();


    int numberofHandsSaidWon;
    if(ui->lineEdit->text().length()>0)
        numberofHandsSaidWon=ui->lineEdit->text().toInt();
    else numberofHandsSaidWon=0;

    if(User->get_client()!=nullptr){
        QByteArray information;
        QDataStream out(&information,QIODevice::WriteOnly);
        out<<'n'<<numberofHandsSaidWon;
        User->get_client()->writeInformation(information);
    }
    else if(User->get_server()!=nullptr)
        User->get_server()->setNumberOfHandsServerPlayerSaidWons(numberofHandsSaidWon);
}

//void PlayWindow::on_pushButton_clicked()
//{
//    sentNumberOfHandsSaidWon();
//}

