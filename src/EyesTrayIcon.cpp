#include "EyesTrayIcon.h"
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QCursor>
#include <QScreen>
#include <QApplication>
#include <QIcon>
#include <QPixmap>
#include <cmath>

EyesTrayIcon::EyesTrayIcon(QObject *parent) : QObject(parent)
{
    m_trayIcon = new QSystemTrayIcon(this);
    
    QMenu *trayMenu = new QMenu();
    QAction *quitAction = trayMenu->addAction("Exit");
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    m_trayIcon->setContextMenu(trayMenu);
    m_trayIcon->setToolTip("QEyes - Eyes Follow Cursor");
    
    // 连接激活信号，支持左键和右键点击都显示菜单
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &EyesTrayIcon::onTrayIconActivated);
    
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &EyesTrayIcon::updateEyes);
    m_timer->start(50);
    
    updateEyes();
    m_trayIcon->show();
}

void EyesTrayIcon::updateEyes()
{
    QPoint cursorPos = QCursor::pos();
    
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    
    QPoint eyeCenter(screenGeometry.right() - 50, screenGeometry.bottom() - 50);
    
    QPixmap pixmap(64, 64);
    pixmap.fill(Qt::transparent);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QPointF leftEyeCenter(20, 32);
    QPointF rightEyeCenter(44, 32);
    
    drawEye(&painter, leftEyeCenter, cursorPos, eyeCenter);
    drawEye(&painter, rightEyeCenter, cursorPos, eyeCenter);
    
    painter.end();
    
    m_trayIcon->setIcon(QIcon(pixmap));
}

void EyesTrayIcon::drawEye(QPainter *painter, const QPointF &eyePos, const QPoint &cursorPos, const QPoint &eyeCenter)
{
    const double eyeRadius = 10.0;
    const double pupilRadius = 5.0;
    const double maxPupilOffset = eyeRadius - pupilRadius;
    
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::black, 1.5));
    painter->drawEllipse(eyePos, eyeRadius, eyeRadius);
    
    double dx = cursorPos.x() - eyeCenter.x();
    double dy = cursorPos.y() - eyeCenter.y();
    double distance = std::sqrt(dx * dx + dy * dy);
    
    QPointF pupilPos = eyePos;
    if (distance > 0.1)
    {
        double dirX = dx / distance;
        double dirY = dy / distance;
        
        double offset = std::min(maxPupilOffset, distance / 20.0);
        
        pupilPos.setX(eyePos.x() + dirX * offset);
        pupilPos.setY(eyePos.y() + dirY * offset);
    }
    
    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(pupilPos, pupilRadius, pupilRadius);
    
    painter->setBrush(Qt::white);
    painter->drawEllipse(QPointF(pupilPos.x() - 1.5, pupilPos.y() - 1.5), 1.5, 1.5);
}

void EyesTrayIcon::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    // 当左键单击或中键点击托盘图标时显示菜单
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::MiddleClick)
    {
        m_trayIcon->contextMenu()->popup(QCursor::pos());
    }
}
