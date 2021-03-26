#pragma once

#include <QObject>
#include <QtWidgets>
#include <qtcpsocket.h>
#include <qtcpserver.h>

const int mPort = 2323;

class MyClient : public QWidget
{
	Q_OBJECT

private:
	QLineEdit*		m_ipEnter;
	QPushButton*	m_conBtn;
	QPushButton*	m_sendBtn;
	QPushButton*	m_hostBtn;
	QLabel*			m_serverIp;
	QTextEdit*		m_chatLog;
	QLineEdit*		m_inputBox;
	QTcpServer*		m_server;
	QTcpSocket*		m_socket;
	quint16 m_nNextBlockSize;
	QTcpSocket* pServerSideSocket;
	QVector<QTcpSocket*> mClients;

	bool isServer;

public:
	MyClient();
	
private slots: //server slots
	void slotHostBtnClicked();
	void slotNewConnection();
	void slotReadClient();

private: //server function
	void sendToClient(QTcpSocket* pServerSocket, const QString& str);

private slots: //socket slots
	void slotConBtnClicked();
	void slotReadyRead();
	void slotError(QAbstractSocket::SocketError);
	void slotSendToServer();
};
