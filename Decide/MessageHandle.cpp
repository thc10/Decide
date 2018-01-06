#include "stdafx.h"
#include "cJSON.h"
#include "Decide.h"
#include "MessageHandle.h"
#include "MSG.h"

void HandleLoginMsg(char *pMsg)
{
	cJSON *root = NULL;

	root = cJSON_Parse(pMsg);
	if (root)
	{
		AfxMessageBox(_T("消息格式错误"));
		return;
	}

	char *ip = cJSON_GetObjectItem(root, "ip")->valuestring;
	int port = cJSON_GetObjectItem(root, "port")->valueint;

	//加入表项
	IPINFO NewNode;
	strncpy(NewNode.ip, ip, 20);
	NewNode.port = port;
	theApp.IPList.push_back(NewNode);

	cJSON_Delete(root);
}

void HandleVersionMsg(char *pMsg) {
	cJSON *root = NULL;

	root = cJSON_Parse(pMsg);
	if (root)
	{
		AfxMessageBox(_T("消息格式错误"));
		return;
	}

	int version = cJSON_GetObjectItem(root, "version")->valueint;

	cJSON_Delete(root);
}

void HandleRequstMsg(char *pMsg) {
	AfxMessageBox(_T("recvie requst msgs"));
}

void HandleListMsg(char *pMsg) {
	AfxMessageBox(_T("recvie list msgs"));
}