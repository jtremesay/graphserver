#pragma once

#include <QDebug>
#include <QObject>
#include <QtNetwork>

#include "IManager.h"

class Server :
	public QObject
{
	    Q_OBJECT

public:
	Server();
	~Server();


private slots:
	void nouvelleConnexion();
	void donneesRecues();
	void deconnexionClient();

private:
	QTcpServer *serveur;
	QTcpSocket *client;
	quint16 tailleMessage;



};
