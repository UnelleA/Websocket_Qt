#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();

private:
    Ui::MainWindow *ui;
    QWebSocketServer *webSocketServer;
    QList<QWebSocket *> clients;

    void broadcastMessage(const QString &message, QWebSocket *excludeClient = nullptr);
    void updateClientList();
};

#endif // MAINWINDOW_H
