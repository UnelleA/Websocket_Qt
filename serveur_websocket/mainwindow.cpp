#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    webSocketServer(new QWebSocketServer(QStringLiteral("Chat Server"), QWebSocketServer::NonSecureMode, this)) {
    ui->setupUi(this);

    // Écoute sur le port 12345
    if (webSocketServer->listen(QHostAddress::Any, 149)) {
        ui->statusLabel->setText("Serveur démarré");
        connect(webSocketServer, &QWebSocketServer::newConnection, this, &MainWindow::onNewConnection);
    } else {
        ui->statusLabel->setText("Échec du démarrage du serveur !");
    }
}

MainWindow::~MainWindow() {
    webSocketServer->close();
    qDeleteAll(clients.begin(), clients.end());
    delete ui;
}

// Nouvelle connexion d'un client
void MainWindow::onNewConnection() {
    QWebSocket *clientSocket = webSocketServer->nextPendingConnection();

    clients.append(clientSocket);
    ui->statusLabel->setText("Nouveau client connecté");

    // Ajouter le client à la liste des clients connectés
    ui->clientsListWidget->addItem(clientSocket->peerAddress().toString());

    connect(clientSocket, &QWebSocket::textMessageReceived, this, &MainWindow::processMessage);
    connect(clientSocket, &QWebSocket::disconnected, this, &MainWindow::socketDisconnected);
}

// Traitement des messages reçus
void MainWindow::processMessage(QString message) {
    QWebSocket *senderSocket = qobject_cast<QWebSocket *>(sender());
    if (!senderSocket) return;

    // Ajouter un horodatage au message
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject json = doc.object();
    json["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QString broadcastMsg = QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Compact));

    // Afficher le message dans l'interface
    ui->messagesTextEdit->append(broadcastMsg);

    // Diffuser le message à tous les clients
    broadcastMessage(broadcastMsg);
}

// Déconnexion d'un client
void MainWindow::socketDisconnected() {
    QWebSocket *clientSocket = qobject_cast<QWebSocket *>(sender());
    if (clientSocket) {
        clients.removeAll(clientSocket);

        // Supprimer le client de la liste des clients connectés
        QList<QListWidgetItem *> items = ui->clientsListWidget->findItems(clientSocket->peerAddress().toString(), Qt::MatchExactly);
        for (QListWidgetItem *item : items) {
            delete ui->clientsListWidget->takeItem(ui->clientsListWidget->row(item));
        }

        clientSocket->deleteLater();
        ui->statusLabel->setText("Client déconnecté");
    }
}

// Diffusion du message à tous les clients
void MainWindow::broadcastMessage(const QString &message, QWebSocket *excludeClient) {
    for (QWebSocket *client : qAsConst(clients)) {
        if (client != excludeClient) {
            client->sendTextMessage(message);
        }
    }
}
