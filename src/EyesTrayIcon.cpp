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
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &EyesTrayIcon::updateEyes);
    m_timer->start(50);
    
    updateEyes();
    m_trayIcon->show();
}

void EyesTrayIcon::updateEyes()
{
    QPoint cursorPos = QCursor::pos();
    
    // 获取托盘图标的实际位置
    QRect trayGeometry = m_trayIcon->geometry();
    QPoint trayCenter = trayGeometry.center();
    
    // 如果托盘图标位置无效，使用屏幕右下角作为默认位置
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
    
    // 绘制眼白
    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::black, 5.0));
    painter->drawEllipse(eyePos, eyeRadiusX, eyeRadiusY);
    
    // 计算从眼睛到鼠标的方向
    double dx = cursorPos.x() - eyeCenter.x();
    double dy = cursorPos.y() - eyeCenter.y();
    double distance = std::sqrt(dx * dx + dy * dy);
    
    QPointF pupilPos = eyePos;
    if (distance > 0.1)
    {
        // 归一化方向向量
        double dirX = dx / distance;
        double dirY = dy / distance;
        
        // 椭圆眼睛中瞳孔的最大偏移量
        // 对于椭圆形眼睛，瞳孔在不同方向上的最大移动距离不同
        // 使用椭圆的参数方程来计算最大偏移
        double maxOffsetX = eyeRadiusX - pupilRadius - 2.0; // 留一点边距
        double maxOffsetY = eyeRadiusY - pupilRadius - 2.0; // 留一点边距
        
        // 计算瞳孔在椭圆约束下的位置
        // 使用缩放因子来确保瞳孔不会超出眼白边界
        double offsetX = dirX * maxOffsetX;
        double offsetY = dirY * maxOffsetY;
        
        // 限制瞳孔在椭圆内部
        // 使用椭圆方程: (x/a)^2 + (y/b)^2 <= 1
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
    
    // 绘制瞳孔
    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(pupilPos, pupilRadius, pupilRadius);
    
    // 绘制高光
    painter->setBrush(Qt::white);
    painter->drawEllipse(QPointF(pupilPos.x() - 5, pupilPos.y() - 5), 4, 4);
}

void EyesTrayIcon::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    // 当左键单击或中键点击托盘图标时显示菜单
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::MiddleClick)
    {
        m_trayIcon->contextMenu()->popup(QCursor::pos());
    }
}
