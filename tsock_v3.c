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


void construire_message(char *message, char motif, int lg) { 
	int i;
	for (i=0;i<lg;i++){
		message[i] = motif;
	}
} 

void afficher_chaine(char *message, int lg) { 
	int i; 
	for (i=0;i<lg;i++){
		printf("%c", message[i]); 
	}
}

int longueur_nombre(int Nombre) {
  int lg=0;
  int leNombre = Nombre; 
  if (leNombre>=0) {  
    while (leNombre >= 1) {
      leNombre = leNombre/10;
      lg++;
    }
  }
  else {
    printf("Le numero du message ne doit pas etre negatif\n");
    exit(1);
  } 
  return lg;
}
void entete_message(int num_recepteur) {
  if (num_recepteur < 99999){
    int lg = 5-longueur_nombre(num_recepteur);
    int i;
    for (i=0;i<lg;i++){
      printf("-");
    }
    printf("%d",num_recepteur);
  }
  else {
    printf("le numero du recepteur doit etre inferieur a 99999\n");
    exit(1);
  }
}

void message_identification(char *identification, int i,  int nb_message, int lg_message, int num_recepteur){
	identification[0] = i;
	identification[1] = nb_message;
	identification[2] = lg_message;
	identification[3] = num_recepteur;
	}	

void message_identification_bal(char* identification_bal, int nb_message){
	int i;
	for(i=0; i<nb_message; i++){
		identification_bal[i]=0;
	} 
}


void afficher_message(char *message, int lg_message, int num_recepteur){	
	printf("[");
	entete_message(num_recepteur);
	afficher_chaine(message, lg_message);
	printf("]\n");
}

int ouvrir_socket_tcp() {
	int sock;
  	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    		printf("échec lors de la création du socket TCP\n");
    		exit(1);
  	}
	return sock;
}

void fermer_socket(int sock) {
  	if (close(sock) == -1) {
    		printf("échec lors de la fermeture du socket\n");
    		exit(1);
  	}
}
  
