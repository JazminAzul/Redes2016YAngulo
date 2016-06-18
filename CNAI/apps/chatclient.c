/* chatclient.c */

#include <stdlib.h>
#include <stdio.h>
#include <cnaiapi.h>
#include <string.h>
#include <sys/select.h>

#define BUFFSIZE		256
#define INPUT_PROMPT		"Input   > "
#define RECEIVED_PROMPT		"Received> "

int recvln(connection, char *, int);
int readln(char *, int);

/*-----------------------------------------------------------------------
 *
 * Program: chatclient
 * Purpose: contact a chatserver and allow users to chat
 * Usage:   chatclient <compname> <appnum>
 *
 *-----------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	computer	comp;
	connection	conn;
	char		buff[BUFFSIZE];
	char		buff2[BUFFSIZE];
	int		len;
	char*		cliente;
	char*		servidor;
	struct timeval timer;
	fd_set readfds;
	int sret;


	printf("Ingrese Nombre: \n");
	len= readln(buff2, BUFFSIZE);

	cliente=(char*)malloc(sizeof(char)*len);
	memcpy(cliente, buff2, len-1);
	buff[len-1]= '\n';
	printf("El nombre es: %s/n", cliente);
	
	if (argc != 3) {
		(void) fprintf(stderr, "usage: %s <compname> <appnum>\n",
			       argv[0]);
		exit(1);
	}

	/* convert the compname to bynary form comp*/

	comp = cname_to_comp(argv[1]);
	if (comp == -1)
		exit(1);

	/* make a connection to the chatserver */
	
	conn = make_contact(comp, (appnum) atoi(argv[2]));
	if (conn < 0) 
		exit(1);

	(void)send(conn, buff2, len,0);
	
	len=recvln(conn, buff, BUFFSIZE);
	servidor=(char*)malloc(sizeof(char)*len);
	memcpy(servidor, buff,len-1);
	printf("el server se llama: %s\n", servidor);
	
	(void) printf("Chat Connection Established.\n"); 
	(void) printf("%s>", cliente);
	(void) fflush(stdout);
	
	/* iterate, reading from local user and then from chatserver */

	while(1){
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);
		FD_SET(conn, &readfds);
		
		timer.tv_sec = 10;
		timer.tv_usec= 0;
		
		sret= select(conn+1,   &readfds,
					NULL,
					NULL,
					&timer);

		if (sret == 0){
			printf("time out\n");
			break;
		}
		else {
			if (FD_ISSET(0,&readfds)) {
				if((len=readln(buff,BUFFSIZE))<1)
					break;
			}

	/*receive and print a line from the chatserver*/

			if (FD_ISSET(conn,&readfds)){
				if((len = recvln(conn, buff, BUFFSIZE))<1);
					break;
			(void) printf("%s>", servidor);
			(void) fflush(stdout);
			(void) write(STDOUT_FILENO, buff, len);
			}
		}
	}

	/* iteration ends when stdin or the connection indicates EOF */

	(void) printf("\n Chat connection Closed.\n");
	(void) send_eof(conn);
	exit(0);



















	/* iteration ends when stdin or the connection indicates EOF */

	(void) printf("\nChat Connection Closed.\n");
	(void) send_eof(conn);
	exit(0);
}
