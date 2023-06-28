QT       += core gui
QT       +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    editprofilewindow.cpp \
    buy.cpp \
    game.cpp \
    historywindow.cpp \
    login.cpp \
    main.cpp \
    person.cpp \
    personalwindow.cpp \
    player.cpp \
    register_login.cpp \
    registeraccount.cpp \
    server.cpp \
    serverorclientwindow.cpp

HEADERS += \
    client.h \
    editprofilewindow.h \
    buy.h \
    game.h \
    historywindow.h \
    login.h \
    person.h \
    personalwindow.h \
    player.h \
    register_login.h \
    registeraccount.h \
    server.h \
    serverorclientwindow.h

FORMS += \
    client.ui \
    editprofilewindow.ui \
    historywindow.ui \
    buy.ui \
    login.ui \
    personalwindow.ui \
    register_login.ui \
    registeraccount.ui \
    server.ui \
    serverorclientwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
   Images.qrc \
   sounds.qrc
