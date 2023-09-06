#include <gtk/gtk.h>

typedef struct Entity{
    char *cle;
    GtkWidget *Refer;
    struct Entity **next;
}interfaceItem;

typedef struct{
    interfaceItem **Table;
    int taille;
    int NbElem;
}TableInterface;

interfaceItem *creerRef(char *cle, GtkWidget *ref);
TableInterface *creationTable(int taille);
void free_item(interfaceItem* item);
void free_table(TableInterface* table);
unsigned int hashFunction(char *id, int taille);
void insertionEntite(TableInterface *table, char *cle, GtkWidget *ref);
GtkWidget* chercherRef(TableInterface *table, char *id);


//creation de l'entite
interfaceItem *creerRef(char *cle, GtkWidget *ref)
{
	//creation de l'entite 
	interfaceItem *Entite = (interfaceItem*)malloc(sizeof(interfaceItem));
	Entite->cle = (char*)malloc(strlen(cle) + 1);

	printf("\nOK\n");
	//allocation pour la reference 
	Entite->Refer = (GtkWidget*)malloc(sizeof(GtkWidget));


	if (!Entite)
	{
		printf("\nAllocation Problem\n");
		exit(-1);
	}

	//insertion de l'entite 
	strcpy(Entite->cle, cle);
	Entite->Refer = ref;

	Entite->next = NULL;


	return Entite;
}


//creation de la table des references 
TableInterface *creationTable(int taille)
{
	//allocation dynamique 
	TableInterface *table = (TableInterface*)malloc(sizeof(TableInterface) * 1);

	//definition de la taille 
	table->taille = taille;
	table->NbElem = 0;
	table->Table = (interfaceItem**)malloc(sizeof(interfaceItem*) * taille);
	int i;
	//initialisation de contenu 
	for (i = 0; i < taille; i++)
		table->Table[i] = NULL;


	return table;
}

/**ces deux fonctions free_item et free_table
 *  ne sont pas necessaire dans le compte rendu 
*/

void free_item(interfaceItem* item) {
	// Frees an item
	free(item->cle);
	free(item->Refer);
	free(item);
}

void free_table(TableInterface* table) {
	// Frees the table
	for (int i = 0; i < table->taille; i++) {
		interfaceItem* item = table->Table[i];
		if (item != NULL)
			free_item(item);
	}

	free(table->Table);
	free(table);
}



//generation d'indice d'apres l id et la taille 
unsigned int hashFunction(char *id, int taille)
{
	unsigned long int valeur = 0;
	unsigned int i = 0;
	unsigned int len = strlen(id);

	for (i = 0; i < len; i++)
		valeur = valeur * 37 + id[i];

	valeur = valeur % taille;
	return valeur;
}


//insertion d'un reference
void insertionEntite(TableInterface *table, char *cle, GtkWidget *ref)
{

	//obtenir la cle d'apres l'id 
	unsigned int indice = hashFunction(cle, table->taille);
	printf("\n%d\n", indice);
	interfaceItem *Entite = table->Table[indice];

	if (Entite == NULL)
	{
		table->Table[indice] = creerRef(cle, ref);
		return;
	}


	//assurer les collisions 
	interfaceItem *prev=(interfaceItem*)malloc(sizeof(interfaceItem));
	prev->next = NULL;

	//s'il y a des colisions 
	while (Entite != NULL)
	{
		if (strcmp(table->Table[indice]->cle, cle) == 0)
		{
			free(Entite->Refer);
			Entite->Refer = (GtkWidget*)malloc(sizeof(GtkWidget));
			Entite->Refer = ref;
			return;
		}

		prev = Entite;
		Entite = Entite->next;
	}
	//creation de reference 
	prev->next = creerRef(cle, ref);

}


//chercher la refererence d'un id donne 
GtkWidget* chercherRef(TableInterface *table, char *id)
{
	//pbtenir l'indice 
	int indice = hashFunction(id, table->taille);
	printf("\nRecherche: %d\n", indice);

	//manipuler l'entite 
	interfaceItem *Entite = table->Table[indice];

	//s'il n'existe pas 
	if (Entite == NULL)
		return NULL;

	//s'il existe 
	while (Entite != NULL)
	{
		//on fait une gestion des collisions 
		if (strcmp(Entite->cle, id) == 0)
			return Entite->Refer;

		Entite = Entite->next;
	}

	return NULL;

}

/*
	quand on veut associer un Widget a des signaux on va
	faire
	GtkWidget *ref = chercherRef(TableApp, noued->id);

	Alors on va obtenir un reference s'il est different de
	NULL
	on va lui associer certain signal

	alors il faut assurer la recherche de l id dans la table
	qu'on a creer d'apres le fichier XML


	et chaque fois on a creer un widget on va enregistrer ce widget
	dans la TableInterface

	MOhamed AHRRASS
// */