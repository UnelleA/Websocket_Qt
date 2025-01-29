# 💬 Application de Chat avec WebSockets et Qt

## 📝 Description

Ce projet consiste à développer une **application de chat en temps réel** utilisant le **protocole WebSocket** pour permettre une communication fluide entre un **serveur en Qt** et un **client en JavaScript**. Contrairement aux versions précédentes utilisant TCP ou UDP, cette implémentation permet une **interopérabilité** accrue, un **échange de messages en JSON**, et une meilleure **extensibilité** du protocole.

L’application est composée de **deux parties distinctes** :
- **Le Serveur (Qt)** : Gère les connexions, reçoit et retransmet les messages aux utilisateurs connectés.
- **Le Client (HTML/JavaScript)** : Interface web permettant aux utilisateurs de se connecter, d’envoyer et de recevoir des messages.

---

## 🛠️ Technologies utilisées

- **Qt Framework** : Pour la création du serveur WebSocket.
- **C++ (Qt WebSockets)** : Gestion du serveur WebSocket avec `QWebSocketServer` et `QWebSocket`.
- **HTML / CSS** : Interface utilisateur simple et intuitive.
- **JavaScript** : Gestion des connexions WebSocket côté client.
