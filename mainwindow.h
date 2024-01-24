#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QtNetwork>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtSql>
#include <QsqlDatabase>
#include <QSqlQuery>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void receiver();
    void on_pushButton_clicked();
    void collect();
    void on_pushButton_pressed();
    void getNewConnection();
    void on_pushButton_released();
    void insertrect(int x, int y, int width, int height, int OutThick, int r, int g, int g);
    void insertcirc(int x, int y, int radius, int r, int g, int g);
private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *client;
    QSqlDatabase db;
    QSqlQuery *query;
    int client_port, server_port;
    bool clicked = false;
};
#endif // MAINWINDOW_H
