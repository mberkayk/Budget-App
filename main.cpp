#include "database.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Database db;
	MainWindow w;
	w.setDatabase(&db);
	w.show();

	return a.exec();
}
