/* chatclient.c */

#include <stdlib.h>
#include <stdio.h>
#include <cnaiapi.h>
#include <string.h>

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
	if (argc != 3) {
		(void) fprintf(stderr, "usage: %s <compname> <appnum>\n",
			       argv[0]);
		exit(1);
	}

	(void) printf("Ingrese Nombre: \n");
	len= readln(buff2, BUFFSIZE);
	cliente=(char*)malloc(sizeof(char)*len);
	(void) memcpy(cliente, buff2, len-1);
	buff[len-1]= '\n';
	(void) printf("El nombre es: %s/n", cliente);
	/* convert the compname to binary form comp */

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

	while((len = readln(buff, BUFFSIZE)) > 0) {
		buff[len - 1] = '\n';
		(void) send(conn, buff, len, 0);
		
		/* receive and print a line from the chatserver */
		if ((len = recvln(conn, buff, BUFFSIZE)) < 1)
			break;
		(void) printf("%s>", servidor);
		(void) fflush(stdout);
		(void) write(STDOUT_FILENO, buff, len);

		(void) printf("%s>", cliente);
		(void) fflush(stdout);
	}

	/* iteration ends when stdin or the connection indicates EOF */

	(void) printf("\nChat Connection Closed.\n");
	(void) send_eof(conn);
	exit(0);
}
