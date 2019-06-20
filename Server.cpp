// ImoryChatServer.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include "pch.h"
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>

using namespace std;

int main()
{
	WSADATA wsaData; // Oggetto che da tutte le info generali per l'oggetto winsock
	WORD Dllversion = MAKEWORD(2, 2); // Versione del socket che usiamo

	if (WSAStartup(Dllversion, &wsaData) != 0) // controllo errori
	{
		MessageBoxA(NULL, "Startup error", "ERROR", MB_OK | MB_ICONERROR); // finestra mess. errore

		exit(1); // chiude l'applicazione con errore
	}

	SOCKADDR_IN addr; //indirizzo ip
	int sizeofaddr = sizeof(addr); // lunghezza in int dell'indirizzo

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip
	addr.sin_port = htons(1111); // porta
	addr.sin_family = AF_INET; // tipo di connessione - in questo caso ipv4

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); // socket
	bind(sListen, (SOCKADDR*)&addr, sizeofaddr); // legamento
	listen(sListen, SOMAXCONN); // ascolto - SOMAXCONN = numero massimo di richieste di connessione contemporanee

	SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); // nuova connessione
	
	if (newConnection == 0)
	{
		cout << "No connections... " << endl;
	}
	else 
	{ 
		cout << "Connected" << endl;
	    char messaggio[256] = "messaggio"; // array di caratteri
	    send(newConnection, messaggio, sizeof(messaggio), NULL);
	}

	system("pause");
	return 0;
}

