#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>


#define SERVPORT       3333
#define BACKLOG        10
#define	MAX_BUFF_SIZE  200000
#define SEND_TIMES     5
#define BUFFER_SIZE    256
#define DATA_SIZE      1024
#define RECV_MODE      0
#define SEND_MODE      1

