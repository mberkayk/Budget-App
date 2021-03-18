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
    QObject::connect(monthlyView, SIGNAL(weeklyBudgetChangedSignal(int)),
                     this,SLOT(weeklyBudgetChangedSlot(int)));
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

void MainWindow::weeklyBudgetChangedSlot(int b){
    double dailyBudget = b/31.00;
    double weeklyBudget = dailyBudget*7;
//    weeklyView->setBudget((int)weeklyBudget);

    //update every week's budget for this month
    QDate date = QDate::currentDate().addDays(1-QDate::currentDate().day()); //first day of the month
    for(int i = 0; i <  date.daysInMonth(); i++){ //check everyday to see if it's a monday
        QDate d = date.addDays(i);
        if(d.dayOfWeek() == 1){
            db->setWeeklyBudget(d, (int)weeklyBudget);
        }
    }
    db->saveWeekDataToFile();
    weeklyView->loadFromDatabase();
}

QStackedWidget * MainWindow::getWidget(){return stackedWidget;}
