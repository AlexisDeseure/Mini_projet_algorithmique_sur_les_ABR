#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp4.h"


int main() {
    /*char a[92] = "AghhgftvfvgGYUFYTyuyufg545645645+-+-+-...A,,,;;:;;,;:,zeygfyuzYGYUGUYGYUuyidfFTGgvhbjhjghjA";
    char a2[99];
    strcpy(a2,a);
    ignorerCasse(a);
    printf("mot : %s\n", a);
    printf("mot : %s\n", a2);*/
    T_Index *index = creerIndex();
    printf("%d\n",indexerFichier(index, "../KEVIN.txt"));
    afficherIndex(*index);
    printf("\n%d,%d",index->nbMotsTotal,index->nbMotsDistincts);
    T_Noeud *n = rechercherMot(*index, "B");
    if (n == NULL){
        printf("\nnon");
    }
    else{
        printf("\noui");
    }
    return 0;
}
