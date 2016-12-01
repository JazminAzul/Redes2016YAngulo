import socket
import sys
import argparse

# ---------------------- ECHO SERVER ---------------------
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

# Funcionalidad del servidor
# Espera una conexion
connection, client_address = sock.accept()
print ("Conexion establecida con el cliente:", client_address)

f = open("archivo.txt", "a")
f.write("holaaa\n")
try:
	while True:
		# Recibe datos de la conexion
		data = connection.recv(200)
		if data:
			# Reenvia los datos recibidos
			connection.sendall(data)
		else:
			break
finally:
	# Limpia la conexion
	connection.close()
	f.close()
