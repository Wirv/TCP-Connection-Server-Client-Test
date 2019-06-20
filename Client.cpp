// ImoryChat.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
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

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL); // creazione socket
	
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //controllo errori
	{
		MessageBoxA(NULL, "Connection error", "ERROR", MB_OK | MB_ICONERROR); // finestra mess. errore
	}
	else
	cout << "Connected" << endl;

	char messaggio[256];
	recv(Connection, messaggio, sizeof(messaggio), NULL); // ricevi messaggio dal server

	cout << messaggio << endl;

	system("pause");
	return 0;
}

