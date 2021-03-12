#include "database.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("mberkayk");
    QCoreApplication::setApplicationName("Budget-App");

    QFile styleFile(":/style/stylesheet.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    Database db;
    MainWindow w(&db);
    w.show();

    return a.exec();
}
