#include "tasklist.h"

// adds a task to the taskList
void TaskList::addTask(const Task& task) {
    list.append(task);
}

// removes a task from the tasklist
void TaskList::removeTask(const int index) {
    if(index >= 0 && index < list.size()) {
        list.removeAt(index);
    }
}

// as long as the index is within taskList range, a task is placed within the taskList
void TaskList::updateTask(int index, const Task &updatedTask) {
    if (index >= 0 && index < list.size()) {
        list[index] = updatedTask;
    }
}

// tasks are sorted by date, using swap function
void TaskList::sortTasksByDate() {
    int size =list.size();
    bool isSorted;

    while (!isSorted) {
        isSorted = true;
        for (int i = 0; i < size - 1; ++i) {
            if (list[i].getDueDate() > list[i + 1].getDueDate()) {
                std::swap(list[i], list[i + 1]);
                isSorted = false;
            }
        }
    }
}

void TaskList::sortTasksByPrio() {
    int size = list.size();
    bool isSorted;
    int leftTaskPrio, rightTaskPrio;
    while(!isSorted) {
        isSorted = true;
        for (int i = 0; i < size - 1; ++i) {
            leftTaskPrio = taskPrioToInt(list[i].getPriority());
            rightTaskPrio = taskPrioToInt(list[i + 1].getPriority());
            if (leftTaskPrio < rightTaskPrio) {
                std::swap(list[i], list[i + 1]);
                isSorted = false;
            }
        }
    }
}

// convert string priority to an integer
int TaskList::taskPrioToInt(const QString& priority) {
    if (priority == "High") {
        return 3;
    }

    else if (priority == "Medium") {
        return 2;
    }

    else if (priority == "Low") {
        return 1;
    }

    return -1;
}

// returns task at passed in index
Task TaskList::getTask(int index) const {
    return list.at(index);
}

// returns index of passed in task
int TaskList::getTaskIndex(const Task& task) {
    int size = list.size();
    for (int i = 0; i < size; ++i) {
        if (task.getTitle() == list[i].getTitle() &&
            task.getDescription() == list[i].getDescription() &&
            task.getDueDate() == list[i].getDueDate() &&
            task.getPriority() == list[i].getPriority()) {
            return i;
        }
    }

    return -1;
}

// returns the taskList size
int TaskList::getTasksSize() const {
    return list.size();
}

