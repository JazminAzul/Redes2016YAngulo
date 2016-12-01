import socket
import sys
import argparse
import select

PROMP = "> "

# ---------------------- CHAT CLIENT ---------------------
# Descripcion: Cliente echo que replica todo lo que envia
# 			   mensajes al servidor y espera como respues-
#			   ta lo enviado.
#
# Funcionamiento:
#			python echoclient.py ipaddress port
#
#			ipaddress 	- direccion ip donde se ejecuta el 
#						  programa servidor.
#			port 		- puerto donde se ejectua el pro-
#						  grama servidor
# --------------------------------------------------------

# Parseamos los argumentos
parser = argparse.ArgumentParser(description='Echo client.')
parser.add_argument('ip_address', metavar='ip_address', type=str, 
					help='La direccion IP del servidor')
parser.add_argument('port', metavar='port', type=int, 	
                   help='El puerto donde escucha el servidor')
args = parser.parse_args()

# Crea un socket TCP/IP 
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Conecta el socket con el puerto donde el servidor esta escuchando
server_address = (args.ip_address, args.port)
sock.connect(server_address)
nombre = "juan"
# Funcionalidad del cliente 
try:
	while True:
		# Leemos datos de consola.
		#Imprime el nombre del cliente
		message = raw_input(nombre+" > ")
		if (len(message) == 0):
			break
		# Enviamos los datos
		sock.sendall(message)
		# Recibe datos de la conexion
		data = sock.recv(200)
		if data:
			# imprime los datos recibidos
			print PROMP + data
		else:
			break

# En el caso de enviar un ctrl+d, se resuelve el error
except EOFError:
	sock.close()
