/* chatserver.c */

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
 * Program: chatserver
 * Purpose: wait for a connection from a chatclient & allow users to chat
 * Usage:   chatserver <appnum>
 *
 *-----------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	connection	conn;
	int		len, len2;
	char		buff[BUFFSIZE];
	char		buff2[BUFFSIZE];
	char*		servidor;
	char*		cliente;
	struct timeval timer;
	fd_set readfds;
	int sret;
	if (argc != 2) {
		(void) fprintf(stderr, "usage: %s <appnum>\n", argv[0]);
		exit(1);
	}

	(void) printf("Ingrese Nombre:\n");
	len= readln(buff2, BUFFSIZE);
	servidor=(char*)malloc(sizeof(char)*len);
	(void) memcpy(servidor, buff2, len-1);
	(void) printf("El nombre es:%s/n", servidor);

	(void) printf("Chat Server Waiting For Connection.\n");

	/* wait for a connection from a chatclient */

	conn = await_contact((appnum) atoi(argv[1]));
	if (conn < 0)
		exit(1);

	len2=recvln(conn, buff,BUFFSIZE);
	cliente=(char*)malloc(sizeof(char)*len2);
	memcpy(cliente, buff, len2-1);
	printf("el cliente se llama:%s\n", cliente);
	(void) send(conn,buff2, len,0);
	
	(void) printf("Chat Connection Established.\n");
	
	/* iterate, reading from the client and the local user */


	while (1){ //Esto hace que nuestro programa se ejecute 
				 //indefinidamente
		FD_ZERO(&readfds); 		// Limpia nuestro fd_set
		FD_SET(0, &readfds);	// Setea el teclado para escucharlo
		FD_SET(conn, &readfds); // Setea la conexion para escucharla

		timer.tv_sec = 10;	// Seteamos los segundos del timer a 10
		timer.tv_usec = 0;	// Seteamos los microseg. del timer a 0

		sret = select(conn+1,	// Le decimos el ultimo numero a escuchar
					  &readfds,	// Le decimos la estructura que controla
					  NULL,		// Este parametro no interesa
					  NULL,		// Este parametro no interesa
					  &timer);	// Nuestro temporizador

		if (sret == 0){
			
			printf ("time out\n");
			break;
			/*---------------------------------
			  Quiere decir que se nos acabo el 
			  tiempo, por eso select devuelve 0.
			  Se debe cerrar la conexion y 
			  terminar. Aqui ejecutamos "break".
			---------------------------------*/
		}


		/*---------------------------------
			  Quiere decir que se nos acabo el 
			  tiempo, por eso select devuelve 0.
			  Se debe cerrar la conexion y 
			  terminar. Aqui ejecutamos "break".
			---------------------------------*/
	
		else{
		/*-------------------------------------
		  Quiere decir que ocurrio un evento.
		  Luego debemos ver cual de los descriptores 
		  reporto que algo ocurrio.
		---------------------------------------*/
			if (FD_ISSET(0, &readfds)){

				if ((len = readln(buff, BUFFSIZE)) < 1)
					break;
				buff[len - 1] = '\n';
				(void) send(conn, buff, len, 0);
				
			/* ----------------------------------
			Fue el teclado quien genero el evento. 
			Aca debe leerle desde teclado. Ya se 
			tiene esa funcionalidad andando.
			------------------------------------*/
			}

			if (FD_ISSET(conn, &readfds)){

				len = recvln(conn, buff, BUFFSIZE);
				(void) printf("%s>", cliente);
				(void) fflush(stdout);
				(void) write(STDOUT_FILENO, buff, len);
			/* -----------------------------------
			Fue el otro extremo de la conexion
			quien genero el evento. Debemos leer
			desde el socket. Ya se tiene esa funcio-
			nalidad corriendo.
			------------------------------------*/
			}

	
		}

	}

	/*----------------------------------------------
	Estas lineas se alcanzan s+olo cuando se sale 
	del ciclo while. Esto quiere decir que, o bien
	se acaba el tiempo, o bien el otro extremo cerro
	la conexion. Debemos enviar un fin de conexion.
	----------------------------------------------*/








	/* iteration ends when EOF found on stdin or chat connection */
		(void) send_eof(conn);
		(void) printf("\nChat Connection Closed.\n");
		return 0;
}
