#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Pr
{
	char name[20];
	char *Value;
	struct Pr*next;
}Prop;

typedef struct Nd
{
	char id[20];
	char classBalise[20];
	Prop*List;
	struct Nd*fils;
	struct Nd*freres;
}Node;
void trans_tir_to_blanc(char *val)
{
	int i = 0;
	for (i = 0; val[i] != '\0'; i++)
		if (val[i] == '~') val[i] = ' ';
}
Prop*Create_Prop(char name[20], char* Value);
Node *Create_Node(char id[20], char classBalise[20]);
Node *start_Reading(FILE*File, Node*Racine);
Node *insert_in_list_Child(Node*list, Node*newN);
Prop*insert_in_list_props(Prop*list,char name[20], char *Value);
void ReadBlancs_OpenBalise(FILE*file);
void close_balise(FILE*file);
void Read_childEnd(FILE*file);
int One_prop_read(FILE*file, Node*Racine);
void PropReader(FILE*file, Node*Racine);
Node *Object_reader(FILE*file,Node*Racine);
Node *start_Reading(FILE*File,Node*Racine);
void Affichage(Node* nd, int spac, int *nbOb);
Node* Reader_XML(char*nomfich);

void Affichage(Node* nd, int spac, int *nbOb)
{
	if (nd)
	{
		Prop *crt = nd->List;
		printf("\n%*cobject.class: %s, #id: %s", spac * 2, '-', nd->classBalise, nd->id);
		printf("\n%*cProps: ", spac * 2, '-');
		*nbOb += 1;
		while (crt)
		{
			printf(" name: %s / valeur: %s, ", crt->name, crt->Value);
			crt = crt->next;
		}

		Affichage(nd->freres, spac, nbOb);
		Affichage(nd->fils, spac + 2, nbOb);

	}
}


