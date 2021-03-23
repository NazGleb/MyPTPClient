#pragma once

#include <QObject>
#include <QtWidgets>
#include <qtcpsocket.h>
#include <qtcpserver.h>

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
	QTextEdit*		m_txtBox;
	QTcpSocket* m_socket;
	QTcpServer* m_server;


public:
	MyClient();

public slots:
	void slotConBtnClicked();
	void slotHostBtnClicked();
	void slotSendBtnClicked();

};
