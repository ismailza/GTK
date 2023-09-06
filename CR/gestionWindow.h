#include "gestionFixedBox.h"


gboolean ReturnIntoBoolen(int a)
{
	if (a)return(TRUE);
	return(FALSE);
}
GtkWindowType ReadTypewin(int a)
{
	if (a == 1)return GTK_WINDOW_TOPLEVEL;
	return GTK_WINDOW_POPUP;
}
GtkWindowPosition Read_pos_win(int a)
{
	switch (a)
	{
	case 0:return GTK_WIN_POS_NONE; break;
	case 1:return GTK_WIN_POS_CENTER; break;
	case 2:return GTK_WIN_POS_MOUSE; break;
	case 3:return GTK_WIN_POS_CENTER_ON_PARENT; break;
	case 4:return GTK_WIN_POS_CENTER_ALWAYS; break;
	default:printf("the file might be bad the pos was read badly");
		return GTK_WIN_POS_CENTER;
	}
}

void Init_default_window_prop(windowProp*wind)
{
	wind->isResisable = TRUE;
	wind->isDecorated = TRUE;
	wind->isFullscreen = FALSE;
	wind->isMain = TRUE;
	wind->isMaximized = FALSE;
	wind->isModale = FALSE;
	strcpy(wind->iconPlace, "default");
	wind->Pos = GTK_WIN_POS_CENTER;
	wind->child = NULL;
	wind->Largeur = 1000;
	wind->Hauteur = 1000;
	strcpy(wind->title, "default title");
	wind->Type = GTK_WINDOW_TOPLEVEL;
}

void One_wind_prop_read(Prop*pr, windowProp* wind)
{
	if (!strcmp(pr->name, "title"))strcpy(wind->title, pr->Value);
	else if (!strcmp(pr->name, "iconPlace"))strcpy(wind->iconPlace, pr->Value);
	else if (!strcmp(pr->name, "Hauteur"))wind->Hauteur = (int)atoi(pr->Value);
	else if (!strcmp(pr->name, "Largeur"))wind->Largeur = (int)atoi(pr->Value);
	else if (!strcmp(pr->name, "type"))wind->Type = ReadTypewin((int)atoi(pr->Value));
	else if (!strcmp(pr->name, "Pos"))wind->Pos = Read_pos_win((int)atoi(pr->Value));
	else if (!strcmp(pr->name, "isMain"))wind->isMain = ReturnIntoBoolen((int)atoi(pr->Value));
	else if (!strcmp(pr->name, "isResisable"))wind->isResisable = ReturnIntoBoolen((int)atoi(pr->Value));
	else if (!strcmp(pr->name, "isModale"))wind->isModale = ReturnIntoBoolen((int)atoi(pr->Value));
	else if (!strcmp(pr->name, "isMaximized"))wind->isMaximized = ReturnIntoBoolen((int)atoi(pr->Value));
	else if (!strcmp(pr->name, "isFullscreen"))wind->isFullscreen = ReturnIntoBoolen((int)atoi(pr->Value));
	else if (!strcmp(pr->name, "isDecorated"))wind->isDecorated = ReturnIntoBoolen((int)atoi(pr->Value));
	else if (!strcmp(pr->name, "isDeletable"))wind->isDeletable = ReturnIntoBoolen((int)atoi(pr->Value));
}


void windowPropReader(Prop*List, windowProp* wind)
{
	Prop*crt = List;
	while (crt) 
	{
		One_wind_prop_read(crt, wind);
		crt = crt->next;
	}
}


GtkWidget*Creatwindow(windowProp props)
{
	GtkWidget *window;
	window = gtk_window_new(props.Type);
	gtk_window_set_default_size(window, props.Hauteur, props.Largeur);
	gtk_window_set_position(GTK_WINDOW(window),props.Pos);
	if (props.Type == GTK_WINDOW_TOPLEVEL)
	{
		gtk_window_set_title(GTK_WINDOW(window), props.title);
		if (props.isMain)g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);
		if(!props.isResisable)
		{
			gtk_window_set_resizable(GTK_WINDOW(window), props.isResisable);
			gtk_widget_set_size_request(window, props.Hauteur, props.Largeur);
		}
		if(strcmp(props.iconPlace,"default"))gtk_window_set_icon_from_file(GTK_WINDOW(window),props.iconPlace,NULL);
		if (!props.isDecorated)gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
		else if (!props.isDeletable)gtk_window_set_deletable(GTK_WINDOW(window), FALSE);
	}
	if (props.isMaximized)gtk_window_maximize(GTK_WINDOW(window));
	if (props.isFullscreen)gtk_window_fullscreen(GTK_WINDOW(window));
	if (props.isModale)gtk_window_set_modal(GTK_WINDOW(window),TRUE);
	if (props.child)gtk_container_add(GTK_CONTAINER(window), props.child);
	return((GtkWidget*)window);
}

GtkWidget *Create_window_from_Node(Node*Racine, TableInterface *table)
{
	windowProp wind;
	Init_default_window_prop(&wind);
	windowPropReader(Racine->List, &wind);
	wind.child = createWidget(Racine->fils, table);
	return((GtkWidget*)Creatwindow(wind));
}


GtkWidget*Create_ScrolledWind_from_node(Node*Racine, TableInterface *table)
{
	GtkWidget*scrolled = gtk_scrolled_window_new(NULL, NULL),*child;
	child = createWidget(Racine->fils, table);
	gtk_scrolled_window_add_with_viewport(scrolled, child);
	return((GtkWidget*)scrolled);
}