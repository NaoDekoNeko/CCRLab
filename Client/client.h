#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private:
    Ui::Client *ui;
    QTcpSocket *clientSocket = nullptr; //iniciar el socket de cliente, maneja la comunicación
    QDataStream in; //donde vamos a manejar el dato que está llegando, se guardan los datos
    QString receivedMessage; //almacena el mensaje recibido
private slots:
    void requestMessage(); //solicita mensaje
    void readMessage(); //lee el mensaje
};
#endif // CLIENT_H
