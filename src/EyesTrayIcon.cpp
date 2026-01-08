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
    
    // Connect activation signal to handle both left and right click to show menu
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &EyesTrayIcon::onTrayIconActivated);
    
    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &EyesTrayIcon::updateEyes);
    m_timer->start(50);
    
    updateEyes();
    m_trayIcon->show();
}

void EyesTrayIcon::updateEyes()
{
    QPoint cursorPos = QCursor::pos();
    
    // Get the actual position of the tray icon
    QRect trayGeometry = m_trayIcon->geometry();
    QPoint trayCenter = trayGeometry.center();
    
    // If tray icon position is invalid, use bottom-right corner of screen as default
    if (trayGeometry.isNull() || !trayGeometry.isValid())
    {
        QScreen *screen = QApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        trayCenter = QPoint(screenGeometry.right() - 50, screenGeometry.bottom() - 50);
    }
    
    QPixmap pixmap(128, 128);
    pixmap.fill(Qt::transparent);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QPointF leftEyeCenter(34, 64);
    QPointF rightEyeCenter(94, 64);
    
    drawEye(&painter, leftEyeCenter, cursorPos, trayCenter);
    drawEye(&painter, rightEyeCenter, cursorPos, trayCenter);
    
    painter.end();
    
    m_trayIcon->setIcon(QIcon(pixmap));
}

void EyesTrayIcon::drawEye(QPainter *painter, const QPointF &eyePos, const QPoint &cursorPos, const QPoint &eyeCenter)
{
    const double eyeRadiusX = 24.0;
    const double eyeRadiusY = 34.0;
    const double pupilRadius = 15.0;
    
    // Draw sclera (white of the eye)
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::black, 5.0));
    painter->drawEllipse(eyePos, eyeRadiusX, eyeRadiusY);
    
    // Calculate direction from eye to mouse cursor
    double dx = cursorPos.x() - eyeCenter.x();
    double dy = cursorPos.y() - eyeCenter.y();
    double distance = std::sqrt(dx * dx + dy * dy);
    
    QPointF pupilPos = eyePos;
    if (distance > 0.1)
    {
        // Normalize direction vector
        double dirX = dx / distance;
        double dirY = dy / distance;
        
        // Maximum pupil offset in elliptical eye
        // For elliptical eyes, the maximum movement distance differs in different directions
        // Use ellipse parametric equations to calculate maximum offset
        double maxOffsetX = eyeRadiusX - pupilRadius - 2.0; // Leave some margin
        double maxOffsetY = eyeRadiusY - pupilRadius - 2.0; // Leave some margin
        
        // Calculate pupil position constrained by ellipse
        // Use scaling factor to ensure pupil stays within the sclera
        double offsetX = dirX * maxOffsetX;
        double offsetY = dirY * maxOffsetY;
        
        // Constrain pupil inside ellipse
        // Using ellipse equation: (x/a)^2 + (y/b)^2 <= 1
        double scale = std::sqrt((offsetX * offsetX) / (maxOffsetX * maxOffsetX) + 
                                  (offsetY * offsetY) / (maxOffsetY * maxOffsetY));
        if (scale > 1.0)
        {
            offsetX /= scale;
            offsetY /= scale;
        }
        
        pupilPos.setX(eyePos.x() + offsetX);
        pupilPos.setY(eyePos.y() + offsetY);
    }
    
    // Draw pupil
    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(pupilPos, pupilRadius, pupilRadius);
    
    // Draw highlight
    painter->setBrush(Qt::white);
    painter->drawEllipse(QPointF(pupilPos.x() - 5, pupilPos.y() - 5), 4, 4);
}

void EyesTrayIcon::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    // Show menu when left-clicking or middle-clicking on tray icon
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::MiddleClick)
    {
        m_trayIcon->contextMenu()->popup(QCursor::pos());
    }
}
