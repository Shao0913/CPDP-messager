#include "messenger_server.h"
#include <unistd.h>

messenger_server *ptr_this;

void sig_int_handler(int sig_num)
{
	ptr_this->exit_server();
	exit(0);
}

vector<string>* split_string(string s, const char *dev)
{
        vector<string> *vs = new vector<string>();
        char *c =  const_cast<char*>(s.c_str());        //const_cast is used to cast away the constness of variables
        char *token = std::strtok(c, dev);
        while(token != NULL)
        {
                vs->push_back(token);
                token = std::strtok(NULL, dev);
        }
        return vs;
}


void *pthread_fun(void *arg)
{
	pthread_arg *p_arg = (pthread_arg*)arg;
	messenger_server *server_arg = p_arg->server;
	int cli_socket = *(p_arg->socket);
	char recv_buf[MSG_LEN];
	socklen_t recv_length;
	for( ; ; )
	{
		recv_length = recv(cli_socket, recv_buf, MSG_LEN,0);
		if(recv_length > 0)
		{
			if(strlen(recv_buf) > 0)
				fprintf(stderr, "Line: %d. Received a msg from [%d]: [%s]\n", __LINE__, cli_socket, recv_buf);
			else
				fprintf(stderr, "Line: %d. Received a msg from [%d]: [UNKNOWN MSG]\n", __LINE__, cli_socket);
			
			switch(server_arg->parse_cmd(recv_buf))
			{
				case CMD_REGISTER:
					server_arg->user_register(recv_buf, cli_socket);
					break;
				case CMD_LOGIN:
					server_arg->user_login(recv_buf, cli_socket);
					break;
				case CMD_LOGOUT:
					server_arg->user_logout(recv_buf, cli_socket);
					break;
				case CMD_INVITE:
					server_arg->user_invite(recv_buf, cli_socket);
					break;
				case CMD_ACCEPT:
					server_arg->user_accept(recv_buf, cli_socket);
					break;
				case CMD_REQUEST_FRIEND_ID:
					server_arg->user_friend(recv_buf, cli_socket);
					break;
				case CMD_EXIT:
					server_arg->user_exit(recv_buf, cli_socket);
					--(server_arg->num_thread);
					return NULL;
				default:
					fprintf(stderr, "Line: %d. unknown cmd_type: %s\n", __LINE__, recv_buf);
			}
			memset(recv_buf, 0, sizeof(recv_buf));
		}
		else if(recv_length == 0)
		{
			fprintf(stderr, "Line: %d Client [%d] has exit. \n", __LINE__, cli_socket);
			server_arg->user_exit("CMD_EXIT||", cli_socket);
			--(server_arg->num_thread);
			return NULL;
		}
		else
		{
			fprintf(stderr,"Line: %d, receive msg error. \n", __LINE__);
		}
	}
	--(server_arg->num_thread);
	return NULL;
}

int main(int argc, char const *argv[])
{
	if (argc != 3){
		fprintf(stderr, "Usage: messenger_client user_info_file configuration_file\n");
		return 0;
	}

	messenger_server  *myserver = new messenger_server(argv[1], argv[2]);
	myserver->print_local_ip();
	int init;
	init = myserver -> init_socket();
	if(init){
		fprintf(stderr, "Line: %d. Init_socket failed!\n", __LINE__);
		return 0;
	}
	ptr_this = myserver;
	signal(SIGINT, sig_int_handler);
	myserver->loop();
	
	return 0;
}

