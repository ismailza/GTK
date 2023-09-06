#include"gestionImage.h"

void creat_toolbar(tool* p) {

	p->toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(p->toolbar), GTK_TOOLBAR_ICONS);
}

void creat_toolitem(tool* p, gchar* icone) 
{
	p->toolitem = gtk_tool_button_new_from_stock(icone);
	gtk_toolbar_insert(GTK_TOOLBAR(p->toolbar), p->toolitem, -1);
}

void connectre_item_sig(GtkToolItem*   p, gchar* signal) 
{
	g_signal_connect(G_OBJECT(p), signal, G_CALLBACK(gtk_main_quit), NULL);
}
void creat_tool_xml(tool *t, Node*racine,TableInterface*inter) 
{
	if (racine) 
	{
		if (!(strcmp((racine->classBalise), "toolitem"))) {
			Prop*crt = racine->List;
			while (crt)
			{
				if (!(strcmp((crt->name), "title"))) {
					if (!(strcmp((crt->Value), "1")))   creat_toolitem(t, GTK_STOCK_OPEN);
					if (!(strcmp((crt->Value), "2")))   creat_toolitem(t, GTK_STOCK_SAVE);
					if (!(strcmp((crt->Value), "3")))   creat_toolitem(t, GTK_STOCK_NEW);
					if (!(strcmp((crt->Value), "4")))
					{
						creat_toolitem(t, GTK_STOCK_QUIT);
						connectre_item_sig(t->toolitem, "clicked");
					}
				}
				crt = crt->next;
			}
			insertionEntite(inter, racine->id, GTK_WIDGET(t->toolitem));
		}
		creat_tool_xml(t, racine->fils,inter);
		creat_tool_xml(t, racine->freres,inter);
	}
}

GtkWidget* creat_toolbar_from_node(Node*racine,TableInterface*table)
{
	tool toolp;
	creat_toolbar(&toolp);
	creat_tool_xml(&toolp, racine->fils,table);
	return((GtkWidget*)toolp.toolbar);
}