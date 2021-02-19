#include "database.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("mberkayk");
    QCoreApplication::setApplicationName("Budget-App");

    Database db;
    MainWindow w(&db);
    w.show();

    return a.exec();
}
