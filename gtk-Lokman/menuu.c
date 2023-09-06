#include <gtk/gtk.h>
#include <libxml/parser.h>

typedef struct mennu{
  GtkWidget *menu;
  struct item *fils;
}Menu;

typedef struct item
{
  GtkWidget *item;
  gchar* titre;
  struct mennu *menu;
  struct item *next;
}Item;

Menu *initMenu(Menu *menu)
{
  menu = (Menu*)malloc(sizeof(Menu));
  menu->menu = NULL;
  menu->fils = NULL;
  return menu;
}

Item *initItem(Item *item)
{
  item = (Item*)malloc(sizeof(Item));
  item->item = NULL;
  item->titre = NULL;
  item->menu = NULL;
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

Menu *creemenubar(Menu *menu)
{
  menu = initMenu(menu);
  menu->menu = gtk_menu_bar_new();
  return menu;
}

Menu *creemenu(Menu *menu)
{
  menu = initMenu(menu);
  menu->menu = gtk_menu_new();
  return menu;
}


Menu *Ajouteraumenu(Menu *menu,Item *item)
{
  if(menu->fils)item->next = menu->fils->next;
  menu->fils = item ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu->menu), menu->fils->item);
  return menu;
}

Item *Creesousmenu(Item *item)
{
  if(!item) return NULL;
  item->menu = creemenu(item->menu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(item->item),item->menu->menu);
  return item;
}




static void create_menu_items(xmlNodePtr node, Menu *menu) 
{
    for (xmlNodePtr child = node->children; child != NULL; child = child->next) {
        if (child->type == XML_ELEMENT_NODE && strcmp((char *)child->name, "item") == 0) {
            Item * item = creeItem(item,(char *)xmlGetProp(child, (xmlChar *)"label"));
            menu = Ajouteraumenu(menu,item);
            if (child->children != NULL) {
                item = Creesousmenu(item);
                create_menu_items(child, item->menu);
            }
        }
    }
}

Menu *create_menu_bar_from_xml(const char *filename) 
{
    Menu *menu = creemenubar(menu);
    xmlDocPtr doc = xmlParseFile(filename);
    if (doc == NULL) {
        g_warning("Failed to parse XML file %s", filename);
        return menu;
    }
    xmlNodePtr root = xmlDocGetRootElement(doc);
    create_menu_items(root, menu);
    xmlFreeDoc(doc);
    return menu;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    Menu *menu = create_menu_bar_from_xml("menu.xml");
    gtk_box_pack_start(GTK_BOX(vbox), menu->menu, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
