#ifndef TP4_TP4_H
#define TP4_TP4_H
typedef struct Position{
    int numeroLigne;
    int ordre;
    int numeroPhrase;
    struct Position* suivant;
}T_Position;

typedef struct Noeud{
    char* mot;
    int nbOccurences;
    T_Position *ListePositions;
    struct Noeud* filsGauche;
    struct Noeud* filsDroite;
}T_Noeud;

typedef struct Index{
    T_Noeud* racine;
    int nbMotsDistincts;
    int nbMotsTotal;
}T_Index;

typedef struct Pile {
    int N;
    T_Noeud* noeud;
    struct Pile* suivant;
}Pile;

Pile* creerPile(T_Noeud* noeud, int N);
Pile *depiler(Pile** pile);
void empiler(Pile** pile,T_Noeud* noeud, int N);
//fonctions d'initialisation
T_Position* creerPosition(int ligne, int ordre, int phrase);
T_Noeud * creerNoeud(char* mot, int ligne, int ordre, int phrase);
T_Index* creerIndex();
void ignorerCasse(char* mot);
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase);

int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase);

int indexerFichier(T_Index *index, char *filename);

void afficherIndex(T_Index index);

T_Noeud* rechercherMot(T_Index index, char *mot);

void afficherOccurencesMot(T_Index index, char *mot);

void construireTexte(T_Index index, char *filename);




#endif //TP4_TP4_H
