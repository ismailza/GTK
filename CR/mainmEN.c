#include <stdio.h>
#include <string.h>

typedef struct ar{
    char nom[25];
    struct ar *fils;
    struct ar *frere;
}Arbre;


Arbre *creerCel(char nom[25])
{
    Arbre *cell =(Arbre*)malloc(sizeof(Arbre));
    strcpy(cell->nom, nom);
    cell->fils = NULL;
    cell->frere =NULL;
    return cell;
}



// Arbre *Transfer_froma(Node *racine, Arbre *menuMacr)
// {
//     if(racine)
//     {
//         menuMacr = creerCel(racine->List->Value);
//         menuMacr->frere = Transfer_froma(racine->freres, menuMacr->frere);
//         menuMacr->fils = Transfer_froma(racine->fils, menuMacr->fils);
//     }
// }


void affichage_arbre(Arbre *test, int spac)
{
    if (test)
	{
		printf("\n%*cnom: %s", spac * 2, '-', test->nom);

		affichage_arbre(test->fils, spac + 2);
		affichage_arbre(test->frere, spac);
	}
}


int main(int argc, char const *argv[])
{
    
    Arbre *hi; 

    hi = creerCel("File");
    hi->frere = creerCel("Edit");
    hi->fils = creerCel("New");
    hi->frere->fils = creerCel("Pdf");
    
    affichage_arbre(hi, 1);
    printf("\n");

    return 0;
}
