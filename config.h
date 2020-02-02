#ifndef __CPDP_CONFIG_H__
#define __CPDP_CONFIG_H__

//#include <stdio.h>
//#include <iostream>
#include <string>

//using namespace std;
using std::string;

//#define DEBUG

#define MAX_U2S 30 	//max user to sever is 30
#define MSG_LEN 4095	//max messeng length
#define MAX_THREAD_NUM 50 //max thread run by server
#define MAX_CMD_LEN 256	//max cmd length
#define MAX_BUF_LEN 4095	//max messenge buff length
#define MAX_C2C 20	//max number of client connection
#define MAX_LEN	4095	//max lenth of sending buffer

typedef enum CMD_TYPE
{
	CMD_REGISTER = 1,
	CMD_LOGIN = 2,
	CMD_LOGOUT = 3,
	CMD_INVITE = 4,
	CMD_ACCEPT = 5,
	CMD_REQUEST_FRIEND_ID = 6,
	CMD_EXIT = 7,
	CMD_UNKNOWN = 8,

	CMD_SENDMSG = 9,
	CMD_FD_ONLINE = 10,
	CMD_FD_OFFLINE = 11,
	CMD_S2CMSG = 12,
	CMD_C2CMSG = 13,
	CMD_NOWONLINE = 14,
	CMD_REGISTER_SUCCESSED = 101,
	CMD_LOGIN_SUCCESSED = 102,

	//CMD_FD_ONLINE = 201,
	
}cmd_type;

typedef struct messenger_user{
	int socket;
	string name;
	string pwd;
	string addr;
}messenger_user;

#endif
