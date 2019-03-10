#include "qeyes.h"

#include <QLabel>
#include <QApplication>

QEyes::QEyes(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(this))
    , m_monitor(new CursorPostionMonitor(this))
{
//    m_label->setText("hello");
}

QEyes::~QEyes()
{

}
