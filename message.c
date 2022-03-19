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

#ifndef message_h
 #include "message.h"
#endif 

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