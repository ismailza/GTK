#include "gestionTextView.h"


typedef struct {
    GtkWidget* parentWidget;
    TableInterface *table;
}ILISIBuilder;

GtkWidget *createButton(Node *nodeObj);
char* getAttrProp(Node *root, char *attribute);
GtkWidget *createWidget(Node *NdObject, TableInterface *interface);
GtkWidget *creationInterface(Node *arbre, TableInterface *interface);
ILISIBuilder *ilisiBuilderReadFromFile(char *fileName);

GtkWidget *createButton(Node *nodeObj)
{
	Prop *crt = nodeObj->List;
	while (crt)
	{
		if (strcmp(crt->name, "label") == 0)
			return gtk_button_new_with_label(crt->Value);

		crt = crt->next;
	}

	return NULL;

}



//fonction qui prend l'attribut d'un noeud et retourne sa valeur 
char* getAttrProp(Node *root, char *attribute)
{
	if (root)
	{
		char *Buff;
		if (strcmp(attribute, "id") == 0)
		{
			Buff = (char*)malloc(sizeof(char) * strlen(root->id));
			strcpy(Buff, root->id);
			return Buff;
		}
		else if (strcmp(attribute, "class") == 0)
		{
			Buff = (char*)malloc(sizeof(char) * strlen(root->classBalise));
			strcpy(Buff, root->classBalise);
			return Buff;
		}
		else
		{
			printf("\nl'attribut que vous chercher n'existe pas \n");
			return NULL;
		}
	}
}



GtkWidget *createWidget(Node *NdObject, TableInterface *interface)
{
	//traitement d'apres la class

	GtkWidget *newWid=NULL;

	//Box 
	if (strcmp(getAttrProp(NdObject, "class"), "box") == 0)
		newWid = createBoxNode(NdObject, interface);


	//Window
	else if (strcmp(getAttrProp(NdObject, "class"), "window") == 0)
		newWid = Create_window_from_Node(NdObject, interface);

	//button
	else if (strcmp(getAttrProp(NdObject, "class"), "button") == 0)
		newWid = createButton(NdObject);



	//fixed Box 
	else if (strcmp(getAttrProp(NdObject, "class"), "fixedBox") == 0)
		newWid = Create_fixedBox_from_node(NdObject, interface);


	//Frame
	else if (strcmp(getAttrProp(NdObject, "class"), "frame") == 0)
		newWid = Create_frame_from_node(NdObject, interface);

	//Scrolled Window 
	else if (strcmp(getAttrProp(NdObject, "class"), "ScrolledWind") == 0)
		newWid = Create_ScrolledWind_from_node(NdObject, interface);


	//Image
	else if (strcmp(getAttrProp(NdObject, "class"), "menubar") == 0)
		newWid = creat_menu_from_node(NdObject, interface);


	//Menu
	else if (strcmp(getAttrProp(NdObject, "class"), "menubar") == 0)
		newWid = creat_menu_from_node(NdObject, interface);


	//Label
	else if (strcmp(getAttrProp(NdObject, "class"), "label") == 0)
		newWid = node_to_label(NdObject);

	//entry
	else if (strcmp(getAttrProp(NdObject, "class"), "menubar") == 0)
		newWid = create_entry_from_node(NdObject);

	//toolbar 
	else if (strcmp(getAttrProp(NdObject, "class"), "toolbar") == 0)
		newWid = creat_toolbar_from_node(NdObject, interface);


	//table


	//TextView
	else if (strcmp(getAttrProp(NdObject, "class"), "textview") == 0)
		newWid = create_textview_from_node(NdObject);



	//Layout 


	//

	//Cas d'eurs

	insertionEntite(interface, NdObject->id, newWid);
	return newWid;

}



GtkWidget *creationInterface(Node *arbre, TableInterface *interface)
{
	if (arbre)
	{
		//traitement de chaque noeud 
		GtkWidget *widget = createWidget(arbre, interface);

		//je pense qu'il y a des erreurs ici a gestionner 

		return widget;
	}
}


ILISIBuilder *ilisiBuilderReadFromFile(char *fileName)
{
	//Creation de l'arbre d'interface 
	Node *arbre = Reader_XML(fileName);

	//nombre des objets
	int NbObject = 0;
	Affichage(arbre, 1, &NbObject);

	printf("\nNbr d'objet: %d\n", NbObject);

	//l'interface resultante
	ILISIBuilder *InterfaceWind = (ILISIBuilder*)malloc(sizeof(ILISIBuilder));

	//creation de la table des references 
	InterfaceWind->table = creationTable(NbObject);

	//creer l'interface et recuperer la widget Pere -Fenetre-
	InterfaceWind->parentWidget = createWidget(arbre, InterfaceWind->table);

	//afficher tous les widget de la Fenetre 
	gtk_widget_show_all(InterfaceWind->parentWidget);

	return ((ILISIBuilder*)InterfaceWind);
}


