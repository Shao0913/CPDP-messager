#include "messenger_client.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

//using namespace std;

using std::ifstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::ios;
using std::pair;

messenger_client *ptr_this;

vector<string>* split_string(string s, const char *dev)
{
	vector<string> *vs = new vector<string>();
	char *c =  const_cast<char*>(s.c_str());	//const_cast is used to cast away the constness of variables
	char *token = std::strtok(c, dev);
	while(token != NULL)
	{
		vs->push_back(token);
		token = std::strtok(NULL, dev);
	}
	return vs;
}

void sig_int_handler(int sig_num)
{
	#ifdef DEBUG
		printf("DEBUG: sig handler, exiting.\n");
	#endif
	ptr_this->user_exit();
	exit(0);
}


void *pthread_fun(void *arg)
{
	for( ; ; )
	{
		ptr_this->menu_2();
		string cmd_buf = ptr_this->get_input();
		#ifdef DEBUG
			printf("cmd get form get_input(): %s\n", cmd_buf.c_str());
		#endif
		if(cmd_buf.size()>0)
		{
			switch(ptr_this->parse_cmd(cmd_buf))
			{
				case CMD_SENDMSG:
					ptr_this->user_sendmsg(cmd_buf);
					break;
				case CMD_INVITE:
					ptr_this->user_invite(cmd_buf);
					break;
				case CMD_ACCEPT:
					ptr_this->user_accept(cmd_buf);
					break;
				case CMD_LOGOUT:
					ptr_this->user_logout();
					ptr_this->thread_finish_flag = 1;
					return NULL;
				default:
					printf("Unknown cmd: [%s]\n", cmd_buf.c_str());
					break;
			}
		}
	}
	ptr_this->thread_finish_flag = 1;
	return NULL;
}


int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		printf("Usage: messenger_client configuration_file\n");
		return 0;
	}
	else
	{
		#ifdef DEBUG
			printf("DEBUG:messenger_client configuration file received. \n");
		#endif
	}
	
	messenger_client *msg_client = new messenger_client(argv[1]);
	if(msg_client == NULL)
	{
		printf("New messenger client create failed.\n");
		return 0;
	}
	else
	{
		#ifdef DEBUG
			printf("DEBUG: msg client created successful.\n");
		#endif
	}

	ptr_this = msg_client;
	#ifdef DEBUG
		printf("ptr_this pointer successful.\n");
	#endif
	signal(SIGINT, sig_int_handler);
	int init;
	init = msg_client->init_socket();
	#ifdef DEBUG
		printf("DEBUG: init_socket successful.\n");
	#endif
	msg_client->print_start(init);
	#ifdef DEBUG
		printf("DEBUG: print_start successful.\n");
	#endif
	if(init)
	{
		#ifdef DEBUG
			printf("DEBUG: msg client socket init successful. \n");
		#endif
		return 0;
	}

	msg_client->loop();
	
	//LOG
	return 0;
}
