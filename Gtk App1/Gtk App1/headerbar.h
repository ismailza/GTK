/* La structure de l'entete ***************************************************/
typedef struct
{
    // pointeur sur le headerbar
    GtkWidget* header_bar;      
    // le titre de l'entete
    gchar* title;               
    // le sous titre de l'entete
    gchar* sub_title;           //gtk_header_bar_set_subtitle ()
    // les bouttons de l'entete (menu:minimize,maximize,close)
    gboolean close_button_show; //gtk_header_bar_set_show_close_button ()
    // 
    gchar* decoration_layout;   // gtk_header_bar_set_decoration_layout ()
}header_bar;
/*******************************************************************************
 * Nom : creer_initialiser_header_bar;
 * Sorties : header_bar *hdb : la structure header_bar initialisee
 * La fonction permet d'allouer un espace memoire pour l'entete et de
 * l'initialisee par des valeurs par defaut
 * */
header_bar* creer_initialiser_header_bar()
{
    // allocation memoire
    header_bar* hbar = (header_bar*)malloc(sizeof(header_bar)); 
    if (!hbar) 
    {
        printf("\nErreur d'allocation memoire pour l'entete!");
        exit(0);
    }
    // initialisation des champs de la structure
    hbar->header_bar = NULL;
    hbar->title = "Untitled";
    hbar->sub_title = NULL;
    hbar->close_button_show = TRUE;
    hbar->decoration_layout = "icon,menu:minimize,maximize,close";
    return (header_bar*)hbar;
}
/******************************************************************************
 * Nom : make_headerbar;
 * Entrees : header_bar *hdb : la structure header_bar
 * La fonction permet de creer une headerbar passer en parametre
 * */
void make_headerbar(header_bar* hbar_info)
{
    if (!hbar_info) // test d'existance de la structure
    {
        printf("\nErreur : headerbar n'xiste pas!");
        exit(0);
    }
    // Creation de header bar
    hbar_info->header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(hbar_info->header_bar), 
                             hbar_info->title);
    if (hbar_info->sub_title)
        gtk_header_bar_set_subtitle(GTK_HEADER_BAR(hbar_info->header_bar),
                                hbar_info->sub_title);
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(hbar_info->header_bar), 
                                         hbar_info->close_button_show);
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(hbar_info->header_bar),
                                         hbar_info->decoration_layout);
}
/******************************************************************************
 * Nom : remplir_header_bar;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : header_bar * : la structure header_bar remplit a partir du fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * header_bar par les valeurs extrait de fichier
 * */
header_bar* remplir_header_bar(FILE* f1)
{
    // initialisation de la structure headerbar
    header_bar* hdd = creer_initialiser_header_bar();
    obj_att* p = NULL;
    // lire les attributs de l'entete a partir du fichier
    p = lire_att(f1);
    while (p)   // tant qu'on a des attributs 
    {
        if (!strcmp(p->attribut, "title")) hdd->title = p->valeur;
        else if (!strcmp(p->attribut, "subtitle")) hdd->sub_title = p->valeur;
        else if (!strcmp(p->attribut, "close-btn"))
        {
            if (!strcmp(p->valeur, "FALSE")) hdd->close_button_show = FALSE;
        }
        else if (!strcmp(p->attribut, "decoration-layaout")) 
            hdd->decoration_layout = p->valeur;
        p = p->suiv;
    }
    return (header_bar*)hdd;
}