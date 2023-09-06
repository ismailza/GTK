/* La structure d'une bar de recherche  ***************************************/
typedef struct 
{
    GtkWidget* serch_bar;       // pointeur sur la bar de recherche
    // si la barre de recherche doit être en mode recherche.
    gboolean search_mod;        //gtk_search_bar_set_search_mode ()
    // afficher ou masquer le closebutton 
    gboolean shwo_close_button; //gtk_search_bar_set_show_close_button ()
}search_bar;
/*******************************************************************************
 * Nom : creer_intialiser_search_bar;
 * Sorties : search_bar * : la structure search_bar initialisee
 * La fonction permet d'allouer un espace memoire pour un search_bar et de
 * l'initialisee par des valeurs par defaut
 * */
search_bar* creer_intialiser_search_bar()
{
    // allocation memoire pour la structure
    search_bar* srchb = (search_bar*)malloc(sizeof(search_bar));
    if (!srchb)
    {
        printf("\nErreur d'allocation memoire pour search bar");
        exit(0);
    }
    // initialisation des champs de la structure
    srchb->serch_bar = NULL;
    srchb->search_mod = TRUE;
    srchb->shwo_close_button = TRUE;
    return (search_bar*)srchb;
}
/******************************************************************************
 * Nom : make_search_bar;
 * Entrees : search_bar * : la structure d'un search_bar
 * La fonction permet de creer un search_bar passee en parametres
 * */
void make_search_bar(search_bar* srbar_info)
{
    if (!srbar_info)    // test d'existance
    {
        printf("\nErreur : search bar n'existe pas");
        exit(0);
    }
    // Creation de la bar de recherche
    srbar_info->serch_bar = gtk_search_bar_new();
    gtk_search_bar_set_search_mode(GTK_SEARCH_BAR(srbar_info->serch_bar), 
                                    srbar_info->search_mod);
    gtk_search_bar_set_show_close_button(GTK_SEARCH_BAR(srbar_info->serch_bar), 
                                    srbar_info->shwo_close_button);
}

search_bar* remplir_search_bar(FILE* f1)
{
    search_bar* srb = creer_intialiser_search_bar();
    obj_att* p = NULL;
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->attribut, "mode"))
        {
            if (!strcmp(p->valeur, "FALSE")) srb->search_mod = FALSE;
        }
        else if (!strcmp(p->attribut, "close-btn"))
        {
            if (!strcmp(p->valeur, "FALSE")) srb->shwo_close_button = FALSE;
        }
        p = p->suiv;
    }
    return (search_bar*)srb;
}