Prop*Create_Prop(char name[20], char* Value)
{
	Prop*Ne;
	Ne = (Prop*)malloc(sizeof(Prop));
	if (!Ne)
	{
		printf("no enough memory");
		exit(-1);
	}

	strcpy(Ne->name, name);
	int T = strlen(Value);
	Ne->Value = (char*)malloc((T + 1) * sizeof(char));
	strcpy(Ne->Value, Value);
	Ne->next = NULL;
	return((Prop*)Ne);
}
Node*Create_Node(char id[20], char classBalise[20])
{
	Node*Ne;
	Ne = (Node*)malloc(sizeof(Node));
	if (!Ne)
	{
		printf("no enough memory");
		exit(-1);
	}
	strcpy(Ne->id, id);
	strcpy(Ne->classBalise, classBalise);
	Ne->fils = NULL;
	Ne->freres = NULL;
	Ne->List = NULL;
	return((Prop*)Ne);
}
/*---------------------------------------------------------
le nom de la fonction: insert_in_list_Child.
les entrées: Node*list:la list des fils.
			 Node*newN:le nud a insérer dans la list des 
					   frères.
les sorties: Node*list:la list des fils après 
					   linsertion de nouveau nud. 
le role: elle permet d'inserer un nud dans la liste.
----------------------------------------------------------*/
Node*insert_in_list_Child(Node*list, Node*newN)
{
	if (!list)return((Node*)newN);
	Node*crt = list;
	while (crt->freres)crt = crt->freres;
	crt->freres = newN;
	return((Node*)list);
}//fin de la fonction Node*insert_in_list_Child(Node*list, Node*newN)
Prop*insert_in_list_props(Prop*list, char name[20], char *Value)
{
	Prop*Newd = Create_Prop(name, Value);
	if (!list)return((Prop*)Newd);
	Prop*crt = list;
	while (crt->next)crt = crt->next;
	crt->next = Newd;
	return((Prop*)list);
}//fin de la fonction Prop*insert_in_list_props(Prop*list, char name[20], char *Value)
void Read_Comment(FILE*file)
{
	char c,d;
	do
	{
		d=fscanf(file, "%c", &c);
		if (d == EOF) 
		{
			printf("you might have forgoten to close the comment");
			exit(0);
		}
	} while (c != '*');
}
/*---------------------------------------------------------
le nom de la fonction: ReadBlancs_OpenBalise.
les entrées: FILE*file:fichier a lire.
les sorties: 
le role: elle permet de lire du fichier tous les espaces 
		jusquà trouver un balise.
----------------------------------------------------------*/
void ReadBlancs_OpenBalise(FILE*file)
{
	char c;
	do
	{
		fscanf(file, "%c", &c);
		if (c == '*')Read_Comment(file);
	} while (c != '<');
}//fin de la void ReadBlancs_OpenBalise(FILE*file)
void close_balise(FILE*file)
{
	char c;
	fscanf(file, ">%c", &c);
}
/*---------------------------------------------------------
le nom de la fonction: Read_childEnd.
les entrées: FILE*file:fichier a lire.
les sorties:
le role:elle permet de lire du fichier la fermeture de 
		des propriétés de fils. 
----------------------------------------------------------*/
void Read_childEnd(FILE*file)
{
	char c;
	ReadBlancs_OpenBalise(file);
	fscanf(file, "/child >%c", &c);
}//fin de la fonction void Read_childEnd(FILE*file)
/*-----------------------------------------------------------------
le nom de la fonction: One_prop_read.
les entrées: FILE*file:fichier a lire.
			 Node*Racine:le nud a Remplire.
les sorties: 1:si il existe des autres propriétés a lire,0:sinon
le role:elle permet de lire du fichier un propriétés d'un widget.
------------------------------------------------------------------*/
int One_prop_read(FILE*file, Node*Racine)
{
	char tmp[20], Value[255];
	fscanf(file, "%s ", tmp);//lecture de la contenue de la balise
	if (!strcmp(tmp, "/object"))//si il'a pas des propriétés a lire.
	{
		close_balise(file);
		return ((int)0);
	}//fin du if (!strcmp(tmp, "/object"))
	else if (!strcmp(tmp, "child"))//si on va lire un nud child
	{
		Node*newN = NULL;
		newN = start_Reading(file, newN);//la lecture du nud child
		Racine->fils = insert_in_list_Child(Racine->fils, newN);
		Read_childEnd(file);
		return((int)1);
	}//fin du else if (!strcmp(tmp, "child"))
	else if (!strcmp(tmp, "property"))//lecture d'un propriétés
	{
		fscanf(file, "name= %s > %s </property>", tmp, Value);
		trans_tir_to_blanc(Value);
		Racine->List = insert_in_list_props(Racine->List, tmp, Value);
		return((int)1);
	}//fin du else if (!strcmp(tmp, "property"))
	else 
	{
		printf("the file might be bad |%s| was read as a proprety or a child", tmp);
		exit(-1);
	}//fin du else 
}//fin de la fonction int One_prop_read(FILE*file, Node*Racine)
void PropReader(FILE*file, Node*Racine)
{
	do
	{
		ReadBlancs_OpenBalise(file);
	} while (One_prop_read(file, Racine));
}
Node*Object_reader(FILE*file, Node*Racine)
{
	char name[20], id[20];
	fscanf(file, "class=%s id=%s >", name, id);
	Racine = Create_Node(id, name);
	PropReader(file, Racine);
	return((Node*)Racine);
}
/*---------------------------------------------
le nom de la fonction: start_Reading.
les entrées: FILE*file:fichier a lire.
			 Node*Racine:le nud a Remplire.
les sorties:
le role:elle permet la lecture d'un object.
----------------------------------------------*/
Node*start_Reading(FILE*File, Node*Racine)
{
	char obj[10];
	ReadBlancs_OpenBalise(File);
	fscanf(File, "%s ", obj);
	if (!strcmp(obj, "object"))return((Node*)Object_reader(File, Racine));
	else
	{
		printf("file is written badly");
		exit(-1);
	}//fin du else 
}//fin de la fonction Node*start_Reading(FILE*File, Node*Racine)
/*---------------------------------------------
le nom de la fonction: Check_id_Valable.
les entrées: Node*Racine:le nud Racine arbre.
			 Node*Elem:  
les sorties:
le role:elle permet la lecture d'un object.
----------------------------------------------*/
int Check_id_Valable(Node*Racine, Node*Elem)
{
	if (Racine && Elem)
	{
		if(Racine!=Elem)if (!strcmp(Racine->id, Elem->id))return((int)0);
		if (!Check_id_Valable(Racine->freres, Elem))return((int)0);
		if (!Check_id_Valable(Racine->fils, Elem))return((int)0);
	}
	return((int)1);
}
int Check_All_ids(Node*Racine,Node*Elem,char idConfli[20])
{
	if (Racine && Elem)
	{
		if (!Check_id_Valable(Racine, Elem->id)) 
		{
			strcpy(idConfli, Elem->id);
			return((int)0);
		}
		if (!Check_All_ids(Racine, Elem->freres,idConfli))return((int)0);
		if (!Check_All_ids(Racine, Elem->fils, idConfli))return((int)0);
	}
	return((int)1);
}
/*------------------------------------------------------------
le nom de la fonction: Reader_XML.
les entrées: char*nomfich:la chemin du fichier a lire.
les sorties: Node*Racine: l'arbre qui contient les propriétés.  
le role:elle permet la lecture du fichier style XML 
		et le transfer a un arbre.
------------------------------------------------------------*/
Node* Reader_XML(char*nomfich)
{
	char id[20];
	Node*Racine = NULL;
	FILE *File = fopen(nomfich, "r");
	Racine = start_Reading(File, Racine);
	if (!Check_All_ids(Racine, Racine,id))
	{
		printf("eror:confliction ids the id |%s| is already taken", id);
		exit(1);
	}
	fclose(File);
	return((Node*)Racine);
}//fin de la fonction Node* Reader_XML(char*nomfich)

