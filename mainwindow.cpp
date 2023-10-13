#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>

// initializes UI interface, allocates memory, for a main window, and allows input widgets of date
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->dueDateInput->setDate(QDate::currentDate());
}

// deletes the ui screen
MainWindow::~MainWindow() {
    delete ui;
}

// returns the value of the current tab
// 0 = Today | 1 = Upcoming | 2 = Completed
int MainWindow::determineTab() const {
    return ui->tabList->currentIndex();
}


// adds actions for the "add" button when its clicked.
void MainWindow::on_addButton_clicked() {
    // variables to hold user inputs
    QString title = ui->taskInput->text();
    QString desc = ui->descriptionInput->text();
    QDate date = ui->dueDateInput->date();
    QString priority = ui->priorityInput->currentText();

    // clears the input
    ui->taskInput->clear();
    ui->descriptionInput->clear();

    // stops the function if nothing is inputted into the title
    if(title.trimmed() == "") {
        QMessageBox msgBox;
        msgBox.setText("Error: Cannot add task with no title.");
        msgBox.exec();
        return;
    }

    // stops the function if the date is before the current date
    if(date < QDate::currentDate()) {
        QMessageBox msgBox;
        msgBox.setText("Error: Cannot add task with date before today.");
        msgBox.exec();
        return;
    }

    // creates a Task given the user inputs
    Task newTask(title, desc, date, priority);

    // Format the task for display and add it to the list widget
    QString taskDisplay; // = QString("%1 - %2: %3      %4").arg(newTask.getDueDate().toString("yyyy/MM/dd"), newTask.getTitle(), newTask.getDescription(), newTask.getPriority());

    // determines whether or not to add to todaysTasks or upcomingTasks
    if (date == QDate::currentDate()) {
        // adds newTask to today list
        todaysTasks.addTask(newTask);
        taskDisplay = QString("%1 - %2: %3      %4").arg(newTask.getDueDate().toString("yyyy/MM/dd"), newTask.getTitle(), newTask.getDescription(), newTask.getPriority());
        ui->todayList->addItem(taskDisplay);
    }

    else {
        // adds newTask to upcoming list
        ui->upcomingList->clear();
        upcomingTasks.addTask(newTask);
        upcomingTasks.sortTasksByDate();
        int size = upcomingTasks.getTasksSize();

        // display the newly sorted upcomingTasks in order
        for (int i = 0; i < size; ++i) {
            Task upTask = upcomingTasks.getTask(i);
            taskDisplay = QString("%1 - %2: %3      %4").arg(upTask.getDueDate().toString("yyyy/MM/dd"), upTask.getTitle(), upTask.getDescription(), upTask.getPriority());
            ui->upcomingList->addItem(taskDisplay);
        }
    }

    // adds newTask to priority list
    ui->priorityList->clear();
    priorityTasks.addTask(newTask);
    priorityTasks.sortTasksByPrio();
    int size = priorityTasks.getTasksSize();

    // display the newly sorted priorityTasks in order
    for (int i = 0; i < size; ++i) {
        Task prioTask = priorityTasks.getTask(i);
        taskDisplay = QString("%1 - %2: %3      %4").arg(prioTask.getDueDate().toString("yyyy/MM/dd"), prioTask.getTitle(), prioTask.getDescription(), prioTask.getPriority());
        ui->priorityList->addItem(taskDisplay);
    }
}

