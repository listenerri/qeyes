#include "qeyes-plugin.h"

#define EfficientHWidth 50
#define EfficientHHeight 30
#define EfficientVWidth 30
#define EfficientVHeight 50

QEyesPlugin::QEyesPlugin(QObject *parent) : QObject(parent)
{

}

const QString QEyesPlugin::pluginName() const
{
    return QStringLiteral("qeyes");
}

void QEyesPlugin::init(PluginProxyInterface *proxyInter)
{
    m_proxyInter = proxyInter;

    m_qeyes = new QEyes;
    positionChanged(position());

    m_proxyInter->itemAdded(this, pluginName());
}

QWidget *QEyesPlugin::itemWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey)
    return m_qeyes;
}

void QEyesPlugin::positionChanged(const Dock::Position position)
{
    Q_UNUSED(position)
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>" << position;
    if (displayMode() == Dock::DisplayMode::Fashion) {
        return;
    }
    m_qeyes->setPreferSize(getPreferSize());
}

QSize QEyesPlugin::getPreferSize()
{
    QSize s;
    switch (position()) {
    case Dock::Position::Top:
    case Dock::Position::Bottom:
        s.setHeight(EfficientHHeight);
        s.setWidth(EfficientHWidth);
        break;
    case Dock::Position::Left:
    case Dock::Position::Right:
        s.setHeight(EfficientVHeight);
        s.setWidth(EfficientVWidth);
        break;
    default:
        break;
    }
    return s;
}
