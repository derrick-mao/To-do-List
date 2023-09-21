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
    // creates variables to hold user inputs
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

    // stops the function if the date is lower than the current date
    if(date < QDate::currentDate()) {
        QMessageBox msgBox;
        msgBox.setText("Error: Cannot add task with before today.");
        msgBox.exec();
        return;
    }

    // creates a Task given the user inputs
    Task newTask(title, desc, date, priority);

    // Format the task for display and add it to the list widget
    QString taskDisplay = QString("%1 - %2: %3      %4").arg(newTask.getDueDate().toString("yyyy/MM/dd"), newTask.getTitle(), newTask.getDescription(), newTask.getPriority());

    // determines whether or not to add to todaysTasks or upcomingTasks
    if (date == QDate::currentDate()) {
        // adds newTask to a list of todaysTasks
        todaysTasks.addTask(newTask);
        ui->todayList->addItem(taskDisplay);
    } else {
        ui->upcomingList->clear();
        upcomingTasks.addTask(newTask);
        upcomingTasks.sortTasksByDate();
        int size = upcomingTasks.getTasksSize();
        for (int i = 0; i < size; ++i) {
            Task upTask = upcomingTasks.getTask(i);
            taskDisplay = QString("%1 - %2: %3      %4").arg(upTask.getDueDate().toString("yyyy/MM/dd"), upTask.getTitle(), upTask.getDescription(), upTask.getPriority());
            ui->upcomingList->addItem(taskDisplay);
        }
    }
    QMessageBox msgBox;
    msgBox.setText("Task successfully added!");
    msgBox.exec();
}

// deletes tasks
void MainWindow::on_deleteButton_clicked() {
    // if the tab selected is today's list
    if (determineTab() == 0) {
        int currentIndex = ui->todayList->currentRow();
        if (currentIndex >= 0) {
            todaysTasks.removeTask(currentIndex);
            delete ui->todayList->takeItem(currentIndex);  // Remove the item from the UI.
        }
    }

    // if the tab selected is upcoming's list
    else if (determineTab() == 1) {
        int currentIndex = ui->upcomingList->currentRow();
        if (currentIndex >= 0) {
            upcomingTasks.removeTask(currentIndex);
            delete ui->upcomingList->takeItem(currentIndex);  // Remove the item from the UI.
        }
    }

    // if the tab selected is completed's list
    else {
        int currentIndex = ui->completedList->currentRow();
        if (currentIndex >= 0) {
            completedTasks.removeTask(currentIndex);
            delete ui->completedList->takeItem(currentIndex);
        }
    }
    QMessageBox msgBox;
    msgBox.setText("Task has been deleted!");
    msgBox.exec();
}

// edits the task's title
void MainWindow::on_editButton_clicked() {

    if (determineTab() == 0) {
        // Gets the index of the currently selected task in the list widget
        int currentIndex = ui->todayList->currentRow();

        // Check if a task is selected (currentIndex will be -1 if no item is selected)
        if (currentIndex >= 0) {
            // Fetches the task from our todaysTasks list using the selected index
            Task currentTask = todaysTasks.getTask(currentIndex);

            // A flag to detect if the user pressed the "OK" button in the input dialog
            bool ok;

            // Displays a dialog to get a new title for the task.
            // This dialog will have the current task's title as default value.
            QString newTitle = QInputDialog::getText(this, "Edit Task", "New Title:", QLineEdit::Normal, currentTask.getTitle(), &ok);

            // If the user pressed "OK" in the dialog and the new title is not empty
            if (ok && !newTitle.isEmpty()) {
                // Set the new title to the task object
                currentTask.setTitle(newTitle);

                // Update the task in our todaysTasks list with the new title
                todaysTasks.updateTask(currentIndex, currentTask);

                // Update the UI list item with the new title. We also display the date and description.
                QString taskDisplay = QString("%1 - %2: %3      %4").arg(currentTask.getDueDate().toString("yyyy/MM/dd"), currentTask.getTitle(), currentTask.getDescription(), currentTask.getPriority());
                ui->todayList->item(currentIndex)->setText(taskDisplay);
            }
        }
    }

    else if (determineTab() == 1) {
        // Gets the index of the currently selected task in the list widget
        int currentIndex = ui->upcomingList->currentRow();

        // Check if a task is selected (currentIndex will be -1 if no item is selected)
        if (currentIndex >= 0) {
            // Fetches the task from our todaysTasks list using the selected index
            Task currentTask = upcomingTasks.getTask(currentIndex);

            // A flag to detect if the user pressed the "OK" button in the input dialog
            bool ok;

            // Displays a dialog to get a new title for the task.
            // This dialog will have the current task's title as default value.
            QString newTitle = QInputDialog::getText(this, "Edit Task", "New Title:", QLineEdit::Normal, currentTask.getTitle(), &ok);

            // If the user pressed "OK" in the dialog and the new title is not empty
            if (ok && !newTitle.isEmpty()) {
                // Set the new title to the task object
                currentTask.setTitle(newTitle);

                // Update the task in our todaysTasks list with the new title
                upcomingTasks.updateTask(currentIndex, currentTask);

                // Update the UI list item with the new title. We also display the date and description.
                QString taskDisplay = QString("%1 - %2: %3      %4").arg(currentTask.getDueDate().toString("yyyy/MM/dd"), currentTask.getTitle(), currentTask.getDescription(), currentTask.getPriority());
                ui->upcomingList->item(currentIndex)->setText(taskDisplay);
            }
        }
    }

    else {
        QMessageBox msgBox;
        msgBox.setText("You are trying to edit a completed task");
        msgBox.exec();
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
            todaysTasks.removeTask(currentIndex);
            ui->completedList->addItem(taskDisplay);
            delete ui->todayList->takeItem(currentIndex);
            QMessageBox msgBox;
            msgBox.setText("Task has been set as completed!");
            msgBox.exec();
        }
    }

    // if a task is completed on the upcoming list
    else if (determineTab() == 1) {
        int currentIndex = ui->upcomingList->currentRow();
        if (currentIndex >= 0) {
            Task completedTask = upcomingTasks.getTask(currentIndex);
            QString taskDisplay = QString("%1 - %2: %3      %4").arg(completedTask.getDueDate().toString("yyyy/MM/dd"), completedTask.getTitle(), completedTask.getDescription(), completedTask.getPriority());
            upcomingTasks.removeTask(currentIndex);
            ui->completedList->addItem(taskDisplay);
            delete ui->upcomingList->takeItem(currentIndex);
            QMessageBox msgBox;
            msgBox.setText("Task has been set as completed!");
            msgBox.exec();
        }
    }

    // if you press complete on a completed task...?
    else {
        QMessageBox msgBox;
        msgBox.setText("This task was already completed");
        msgBox.exec();
    }
}