// completes the task
void MainWindow::on_completeButton_clicked() {
    // if a task is completed on the today list
    if (determineTab() == 0) {
        int currentIndex = ui->todayList->currentRow();
        if (currentIndex >= 0) {
            Task completedTask = todaysTasks.getTask(currentIndex);
            QString taskDisplay = QString("%1 - %2: %3      %4").arg(completedTask.getDueDate().toString("yyyy/MM/dd"), completedTask.getTitle(), completedTask.getDescription(), completedTask.getPriority());

            // remove the completed task from today
            todaysTasks.removeTask(currentIndex);
            //ui->completedList->addItem(taskDisplay);
            delete ui->todayList->takeItem(currentIndex);

            // remove the completed task from priority
            int prioCurrIndex = priorityTasks.getTaskIndex(completedTask);
            priorityTasks.removeTask(prioCurrIndex);
            delete ui->priorityList->takeItem(prioCurrIndex);

            // display the completed task in completedList
            ui->completedList->addItem(taskDisplay);
        }
    }

    // if a task is completed on the upcoming list
    else if (determineTab() == 1) {
        int currentIndex = ui->upcomingList->currentRow();
        if (currentIndex >= 0) {
            Task completedTask = upcomingTasks.getTask(currentIndex);
            QString taskDisplay = QString("%1 - %2: %3      %4").arg(completedTask.getDueDate().toString("yyyy/MM/dd"), completedTask.getTitle(), completedTask.getDescription(), completedTask.getPriority());

            // remove the completed task from upcoming
            upcomingTasks.removeTask(currentIndex);
            delete ui->upcomingList->takeItem(currentIndex);

            // remove the completed task from priority
            int prioCurrIndex = priorityTasks.getTaskIndex(completedTask);
            priorityTasks.removeTask(prioCurrIndex);
            delete ui->priorityList->takeItem(prioCurrIndex);

            // display the completed task in completedList
            ui->completedList->addItem(taskDisplay);
        }
    }

    // if a task is completed on the priority list
    else if (determineTab() == 2) {
        int currentIndex = ui->priorityList->currentRow();
        if (currentIndex >= 0) {
            Task completedTask = priorityTasks.getTask(currentIndex);
            QString taskDisplay = QString("%1 - %2: %3      %4").arg(completedTask.getDueDate().toString("yyyy/MM/dd"), completedTask.getTitle(), completedTask.getDescription(), completedTask.getPriority());

            // the completed task is due today
            if (completedTask.getDueDate() == QDate::currentDate()) {
                // remove the completed task from today
                int todayCurrIndex = todaysTasks.getTaskIndex(completedTask);
                todaysTasks.removeTask(todayCurrIndex);
                delete ui->todayList->takeItem(todayCurrIndex);
            }

            // the completed task is due upcoming date
            else {
                // remove the completed task from upcoming
                int upCurrIndex = upcomingTasks.getTaskIndex(completedTask);
                upcomingTasks.removeTask(upCurrIndex);
                delete ui->upcomingList->takeItem(upCurrIndex);
            }

            // remove the completed task from priority
            priorityTasks.removeTask(currentIndex);
            delete ui->priorityList->takeItem(currentIndex);

            // display the completed task in completedList
            ui->completedList->addItem(taskDisplay);
        }
    }

    // if you press complete on a completed task
    else {
        QMessageBox msgBox;
        msgBox.setText("This task was already completed");
        msgBox.exec();
    }
}

// edits the task's title
void MainWindow::on_editButton_clicked() {

    if (determineTab() == 0) {
        // gets the index of the currently selected task in the list widget
        int currentIndex = ui->todayList->currentRow();

        // check if a task is selected (currentIndex will be -1 if no item is selected)
        if (currentIndex >= 0) {
            // fetches the task from our todaysTasks and index of same task in priorityTasks
            Task currentTask = todaysTasks.getTask(currentIndex);
            int prioCurrIndex = priorityTasks.getTaskIndex(currentTask);
            // a flag to detect if the user pressed the "OK" button in the input dialog
            bool ok;

            // displays a dialog to get a new title for the task.
            // this dialog will have the current task's title as default value.
            QString newTitle = QInputDialog::getText(this, "Edit Task", "New Title:", QLineEdit::Normal, currentTask.getTitle(), &ok);

            // if the user pressed "OK" in the dialog and the new title is not empty
            if (ok && !newTitle.isEmpty()) {
                // set the new title to the task object
                currentTask.setTitle(newTitle);

                // update the task with the new title in the tasklist and the UI list
                QString taskDisplay = QString("%1 - %2: %3      %4").arg(currentTask.getDueDate().toString("yyyy/MM/dd"), currentTask.getTitle(), currentTask.getDescription(), currentTask.getPriority());
                todaysTasks.updateTask(currentIndex, currentTask);
                ui->todayList->item(currentIndex)->setText(taskDisplay);
                priorityTasks.updateTask(prioCurrIndex, currentTask);
                ui->priorityList->item(prioCurrIndex)->setText(taskDisplay);
            }
        }
    }

    else if (determineTab() == 1) {
        // gets the index of the currently selected task in the list widget
        int currentIndex = ui->upcomingList->currentRow();

        // check if a task is selected (currentIndex will be -1 if no item is selected)
        if (currentIndex >= 0) {
            // fetches the task from our todaysTasks and index of same task in priorityTasks
            Task currentTask = upcomingTasks.getTask(currentIndex);
            int prioCurrIndex = priorityTasks.getTaskIndex(currentTask);
            // a flag to detect if the user pressed the "OK" button in the input dialog
            bool ok;

            // displays a dialog to get a new title for the task.
            // this dialog will have the current task's title as default value.
            QString newTitle = QInputDialog::getText(this, "Edit Task", "New Title:", QLineEdit::Normal, currentTask.getTitle(), &ok);

            // if the user pressed "OK" in the dialog and the new title is not empty
            if (ok && !newTitle.isEmpty()) {
                // set the new title to the task object
                currentTask.setTitle(newTitle);

                // update the task with the new title in the tasklist and the UI list
                QString taskDisplay = QString("%1 - %2: %3      %4").arg(currentTask.getDueDate().toString("yyyy/MM/dd"), currentTask.getTitle(), currentTask.getDescription(), currentTask.getPriority());
                upcomingTasks.updateTask(currentIndex, currentTask);
                ui->upcomingList->item(currentIndex)->setText(taskDisplay);
                priorityTasks.updateTask(prioCurrIndex, currentTask);
                ui->priorityList->item(prioCurrIndex)->setText(taskDisplay);
            }
        }
    }

    else if (determineTab() == 2) {
        // gets the index of the currently selected task in the list widget
        int currentIndex = ui->priorityList->currentRow();

        // check if a task is selected (currentIndex will be -1 if no item is selected)
        if (currentIndex >= 0) {
            // fetches the task from our priorityTasks list using the selected index
            Task currentTask = priorityTasks.getTask(currentIndex);
            int todayCurrIndex, upCurrIndex;

            // fetches the index of the same task with due date today
            if (currentTask.getDueDate() == QDate::currentDate()) {
                todayCurrIndex = todaysTasks.getTaskIndex(currentTask);
            }

            // fetches the index of the same task with due date upcoming
            else {
                upCurrIndex = upcomingTasks.getTaskIndex(currentTask);
            }

            // a flag to detect if the user pressed the "OK" button in the input dialog
            bool ok;

            // displays a dialog to get a new title for the task.
            // this dialog will have the current task's title as default value.
            QString newTitle = QInputDialog::getText(this, "Edit Task", "New Title:", QLineEdit::Normal, currentTask.getTitle(), &ok);

            // if the user pressed "OK" in the dialog and the new title is not empty
            if (ok && !newTitle.isEmpty()) {
                // set the new title to the task object
                currentTask.setTitle(newTitle);

                // update the task with the new title in the tasklist and the UI lists
                QString taskDisplay = QString("%1 - %2: %3      %4").arg(currentTask.getDueDate().toString("yyyy/MM/dd"), currentTask.getTitle(), currentTask.getDescription(), currentTask.getPriority());
                priorityTasks.updateTask(currentIndex, currentTask);
                ui->priorityList->item(currentIndex)->setText(taskDisplay);

                // update corresponding task in today
                if (currentTask.getDueDate() == QDate::currentDate()) {
                    todaysTasks.updateTask(todayCurrIndex, currentTask);
                    ui->todayList->item(todayCurrIndex)->setText(taskDisplay);
                }

                // update corresponding task in upcoming
                else {
                    upcomingTasks.updateTask(upCurrIndex, currentTask);
                    ui->upcomingList->item(upCurrIndex)->setText(taskDisplay);
                }
            }
        }
    }

    // if you press edit on a completed task
    else {
        QMessageBox msgBox;
        msgBox.setText("You are trying to edit a completed task");
        msgBox.exec();
    }

}

