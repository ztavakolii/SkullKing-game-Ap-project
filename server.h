#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <QByteArray>
#include <QDataStream>
#include "player.h"
#include "serverwaitwindow.h"
#include "ui_server.h"
#include "playwindow.h"


namespace Ui {
class Server;
}

class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(ServerWaitWindow* waitwindow,QString servername,int numberofclients,QWidget *parent = nullptr);
    ~Server();
    void setNumberOfConnectedClientsChangeStatus(bool status);
    bool getNumberOfConnectedClientsChangeStatus();
    int getMaxNumberOfClients();
    QHostAddress getServerIP();
    QByteArray readPlayersList();
    void serverDeleted();
    void playStarted();
    void serverWantsToStopPlay();
    void serverWantsToResumePlay();
    void serverWantsToExit();
    void serverSelectCard(QString cardCode);
    void sentExchangeRequestToClients(QByteArray information);
    void exchangeTwoCardRandomly(QString senderRequest ,QString receiverRequest);
    void setNumberOfHandsServerPlayerSaidWons(int number);


private:
    Ui::Server *ui;
    QTcpServer*server;
    QString serverName;
    int maxNumberOfClients;//this is the number of clients that server accept
    int numberOfConnectedClients;
    QHostAddress serverIP;
    std::vector<Player>players; //list of clients
    std::vector<std::thread> readingFromPlayersSocketThreads;
     std::thread*t2;// thread of gameLogicControl function
    bool numberOfConnectedClientsChangeStatus;
    std::shared_mutex mx; // this is for mangement writing in newConnectionStatus
                          //by acceptNewConnection and setNewConnectionStatus functions
    std::shared_mutex mx2;
    ServerWaitWindow*waitWindow;


    void shuffleCards(int Round);
    int determineBeginnerOfFirstHand();

protected:
    void closeEvent(QCloseEvent*event);

signals:
    void writeSignal(QByteArray information,QTcpSocket*socket);
    void readSignal(QByteArray* information,QTcpSocket*socket);
    void playersListChange();
    void playStartSignal();
    void playWindowShow();

private slots:
    void acceptNewConnection();
    void readFromPlayersocket(QTcpSocket*socket);
    void writeInPlayerSocket(QByteArray information,QTcpSocket*socket);
    void readFromSocket(QByteArray* information,QTcpSocket*socket);
    void playStartSlot();
    void gameLogicControl();

};

#endif // SERVER_H
