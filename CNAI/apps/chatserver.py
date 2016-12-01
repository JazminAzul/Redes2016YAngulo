import socket
import sys
import argparse
import select

PROMP = "> "

# ---------------------- CHAT SERVER ---------------------
# Descripcion: Servirdor echo que replica todo lo que re-
# 			   cibe del cliente.
#
# Funcionamiento:
#			python echoserver.py port
#
#			port - puerto donde se ejectua el programa
# --------------------------------------------------------

# Parseamos los argumentos
parser = argparse.ArgumentParser(description='Echo client.')
parser.add_argument('port', metavar='port', type=int, 	
                   help='El puerto donde escucha el servidor')
args = parser.parse_args()

# Creamos el socket TCP/IP
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Liga el socket al puerto
server_address = ('localhost', args.port)
sock.bind(server_address)

# Escucha por conexiones entrantes
sock.listen(1)
nombre = "Pedro"
# Funcionalidad del servidor
# Espera una conexion
connection, client_address = sock.accept()
print "Conexion establecida con el cliente:", client_address
try:
	while True:
		# Recibe datos de la conexion
		data = connection.recv(200)
		if data:
			# imprime los datos recibidos
			print PROMP + data
		else:
			break
		message = raw_input(nombre+"> ")
		if (len(message) == 0):
			break
		# Enviamos los datos
		connection.sendall(message)
		# Espera la respuesta del servidor
		#data = sock.recv(len(message))
# En el caso de enviar un ctrl+d, se resuelve el error
except EOFError:
	# Limpia la conexion
	sock.close()
#finally:
	# Limpia la conexion
#	connection.close()
