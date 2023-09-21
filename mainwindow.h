#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include "tasklist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ignore these error messages, only for high level apps
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_completeButton_clicked();

private:
    Ui::MainWindow *ui;
    TaskList todaysTasks;
    TaskList upcomingTasks;
    TaskList completedTasks;
    int determineTab() const;
};
#endif // MAINWINDOW_H
