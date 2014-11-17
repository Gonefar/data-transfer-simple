void Dect_input_parm_num(int num)
{
	if(num <2)
	{
		fprintf(stderr, "Please enter the server's hostname!");
		exit(1);
	}

}

void Dect_hostname(struct hostent *host)
{	
	if(host == NULL)
	{
		herror("gethostbyname error!");
		exit(1);
	}

}

int Socket(int IP, int SOCK_KIND, int FLAG)
{
	int sockfd;
	
	if((sockfd = socket(IP, SOCK_KIND, FLAG)) == -1)
	{
		perror("socket error!");
		exit(1);
	}
	
	return sockfd;
}

void Connect(int sockfd, struct sockaddr_in serv_addr)
{
	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect error");
		exit(1);
	}

}

void Bind(int sockfd, struct sockaddr_in *my_addr)
{
	if(bind(sockfd,(struct sockaddr *)my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("bind error!");
		exit(1);
	}

}

void Listen(int sockfd, int listen_num)
{
	if(listen(sockfd, listen_num) < 0)
	{
		perror("listen");
		exit(1);
	}

}

int Accept(int sockfd, struct sockaddr_in *remote_addr)
{
	int sin_size = sizeof(struct sockaddr_in);
	int client_fd;
	if((client_fd =accept(sockfd,(struct sockaddr *)remote_addr, &sin_size))==-1)
	{
		perror("accept error");
		return 0;
	}
	
	return client_fd;
}
void init_server_add_at_client(struct sockaddr_in *serv_addr, struct hostent *host)
{
	bzero(serv_addr, sizeof(struct sockaddr_in));     
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_port = htons(SERVPORT);
	serv_addr->sin_addr = *((struct in_addr *)host->h_addr);

}

void init_server_add_at_server(struct sockaddr_in *my_addr)
{
	bzero(my_addr, sizeof(struct sockaddr_in));
	my_addr->sin_family = AF_INET;
	my_addr->sin_port = htons(SERVPORT);
	my_addr->sin_addr.s_addr = htonl(INADDR_ANY);

}
float TimeUsed(struct timeval *tcpStart, struct timeval *tcpEnd)
{
	float timeuse;
	
	timeuse = 1000000 * (tcpEnd->tv_sec - tcpStart->tv_sec) + tcpEnd->tv_usec - tcpStart->tv_usec;
	timeuse /= 1000000;

	return timeuse;

}

void Print_Date_Time(void)
{
	time_t ticks;
	char timeBuff[100];
	
	ticks = time(NULL);
	snprintf(timeBuff, sizeof(timeBuff), "%.24s\r\n", ctime(&ticks));
	printf("The ending time %s\n", timeBuff);

}
char *AllocDataAdd(int DataSize)
{
	char *p = NULL;

	p = malloc(DataSize);		
	if(p == NULL)
		{
			printf("Allocate data address wrong\n");
			return NULL;
		}

	return p;
}

void writeValue(char *p)
{
	int i = 0;
	
	for(i = 0; i < MAX_BUFF_SIZE; i++)
		{
			p[i] = 'a';
		}
}
float calcuOnceTransRate(float timeDiff, char *data)
{
	int   datasize = 0;
	float rate = 0;
	
	while(data != NULL)
		{
			datasize++;
			data++;
		}

	rate = datasize * sizeof(char) / timeDiff;

	return rate;
}

void SendMessage( int sockfd, char *buff, int times)
{
	int i = 0;
	int x = 0;
	int tbytes = 4096;
	int buff_len = 0;
	char *temp = NULL;

	temp = buff;

	for(i = 0; i < times; i++)
		{
			buff_len = strlen(temp);
			
			while((x = write(sockfd, temp, tbytes)) > 0)
				{
					buff_len -= x;
					temp     += x;
					if(buff_len < tbytes)
						{
							tbytes = buff_len;
						}
							
					if(buff_len <= 1 )
						{
							temp = buff;
							tbytes = 4096;
							break;
						}
				}
							
		}
}

void RecvMessage(int sockfd, char *buff)
{
	int n;
	char *bp = NULL;
	int bytes_to_read = MAX_BUFF_SIZE * SEND_TIMES;
	int tbytes = 4096;
	
	bp = buff;
	while((n = read(sockfd, bp, 4096)) > 0)
	{
		bp += n;
		bytes_to_read -= n;
		
		if(bytes_to_read < 4096)
			{
				tbytes = bytes_to_read;
			}
		
		if(bytes_to_read <= 1)
			{
				printf("Finish recv\n");
				break;
			}

	}

}

int SendFile_Store_Then_Send(int sockfd)
{
	char FileName[BUFFER_SIZE], buffer[BUFFER_SIZE];
	bzero(buffer, BUFFER_SIZE);
	int file_block_length = 0;
	FILE *fp = NULL;

	//printf("Please input the file name:\n");
	//scanf("%s", FileName);
	strcpy(FileName, "/home/yujianghua/UPLOAD/tcpserver.c");
	
	if((fp = fopen(FileName, "r")) == NULL)
		{
			printf("open file error\n");
			return 0;
		}
	
	while( (file_block_length = fread(buffer,sizeof(char),BUFFER_SIZE,fp))>0)
			{
				printf("file_block_length = %d\n",file_block_length);                
				if(send(sockfd, buffer, file_block_length, 0)<0)
				{
					printf("Send File:\t%s Failed\n", FileName);
					break;
				}
				bzero(buffer, BUFFER_SIZE);
			}
			fclose(fp);
			printf("File:\t%s Transfer Finished\n", FileName);

			return 1;
}

int RecvFile_Store_Then_Send(int sockfd)
{
	char *buff = AllocDataAdd(1024);
	bzero(buff, BUFFER_SIZE);
	int file_block_length = 0, recvbytes;
	FILE *fp = NULL;
		
	if((fp = fopen("temp.txt", "w")) == NULL)
		{
			printf("open file error\n");
			return 0;
		}
		
	while(( recvbytes = read(sockfd, buff, 1024)) > 0)
		{
			printf("recv bytes:%d\n", recvbytes);
			fputs(buff, fp);
			bzero(buff, BUFFER_SIZE);

		}

	fclose(fp);
	free(buff);
	return 1;

}


