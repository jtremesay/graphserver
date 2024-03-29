#include "Server.h"

Server::Server() :
	client(0),
	serveur(new QTcpServer(this))
{
	if (!serveur->listen(QHostAddress::Any, 31337))
	{
		qDebug() << "Echec d�marrage serveur :\n" << serveur->errorString();
	}
	else
	{
		qDebug() << "Serveur lanc� !";
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
		qDebug() << "c'est ballot, y'a d�j� quelqu'un...";
	}
}

void Server::donneesRecues()
{
    // 1 : on re�oit un paquet (ou un sous-paquet) d'un des clients

    // On d�termine quel client envoie le message (recherche du QTcpSocket du client)
    //QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	QTcpSocket *socket = client;
    if (socket == 0) // Si par hasard on n'a pas trouv� le client � l'origine du signal, on arr�te la m�thode
        return;

    // Si tout va bien, on continue : on r�cup�re le message
    QDataStream in(client);

    if (tailleMessage == 0) // Si on ne conna�t pas encore la taille du message, on essaie de la r�cup�rer
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas re�u la taille du message en entier
             return;

        in >> tailleMessage; // Si on a re�u la taille du message en entier, on la r�cup�re
    }

    // Si on conna�t la taille du message, on v�rifie si on a re�u le message en entier
    if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout re�u, on arr�te la m�thode
        return;


    // Si ces lignes s'ex�cutent, c'est qu'on a re�u tout le message : on peut le r�cup�rer !
    QString message;
    in >> message;


    // 2 : on renvoie le message � tous les clients
    qDebug() << message;

    // 3 : remise de la taille du message � 0 pour permettre la r�ception des futurs messages
    tailleMessage = 0;

}

void Server::deconnexionClient()
{

}