/* La structure d'un menu  ****************************************************/
typedef struct      // structure d'un button de menu
{
    GtkWidget* menu_btn;    // pointeur sur le menu button
    // direction
    GtkArrowType direction;//gtk_menu_button_set_direction ()
}menu_button;
typedef struct MItem    // structure d'un element du menu
{
    GtkWidget* menu_item;   // pointeur sur un element du menu
    gchar* title;           // titre de l'element du menu
    gint typetitle;         // indique si le titre est un label ou mnemonic
    gchar* accel;
    int cpt;
    struct MItem* but;      // pointeur sur l'element suivant du menu
    struct MItem* top;      // pointeur utiliser pour remplir l'arbre du menu
    struct MItem* rght;     // pointeur sur l'element suivant du sous menu
}menu_item;
typedef struct            // structure du menu
{
    GtkWidget* menu;    // pointeur sur le widget menu
    // orientation du menu
    GtkPackDirection direction; //gtk_menu_bar_set_pack_direction()
    // les elements du menu
    menu_item* info;
}menu;
/*******************************************************************************
 * Nom : creer_intialiser_menu_button;
 * Sorties : menu_button * : la structure menu_button initialisee
 * La fonction permet d'allouer un espace memoire pour un menu_button et de
 * l'initialisee par des valeurs par defaut
 * */
menu_button* creer_intialiser_menu_button()
{
    // allocation memoire
    menu_button* menubtn = (menu_button*)malloc(sizeof(menu_button));
    if (!menubtn)
    {
        printf("\nErreur d'allocation memoire pour menu button");
        exit(0);
    }
    // initialisation des champs de la structure
    menubtn->menu_btn = NULL;
    menubtn->direction = GTK_ARROW_DOWN;
    return (menu_button*)menubtn;
}
/*******************************************************************************
 * Nom : creer_intialiser_menu_item;
 * Sorties : menu_item * : la structure menu_item initialisee
 * La fonction permet d'allouer un espace memoire pour un menu_item et de
 * l'initialisee par des valeurs par defaut
 * */
menu_item* creer_intialiser_menu_item()
{
    // allocation memoire 
    menu_item* mnit = (menu_item*)malloc(sizeof(menu_item));
    if (!mnit)
    {
        printf("\nErreur d'allocation memoire pour menu item!");
        exit(0);
    }
    // initialisation des champs de la structure
    mnit->menu_item = NULL;
    mnit->title = NULL;
    mnit->typetitle = 0;
    mnit->cpt = 0;
    mnit->but = NULL;
    mnit->rght = NULL;
    mnit->top = NULL;
    return (menu_item*)mnit;
}

menu* crer_initi_menu()
{
    // allocation memoire
    menu* mn = (menu*)malloc(sizeof(menu));
    // initialisation des champs de la structure
    if (!mn)
    {
        printf("\nErreur d'allocation memoire pour menu!");
        exit(0);
    }
    mn->direction = GTK_PACK_DIRECTION_LTR;
    mn->info = NULL;
    mn->menu = NULL;
    return (menu*)mn;
}
/******************************************************************************
 * Nom : make_menu_button;
 * Entrees : menu_button * : la structure d'un menu_button
 * La fonction permet de creer un menu_button passee en parametres
 * */
void make_menu_button(menu_button* menu_btn_info)
{
    if (!menu_btn_info)     // test d'existance
    {
        printf("\nErreur : menu button n'existe pas");
        exit(0);
    }
    // Creation de menu button
    menu_btn_info->menu_btn = gtk_menu_button_new();
    gtk_menu_button_set_direction(GTK_MENU_BUTTON(menu_btn_info->menu_btn), 
                                    menu_btn_info->direction);
}
/******************************************************************************
 * Nom : make_menu_bar;
 * Entrees : menu_item * : la structure d'un menu_item
 *           GtkWidget * : le menu bar
 * La fonction permet de creer un menu bar passee en parametres
 * */
