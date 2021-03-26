#include "MyClient.h"

MyClient::MyClient() : QWidget()
{
	QGridLayout* m_gridLayout = new QGridLayout(this);

	m_ipEnter = new QLineEdit("");
	m_conBtn = new QPushButton("Connect");
	m_sendBtn = new QPushButton("Send");
	m_chatLog = new QTextEdit("");
	m_inputBox = new QLineEdit("");
	m_serverIp = new QLabel("");
	m_hostBtn = new QPushButton("Start server");

	m_nNextBlockSize = 0;
	isServer = false;
	
	

	connect(m_conBtn, SIGNAL(clicked()), this, SLOT(slotConBtnClicked()));
	connect(m_ipEnter, SIGNAL(returnPressed()), this, SLOT(slotConBtnClicked()));
	connect(m_hostBtn, SIGNAL(clicked()), this, SLOT(slotHostBtnClicked()));

	connect(m_inputBox, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
	connect(m_sendBtn, SIGNAL(clicked()), this, SLOT(slotSendToServer()));

	/* Layout setup */
	m_conBtn->setMinimumSize(40, 25);
	m_ipEnter->setMinimumHeight(25);
	m_inputBox->setMaximumHeight(50);
	m_sendBtn->setMinimumSize(50, 50);
	m_chatLog->setMinimumSize(100, 100);
	
	m_chatLog->setReadOnly(true);

	m_gridLayout->addWidget(m_serverIp, 0, 0, 1, 1);
	m_gridLayout->addWidget(m_hostBtn, 0, 1, 1, 1);
	m_gridLayout->addWidget(m_ipEnter, 1, 0, 1, 1);
	m_gridLayout->addWidget(m_conBtn, 1, 1, 1, 1);
	m_gridLayout->addWidget(m_chatLog, 2, 0, 1, 2);
	m_gridLayout->addWidget(m_inputBox, 3, 0, 1, 1);
	m_gridLayout->addWidget(m_sendBtn, 3, 1, 1, 1);
}

//OK
void MyClient::slotHostBtnClicked()
{
	isServer = true;
	m_server = new QTcpServer;
	m_serverIp->setText("Server started");
	m_ipEnter->setText("Your client is server!");
	m_ipEnter->setReadOnly(true);
	if (!m_server->listen(QHostAddress::Any, mPort))
	{
		QMessageBox::critical(0, "Server Error", "Unable to start the server:" + m_server->errorString());
		m_server->close();
		return;
	}
	connect(m_server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

//OK
void MyClient::slotNewConnection()
{
	QTcpSocket* newClient = m_server->nextPendingConnection();
	mClients.push_back(newClient);
	m_chatLog->append("New user connected!");
	connect(newClient, SIGNAL(disconnected()), this, SLOT(deleteLater()));
	connect(newClient, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
	sendToClient(newClient, "Server Response: Connected!");
}

void MyClient::slotReadClient()
{
	QTcpSocket*	pClientSocket = (QTcpSocket*)sender();
	QDataStream in(pClientSocket);
	
	for (;;) {
		if (!m_nNextBlockSize) 
		{
			if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
				break;
			}
			in >> m_nNextBlockSize;
		}

		if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
			break;
		}
		QTime   time;
		QString str;
		in >> time >> str;

		QString strMessage = time.toString() + " " + "Client: " + str; //Клиент отправляет сообщение
		m_chatLog->append(strMessage);

		m_nNextBlockSize = 0;

		//sendToClient(pClientSocket, "Server Response: Received \"" + str + "\"");
	}
}

void MyClient::sendToClient(QTcpSocket* pServerSocket, const QString& str)
{
	QByteArray  arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);

	out << quint16(0) << QTime::currentTime() << " Server: " << str;
	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));

	pServerSocket->write(arrBlock);
}


//OK?
void MyClient::slotConBtnClicked()
{
	if (isServer)
	{
		return;
	}

	QString strHostIP = m_ipEnter->text();
	m_ipEnter->clear();
	m_socket = new QTcpSocket(this);
	m_socket->connectToHost(strHostIP, mPort);
	

	connect(m_socket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
	
}


void MyClient::slotReadyRead()
{
	QDataStream in(m_socket);
	for (;;) {
		if (!m_nNextBlockSize) {
			if (m_socket->bytesAvailable() < sizeof(quint16)) {
				break;
			}
			in >> m_nNextBlockSize;
		}

		if (m_socket->bytesAvailable() < m_nNextBlockSize) {
			break;
		}
		QTime   time;
		QString str;
		in >> time >> str;

		m_chatLog->append(time.toString() + " Client: " + str);
		m_nNextBlockSize = 0;
	}
}

//OK
void MyClient::slotError(QAbstractSocket::SocketError err)
{
	QString strError =
		"Error: " + (err == QAbstractSocket::HostNotFoundError ?
			"The host was not found." :
			err == QAbstractSocket::RemoteHostClosedError ?
			"The remote host is closed." :
			err == QAbstractSocket::ConnectionRefusedError ?
			"The connection was refused." :
			QString(m_socket->errorString())
			);
	m_chatLog->append(strError);
}

//OK?
void MyClient::slotSendToServer()
{
	QByteArray  arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);
	out << quint16(0) << QTime::currentTime() << m_inputBox->text();
	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));
	m_inputBox->setText("");


	if (!isServer)
	{
		m_socket->write(arrBlock);
	}
	else
	{
		for (auto& client : mClients)
		{
			client->write(arrBlock);
		}
	}
}