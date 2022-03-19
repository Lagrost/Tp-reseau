#define message_h 1

void construire_message(char *message, char motif, int lg);
void afficher_chaine(char *message, int lg);
void entete_message(int num_recepteur);
void message_identification(char *identification, int i,  int nb_message, int lg_message, int num_recepteur);
void message_identification_bal(char* identification_bal, int nb_message);
void afficher_message(char *message, int lg_message, int num_recepteur);
int ouvrir_socket_tcp();
void fermer_socket(int sock);



  
