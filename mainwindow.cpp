#include "mainwindow.h"

MainWindow::MainWindow(Database *database) : QMainWindow() {

    db = database;

    mainLayout = new QVBoxLayout;
    centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);

    stackedWidget = new QStackedWidget();
    mainLayout->addWidget(stackedWidget);
    mainLayout->setStretchFactor(stackedWidget, 9);

    weeklyView = new WeeklyView(db);
    stackedWidget->addWidget(weeklyView);

    monthlyView = new MonthlyView(db);
    stackedWidget->addWidget(monthlyView);

    settingsView = new SettingsView;
    stackedWidget->addWidget(settingsView);

    bottomBar = new BottomBar();
    mainLayout->addWidget(bottomBar);
    mainLayout->setStretchFactor(bottomBar, 1);

    setCentralWidget(centralWidget);

    //bottombar actions
    QObject::connect(bottomBar->getWeeklyViewButton(), SIGNAL(pressed()),
                     this, SLOT(showWeeklyView()));
    QObject::connect(bottomBar->getMonthlyViewButton(), SIGNAL(pressed()),
                     this, SLOT(showMonthlyView()));
    QObject::connect(bottomBar->getSettingsViewButton(), SIGNAL(pressed()),
                     this, SLOT(showSettingsView()));

    //budget signals
    QObject::connect(monthlyView, SIGNAL(budgetEdited(int)),
                     this,SLOT(budgetEdited(int)));
}

MainWindow::~MainWindow() {
    delete weeklyView;
    delete monthlyView;
}

void MainWindow::showMonthlyView(){
    stackedWidget->setCurrentWidget(monthlyView);
}

void MainWindow::showWeeklyView(){
    stackedWidget->setCurrentWidget(weeklyView);
}

void MainWindow::showSettingsView(){
    stackedWidget->setCurrentWidget(settingsView);
}

void MainWindow::budgetEdited(int b){
    double dailyBudget = b/31;
    double weeklyBudget = dailyBudget*7;
    weeklyView->setBudget((int)weeklyBudget);
    //TODO: update every week's budget for this month
}

QStackedWidget * MainWindow::getWidget(){return stackedWidget;}
