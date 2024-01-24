#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server_port = 5271;
    client_port = 5270;

    client = new QTcpSocket();
    server = new QTcpServer();

    client->bind(QHostAddress::AnyIPv4, client_port);
    server->listen(QHostAddress::AnyIPv4, server_port);

    connect(server, SIGNAL(newConnection()), this, SLOT(getNewConnection()));

    db = QSqlDatabase::addDatabase("QSQLITE");//открытие базы данных
    if (QFile::exists("json.db")){
        db.setDatabaseName("json.db");
        db.open();
    } else{
        ui->label->setText("Notopened");
    }

    query = new QSqlQuery(db);
}

MainWindow::~MainWindow()
{
    db.close();
    delete query;
    delete ui;
    delete server;
    delete client;
}
void MainWindow::getNewConnection()//подключаемся к серверу
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), SLOT(collect()));
    }
}
void MainWindow::receiver(){//бесполезная приколюха
}

void MainWindow::on_pushButton_clicked()//отправка данных
{
    QString key = "key";
    QJsonValue value = "value";
    QJsonObject obj;
    obj.insert(key, value);
    QJsonDocument doc(obj);
    QByteArray ar = doc.toJson();
    QHostAddress Addr = QHostAddress("26.213.232.21");
    client->connectToHost(Addr, server_port);
    client->write(ar);
    //ui->label->setText("wada");
}

void MainWindow::collect()//принятие данных
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray buf;
    while (socket->bytesAvailable() > 0)
    {
        buf.append(socket->readAll());
    }
    QJsonDocument doc = QJsonDocument::fromJson(buf);
    QJsonObject ob = doc.object();
    QString str = ob.value("type").toString();
    if (str == "RectangleShape"){
       insertrect(ob.value("x").toInt(), ob.value("y").toInt(), ob.value("width").toInt(), ob.value("height").toInt(), ob.value("OutThick").toInt(), ob.value("red").toInt(), ob.value("green").toInt(), ob.value("blue").toInt());
    }
    else {
        insertcirc(ob.value("x").toInt(), ob.value("y").toInt(), ob.value("radius").toInt(), ob.value("red").toInt(), ob.value("green").toInt(), ob.value("blue").toInt());
    }
    ui->label->setText(str);
}

void MainWindow::on_pushButton_pressed()
{
    clicked = true;
}


void MainWindow::on_pushButton_released()
{
    clicked = false;
}

void MainWindow::insertrect(int x, int y, int width, int height, int OutThick, int r, int g, int b)
{
    QString quinfo = "INSERT INTO Rect (x, y, width, height, OutlineThick, R, G, B) VALUES ('" + x + "', '" + y + "', '" + width + "', '" + height + "', '" + OutThick + "', '" + r + "', '" + g + "', '" + b + "');";
    query->exec(quinfo);
    db.commit();
}

void MainWindow::insertcirc(int x, int y, int  radius, int r, int g, int b)
{
    QString quinfo = "INSERT INTO Circle (x, y, Radius, R, G, B) VALUES ('" + x + "', '" + y + "', '" + radius + "', '" + r + "', '" + g + "', '" + b + "');";
    query->exec(quinfo);
    db.commit();
}
