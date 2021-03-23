#include "MyClient.h"

MyClient::MyClient() : QWidget()
{
	QGridLayout* m_gridLayout = new QGridLayout(this);

	m_ipEnter = new QLineEdit("");
	m_conBtn = new QPushButton("Connect");
	m_sendBtn = new QPushButton("Send");
	m_chatLog = new QTextEdit("");
	m_txtBox = new QTextEdit("");
	m_serverIp = new QLabel("");
	m_hostBtn = new QPushButton("Start server");
	

	m_conBtn->setMinimumSize(40, 25);
	m_ipEnter->setMinimumHeight(25);
	m_txtBox->setMaximumHeight(50);
	m_sendBtn->setMinimumSize(50, 50);
	m_chatLog->setMinimumSize(100, 100);
	
	m_chatLog->setReadOnly(true);


	m_gridLayout->addWidget(m_serverIp, 0, 0, 1, 1);
	m_gridLayout->addWidget(m_hostBtn, 0, 1, 1, 1);
	m_gridLayout->addWidget(m_ipEnter, 1, 0, 1, 1);
	m_gridLayout->addWidget(m_conBtn, 1, 1, 1, 1);
	m_gridLayout->addWidget(m_chatLog, 2, 0, 1, 2);
	m_gridLayout->addWidget(m_txtBox, 3, 0, 1, 1);
	m_gridLayout->addWidget(m_sendBtn, 3, 1, 1, 1);


	m_socket = new QTcpSocket;
	

	connect(m_conBtn, SIGNAL(clicked()), this, SLOT(slotConBtnClicked()));
	connect(m_hostBtn, SIGNAL(clicked()), this, SLOT(slotHostBtnClicked()));
	connect(m_sendBtn, SIGNAL(clicked()), this, SLOT(slotSendBtnClicked()));
}



void MyClient::slotConBtnClicked()
{
	qDebug() << "Attempting connection.";
	m_socket->connectToHost(m_ipEnter->text() , 2323, QIODevice::ReadWrite);
	m_ipEnter->clear();

}

void MyClient::slotHostBtnClicked()
{
	m_server = new QTcpServer;

	QTime t = QTime::currentTime();
	QString str = t.toString("hh:mm:ss -> ") + "Server started";
	m_chatLog->append(str);

	m_server->listen(QHostAddress::Any, 2323);
	m_serverIp->setText(m_server->serverAddress().toString());
}

void MyClient::slotSendBtnClicked()
{
	QTime t = QTime::currentTime();
	QString str = t.toString("hh:mm:ss -> ") + m_socket->state();
	qDebug() << str;
	m_chatLog->append(str);

}