void emetteur(char *adresse, int port, int nb_message, int lg_message, int num_recepteur) {
	int sock = ouvrir_socket_tcp();
	struct hostent *hp = NULL;
	struct sockaddr_in adr_distant;
	memset((char *)&adr_distant, 0, sizeof(adr_distant));
	adr_distant.sin_family = AF_INET; 
	adr_distant.sin_port = htons(port); 
	if ((hp = gethostbyname(adresse)) == NULL){
		printf("Erreur gethostbyname\n");
		exit(1);
	}
	memcpy((char*)&(adr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);
	if (connect(sock,(struct sockaddr *)&adr_distant,sizeof(adr_distant)) == -1){
		perror("error connect \n");
		exit(1);
	}
	char* identification = malloc(lg_message*sizeof(char));
	message_identification(identification,1,nb_message,lg_message, num_recepteur);
	if (write(sock, identification, 4) == -1){
			perror("echec de l'envoi du messsage d'identification\n"); 
			exit(1);
		}

	char *message = malloc(lg_message*sizeof(char));
	int i;
	for (i=0;i<nb_message;i++) {
	  	construire_message(message, 65+(i%26),lg_message);
		printf("Emetteur : envoi du message n° %d au recepteur n°%d",i+1, num_recepteur);
		afficher_message(message, lg_message,num_recepteur);
		if (write(sock, message, lg_message) == -1){
			perror("echec de l'envoi\n"); 
			exit(1);
		}
		
  	}
  	printf("SOURCE : Fin des envois!\n");
    shutdown(sock,1);
	fermer_socket(sock);
	free(message);
}

void recepteur (char *adresse, int port, int num_recepteur, int lg_message) {
	int sock = ouvrir_socket_tcp();
	struct hostent *hp = NULL;
	struct sockaddr_in adr_distant;
	memset((char *)&adr_distant, 0, sizeof(adr_distant));
	adr_distant.sin_family = AF_INET; 
	adr_distant.sin_port = htons(port); 
	if ((hp = gethostbyname(adresse)) == NULL){
		printf("Erreur gethostbyname\n");
		exit(1);
	}

	memcpy((char*)&(adr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);
	if (connect(sock,(struct sockaddr *)&adr_distant,sizeof(adr_distant)) == -1){
		perror("error connect \n");
		exit(1);
	}

	char* identification = malloc(sizeof(char));
	message_identification(identification,0,0,0,num_recepteur);
	if (write(sock, identification, 4) == -1){
			perror("echec de l'envoi du messsage d'identification\n"); 
			exit(1);
		}

	int nb_message;  
	char *identification_bal = malloc((sizeof(1))); 
	nb_message = read(sock,identification_bal,lg_message);
	char *message = malloc((sizeof(char))*lg_message);
	int i;
	for (i=0; i<nb_message; i++){
		if ((lg_message=read(sock, message, lg_message))<0){
			printf("échec du read \n");
			exit(1);
			}
		printf("Recepteur : Reception n° %d (%d)",i+1, lg_message);
		afficher_message(message,lg_message, num_recepteur);
		printf("\n");	
			}
	printf("Recepteur : Fin de la reception\n");
	fermer_socket(sock);
}	



void BAL(int port) {
	int sock, sock_bis;
	int nb_max;
	sock = ouvrir_socket_tcp();
	struct sockaddr_in adr_local;
	memset((char *)&adr_local, 0, sizeof(adr_local));
	adr_local.sin_family = AF_INET;
	adr_local.sin_port = htons(port);
	adr_local.sin_addr.s_addr = INADDR_ANY; 
	if(bind(sock, (struct sockaddr *)&adr_local, sizeof(adr_local)) == -1){
		printf("Echec du bind\n");
		exit(1);
	}
	nb_max = 10;
	listen(sock,nb_max);
	struct sockaddr *padr_em;
	padr_em=malloc(sizeof(struct sockaddr));
	int *plg_adr_em;
	plg_adr_em=malloc(sizeof(int));
		if ((sock_bis = accept(sock,padr_em,plg_adr_em)) == -1)
		{
			printf("échec du accept \n");
			exit(1);
			}
			char *identification = malloc((sizeof(4))); 
			int lg_message_id;
			if ((lg_message_id=read(sock_bis, identification, sizeof(4)))<0){
							printf("échec du read \n");
							exit(1);
							}
						printf("BAL : Reception message identification");
						int em_re=identification[0];
						int nb_message=identification[1];
						int lg_message=identification[2];
						int num_recepteur=identification[3];	
						printf("%d,%d,%d, %d \n", em_re,nb_message,lg_message,num_recepteur);
	
			char *message = malloc((sizeof(char))*lg_message);  
			switch(em_re){
				int i;
				case -1 :
					printf("erreur de source \n");
					exit(1);
				case 1 :					
					for (i=0; i<nb_message; i++){
						if ((read(sock_bis, message, lg_message))<0){
							printf("échec du read \n");
							exit(1);
							}
						printf("BAL : reception du message n° %d pour le recepteur n°%d",i+1, num_recepteur);
						afficher_message(message,lg_message,num_recepteur);
						int num_recepteur;
						num_recepteur = message[4];
						} 
						exit(0);
				case 0 : 
					printf(" ");
					char* identification_bal = malloc(sizeof(char));
					message_identification_bal(identification_bal,nb_message);
					write(sock,message_identification_bal,sizeof(identification_bal));
					exit(0);

				default :
					close(sock_bis);
				}				
	printf("BAL : Fermeture de la boite aux lettres\n");
	fermer_socket(sock);
}	




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
      printf("usage: cmd [-r|-e][-n ##][-l##]\n");
      break;
    }
  }

  port = atoi(argv[argc-1]);
  if (source == 1) {
		  adresse = argv[argc-2];
	}
	
	
	
	switch (source) {
	case 1:		
			printf("SOURCE : lg_mesg_emis : %d, port : %d, nb_envois : %d, dest : %s \n",lg_message, port, nb_message, adresse);
			break;

	case 0:	
			printf("PUIT : lg_mesg_recu : %d, port : %d, nb_reception : %d\n",lg_message, port, nb_message);
			break;
	case 2: 
			printf("BAL : lg_mesg_recu : %d, port : %d, nb_reception : %d,\n",lg_message, port, nb_message);
			break;
	}


	    if (source==1){
	    	emetteur(adresse, port, nb_message, lg_message, num_recepteur);
	    }
	    if (source==0){
	        recepteur(adresse, port, num_recepteur, lg_message);
	    }
		if (source==2){
			BAL(port);
		}
}
