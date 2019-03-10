#ifndef QEYES_H
#define QEYES_H

#include "cursorpostionmonitor.h"

#include <QWidget>

class QLabel;

class QEyes : public QWidget
{
    Q_OBJECT

public:
    QEyes(QWidget *parent = 0);
    ~QEyes();

private:
    QLabel *m_label;
    CursorPostionMonitor *m_monitor;
};

#endif // QEYES_H
