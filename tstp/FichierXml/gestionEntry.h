#include"gestionToolBar.h"


void init_default_entry(Entry *e)
{
	e->visible = TRUE;
	strcpy(e->text_to_set, "");
	e->editable = TRUE;
    e->len = 10;
    e->width = 20;
}


GtkWidget *init_entry(Entry *e)
{
	GtkWidget *entry;
	entry = gtk_entry_new();
	gtk_widget_set_size_request(entry, e->len, e->width);
	gtk_entry_set_visibility(GTK_ENTRY(entry), e->visible);
	if (!e->visible) gtk_entry_set_invisible_char(GTK_ENTRY(entry), e->invis_car);
	if (e->text_to_set)  gtk_entry_set_text(GTK_ENTRY(entry), e->text_to_set);
	gtk_editable_set_editable(GTK_EDITABLE(entry), e->editable);
	return (GtkWidget*)entry;
}

void remplirStructEntry(Entry *en, char *nomProp, char *contentProp)
{

	if (!strcmp(nomProp, "longeur"))
		en->len = (int)(atof(contentProp));

	else if (!strcmp(nomProp, "largeur"))
		en->width = (int)(atof(contentProp));

	else if (!strcmp(nomProp, "visible"))
	{
		if (!strcmp(contentProp, "true"))
			en->visible = TRUE;
		else    en->visible = FALSE;
	}

	else if (!strcmp(nomProp, "invis-car"))
		en->invis_car = contentProp[0];

	else if (!strcmp(nomProp, "editable"))
	{
		if (!strcmp(contentProp, "true"))
			en->editable = TRUE;
		else    en->editable = FALSE;
	}
	else if (!strcmp(nomProp, "text-to-set"))
		strcpy(en->text_to_set, contentProp);

}

void linkPropsEntry(Entry *props, Prop *List)
{
	Prop *crt = List;
	while (crt)
	{
		remplirStructEntry(props, crt->name, crt->Value);
		crt = crt->next;
	}
}

GtkWidget *create_entry_from_node(Node *noeud)
{
	Entry props;
	init_default_entry(&props);
	linkPropsEntry(&props, noeud->List);
	return((GtkWidget*)init_entry(&props));
}