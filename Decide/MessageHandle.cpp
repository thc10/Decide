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

	AfxMessageBox(_T("新的用户加入"));
}

void HandleVersionMsg(char *pMsg) {
	cJSON *root = NULL;

	AfxMessageBox(_T("recieve version MSg"));
	/*
	root = cJSON_Parse(pMsg);
	if (root)
	{
		AfxMessageBox(_T("消息格式错误"));
		return;
	}

	int version = cJSON_GetObjectItem(root, "version")->valueint;

	cJSON_Delete(root);*/
}

void HandleRequstMsg(char *pMsg) {
	AfxMessageBox(_T("recvie requst msgs"));
}

void HandleListMsg(char *pMsg) {
	AfxMessageBox(_T("recvie list msgs"));
}