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

#ifndef bal_h
 #include "bal.h"
#endif

#ifndef message_h
 #include "message.h"
#endif 


//---Initialisation des boites aux lettres----

LISTE_BAL * init_BAL(){
    LISTE_BAL* liste =(LISTE_BAL*)malloc(sizeof(struct tete));
	liste->courant=NULL;
	liste->premier=NULL;
	liste->dernier=NULL;
	int nb = 0; 
	return liste;
}

//----Afficher le contenu d'une Liste de BAL------

void afficherLISTE(LISTE_BAL* liste)
{
    printf("              __________________________________________\n");
    printf("                Check général des BAL de notre liste :\n\n");
    printf("                        %d BAL dans notre liste       \n\n",liste->nb);
    liste->courant=liste->premier;
    while (liste->courant!=NULL)
    {
    printf("                        BAL n°%d : %d Lettres          \n",liste->courant->num,liste->courant->nb);
        liste->courant=liste->courant->suiv;
    }
    printf("              __________________________________________\n\n");

} 

//--------Afficher le contenu d'une BAL-----------
void afficherBAL(BAL* bal,int lg)
{
    printf("Contenu de la BAL n°%d qui contient %d lettres \n",bal->num,bal->nb) ;
    bal->l_courant=bal->l_premier;
    printf("\n");
    int n=1;
    while(bal->l_courant!=NULL)
    {
        printf("BAL n°%d | %d Lettres, lettre n°%d : [",bal->num,bal->nb,n);
        afficher_message(bal->l_courant->message,lg,bal->num);
        bal->l_courant=bal->l_courant->suiv;
        n++;
    }
    printf("\n\n");

}

//----------------Ajouter une BAL-----------------
void ajouter_BAL(int n, LISTE_BAL * liste){
	BAL *nouv =malloc(sizeof(struct BAL));
    nouv->num=n;
    nouv->nb=0;
    nouv->l_premier=NULL;
    nouv->l_dernier=NULL;
    nouv->l_courant=NULL;
    nouv->suiv=NULL;

    if (liste->premier == NULL)
    {
        liste->premier = nouv ;
        liste->dernier = nouv ;
    }
    else
    {
        liste->dernier->suiv= nouv ;
        liste->dernier=nouv ;
    }
    liste->nb++;
}

//----------------Ajouter une lettre en fin de BAL-----------------
void ajouter_lettre(int n, int lg, BAL* bal, char * mess){
	  bal->nb=(bal->nb)+1;
	lettre* nouv;
	nouv=(lettre*)malloc(sizeof(lettre));
	nouv->num=n+1;
	nouv->lg=lg;
	nouv->suiv=NULL;

	if (bal->l_premier==NULL)
	{
        bal->l_premier=nouv;
        bal->l_dernier=nouv;
        bal->l_courant=nouv;
    }

	else
    {
	    bal->l_dernier->suiv=nouv;
        bal->l_dernier=bal->l_dernier->suiv;
    }
    nouv->message=malloc(lg* sizeof(char));
    for (int i=0 ; i<lg ; i++)
        nouv->message[i] = mess[i];
}

//----------------Détruit une liste de BAL en fin d'utilisation de BAL-----------------
void supprimer_bal( BAL * bal){
	 bal->l_courant=bal->l_premier;
    while(bal->l_courant!=NULL)
    {
        bal->l_courant=bal->l_courant->suiv;
        free(bal->l_premier);
        bal->l_premier=bal->l_courant;
        (bal->nb)--;
    }
}

//----Retourne une BAL en fonction de son num-----
BAL* trouver_BAL(LISTE_BAL*liste, int num){
    BAL* bal=malloc(sizeof(struct BAL));
    liste->courant=liste->premier;
    if (liste->premier==NULL)
    {
        ajouter_BAL(num,liste);
        bal=liste->premier;
    }
    else
    {
        liste->courant=liste->premier;

        if (liste->premier==liste->dernier)
        {
            if (liste->premier->num==num)
                bal=liste->courant;
            else
            {
                ajouter_BAL(num,liste);
                bal=liste->dernier;
            }
        }
        else if (liste->premier->num==num)
            bal=liste->premier;
        else
        {
            int var=0;
            while(var==0)
            {
                if (liste->courant->suiv==NULL)
                    var=-1;
                else
                {
                    liste->courant=liste->courant->suiv;
                    if (liste->courant->num==num)
                        var=1;
                    if (liste->courant==NULL)
                        var=-1;
                }

            }
            if (var==1)
                bal=liste->courant;
            else
            {
                ajouter_BAL(num,liste);
                bal=liste->dernier;
            }
        }
    }
    return bal;
}

