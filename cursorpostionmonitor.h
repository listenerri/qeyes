#ifndef CURSORPOSTIONMONITOR_H
#define CURSORPOSTIONMONITOR_H

#include <QObject>

class CursorPostionMonitor : public QObject
{
    Q_OBJECT
public:
    explicit CursorPostionMonitor(QObject *parent = nullptr);

signals:
    void failed();
    void cursorMoved(const QPoint &newPos);

public slots:
    void startMonitor();
};

#endif // CURSORPOSTIONMONITOR_H
