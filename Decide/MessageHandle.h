#pragma once
#include "ClientSocket.h"
void HandleLoginMsg(char *pMsg);
void HandleVersionMsg(char *pMsg);
void HandleListMsg(char *pMsg);
void HandleRequstMsg(char *pMsg);
void HandleVoteMsg(char *pMsg);
void HandleStartVoteMsg(char *pMsg);