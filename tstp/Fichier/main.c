#include "fichier.h"
int num_object=0;

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
	Node *Ne;
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
	return((Node*)Ne);
}
Node*start_Reading(FILE*File, Node*Racine);
Node*insert_in_list_Child(Node*list, Node*newN)
{
	if (!list)return((Node*)newN);
	Node*crt=list;
	while (crt->freres)crt = crt->freres;
	crt->freres = newN;
	return((Node*)list);
}
Prop*insert_in_list_props(Prop*list,char name[20], char *Value)
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
	} while (c!='<');
}
void close_balise(FILE*file)
{
	char c;
	fscanf(file, ">%c", &c);
	//int a;
}
void Read_childEnd(FILE*file)
{
	char c;
	ReadBlancs_OpenBalise(file);
	fscanf(file, "/child >%c", &c);
}
int One_prop_read(FILE*file, Node*Racine)
{
	char tmp[20],Value[255];
	fscanf(file, "%s ", tmp);
	if (!strcmp(tmp, "/object"))
	{
		close_balise(file);
		return ((int)0);
	}
	else if (!strcmp(tmp, "child"))
	{
		Node*newN=NULL;
		newN=start_Reading(file, newN);
		Racine->fils = insert_in_list_Child(Racine->fils, newN);
		Read_childEnd(file);
		return((int)1);
	}
	fscanf(file, "name= %s > %s </property>", tmp,Value);
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
Node*Object_reader(FILE*file,Node*Racine)
{
	char name[20], id[20];
	fscanf(file, "class=%s id=%s >", name,id);
	Racine = Create_Node(id, name);
	PropReader(file, Racine);
	num_object++;
	return((Node*)Racine);
}
Node*start_Reading(FILE*File,Node*Racine)
{
	char obj[10];
	ReadBlancs_OpenBalise(File);
	fscanf(File, "%s ", obj);
	if (!strcmp(obj, "object"))return((Node*)Object_reader(File,Racine));
	else
	{
		printf("file is written badly");
		exit(-1);
	}
}
Node* Reader_XML(char*nomfich)
{
	Node*Racine=NULL;
	FILE *File = fopen(nomfich, "r");
	Racine = start_Reading(File,Racine);
	fclose(File);
	return((Node*)Racine);
}

void Affichage(Node* nd, int spac)
{
	if(nd)
	{
		Prop *crt = nd->List;
		printf("\n%*cobject.class: %s, #id: %s", spac*2, '-', nd->classBalise, nd->id);
		printf("\n%*cProps: ", spac*2, '-');
		while (crt)
		{
			printf(" name: %s / valeur: %s, ", crt->name,crt->Value);
			crt = crt->next;
		}

		Affichage(nd->freres, spac);
		Affichage(nd->fils, spac+1);

	}
}
int main(int argc, char* argv[])
{
    Node *arb=NULL,*crt=NULL;
    GtkWidget *label,*window;
    gtk_init(&argc, &argv);
    label=gtk_text_view_new();
    arb=Reader_XML("xml.txt");
    Affichage(arb,5);
    crt=arb;
    while(crt)
    {
    if(!strcmp(crt->classBalise,"textview"))
     label=node_to_textview(crt);
        crt=crt->fils;
    }
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "hello!");
    gtk_widget_set_size_request(window, 400, 400);
    gtk_container_add(GTK_CONTAINER(window),label);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;

}









