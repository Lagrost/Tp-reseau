
#ifndef message_h
 #include "message.h"
#endif
#define bal_h 2

typedef struct lettre {
	int num;
	int lg;
	char * message;
	struct lettre * suiv;
}lettre;

typedef struct BAL {
	int num;
	struct lettre * l_premier ;
    struct lettre * l_dernier ;
    struct lettre * l_courant ;
    struct BAL * suiv ;
	int nb;
}BAL;

typedef struct tete {
	struct BAL * courant;
	struct BAL * dernier;
	struct BAL * premier;
	int nb;
}LISTE_BAL;


LISTE_BAL * init_BAL();
void ajouter_BAL(int n, LISTE_BAL * liste);
void ajouter_lettre(int n, int lg, BAL* bal, char * mess);
void supprimer_bal( BAL * bal);
void emetteur(char *adresse, int port, int nb_message, int lg_message, int num_recepteur);
void recepteur (char *adresse, int port, int num_recepteur, int lg_message, int nb_message);
void SBAL(int port);
BAL* trouver_BAL(LISTE_BAL*liste, int num);
int taille_prem_l(LISTE_BAL*liste, int num);
void afficherLISTE(LISTE_BAL* liste);
void afficherBAL(BAL* bal,int lg);