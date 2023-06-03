#include "qtserver.h"
#include "ui_qtserver.h"

QtServer::QtServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QtServer)
{
    ui->setupUi(this);
    initServer(); //iniciar el servidor
    //señal es la solicitud y el slot es la respuesta a esa solicitud
    //la solicitud por lo general no lo definimos, pero los slots sí
    connect(ui->btn_close, //primer objeto, boton para cerrar
            &QAbstractButton::clicked, // Señal: acción de clickeo que pide cerrar
            this, //segundo objeto de la conección, este objeto, sus slots y además sus heredados
            &QWidget::close //Slot: hereda la funcionalidad de cerrar de la clase Qwidget
            );
    connect(serverSocket,
            &QTcpServer::newConnection, //Señal: estoy recibiendo un pedido de nueva conección
            this,
            &QtServer::sendRandomMessage
            ); //Slot: voy a enviar un mensaje
    // función para inicializar el servidor
    // fill the vector
    for(int i=1;i<=50;i++){
        randomMessages << "Mensaje aleatorio " + QString::number(i);
    }
    // qInfo() << randomMessage; //función para verificar en consola
}

QtServer::~QtServer()
{
    delete ui;
}

void QtServer::sendRandomMessage() //slot que va a enviar el mensaje
{
    qInfo() << "sendRandomMessage() ...";
    //Stream and channel
    QByteArray qByteArray;
    QDataStream qDataStream(&qByteArray, QIODevice::WriteOnly); //en ese stream de ese canal solo voy a escribir
    // random message
    QString randomMessage = randomMessages[
        QRandomGenerator::global()->bounded(randomMessages.size())];
    //write to label
    ui->lbl_message->setText(randomMessage);
    qDataStream << randomMessage;
    // enviarlo
    QTcpSocket * clientSocket = serverSocket->nextPendingConnection();
    connect(clientSocket, &QAbstractSocket::disconnected //cuando está desconectado
            , clientSocket, &QObject::deleteLater //que se borre luego de desconectarse
            );
    clientSocket->write(qByteArray); //lo escribe en el stream
    clientSocket->disconnectFromHost(); //acá desconectamos del host
}

void QtServer::initServer()
{
    qInfo() <<"initServer()";
    serverSocket = new QTcpServer(this); //inicializa el socket
    if(!serverSocket ->listen()){
        qInfo() << "Server error: " + serverSocket->errorString();
        return;
    }
    QString port = QString::number(serverSocket->serverPort(),10); //guardamos el puerto numérico en un string
    QString ipAddress;
    //todas las direcciones que se encuentren en esta máquina van a esa lista
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    //qInfo() << ipAddressList;
    for(int i=0;i <ipAddressList.length();i++){
        //no queremos localhost pues no sirve para conectar diferentes máquinas
        if(ipAddressList.at(i) != QHostAddress::LocalHost and
            ipAddressList.at(i).toIPv4Address()//si es ipV6 no se convierte a ipV4 y da falso
            and ipAddressList.at(i).toString() != "172.29.16.1" //acá omito la conexión del WSL
            //and ipAddressList.at(i).isLoopback() //acá debería ir el filtro para que no aparezca el WSL
            ){
            ipAddress = ipAddressList.at(i).toString();
            break;
        }
    }
    QString info = "Ip: "+ipAddress + ",port: " + port;
    qInfo() <<"Ip: "+ipAddress + ",port: " + port;
    ui->lbl_IP_Port->setText(info);
}

/*
void QtServer::on_btn_close_clicked()
{
    qDebug() << "close";
}
*/
