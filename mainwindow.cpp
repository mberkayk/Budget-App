#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	wv = new WeeklyView();
	setCentralWidget(wv);
}

MainWindow::~MainWindow() {
	delete wv;
}