//-------Retourne -1 si BAL inexistante ou BAL Vide, lg 1ère lettre si BAL existante----------
int taille_prem_l(LISTE_BAL*liste, int num){
    int ret;
    if (liste->premier==NULL)
    {
        ret=-1;
    }
    else
    {
        liste->courant=liste->premier;
        if (liste->courant==liste->dernier)
        {
            if (liste->courant->num==num)
            {
                if (liste->courant->l_premier==NULL)
                    ret=-1;
                else
                    ret=liste->courant->l_premier->lg;
            }
            else
                ret=-1;
        }
        else if (liste->premier->num==num)
        {
            if (liste->courant->l_premier==NULL)
                ret=-1;

            else
                ret=liste->courant->l_premier->lg;
        }
        else
        {
            int var=0;
            while(var==0)
            {
                if (liste->courant->suiv==NULL)
                    var=-1;
                else
                {
                    liste->courant=liste->courant->suiv;
                    if (liste->courant->num==num)
                        var=1;
                    if (liste->courant==NULL)
                        var=-1;
                }

            }
            if (var==1)
            {
                if (liste->courant->l_premier==NULL)
                    ret=-1;
                else
                    ret=liste->courant->l_premier->lg;
            }
            else
                ret=-1;
        }

    }
    return ret;
}


