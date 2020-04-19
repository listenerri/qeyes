#include "qeyes.h"

#include <QFile>
#include <QDebug>

#define MyWidth 180
#define MyHeight 140
#define XeyesFilePath "/usr/bin/xeyes"

QEyes::QEyes(QWidget *parent)
    : QWidget(parent)
    , m_mainVBoxLayout(new QVBoxLayout(this))
    , m_xeyesProcess(new QProcess(this))
    , m_container(nullptr)
    , m_embedDelayTimer(new QTimer(this))
{
    m_embedDelayTimer->setSingleShot(true);
    m_embedDelayTimer->setInterval(500);

    initConnection();

    QTimer::singleShot(0, this, &QEyes::startXeyesProcess);
}

QEyes::~QEyes()
{
    m_xeyesProcess->kill();
}

QSize QEyes::sizeHint() const
{
    return QSize(MyWidth, MyHeight);
}

void QEyes::startXeyesProcess()
{
    if (!xeyesCmdExists()) {
        qDebug() << "The xeyes cmd is not exists";
        return;
    }
    if (m_xeyesProcess->state() != QProcess::ProcessState::NotRunning) {
        qDebug() << "The xeyes process is staring or running";
        return;
    }
    QStringList args = {"-geometry", "1x2+3+4"};
    m_xeyesProcess->setProgram(XeyesFilePath);
    m_xeyesProcess->setArguments(args);
    m_xeyesProcess->start();
}

void QEyes::onXeyesProcessError(QProcess::ProcessError err)
{
    qDebug() << "Xeyes process error occurred: " << err;
    removeContainer();
}

void QEyes::embedXeyesWindow()
{
    // call timer's start function instead of call this directly
    if (sender() != m_embedDelayTimer) {
        qDebug() << "Do not call embed function directly";
        return;
    }

    const WId id = getXeyesWID();
    if (id == 0) {
        qDebug() << "Not find the xeyes WId";
        return;
    }
    removeContainer();
    QWindow *xeyesWindow = QWindow::fromWinId(id);
    m_container = QWidget::createWindowContainer(xeyesWindow, this);
    if (!m_container) {
        qDebug() << "Embed xeyes window failed";
    }
    m_mainVBoxLayout->addWidget(m_container);
}

void QEyes::initConnection()
{
    connect(m_xeyesProcess, SIGNAL(started()), m_embedDelayTimer, SLOT(start()));
    connect(m_xeyesProcess, SIGNAL(finished(int)), this, SLOT(startXeyesProcess()));
    connect(m_xeyesProcess, SIGNAL(errorOccurred(QProcess::ProcessError)),
            this, SLOT(onXeyesProcessError(QProcess::ProcessError)));
    connect(m_embedDelayTimer, SIGNAL(timeout()), this, SLOT(embedXeyesWindow()));
}

bool QEyes::xeyesCmdExists()
{
    return QFile::exists(XeyesFilePath);
}

WId QEyes::getXeyesWID()
{
    QProcess *findExeysWIdProcess = new QProcess(this);
    QStringList args = {"-root", "-tree"};
    findExeysWIdProcess->setProgram("/usr/bin/xwininfo");
    findExeysWIdProcess->setArguments(args);
    findExeysWIdProcess->start();
    findExeysWIdProcess->waitForFinished();
    const QString &data = findExeysWIdProcess->readAll();
    for (const QString &line : data.split("\n")) {
        if (line.contains("xeyes") && line.contains("1x2+0+0") && line.contains("+3+4")) {
            const QString widStr = line.trimmed().split(" ").first();
            bool ok;
            WId id = widStr.toUInt(&ok, 16);
            if (ok) {
                return id;
            }
        }
    }
    return 0;
}

void QEyes::removeContainer()
{
    if (m_container) {
        m_container->deleteLater();
        m_mainVBoxLayout->removeWidget(m_container);
    }
}
