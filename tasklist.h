#ifndef TASKLIST_H
#define TASKLIST_H
#pragma once

#include <QList>
#include "task.h"

class TaskList {
private:
    QList<Task> list;

public:
    void addTask(const Task& task);
    void removeTask(int index);
    void updateTask(int index, const Task &updatedTask);
    void sortTasksByDate();
    Task getTask(int) const;
    int getTasksSize() const;
};

#endif // TASKLIST_H
