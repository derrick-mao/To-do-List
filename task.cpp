#include "task.h"

QString Task::getTitle() const {
    return title;
}

QString Task::getDescription() const {
    return description;
}

QDate Task::getDueDate() const {
    return dueDate;
}

QString Task::getPriority() const {
    return priority;
}

void Task::setTitle(const QString& newTitle) {
    title = newTitle;
}

void Task::setDescription(const QString& newDescription) {
    description = newDescription;
}

void Task::setDueDate(const QDate& newDueDate) {
    dueDate = newDueDate;
}

void Task::setPriority(const QString& newPriority) {
    priority = newPriority;
}
