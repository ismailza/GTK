/* La structure d'un linkbutton ***********************************************/
typedef struct
{
    GtkWidget* link_btn; // pointeur sur le widget linkbutton
    // label du button
    gchar* label;       //gtk_link_button_new_with_label ()
    // le lien 
    gchar* url;         //gtk_link_button_set_uri ()
    gboolean visited;  //gtk_link_button_set_visited ()
}link_button;
/*******************************************************************************
 * Nom : creer_initialiser_link_button;
 * Sorties : link_button * : la structure link_buuton initialisee
 * La fonction permet d'allouer un espace memoire pour un button de lien et de
 * l'initialisee par des valeurs par defaut
 * */
link_button* creer_initialiser_link_button()
{
    // allocation memoire pour le button 
    link_button* linbtn = (link_button*)malloc(sizeof(link_button));
    if (!linbtn)
    {
        printf("\nErreur d'allocation memoire pour link button");
        exit(0);
    }
    // initialisation des champs de la structure
    linbtn->link_btn = NULL;
    linbtn->label = "Link button";
    linbtn->url = NULL;
    linbtn->visited = FALSE;
    return (link_button*)linbtn;
}
/******************************************************************************
 * Nom : make_link_button;
 * Entrees : label * : la structure d'un link_button
 * La fonction permet de creer un link_button passee en parametres
 * */
void make_link_button(link_button* link_btn_info)
{
    if (!link_btn_info) // test d'existance
    {
        printf("\nErreur : link button n'existe pas!");
        exit(0);
    }
    // Creation de link_buttob
    link_btn_info->link_btn = gtk_link_button_new_with_label(link_btn_info->url,
                                                          link_btn_info->label);
    gtk_link_button_set_visited(GTK_LINK_BUTTON(link_btn_info->link_btn),
                                link_btn_info->visited);
}
/******************************************************************************
 * Nom : remplir_link_button;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : toggle_button * : la structure link_button remplit a partir du
 *           fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * link_button par les valeurs extrait de fichier
 * */
link_button* remplir_link_button(FILE* f1)
{
    link_button* linkb = creer_initialiser_link_button();
    obj_att* p = NULL;
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->valeur, "label")) linkb->label = p->valeur;
        else if (!strcmp(p->valeur, "url")) linkb->url = p->valeur;
        else if (!strcmp(p->valeur, "visited"))
        {
            if (!strcmp(p->valeur, "TRUE")) linkb->visited = TRUE;
        }
        p = p->suiv;
    }
    return (link_button*)linkb;
}