#include"gestionFrame.h"
void creat_item(GtkWidget *nn, char nom[25], menu*me) {
	me->menu_item = gtk_menu_item_new_with_label(nom);
	gtk_menu_shell_append(GTK_MENU_SHELL(nn), me->menu_item);

}

void creat_champ(GtkWidget * menu_cha, char nom[25], menu*me) {

	me->menu_champ = gtk_menu_new();
	me->menu_item = gtk_menu_item_new_with_label(nom);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(me->menu_item), me->menu_champ);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_cha), me->menu_item);

}
void creat_menu_xml(menu*me, Node*tmp, GtkWidget*mmm, TableInterface*table) 
{
	if (tmp) {
		if (!(strcmp((tmp->classBalise), "menuchamp"))) {
			creat_champ(mmm, tmp->List->Value, me);
			insertionEntite(table, tmp->id, mmm);
		}
		if (!(strcmp((tmp->classBalise), "menuitem"))) {
			creat_item(me->menu_champ, tmp->List->Value, me);
			insertionEntite(table, tmp->id, me->menu_champ);
		}
		if (!(strcmp((tmp->classBalise), "sousmenu"))) {
			creat_champ(me->menu_champ, tmp->List->Value, me);
			insertionEntite(table, tmp->id, me->menu_champ);
		}
		creat_menu_xml(me, tmp->fils, me->menu_champ,table);
		creat_menu_xml(me, tmp->freres, me->menu_barre,table);
	}
}
GtkWidget* creat_menu_from_node(Node*racine,TableInterface*inter)
{
	menu me;
	me.menu_barre = gtk_menu_bar_new();
	creat_menu_xml(&me, racine->fils, me.menu_barre, inter);
	return((GtkWidget*)me.menu_barre);
}