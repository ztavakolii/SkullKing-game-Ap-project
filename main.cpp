#include "register_login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    register_login w(&a);
    w.show();
    return a.exec();
}
