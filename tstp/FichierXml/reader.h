#define _CRT_SECURE_NO_WARNINGS
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

Prop *Create_Prop(char name[20], char* Value);
Node *Create_Node(char id[20], char classBalise[20]);
Node *start_Reading(FILE*File, Node*Racine);
Node *insert_in_list_Child(Node*list, Node*newN);
Prop *insert_in_list_props(Prop*list,char name[20], char *Value);
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
			// printf(" name: %s / valeur: %s, ", crt->name, crt->Value);
			crt = crt->next;
		}

		Affichage(nd->fils, spac + 2, nbOb);
		Affichage(nd->freres, spac, nbOb);

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

Node*start_Reading(FILE*File, Node*Racine);
Node*insert_in_list_Child(Node*list, Node*newN)
{
	if (!list)return((Node*)newN);
	Node*crt = list;
	while (crt->freres)crt = crt->freres;
	crt->freres = newN;
	return((Node*)list);
}

Prop*insert_in_list_props(Prop*list, char name[20], char *Value)
{
	Prop*Newd = Create_Prop(name, Value);
	if (!list)return((Prop*)Newd);
	Prop*crt = list;
	while (crt->next)crt = crt->next;
	crt->next = Newd;
	return((Prop*)list);
}

void ReadBlancs_OpenBalise(FILE*file)
{
	char c;
	do
	{
		fscanf(file, "%c", &c);
	} while (c != '<');
}
void close_balise(FILE*file)
{
	char c;
	fscanf(file, ">%c", &c);
	int a;
}
void Read_childEnd(FILE*file)
{
	char c;
	ReadBlancs_OpenBalise(file);
	fscanf(file, "/child >%c", &c);
}
int One_prop_read(FILE*file, Node*Racine)
{
	char tmp[20], Value[255];
	fscanf(file, "%s ", tmp);
	if (!strcmp(tmp, "/object"))
	{
		close_balise(file);
		return ((int)0);
	}
	else if (!strcmp(tmp, "child"))
	{
		Node*newN = NULL;
		newN = start_Reading(file, newN);
		Racine->fils = insert_in_list_Child(Racine->fils, newN);
		Read_childEnd(file);
		return((int)1);
	}
	fscanf(file, "name= %s > %s </property>", tmp, Value);
	Racine->List = insert_in_list_props(Racine->List, tmp, Value);
	return((int)1);
}
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
	}
}
Node* Reader_XML(char*nomfich)
{
	Node*Racine = NULL;
	FILE *File = fopen(nomfich, "r");
	Racine = start_Reading(File, Racine);
	fclose(File);
	return((Node*)Racine);
}


