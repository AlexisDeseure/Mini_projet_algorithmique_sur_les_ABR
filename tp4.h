#ifndef TP4_TP4_H
#define TP4_TP4_H

//============  Structures  ============

// Principales
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

//Supplémentaires
typedef struct Pile {
    int N;
    T_Noeud* noeud;
    struct Pile* suivant;
}Pile;

typedef struct Mot { //stocke les mots des phrase dans l'ordre dans lequel ils apparaissent dans la phrase
    int numeroLigne;
    int ordre;
    char* nom;
    struct Mot* suivant;
}Mot;

typedef struct Phrase { //stocke les phrases et leur numéro associé
    int numero;
    Mot* listeMot;
    struct Phrase* suivant;
}Phrase;


//===========  Prototypes  ===========

// Fonctions principales

// Ajouter un élément dans une liste de positions triées
T_Position *ajouterPosition(T_Position *listeP, int ligne, int ordre, int phrase);
// Ajouter un mot dans l'index en respectant les règles d'insertion dans les ABR
int ajouterOccurence(T_Index *index, char *mot, int ligne, int ordre, int phrase);
// Indexer un fichier texte
int indexerFichier(T_Index *index, char *filename);
// Afficher la liste des mots classés par ordre alphabétique
void afficherIndex(T_Index index);
// Rechercher un mot dans un index
T_Noeud* rechercherMot(T_Index index, char *mot);
// Afficher l’ensemble des phrases dans lesquelles se trouve un mot
void afficherOccurencesMot(T_Index index, char *mot);
// Construire un texte à partir de son index
void construireTexte(T_Index index, char *filename);

// Fonctions d'initialisation
T_Position* creerPosition(int ligne, int ordre, int phrase);
T_Noeud * creerNoeud(char* mot, int ligne, int ordre, int phrase);
T_Index* creerIndex();
Pile* creerPile(T_Noeud* noeud, int N);
Mot* creerMot(int ordre,int ligne,char* mot);
Phrase* creerPhrase(int n);

// Fonctions annexes

// Vider le buffer
void viderBuffer();
// Insérer et créer les mots et phrases nécessaires
Phrase* ajouterPhrase(Phrase** phrase, int n);
void ajouterMot(Phrase** phrase, int numeroLigne, int ordre, char* nom, int numeroPhrase);
// Empiler et dépiler un élément d'une pile (implémentée sous forme d'une liste chaînée)
Pile *depiler(Pile** pile);
void empiler(Pile** pile,T_Noeud* noeud, int N);
// Mettre un mot en minuscule
void ignorerCasse(char* mot);
// Libérer la mémoire allouée pour l'index et tous les éléments associés
void libererIndex(T_Index* index);
void libererNoeud(T_Noeud* noeud);
void libererPosition(T_Position* position);

#endif //TP4_TP4_H
