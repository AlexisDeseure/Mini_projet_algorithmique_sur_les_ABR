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

#endif //TP4_TP4_H
