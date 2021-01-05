#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	centralWidget = new QStackedWidget();

	weeklyView = new WeeklyView();

	centralWidget->addWidget(weeklyView->getWidget());

	monthlyView = new MonthlyView();
	centralWidget->addWidget(monthlyView->getWidget());

	sidebar = new Sidebar();
	centralWidget->addWidget(sidebar->getWidget());

//	centralWidget->setCurrentWidget(sidebar->getWidget());
	setCentralWidget(centralWidget);

	QObject::connect(weeklyView->getMenuBtn(),SIGNAL(pressed()),
					 this, SLOT(showSideBar()));
	QObject::connect(monthlyView->getMenuBtn(),SIGNAL(pressed()),
					 this, SLOT(showSideBar()));
//	QObject::connect(sidebar,SIGNAL(exit),
//					 this, SLOT(showPreviousWidget()));
}

MainWindow::~MainWindow() {
	delete centralWidget;
	delete weeklyView;
	delete monthlyView;
}

void MainWindow::showSideBar(){
	previous = centralWidget->currentWidget();
	centralWidget->setCurrentWidget(sidebar->getWidget());
}

void MainWindow::showPreviousWidget(){
	centralWidget->setCurrentWidget(previous);
}

void MainWindow::setDatabase(Database *d){
	db = d;
}

QStackedWidget * MainWindow::getWidget(){return centralWidget;}
