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

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onCursorMoved(const QPoint &newPos);

private:
    QLabel *m_label;
    CursorPostionMonitor *m_monitor;
    QThread *m_thread;

    QPoint m_cursorPos;
};

#endif // QEYES_H
