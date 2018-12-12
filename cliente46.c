/*******************************************************
Protocolos de Transporte
Grado en Ingeniería Telemática
Dpto. Ingeníería de Telecomunicación
Univerisdad de Jaén

Fichero: cliente.c
Versión: 2.0
Fecha: 09/2017
Descripción:
	Cliente sencillo TCP.

Autor: Juan Carlos Cuevas Martínez

*******************************************************/
#include <stdio.h>
#include <ws2tcpip.h>//Necesaria para las funciones IPv6
#include <conio.h>
#include "protocol.h"
#include "cliente46.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int *argc, char *argv[])
{
	SOCKET sockfd;
	struct sockaddr *server_in;
	struct sockaddr_in server_in4;
	struct sockaddr_in6 server_in6;
	int address_size = sizeof(server_in4);
	char buffer_in[1024], buffer_out[1024],input[1024];
	int recibidos=0,enviados=0;
	int estado=S_HELO;
	char option;
	int ipversion=AF_INET;//IPv4 por defecto
	char ipdest[256];
	char default_ip4[16]="192.168.1.104";
	char default_ip6[64]="::1";
	
	unsigned long ipdestl;
	struct in_addr address;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
   
	//Inicialización Windows sockets - SOLO WINDOWS
	wVersionRequested=MAKEWORD(1,1);
	err=WSAStartup(wVersionRequested,&wsaData);
	if(err!=0)
		return(0);

	if(LOBYTE(wsaData.wVersion)!=1||HIBYTE(wsaData.wVersion)!=1){
		WSACleanup();
		return(0);
	}
	//Fin: Inicialización Windows sockets
	
	printf("**************\r\nCLIENTE TCP SENCILLO SOBRE IPv4 o IPv6\r\n*************\r\n");
	

	do{

		printf("CLIENTE> ¿Qué versión de IP desea usar? 6 para IPv6, 4 para IPv4 [por defecto] ");
		gets_s(ipdest, sizeof(ipdest));

		if (strcmp(ipdest, "6") == 0) {
			ipversion = AF_INET6;

		}
		else { //Distinto de 6 se elige la versión 4
			ipversion = AF_INET;
		}

		sockfd=socket(ipversion,SOCK_STREAM,0);
		if(sockfd==INVALID_SOCKET){
			printf("CLIENTE> ERROR\r\n");
			exit(-1);
		}
		else{
			printf("CLIENTE> Introduzca la IP destino (pulsar enter para IP por defecto): ");
			gets_s(ipdest,sizeof(ipdest));
			ipdestl = inet_addr(ipdest);
			if (ipdestl == INADDR_NONE) {
				//La dirección introducida por teclado no es correcta o
				//corresponde con un dominio.
				struct hostent *host;
				host = gethostbyname(ipdest);
				if (host != NULL) {
					memcpy(&address, host->h_addr_list[0], 4);
					printf("\nDireccion %s\n", inet_ntoa(address));
					strcpy(ipdest, inet_ntoa(address));
				}
			}
			//Dirección por defecto según la familia
			if(strcmp(ipdest,"")==0 && ipversion==AF_INET)
				strcpy_s(ipdest,sizeof(ipdest),default_ip4);

			if(strcmp(ipdest,"")==0 && ipversion==AF_INET6)
				strcpy_s(ipdest, sizeof(ipdest),default_ip6);


	

			if(ipversion==AF_INET){
				server_in4.sin_family=AF_INET;
				server_in4.sin_port=htons(TCP_SERVICE_PORT);
				//server_in4.sin_addr.s_addr=inet_addr(ipdest);
				inet_pton(ipversion,ipdest,&server_in4.sin_addr.s_addr);
				server_in=(struct sockaddr*)&server_in4;
				address_size = sizeof(server_in4);
			}

			if(ipversion==AF_INET6){
				memset(&server_in6, 0, sizeof(server_in6));
				server_in6.sin6_family=AF_INET6;
				server_in6.sin6_port=htons(TCP_SERVICE_PORT);
				inet_pton(ipversion,ipdest,&server_in6.sin6_addr);
				server_in=(struct sockaddr*)&server_in6;
				address_size = sizeof(server_in6);
			}
			//añadir en preprocesador la funcion winsock-no noseque deprecated


			estado=S_WELC;

			if(connect(sockfd, server_in, address_size)==0){
				printf("CLIENTE> CONEXION ESTABLECIDA CON %s:%d\r\n",ipdest,TCP_SERVICE_PORT);
			
				//Inicio de la máquina de estados
				do{

				
					switch(estado){
					case S_WELC:
						// Se recibe el mensaje de bienvenida
						//recibidos = recv(sockfd, buffer_in, 512, 0);
						//buffer_in[recibidos] = 0x00;
						
						//printf("datos recibidos [%d bytes] %s \r\n", recibidos, buffer_in);
						//estado = S_HELO;

						break;
					case S_HELO:
						// establece la conexion de aplicacion 
						printf("CLIENTE> Introduzca el comando helo (enter para salir): ");
						gets_s(input,sizeof(input));
						if(strlen(input) == 0){
							sprintf_s (buffer_out, sizeof(buffer_out), "%s%s",input,CRLF);
							estado=S_QUIT;
						}
						else 
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s",input, CRLF);
							//estado = S_MAIL;
						 
						
						break;   


						


					case S_MAIL:

						printf("CLIENTE> Introduzca  mail from: 'usuario' (enter para salir): ");
						gets_s(input, sizeof(input));
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s%s",input, CRLF);
							estado = S_QUIT;
						}
						else
						sprintf_s(buffer_out, sizeof(buffer_out), "%s %s%s", MF, input, CRLF);
						//estado = S_RCPT;


						break;

					case S_RCPT:
						printf("CLIENTE> Introduzca el usuario al que quiere enviar (enter para salir): ");
						gets_s(input,sizeof(input));
						if(strlen(input)==0){
							sprintf_s (buffer_out, sizeof(buffer_out), "%s%s",input,CRLF);
							estado=S_QUIT;
						}
						else
							sprintf_s (buffer_out, sizeof(buffer_out), "%s %s%s",RCPT,input,CRLF);
						    //estado=S_DATA;
						break;



					case S_DATA: //TODO mandar cada linea al servido hasta que llegue a un punto
						printf("CLIENTE> si estan correctos los datos pulse alguna tecla de lo contrario pulse enter:  ");
						gets_s(input, sizeof(input));
						if(strlen(input)==0){
							sprintf_s (buffer_out, sizeof(buffer_out), "%s%s",input,CRLF);
							estado=S_MAIL;
						}
						else
							sprintf_s (buffer_out, sizeof(buffer_out), "%s%s",DATA,CRLF);
						break;

					case S_MENS:
						printf("CLIENTE> empiece a envia el contenido del mensaje:('para terminar pulse enter')");
						gets_s(input, sizeof(input));
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", FMENS, CRLF);
						}
						else
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", input, CRLF);
						break;
					case S_FINM:
						//sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", FMENS, CRLF);

						break;
					}

					if(estado!=S_WELC){ // y distinto del data
						enviados=send(sockfd,buffer_out,(int)strlen(buffer_out),0);
						if(enviados==SOCKET_ERROR){
							 estado=S_QUIT;
							 continue;
						}
					}  
					if (estado != S_MENS) {	// si estado es distinto del data
						recibidos = recv(sockfd, buffer_in, 512, 0);
						if (recibidos <= 0) {
							DWORD error = GetLastError();
							if (recibidos < 0) {
								printf("CLIENTE> Error %d en la recepción de datos\r\n", error);
								estado = S_QUIT;
							}
							else {
								printf("CLIENTE> Conexión con el servidor cerrada\r\n");
								estado = S_QUIT;
							}
						}else {
						
							switch (estado) {

							case S_WELC:
								buffer_in[recibidos] = 0x00;
								printf(buffer_in);
								if (strncmp(buffer_in, OK, 3) == 0) {

									estado++;

								}
								else estado = S_WELC;

								break;

							case S_HELO:
								buffer_in[recibidos] = 0x00;
								printf(buffer_in);
								if (strncmp(buffer_in, OKW, 3) == 0) {

									estado++;

								}
								else estado = S_HELO;

								break;

							case S_MAIL:
								buffer_in[recibidos] = 0x00;
								printf(buffer_in);
								if (strncmp(buffer_in, OKW, 3) == 0)
								{
									estado++;
								}
								else estado = S_MAIL;



								break;
							case S_RCPT:
								buffer_in[recibidos] = 0x00;
								printf(buffer_in);
								if (strncmp(buffer_in, OKW, 3) == 0)
								{
									estado++;
								}
								else estado = S_RCPT;


								break;
							case S_DATA:
								buffer_in[recibidos] = 0x00;
								printf(buffer_in);
								if (strncmp(buffer_in, OKDATA, 3) == 0)
								{
									estado++;
								}
								else estado = S_MENS;
								break;
							case S_FINM:
								buffer_in[recibidos] = 0x00;
								printf(buffer_in);
								if (strncmp(buffer_in, OKW, 3) == 0)
								{
									estado = S_MAIL;
								}
								break;
							}
                        }
						// maquina de estados
						//buffer_in[recibidos]=0x00;
						//printf(buffer_in);
						//if(estado!=S_DATA && strncmp(buffer_in,OK,2)==0) 
							//estado++;  
					}

				}while(estado!=S_QUIT);		
			}
			else{
				int error_code=GetLastError();
				printf("CLIENTE> ERROR AL CONECTAR CON %s:%d\r\n",ipdest,TCP_SERVICE_PORT);
			}		
			// fin de la conexion de transporte
			closesocket(sockfd);
			
		}	
		printf("-----------------------\r\n\r\nCLIENTE> Volver a conectar (S/N)\r\n");
		option=_getche();

	}while(option!='n' && option!='N');

	WSACleanup();
	return(0);
}
