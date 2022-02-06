/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>

void main (int argc, char **argv)
{
  char *nom_ordi=argv[argc-2];
  int sock;
  int c;
  extern char *optarg;
  extern int optind;
  int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
  int source = -1 ; /* 0=puits, 1=source */
  int udp = 0;

  void construire_message (char *message, char motif, int lg) {
    int i;
    for (i=0 ; i<lg; i++) message[i] = motif; }

  void afficher_message (char *message, int lg) {
    int i;
    printf("message construit : ");
    for (i=0 ; i<lg ; i++) printf("%c", message[i]) ; printf("\n"); }

  
  void puit_udp(nb_message) {
    sock=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in * adr_local= malloc(sizeof(struct sockaddr_in));
    adr_local->sin_family = AF_INET;
    adr_local->sin_port = atoi(argv[argc-1]);
    adr_local->sin_addr.s_addr = INADDR_ANY;
	  
    bind(sock,adr_local,sizeof(*adr_local));
    for(int I;I<nb_message;I+1)
      {
	char *mess=malloc(30);
	struct sockaddr *padr_em;
	int *plg_adr_em=sizeof(padr_em);
	recvfrom(sock,mess,30,0,padr_em,plg_adr_em);
	printf(mess);
      }
    close(sock);
  }
	
  void source_udp(nb_message,nom_ordi) {
	
    sock=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in * adr_dest= malloc(sizeof(struct sockaddr_in));
    adr_dest->sin_family = AF_INET;
    adr_dest->sin_port = atoi(argv[argc-1]);
    adr_dest->sin_addr.s_addr = gethostbyname(nom_ordi);
    char motif;
    int i;
    char *mess=malloc(30);
    for(i=0;i<nb_message;i+1);
    {
      motif = (char)((int)'a'+i %26);
      struct sockaddr *padr_em;
      int *plg_adr_em=sizeof(padr_em);	
      construire_message(mess,motif,30);
      sendto(sock,mess,30,0,adr_dest->sin_addr.s_addr,sizeof(adr_dest->sin_addr.s_addr));
      }
    close(sock);
  }

  
  while ((c = getopt(argc, argv, "pun:s")) != -1) {
    switch (c) {

        
    case 'p':
      if (source == 1) {
	printf("usage: cmd [-p|-s][-n ##]\n");
	exit(1);
      }
      source = 0;
      break;

    case 's':
      if (source == 0) {
	printf("usage: cmd [-p|-s][-n ##]\n");
	exit(1) ;
      }
      source = 1;
      break;
			
    case 'n':
      nb_message = atoi(optarg);
      break;
			
    case 'u':
      udp = 1;
      break;
		    	        

    default:
      printf("usage: cmd [-p|-s][-n ##]\n");
      break;
    }
  }

  if (source == -1) {
    printf("usage: cmd [-p|-s][-n ##]\n");
    exit(1) ;
  }

  if (source == 1){
    printf("on est dans le source\n");
    if (udp == 1)
      source_udp(nb_message,nom_ordi);
  }
  else {
    printf("on est dans le puits\n");
    if (udp == 1)
      puit_udp(nb_message);
  }

  if (nb_message != -1) {
    if (source == 1)
      printf("nb de tampons à envoyer : %d\n", nb_message);
    else
      printf("nb de tampons à recevoir : %d\n", nb_message);
  }
  else {
    if (source == 1) {
      nb_message = 10 ;
      printf("nb de tampons à envoyer = 10 par défaut\n");
    } else
      printf("nb de tampons à envoyer = infini\n");

  }
	   
}

