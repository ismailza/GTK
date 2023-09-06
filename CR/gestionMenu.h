#include"gestionFrame.h"
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

void affichage_arbre(Arbre *test, int spac)
{
    if (test)
	{
		printf("\n%*cnom: %s", spac * 2, '-', test->nom);

		affichage_arbre(test->fils, spac + 2);
		affichage_arbre(test->frere, spac);
	}
}


Arbre *Transfer_froma(Node *racine, Arbre *menuMacr)
{
    if(racine)
    {
        menuMacr = creerCel(racine->List->Value);
        menuMacr->frere = Transfer_froma(racine->freres, menuMacr->frere);
        menuMacr->fils = Transfer_froma(racine->fils, menuMacr->fils);
    }
	return menuMacr;
	
}


void creat_item(GtkWidget *nn, char nom[25], menu*me) {
	me->menu_item = gtk_menu_item_new_with_label(nom);
	gtk_menu_shell_append(GTK_MENU_SHELL(nn), me->menu_item);

}

void creat_champ(GtkWidget * menu_cha, char nom[25], menu*me) {

	me->menu_champ = gtk_menu_new();
	me->menu_item = gtk_menu_item_new_with_label(nom);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(me->menu_item), me->menu_champ);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_cha), me->menu_item);

}



void create_champs_items(menu*me, Arbre*tmp)
{
	if(tmp)
	{
		if(tmp->fils){
			creat_champ(me->menu_champ, tmp->nom, me);
			create_champs_items(me, tmp->fils);
		}
			
		else	
		{
			creat_item(me->menu_champ, tmp->nom, me);
		
		}
		create_champs_items(me, tmp->frere);
			
	}
}


void creat_menu_macros(menu*me, Arbre*tmp) 
{ 
	Arbre *crt = tmp;
	
	while (crt)
	{
		creat_champ(me->menu_barre, crt->nom, me);

		create_champs_items(me, crt->fils);
		crt = crt->frere;
	}

}

GtkWidget* creat_menu_from_node(Node*racine,TableInterface*inter)
{
	menu me;
	me.menu_barre = gtk_menu_bar_new();
	Arbre *MenuMacr = NULL;
	MenuMacr = Transfer_froma(racine->fils, MenuMacr);
	affichage_arbre(MenuMacr, 1);

	creat_menu_macros(&me, MenuMacr);

	
	return((GtkWidget*)me.menu_barre);
}