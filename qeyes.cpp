#include "qeyes.h"

#include <QLabel>
#include <QApplication>
#include <QThread>
#include <QHBoxLayout>
#include <QPainter>

#define MyWidth 80
#define MyHeight 60
#define MyPadding 5

QEyes::QEyes(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_monitor(new CursorPostionMonitor)
    , m_thread(new QThread(this))
{
    setFixedSize(MyWidth, MyHeight);

    m_label->setText("hello");

    QHBoxLayout *hbLayout = new QHBoxLayout(this);
    hbLayout->addWidget(m_label);
    setLayout(hbLayout);

    m_monitor->moveToThread(m_thread);

    connect(m_monitor, &CursorPostionMonitor::failed, this, [=] {
        qApp->exit(-1);
    });
    connect(m_monitor, &CursorPostionMonitor::cursorMoved, this, &QEyes::onCursorMoved);
    connect(m_thread, &QThread::started, m_monitor, &CursorPostionMonitor::startMonitor);

    m_thread->start();
}

QEyes::~QEyes()
{

}

void QEyes::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.fillRect(rect(), Qt::red);

    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    QRect rLeft = rect();
    rLeft.setRight(rLeft.width() / 2);
    rLeft = rLeft.marginsRemoved(QMargins(MyPadding, MyPadding, MyPadding, MyPadding));
    QPainterPath pathLeft;
    pathLeft.addEllipse(rLeft);
    painter.fillPath(pathLeft, Qt::white);
    painter.drawEllipse(rLeft);


    QRect rRight = rect();
    rRight.setLeft(rRight.width() / 2);
    rRight = rRight.marginsRemoved(QMargins(MyPadding, MyPadding, MyPadding, MyPadding));
    QPainterPath pathRight;
    pathRight.addEllipse(rRight);
    painter.fillPath(pathRight, Qt::white);
    painter.drawEllipse(rRight);
}

void QEyes::onCursorMoved(const QPoint &newPos)
{
    m_cursorPos = newPos;
    m_label->setText(QString("x:%1 y:%2").arg(newPos.x()).arg(newPos.y()));
    update();
}
