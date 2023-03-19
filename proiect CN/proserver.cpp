/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
	Intoarce corect identificatorul din program al thread-ului.


   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct thData{
	int idThread; //id-ul thread-ului tinut in evidenta de acest program
	int cl; //descriptorul intors de accept
	int viteza;//viteza de circulatie
	char LicensePlate[100];//placuta de inmatriculare
	char strada[100]; // strada pe care circula un client
	char plecare[100]; // punctul de plecare
	char destinatie[100]; //destinatia

}thData;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void *);

int main ()
{
  struct sockaddr_in server;	// structura folosita de server
  struct sockaddr_in from;
  int nr;		//mesajul primit de trimis la client
  int sd;		//descriptorul de socket
  int pid;
  pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
	int i=0;


  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));

  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client;
      thData * td; //parametru functia executata de thread
      socklen_t length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      // client= malloc(sizeof(int));
      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}

        /* s-a realizat conexiunea, se astepta mesajul */

	// int idThread; //id-ul threadului
	// int cl; //descriptorul intors de accept

	td=(thData*)malloc(sizeof(thData));
	td->idThread=i++;
	td->cl=client;
    //td->LicensePlate
    //td->strada
    //td->plecare
    //td->destinatie

	pthread_create(&th[i], NULL, &treat, td);

	}//while
};
static void *treat(void * arg)
{
    while(1){
		struct thData tdL;
		tdL= *((struct thData*)arg);
		struct thData loc;
		loc.cl=tdL.cl;
		loc.idThread=tdL.idThread;
		printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
		fflush (stdout);
		pthread_detach(pthread_self());




		//raspunde((struct thData*)arg,loc);

		int nr, i=0;
        int command;
	if (read (tdL.cl, &nr,sizeof(int)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");

			}


	if (loc.cl==4){
        printf("viteza este %d\n",loc.viteza);
        loc.viteza=60;
        printf("viteza este %d\n",loc.viteza);
        command++;
    }

    if (loc.cl==5){
        printf("viteza este %d\n",loc.viteza);
        loc.viteza=30;
        printf("viteza este %d\n",loc.viteza);
        command++;
    }

    strcpy(loc.destinatie,"destinatie");


	printf ("[Thread %d]Mesajul a fost receptionat...%d\n",loc.idThread, nr);

		      /*pregatim mesajul de raspuns */
	printf("[Thread %d]Trimitem mesajul inapoi...%d\n",loc.idThread, nr);

    printf("viteza este %d\n",loc.viteza);
    printf("LicensePlate este %s\n",loc.LicensePlate);
    printf("strada este %s\n",loc.strada);
    printf("plecare este %s\n",loc.plecare);
    printf("destinatie este %s\n",loc.destinatie);

		      /* returnam mesajul clientului */
	 if (write (tdL.cl, &loc.viteza, sizeof(int)) <= 0)
		{
		 printf("[Thread %d] ",loc.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",loc.idThread);





		}
		/* am terminat cu acest client, inchidem conexiunea */
		close ((intptr_t)arg);
		return(NULL);

};

/*
void raspunde(void *arg)
{
        int nr, i=0;
        int command;
	struct thData tdL;
	tdL= *((struct thData*)arg);
	if (read (tdL.cl, &nr,sizeof(int)) <= 0)
			{
			  printf("[Thread %d]\n",tdL.idThread);
			  perror ("Eroare la read() de la client.\n");

			}

    if (command==0){
        printf("viteza este %d\n",loc.viteza);
        loc.viteza=30;
        printf("viteza este %d\n",loc.viteza);
        command++;
    }


	printf ("[Thread %d]Mesajul a fost receptionat...%d\n",loc.idThread, nr);

		      //pregatim mesajul de raspuns
	printf("[Thread %d]Trimitem mesajul inapoi...%d\n",loc.idThread, nr);

    printf("viteza este %d\n",loc.viteza);
    printf("LicensePlate este %s\n",loc.LicensePlate);
    printf("strada este %s\n",loc.strada);
    printf("plecare este %s\n",loc.plecare);
    printf("destinatie este %s\n",loc.destinatie);

		      // returnam mesajul clientului
	 if (write (tdL.cl, &loc.viteza, sizeof(int)) <= 0)
		{
		 printf("[Thread %d] ",loc.idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
	else
		printf ("[Thread %d]Mesajul a fost trasmis cu succes.\n",loc.idThread);

}
*/
