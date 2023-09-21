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
    // Check if the provided index is valid
    if (index >= 0 && index < list.size()) {
        list[index] = updatedTask;
    }
}

// tasks are sorted by date, using swap function
void TaskList::sortTasksByDate() {
    int size =list.size();
    bool isSorted;

    do {
        isSorted = true;
        for (int i = 0; i < size - 1; ++i) {
            if (list[i].getDueDate() > list[i + 1].getDueDate()) {
                std::swap(list[i], list[i + 1]);
                isSorted = false;
            }
        }
    }
    while (!isSorted);
}


// returns task at given index
Task TaskList::getTask(int index) const {
    return list.at(index);
}

// displays the taskList size
int TaskList::getTasksSize() const {
    return list.size();
}

