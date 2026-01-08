#ifndef EYESTRAYICON_H
#define EYESTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>

class QPainter;
class QPoint;
class QPointF;

class EyesTrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit EyesTrayIcon(QObject *parent = nullptr);

private slots:
    void updateEyes();

private:
    void drawEye(QPainter *painter, const QPointF &eyePos, const QPoint &cursorPos, const QPoint &eyeCenter);

    QSystemTrayIcon *m_trayIcon;
    QTimer *m_timer;
};

#endif // EYESTRAYICON_H
