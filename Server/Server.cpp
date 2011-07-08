#include "Server.h"

Server::Server() :
	client(0),
	serveur(new QTcpServer(this))
{
	if (!serveur->listen(QHostAddress::Any, 31337))
	{
		qDebug() << "Echec démarrage serveur :\n" << serveur->errorString();
	}
	else
	{
		qDebug() << "Serveur lancé !";
		connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
	}

	tailleMessage = 0;
}

Server::~Server()
{
}

void Server::nouvelleConnexion()
{
	qDebug() << "Connexion d'un client";
	if (client == 0)
	{
		client = serveur->nextPendingConnection();
		connect(client, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
		connect(client, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
	}
	else
	{
		qDebug() << "c'est ballot, y'a déjà quelqu'un...";
	}
}

void Server::donneesRecues()
{
    // 1 : on reçoit un paquet (ou un sous-paquet) d'un des clients

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    //QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	QTcpSocket *socket = client;
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    // Si tout va bien, on continue : on récupère le message
    QDataStream in(client);

    if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas reçu la taille du message en entier
             return;

        in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère
    }

    // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
    if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode
        return;


    // Si ces lignes s'exécutent, c'est qu'on a reçu tout le message : on peut le récupérer !
    QString message;
    in >> message;


    // 2 : on renvoie le message à tous les clients
    qDebug() << message;

    // 3 : remise de la taille du message à 0 pour permettre la réception des futurs messages
    tailleMessage = 0;

}

void Server::deconnexionClient()
{

}