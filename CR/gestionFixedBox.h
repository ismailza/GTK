
#include "gestionBox.h"


void get_x_y_from_props(Prop *List, int *x, int *y)
{
	Prop *crt = List;
	while (crt)
	{
		if (!strcmp(crt->name, "x"))*x = (int)atoi(crt->Value);
		else if (!strcmp(crt->name, "y"))*y = (int)atoi(crt->Value);
		crt = crt->next;
	}
}
void Put_all_childs_in_fixedBox(Node*Racine,GtkWidget*FixedBox, TableInterface *table)
{
	Node*Crt = Racine;
	int x=0, y=0;
	GtkWidget*Child;
	while (Crt)
	{
		Child = createWidget(Crt, table);
		get_x_y_from_props(Crt->List, &x, &y);
		gtk_fixed_put(FixedBox, Child, x, y);
		Crt = Crt->freres;
	}
}
GtkWidget* Create_fixedBox_from_node(Node*Racine, TableInterface *table)
{
	GtkWidget*widget = gtk_fixed_new();
	Put_all_childs_in_fixedBox(Racine->fils, widget, table);
	return((GtkWidget*)widget);
}