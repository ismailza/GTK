//
// Created by Brahim Vall on 5/18/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "macros.c"


typedef struct ce
{
    struct ce *suivant;
    coordonnees val;
}cellulePliste;

cellulePliste *creer_cellulePliste(coordonnees elem)
{
    cellulePliste *ne;
    ne=(cellulePliste*)malloc(sizeof(cellulePliste));
    if(!ne)
    {
        printf("\nerreur d'allocation");
        exit(-1);
    }
    ne->val=elem;
    ne->suivant=NULL;
    return ((cellulePliste*)ne);
}

cellulePliste *InsererTetePListe(cellulePliste *Pliste ,coordonnees Elem)
{
    cellulePliste *NE;
    NE = creer_cellulePliste(Elem);
    NE->suivant= Pliste;
    return ((cellulePliste*)NE);
}

void afficher_pliste(cellulePliste *maliste)
{
    cellulePliste *courant;
    if(!maliste) printf("la liste n'existe pas");
    else{
        courant=maliste;
        while (courant)
        {
            printf("\n    l'abscisse:%d, et l'ordonnée:%d   ",courant->val.x,courant->val.y );
            courant = courant->suivant;
        }
    }
}
cellulePliste *listeEntour(int surface[60][60],int pas,coordonnees elem)
{
    cellulePliste *Liste=NULL;
    int i,j;
    coordonnees k;
    if(elem.x>59||elem.y>59||elem.x<0||elem.y<0) return NULL;


    for(i=0;i<=pas;i++)
    {
        if(elem.x+i<=59)
        {
            if((surface[elem.x+i][elem.y])==1)
            {
                k.x=elem.x+i;
                k.y=elem.y;
                Liste=InsererTetePListe(Liste,k);
            }
            for(j=1;j<=pas;j++){
                if(elem.y+j<=59)
                {
                    if(surface[elem.x+i][elem.y+j]==1)
                    {
                        k.x=elem.x+i;
                        k.y=elem.y+j;
                        Liste=InsererTetePListe(Liste,k);
                    }
                }
                if(elem.y-j>=0)
                {
                    if(surface[elem.x+i][elem.y-j]==1)
                    {
                        k.x=elem.x+i;
                        k.y=elem.y-j;
                        Liste=InsererTetePListe(Liste,k);
                    }
                }
            }
        }

        if(elem.x-i>=0)
        {
            if(surface[elem.x-i][elem.y]==1)
            {
                k.x=elem.x-i;
                k.y=elem.y;
                Liste=InsererTetePListe(Liste,k);
            }
            for(j=1;j<=pas;j++){
                if(elem.y-j>=0)
                {
                    if(surface[elem.x-i][elem.y-j]==1)
                    {
                        k.x=elem.x-i;
                        k.y=elem.y-j;
                        Liste=InsererTetePListe(Liste,k);
                    }
                }
                if(elem.y+j<=59)
                {
                    if(surface[elem.x-i][elem.y+j]==1)
                    {
                        k.x=elem.x-i;
                        k.y=elem.y+j;
                        Liste=InsererTetePListe(Liste,k);
                    }
                }
            }
        }
    }
    return ((cellulePliste*)Liste);
}