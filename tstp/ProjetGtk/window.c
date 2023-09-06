#include "structures.h"
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

GtkWidget*Creatwindow(windowProp props)
{
	props.window = gtk_window_new(props.Type);
	gtk_window_set_default_size(props.window, props.Hauteur, props.Largeur);
	gtk_window_set_position(GTK_WINDOW(props.window),props.Pos);
	if (props.Type == GTK_WINDOW_TOPLEVEL)
	{
		gtk_window_set_title(GTK_WINDOW(props.window), props.title);
		if (props.isMain)g_signal_connect(G_OBJECT(props.window), "destroy", gtk_main_quit, NULL);
		if(!props.isResisable)
		{
			gtk_window_set_resizable(GTK_WINDOW(props.window), props.isResisable);
			gtk_widget_set_size_request(props.window, props.Hauteur, props.Largeur);
		}
		if(strcmp(props.iconPlace,"default"))gtk_window_set_icon_from_file(GTK_WINDOW(props.window),props.iconPlace,NULL);
		if (!props.isDecorated)gtk_window_set_decorated(GTK_WINDOW(props.window), FALSE);
		else if (!props.isDeletable)gtk_window_set_deletable(GTK_WINDOW(props.window), FALSE);
	}
	if (props.isMaximized)gtk_window_maximize(GTK_WINDOW(props.window));
	if (props.isFullscreen)gtk_window_fullscreen(GTK_WINDOW(props.window));
	if (props.isModale)gtk_window_set_modal(GTK_WINDOW(props.window),TRUE);
	if (props.child)gtk_container_add(GTK_CONTAINER(props.window), props.child);
	return((GtkWidget*)props.window);
}
