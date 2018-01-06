#include "stdafx.h"
#include "cJSON.h"
#include "Decide.h"
#include "MessageHandle.h"
#include "MSG.h"

void HandleLoginMsg(char *pMsg)
{
	cJSON *root = NULL;
	cJSON *temp = NULL;
	root = cJSON_Parse(pMsg);
	if (!root)
	{
		AfxMessageBox(_T("消息格式错误"));
		return;
	}

	temp = cJSON_GetObjectItem(root, "ip");
	if (!temp)
	{
		AfxMessageBox(_T("param错误"));
		cJSON_Delete(root);
		return;
	}
	char *ip = temp->valuestring;
	temp = cJSON_GetObjectItem(root, "port");
	if (!temp)
	{
		AfxMessageBox(_T("param error"));
		return;
	}
	int port = temp->valueint;

	//加入表项
	IPINFO NewNode;
	strncpy(NewNode.ip, ip, 20);
	NewNode.port = port;
	theApp.IPList.push_back(NewNode);

	cJSON_Delete(root);

	//AfxMessageBox(_T("新的用户加入"));
	theApp.setVersion(theApp.version + 1);
}

void HandleVersionMsg(char *pMsg) {
	cJSON *root = NULL;
	cJSON *temp = NULL;
	
	root = cJSON_Parse(pMsg);
	if (!root)
	{
		AfxMessageBox(_T("消息格式错误"));
		return;
	}

	temp = cJSON_GetObjectItem(root, "version");
	if (!temp)
	{
		AfxMessageBox(_T("param error"));
		return;
	}
	int version = temp->valueint;
	CString str;
	str.Format(_T("version is %d。"), theApp.version);
	AfxMessageBox(str);
	cJSON_Delete(root);
}

void HandleListMsg(char *pMsg)
{
	cJSON *json_root = cJSON_Parse(pMsg);
	if (!json_root)
	{
		AfxMessageBox(_T("Message format error"));
		return;
	}

	cJSON *temp = cJSON_GetObjectItem(json_root, "version");
	if (!temp)
	{
		AfxMessageBox(_T("param error"));
		cJSON_Delete(json_root);
		return;
	}

	theApp.version = temp->valueint;
	temp = cJSON_GetObjectItem(json_root, "data");
	if (!temp)
	{
		AfxMessageBox(_T("param error"));
		return;
	}

	theApp.IPList.clear();	//清空IP列表
	//重新写IP列表
	int num = cJSON_GetArraySize(temp);
	cJSON *Node = NULL;
	cJSON *NodeIP = NULL;
	cJSON *NodePort = NULL;
	for (int i = 0; i < num; i++)
	{
		Node = cJSON_GetArrayItem(temp, i);
		if (!Node)
			break;
		NodeIP = cJSON_GetObjectItem(Node, "ip");
		if (!NodeIP)
		{
			AfxMessageBox(_T("param error"));
			break;
		}
		NodePort = cJSON_GetObjectItem(Node, "port");
		if (!NodePort)
		{
			AfxMessageBox(_T("param error"));
			break;
		}

		IPINFO IPNode;
		strncpy(IPNode.ip, NodeIP->valuestring, 20);
		IPNode.port = NodePort->valueint;

		theApp.IPList.push_back(IPNode);
	}

	cJSON_Delete(json_root);
}

void HandleRequstMsg(CClientSocket *socket)
{
	MSGHEAD msg;

	cJSON *json_root = cJSON_CreateObject();
	if (!json_root)
	{
		AfxMessageBox(_T("Memory malloc error"));
		return;
	}

	cJSON *root = cJSON_CreateArray();
	if (!root)
	{
		AfxMessageBox(_T("Memory malloc error"));
		return;
	}

	cJSON_AddNumberToObject(json_root, "version", theApp.version);
	int num = theApp.IPList.size();

	for (int i = 0; i < num; i++)
	{
		cJSON *temp = cJSON_CreateObject();
		cJSON_AddStringToObject(temp, "ip", theApp.IPList[i].ip);
		cJSON_AddNumberToObject(temp, "port", theApp.IPList[i].port);
		cJSON_AddItemToArray(root, temp);
	}
	cJSON_AddItemToObject(json_root, "data", root);

	char *pBuff = cJSON_PrintUnformatted(json_root);

	msg.type = MSG_LIST;
	msg.length = strlen(pBuff);

	socket->SendMSG(pBuff, &msg);

	cJSON_Delete(root);
	free(pBuff);
}