// deletes tasks
void MainWindow::on_deleteButton_clicked() {
    // if the tab selected is today's list
    if (determineTab() == 0) {
        int currentIndex = ui->todayList->currentRow();
        if (currentIndex >= 0) {
            int prioCurrIndex = priorityTasks.getTaskIndex(todaysTasks.getTask(currentIndex));

            // remove the item from the tasklists and the UI lists
            priorityTasks.removeTask(prioCurrIndex);
            todaysTasks.removeTask(currentIndex);
            delete ui->priorityList->takeItem(prioCurrIndex);
            delete ui->todayList->takeItem(currentIndex);
        }
    }

    // if the tab selected is upcoming's list
    else if (determineTab() == 1) {
        int currentIndex = ui->upcomingList->currentRow();
        if (currentIndex >= 0) {
            int prioCurrIndex = priorityTasks.getTaskIndex(upcomingTasks.getTask(currentIndex));

            // remove the item from the tasklists and the UI lists
            priorityTasks.removeTask(prioCurrIndex);
            upcomingTasks.removeTask(currentIndex);
            delete ui->priorityList->takeItem(prioCurrIndex);
            delete ui->upcomingList->takeItem(currentIndex);
        }
    }

    // if the tab selected is priority's list
    else if (determineTab() == 2) {
        int currentIndex = ui->priorityList->currentRow();
        if (currentIndex >= 0) {
            Task delTask = priorityTasks.getTask(currentIndex);

            // remove the item from the todaysTasks and UI todayList
            if (delTask.getDueDate() == QDate::currentDate()) {
                int todayCurrIndex = todaysTasks.getTaskIndex(delTask);
                todaysTasks.removeTask(todayCurrIndex);
                delete ui->todayList->takeItem(todayCurrIndex);
            }

            // remove the item from the upcomingTasks and UI upcomingList
            else {
                int upCurrIndex = upcomingTasks.getTaskIndex(delTask);
                upcomingTasks.removeTask(upCurrIndex);
                delete ui->upcomingList->takeItem(upCurrIndex);
            }

            // remove the item from priorityTasks and UI priorityList
            priorityTasks.removeTask(currentIndex);
            delete ui->priorityList->takeItem(currentIndex);
        }
    }

    // if the tab selected is completed's list
    else {
        int currentIndex = ui->completedList->currentRow();
        if (currentIndex >= 0) {
            //remove the item from completedTasks and UI completedList
            completedTasks.removeTask(currentIndex);
            delete ui->completedList->takeItem(currentIndex);
        }
    }
}
