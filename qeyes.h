#ifndef QEYES_H
#define QEYES_H

#include <QWidget>
#include <QProcess>
#include <QWindow>
#include <QVBoxLayout>
#include <QTimer>
#include <QPointer>

class QEyes : public QWidget
{
    Q_OBJECT

public:
    QEyes(QWidget *parent = 0);
    ~QEyes();

public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private slots:
    void startXeyesProcess();
    void onXeyesProcessError(QProcess::ProcessError err);
    void embedXeyesWindow();

private:
    void initConnection();
    bool xeyesCmdExists();
    WId getXeyesWID();
    void removeContainer();

private:
    QVBoxLayout *m_mainVBoxLayout;
    QProcess *m_xeyesProcess;
    QPointer<QWidget> m_container;
    QTimer *m_embedDelayTimer;
};

#endif // QEYES_H
