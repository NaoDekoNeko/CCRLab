#ifndef QTSERVER_H
#define QTSERVER_H

#include <QWidget>
#include <QDebug>
#include <QTcpServer>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class QtServer; }
QT_END_NAMESPACE

class QtServer : public QWidget //acá se puede ver la herencia
{
    Q_OBJECT

public:
    QtServer(QWidget *parent = nullptr);
    ~QtServer();

private:
    Ui::QtServer *ui;
    QTcpServer *serverSocket = nullptr; //maneja la comunicación, es el listener
    QVector<QString> randomMessages; //vector que almacenará los mensajes
    void initServer();


private slots:
    void sendRandomMessage();
};
#endif // QTSERVER_H
