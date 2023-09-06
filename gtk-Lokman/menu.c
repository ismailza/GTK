#include <gtk/gtk.h>

typedef struct{
  GtkWidget *bar;
  struct item *fils;
}Menubar;

typedef struct item
{
  GtkWidget *item;
  gchar* titre;
  GtkWidget *menu;
  struct item *sousitem;
  struct item *next;
}Item;

Menubar *initMenubar(Menubar *menubar)
{
  menubar = (Menubar*)malloc(sizeof(Menubar));
  menubar->bar = NULL;
  menubar->fils = NULL;
  return menubar;
}

Item *initItem(Item *item)
{
  item = (Item*)malloc(sizeof(Item));
  item->item = NULL;
  item->titre = NULL;
  item->menu = NULL;
  item->sousitem = NULL;
  item->next = NULL;
  return item;
}

Item *creeItem(Item *item,gchar *titre)
{
  item = initItem(item);
  item->titre = titre;
  item->item = gtk_menu_item_new_with_label(titre);
  return item;
}

Menubar *creemenubar(Menubar *menubar)
{
  menubar = initMenubar(menubar);
  menubar->bar = gtk_menu_bar_new();
  return menubar;
}

Menubar *Ajouteraumenubar(Menubar *menubar,Item *item)
{
  if(menubar->fils)item->next = menubar->fils->next;
  menubar->fils = item ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar->bar), menubar->fils->item);
  return menubar;
}

Item *Creesousmenu(Item *item)
{
  if(!item) return NULL;
  item->menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(item->item),item->menu);
  return item;
}

Item *Ajoutersousitem(Item *item,Item *sousitem)
{
  if(item->menu)
  {
    if(item->sousitem)sousitem->next = item->sousitem->next;
    item->sousitem = sousitem;
    gtk_menu_shell_append(GTK_MENU_SHELL(item->menu), sousitem->item);
  }
  return item;
}





int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *vbox;
  Menubar *menubar;
  Item *item;
  Item *sousitem;



  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_title(GTK_WINDOW(window), "Test menu");

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  menubar = creemenubar(menubar);
  item = creeItem(item,"File");
  item = Creesousmenu(item);
  sousitem = creeItem(sousitem,"Edit");
  sousitem = Creesousmenu(sousitem);
  item = Ajoutersousitem(item,sousitem);
  sousitem = creeItem(sousitem,"Undo");
  item->sousitem = Ajoutersousitem(item->sousitem,sousitem);
  sousitem = creeItem(sousitem,"Redo");
  item->sousitem = Ajoutersousitem(item->sousitem,sousitem);

  menubar = Ajouteraumenubar(menubar,item);
  item = creeItem(item,"Help");
  menubar = Ajouteraumenubar(menubar,item);

  gtk_box_pack_start(GTK_BOX(vbox), menubar->bar, FALSE, FALSE, 0);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}