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
/* fonctions de BAL */


#ifndef message_h
 #include "message.h" //GESTION DES MESSAGES ENVOYES ET DES MESSAGES D'IDENTIFICATION
#endif


#ifndef bal_h
 #include "bal.h" //GESTION DES BOITES AUX LETTRES
#endif 



void main (int argc, char **argv)
{
	int c;
	int n = 0;
	extern char *optarg;
	extern int optind;
	int nb_message = 10; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int lg_message = 30; /* Longueur des messages à envoyer ou à recevoir, par défaut : 30 en émission et en réception */
	int source = -1 ; /* 0=emetteur, 1=recepteur , 2=BAL*/
	char * adresse;
	int port;
	int num_recepteur;

  
  while ((c = getopt(argc, argv, "n:l:e:r:b")) != -1) {
    switch (c) {
    case 'r':
      source = 0;
	    num_recepteur = atoi(optarg);
      break;

    case 'e':
      source = 1;
	    num_recepteur = atoi(optarg);
      break;
			
    case 'n':
      nb_message = atoi(optarg);
      break;
	  
    case 'l':
      lg_message = atoi(optarg);   
      break;

  	case 'b':
      source = 2;
      break;
	
    default:
      printf("usage: cmd [-r|-e|-b][-n ##][-l##][adresse][numéro de port]\n");
      break;
    }
  }

  port = atoi(argv[argc-1]);
  if (source == 1) {
		  adresse = argv[argc-2];
	}

	    if (source==1){
        printf("____________________________________________________________________\n\n");
        printf("                             EMETTEUR BAL                           \n");
        printf("____________________________________________________________________\n\n");
	    	emetteur(adresse, port, nb_message, lg_message, num_recepteur);
	    }
	    if (source==0){
        adresse = argv[argc-2];
        printf("____________________________________________________________________\n\n");
        printf("                            RECEPTEUR BAL                           \n");
        printf("____________________________________________________________________\n\n");
	      recepteur(adresse, port, num_recepteur, lg_message, nb_message);
	    }
		  if (source==2){
        printf("____________________________________________________________________\n\n");
        printf("                             SERVEUR BAL                            \n");
        printf("____________________________________________________________________\n\n");
			  SBAL(port);
		  }
      exit(0);
}