void emetteur(char *adresse, int port, int nb_message, int lg_message, int num_recepteur) {
	int sock = ouvrir_socket_tcp();  //Création socket
	struct hostent *hp;
	struct sockaddr_in adr_distant;
	memset((char *)&adr_distant, 0, sizeof(adr_distant));
	adr_distant.sin_family = AF_INET;   //Construction adresse socket distant
	adr_distant.sin_port = port; 
	char* identification = malloc(lg_message*sizeof(char));

	if ((hp = gethostbyname(adresse)) == NULL){ //Affectation IP
		printf("Erreur gethostbyname\n");
		exit(1);
	}
	memcpy((char*)&(adr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);
	if (connect(sock,(struct sockaddr *)&adr_distant,sizeof(adr_distant)) == -1){ //Demande de connexion
		perror("error connect \n");
		exit(1);
	}

	message_identification(identification,1,nb_message,lg_message, num_recepteur); //----------------Envoi PDU----------------
	if (write(sock, identification, 4) == -1){
			perror("echec de l'envoi du messsage d'identification\n"); 
			exit(1);
		}

	char *message = malloc(lg_message*sizeof(char));
	int i;
	for (i=0;i<nb_message;i++) {
	  	construire_message(message, 65+(i%26),lg_message);  //Création du message
		printf("Emetteur : envoi du message n° %d au recepteur n°%d",i+1, num_recepteur);
		afficher_message(message, lg_message,num_recepteur);
		if (write(sock, message, lg_message) == -1){    //Envoi du message

			perror("echec de l'envoi\n"); 
			exit(1);
		}
		
  	}
  	printf("SOURCE : Fin des envois!\n");
    shutdown(sock,1); //Fermeture connexion
	fermer_socket(sock);  //Fermeture Socket
	free(message);
}

void recepteur (char *adresse, int port, int num_recepteur, int lg_message, int nb_message) {

	int sock;
	struct hostent *hp;
	struct sockaddr_in adr_distant;
	memset((char *)&adr_distant, 0, sizeof(adr_distant));  //Construction adresse socket distant
	adr_distant.sin_family = AF_INET; 
	adr_distant.sin_port = port; 

	 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Erreur à l'ouverture du Socket Stream");
        exit(1);
    }
	if ((hp = gethostbyname(adresse)) == NULL){  //Affectation IP
		printf("Erreur gethostbyname\n");
		exit(1);
	}

	memcpy((char*)&(adr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);
	if (connect(sock,(struct sockaddr *)&adr_distant,sizeof(adr_distant)) == -1){ //Demande de connexion
		perror("error connect \n"); 
		exit(1);
	}

	char* identification = malloc(sizeof(char));
	message_identification(identification,0,0,0,num_recepteur);
	if (write(sock, identification, 4) == -1){
			perror("echec de l'envoi du messsage d'identification\n"); 
			exit(1);
		}

  
	char *identification_bal = malloc((sizeof(1))); 
	nb_message = read(sock,identification_bal,lg_message);
	printf("reception du message d'identification\n");
	printf("il y a %d messages dans la bal\n\n",nb_message);
	printf("messages de longueurs %d dans la bal\n\n",lg_message);
	char *message = malloc((sizeof(char))*lg_message);
	int i=0;
	printf("             PUITS : Réception du contenu de la BAL n°%d\n",num_recepteur);
    printf("____________________________________________________________________\n\n");
	while(i<nb_message){
		if ((lg_message=read(sock, message, lg_message))<0){
			printf("échec du read \n");
			exit(1);
			}
		printf("Recepteur : Reception n° %d (%d)",i+1, lg_message);
		afficher_message(message,lg_message, num_recepteur);
		printf("\n");	
		i++;
			}
	printf("Recepteur : Fin de la reception\n");
	fermer_socket(sock);
}	


void SBAL(int port) {
	int sock, sock_bis;
	int nb_max = 100;
	sock = ouvrir_socket_tcp();  //Création socket local
	struct sockaddr_in adr_local;
	memset((char *)&adr_local, 0, sizeof(adr_local));  //Construction adresse socket local | Affectation port et domaine
	adr_local.sin_family = AF_INET;
	adr_local.sin_port = port;
	adr_local.sin_addr.s_addr = INADDR_ANY; 
	BAL*bal=malloc(sizeof(struct BAL));
	LISTE_BAL* liste;
	struct sockaddr *padr_em;
	padr_em=malloc(sizeof(struct sockaddr));
	int *plg_adr_em;
	plg_adr_em=malloc(sizeof(int));

	if(bind(sock, (struct sockaddr *)&adr_local, sizeof(adr_local)) == -1){   //Bind
		printf("Echec du bind\n"); 
		exit(1);
	}
	printf("En attente de connexion\n");
	if (listen(sock,nb_max)<0)  //Check connexions entrantes
    {
        printf("Trop de connexions en attentes, échec de la demande\n");
        exit(1);
    }
	liste = init_BAL();
	printf("\n");
	
	while(1){  //RECEPTION DU PDU D'IDENTIFICATION
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
			printf("BAL : Réception du message identification :\n");
			int em_re=identification[0];
			int nb_message=identification[1];
			int lg_message=identification[2];
			int num_recepteur=identification[3];	
			printf("Emetteur(1)/Recepteur(0) :%d,\nNombre de messages : %d,\nLongueur des messsages : %d,\nRécepteur Concerné : %d \n\n", em_re,nb_message,lg_message,num_recepteur);
		
			char *message = malloc((sizeof(char))*lg_message);  

			switch(em_re){
				int i;
				case -1 :
					printf("erreur de source \n");
					break;

				case 1 :    //GESTION EMETTEUR
						
					bal = trouver_BAL(liste,num_recepteur);	
					printf("BAL : réception des messages !\n");
					for (i=0; i<nb_message; i++){
						if ((read(sock_bis, message, lg_message))<0){
							printf("échec du read \n");
							exit(1);
							}
							
						printf("BAL : reception du message n° %d pour le recepteur n°%d",i+1, num_recepteur);
						afficher_message(message,lg_message,num_recepteur);
						ajouter_lettre(i,lg_message,bal,message); 
						}
					printf("Tous les messages ont été stockés dans la BAL ! En attente des prochains...\n\n");
					i++;
					afficherBAL(bal,lg_message);		

					break;

				case 0 :     //GESTION RECEPTEUR
					printf("        ||||||| Restitution des lettres de la BAL n°%d |||||||\n\n",num_recepteur);
					
					lg_message = taille_prem_l(liste,num_recepteur);
					printf("lg messages : %d\n",nb_message);					
					
					if (lg_message==1) // Gestion du cas ou la BAL est vide
            			{
                		printf("        BAL inexistante        \n\n");
						printf("       Creation de la BAL      \n\n");
						}
					else{  
						i=0;
						bal = trouver_BAL(liste,num_recepteur);
						bal->l_courant=bal->l_premier;
						while(bal->l_courant!=NULL)
						{
							lg_message=bal->l_courant->lg;
							nb_message=bal->nb;
							char* identification_bal = malloc(sizeof(char));
							message_identification_bal(identification_bal,nb_message);
							write(sock_bis,identification_bal,sizeof(identification_bal));
							
							message=malloc(lg_message*sizeof(char));
                    		message=bal->l_courant->message;
							if ((write(sock_bis, message, lg_message))<0)
							{
								printf("échec du lors de l'envoi \n");
								exit(1);
								}
							printf("BAL n°%d : Restitution de la lettre n°%d (%d) [",num_recepteur,i,lg_message);
                    		afficher_message(message,lg_message,num_recepteur);
                    		bal->l_courant=bal->l_courant->suiv;
							i++;
						}	
						supprimer_bal(bal);
						shutdown(sock_bis,2);
					}		
					break;

				default :
					close(sock_bis);
					}	
	}			
		printf("BAL : Fermeture de la boite aux lettres\n");
		fermer_socket(sock);

}
