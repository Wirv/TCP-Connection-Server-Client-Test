// ServerTest.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &data);

	if (wsOk != 0)
	{
		cerr << "non puoi inizializzare il winsock!" << endl;
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "non posso creare il socket!" << endl;
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(1111);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

#pragma region NewPart-MultiClient
	fd_set master;

	FD_ZERO(&master);

	FD_SET(listening, &master);

	while (true)
	{
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{

			SOCKET sock = copy.fd_array[i];
			if (sock == listening)
			{
				// Accetta nuova Connessione
				SOCKET client = accept(listening, nullptr, nullptr);

				// Aggiungi nuova connessione alla lista di clients connessi
				FD_SET(client, &master);

				// Manda un messaggio di benvenuto al client connesso
				string welcomeMsg = "Benvenuto in chat!\r\n";
				send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
				

			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Ricevi messaggio
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop Client
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					// Manda messaggio a altri clients, e non ascoltare socket

					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != sock)
						{
							ostringstream ss;
							ss << "SOCKET #" << sock << ":" << buf << "\r\n";
							string strOut = ss.str();

							send(outSock, strOut.c_str(), strOut.size() + 1, 0);

						}
					}

				}
				
			}

		}

	}

	//Pulisci winsock
	WSACleanup();

	system("pause");
#pragma endregion
#pragma region OldPart-1Client
	/*sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "connessione alla port" << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connessione alla porta" << ntohs(client.sin_port) << endl;
	}

	closesocket(listening);

	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);

		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "errore in recv!" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnesso" << endl;
			break;
		}
		cout << string(buf, 0, bytesReceived) << endl;

		send(clientSocket, buf, bytesReceived + 1, 0);

	}

	closesocket(clientSocket);

	WSACleanup();

	system("pause");*/

#pragma endregion
}

