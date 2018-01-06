#pragma once

// CServerSocket ÃüÁîÄ¿±ê

class CServerSocket : public CSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	virtual void OnAccept(int nErrorCode);
};