void make_menu_bar(menu_item* menu_item_info, GtkWidget* mn_bar)
{
    if (!menu_item_info)    // test d'existance
    {
        printf("\nErreur : menu_item n'existe pas!");
        exit(0);
    }
    // Creation de menu bar
    if (!menu_item_info->typetitle) 
        menu_item_info->menu_item = gtk_menu_item_new_with_label(
                                                            menu_item_info->title);
    else menu_item_info->menu_item = gtk_menu_item_new_with_mnemonic(
                                                            menu_item_info->title);
    gtk_menu_shell_append(GTK_MENU_SHELL(mn_bar), menu_item_info->menu_item);
    if (menu_item_info->but) make_menu_bar(menu_item_info->but, mn_bar);
    if (menu_item_info->rght)
    {
        GtkWidget* submenu = gtk_menu_new();
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_info->menu_item), 
                                                submenu);
        make_menu_bar(menu_item_info->rght, submenu);
    }
}
/******************************************************************************
 * Nom : make_menu;
 * Entrees : menu * : la structure d'un menu
 * La fonction permet de creer un menu passee en parametres
 * */
void make_menu(menu* menu_info)
{
    if (!menu_info)     // test d'existance
    {
        printf("\nErreur: menu n'existe pas");
        exit(0);
    }
    // Creation de menu
    menu_info->menu = gtk_menu_new();
    gtk_menu_bar_set_pack_direction(GTK_MENU_BAR(menu_info->menu),
                                    menu_info->direction);
    make_menu_bar(menu_info->info, menu_info->menu);
}
/******************************************************************************
 * Nom : remplir_menu_button;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : menu_button * : la structure menu_button remplit a partir du
 *           fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * menu_button par les valeurs extrait de fichier
 * */
menu_button* remplir_menu_button(FILE* f1)
{
    menu_button* mnb = creer_intialiser_menu_button(); // initialisation
    obj_att* p = NULL;
    // lire les attributs de menu button a partir du fichier
    p = lire_att(f1);   
    while (p)
    {
        if (!strcmp(p->attribut, "direction"))
        {
            if (!strcmp(p->valeur, "UP")) mnb->direction = GTK_ARROW_UP;
            else if (!strcmp(p->valeur, "LEFT")) mnb->direction = GTK_ARROW_LEFT;
            else if (!strcmp(p->valeur, "RIGHT")) mnb->direction = GTK_ARROW_RIGHT;
            else if (!strcmp(p->valeur, "NONE")) mnb->direction = GTK_ARROW_NONE;
        }
        p = p->suiv;
    }
    return ((menu_button*)mnb);
}
menu_item* remplir_menu_item(FILE* f1)
{
    char mot[18];
    int k = 1;
    menu_item* in, * p, * ne;
    obj_att* liste = NULL;
    fscanf(f1, "<%30[^ ] ", mot);
    liste = lire_att(f1);
    in = creer_intialiser_menu_item();
    while (liste)
    {
        if (!strcmp(liste->attribut, "label")) in->title = liste->valeur;
        else if (!strcmp(liste->attribut, "mnemonic"))
        {
            in->title = liste->valeur;
            in->typetitle = 1;
        }
        liste = liste->suiv;
    }
    p = in;
    do {
        fscanf(f1, "<%30[^ ] ", mot);
        if (!strcmp(mot, "item"))
        {
            liste = lire_att(f1);
            ne = creer_intialiser_menu_item();
            while (liste)
            {
                if (!strcmp(liste->attribut, "label")) ne->title = liste->valeur;
                else if (!strcmp(liste->attribut, "mnemonic"))
                {
                    ne->title = liste->valeur;
                    ne->typetitle = 1;
                }
                if (k == 0)
                {
                    p->but = ne;
                    k = 1;
                }
                else p->rght = ne;
                ne->top = p;
                p = ne;
                liste = liste->suiv;
            }
        }
        else if (!strcmp(mot, "/item"))
        {
            fscanf(f1, ">\n");
            epurer_blan(f1);
            while (p->cpt == 1) p = p->top;
            p->cpt = 1;
            k = 0;
        }
    } while (strcmp(mot, "/menu") != 0);
    fscanf(f1, ">\n");
    epurer_blan(f1);
    return (menu_item*)in;
}

menu* remplir_menu(FILE* f1)
{
    menu* mn = crer_initi_menu();
    obj_att* liste = NULL;
    liste = lire_att(f1);
    if (!strcmp(liste->valeur, "RTL")) mn->direction = GTK_PACK_DIRECTION_RTL;
    else if (!strcmp(liste->valeur, "TTB")) mn->direction = GTK_PACK_DIRECTION_TTB;
    else if (!strcmp(liste->valeur, "BTT")) mn->direction = GTK_PACK_DIRECTION_BTT;
    mn->info = remplir_menu_item(f1);
    return (menu*)mn;
}