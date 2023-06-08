#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    clientSocket = new QTcpSocket(this);
    in.setDevice(clientSocket);      //Device: podría ser una impresora, una consola
    connect(ui->btn_close,
            &QAbstractButton::clicked,
            this, //por definición es un puntero
            &QWidget::close
            );
    connect(ui->btn_connect,
            &QAbstractButton::clicked,
            this, //por definición es un puntero
            &Client::requestMessage);
    connect(clientSocket,
            &QIODevice::readyRead, //esta es la señal que recibe el mensaje del servidory está listo para leer
            this,
            &Client::readMessage);
}

Client::~Client()
{
    delete ui;
}

void Client::requestMessage()
{
    qInfo() <<"requestMessage() ...";
    QString ipAddress = ui->le_ip->text();
    QString port = ui->le_port->text();
    qInfo() << "ip: " +ipAddress + ", port: " + port;
    clientSocket->connectToHost(ipAddress,port.toInt());
}

void Client::readMessage()
{
    qInfo() <<"readMessage() ...";
    in.startTransaction(); //inicia la lectura (transacción)
    QString message;
    in >> message; //guarda en memoria (buffer) el mensaje
    //un buffer es el contenedor donde se almacenan trocitos de un programa muy grande
    //no se puede almacenar un trozo más grande que lo que el buffer soporta
    //devuelve true (si se logró almacenar en buffer)
    //or false (si excedió el tamaño de buffer)
    if(!in.commitTransaction()){
        qInfo() << "Error in data stream";
        return;
    }
    ui->lbl_message->setText(message);
}

