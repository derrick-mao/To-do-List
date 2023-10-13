#ifndef TASKLIST_H
#define TASKLIST_H
#pragma once

#include <QList>
#include "task.h"

class TaskList {
private:
    QList<Task> list;
    int taskPrioToInt(const QString& priority);

public:
    void addTask(const Task& task);
    void removeTask(int index);
    void updateTask(int index, const Task &updatedTask);

    //sorters
    void sortTasksByDate();
    void sortTasksByPrio();

    //getters
    Task getTask(int) const;
    int getTaskIndex(const Task& task);
    int getTasksSize() const;
};

#endif // TASKLIST_H
