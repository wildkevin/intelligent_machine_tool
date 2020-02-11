#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>


class Context : public QObject
{
    Q_OBJECT
public:
    explicit Context(QObject *parent = nullptr);
    QString xmlFile="p.xml";

signals:

public slots:
};

#endif // CONTEXT_H
