#include "gestionComboBox.h"


typedef struct 
{
    GtkWidget* parentWidget;
    TableInterface *table;
}ILISIBuilder;

GtkWidget *createButton(Node *nodeObj);
char* getAttrProp(Node *root, char *attribute);
GtkWidget *createWidget(Node *NdObject, TableInterface *interface);
GtkWidget *creationInterface(Node *arbre, TableInterface *interface);
ILISIBuilder *ilisiBuilderReadFromFile(char *fileName);



//fonction qui prend l'attribut d'un noeud et retourne sa valeur 
char* getAttrProp(Node *root, char *attribute)
{
	if (root)
	{
		char *Buff;
		//s'il s'agit d'un ID
		if (strcmp(attribute, "id") == 0)
		{
			Buff = (char*)malloc(sizeof(char) * strlen(root->id));
			strcpy(Buff, root->id);
			return Buff;
		}
		//s'il s'agit d'une class 
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

	//Check Button 
	else if (strcmp(getAttrProp(NdObject, "class"), "chekbutton") == 0)
		newWid = Create_chekButton_from_node(NdObject);
		
	//simple button 
	else if (strcmp(getAttrProp(NdObject, "class"), "button") == 0)
		newWid = Create_Button_from_node(NdObject);

	
	//button radio 
	else if (strcmp(getAttrProp(NdObject, "class"), "groupRadioBox") == 0)
		newWid = Create_GroupRadioButton_from_node(NdObject, interface);
	
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
	else if (strcmp(getAttrProp(NdObject, "class"), "image") == 0)
		newWid = creat_Image_from_node(NdObject);

	//Menu
	else if (strcmp(getAttrProp(NdObject, "class"), "menubar") == 0)
		newWid = creat_menu_from_node(NdObject, interface);

	//Label
	else if (strcmp(getAttrProp(NdObject, "class"), "label") == 0)
		newWid = node_to_label(NdObject);

	//entry
	else if (strcmp(getAttrProp(NdObject, "class"), "entry") == 0)
		newWid = create_entry_from_node(NdObject);

	//toolbar 
	else if (strcmp(getAttrProp(NdObject, "class"), "toolbar") == 0)
		newWid = creat_toolbar_from_node(NdObject, interface);

	//TextView
	else if (strcmp(getAttrProp(NdObject, "class"), "textview") == 0)
		newWid = create_textview_from_node(NdObject);

	//Table 
	else if (strcmp(getAttrProp(NdObject, "class"), "table") == 0)
		newWid = createTable_fromNode(NdObject, interface);

	//NoteBook
	else if (strcmp(getAttrProp(NdObject, "class"), "notebook") == 0)
		newWid = createNoteBookFromNode(NdObject, interface);
	
	//comboBox
	else if (strcmp(getAttrProp(NdObject, "class"), "comboBox") == 0)
		newWid = createComboBox_fromNode(NdObject);
	

	//Insertion de la widget dans la table des references apres creation 
	insertionEntite(interface, NdObject->id, newWid);
	return newWid;
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


