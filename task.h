#ifndef TASK_H
#define TASK_H
#pragma once

#include <QString>
#include <QDate>

class Task {
public:
    // private attributes of object type task
private:
    QString title;
    QString description;
    QDate dueDate;
    QString priority;

public:
    // Constructor, task attributes are title, expected due date, description, priority of importance, and status of completion.
    Task(const QString& title, const QString& description, const QDate& dueDate, QString priority) :
        title(title),
        description(description),
        dueDate(dueDate),
        priority(priority) {}

    // Deconstructor
    ~Task() {
        // left empty as no allocation of memory within the constructor or setter/getter functions
    }

    // Getters
    QString getTitle() const;
    QString getDescription() const;
    QDate getDueDate() const;
    QString getPriority() const;

    // Setters
    void setTitle(const QString& newTitle);
    void setDescription(const QString& newDescription);
    void setDueDate(const QDate& newDueDate);
    void setPriority(const QString& newPriority);
};

#endif // TASK_H
