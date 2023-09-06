/* La structure d'un radiobutton  *********************************************/
typedef struct
{
    GtkWidget* radio_btn; // pointeur sur le widget radio button
    gchar* label;       // label bu radio button
    gint typelabel;
    /*
     * 0 : gtk_radio_button_new_with_label();
     * 1: gtk_radio_button_new_with_mnemonic ();
     * */
    gboolean main_rdb;  // pour savoir que le radio button est le pere
    gboolean mode;
}radio_button;
/*******************************************************************************
 * Nom : creer_initialiser_radio_button;
 * Sorties : radio_button * : la structure radio_button initialisee
 * La fonction permet d'allouer un espace memoire pour un radiobutton et de
 * l'initialisee par des valeurs par defaut
 * */
radio_button* creer_initialiser_radio_button()
{
    // allocation memoire
    radio_button* rdbtn = (radio_button*)malloc(sizeof(radio_button));
    if (!rdbtn)
    {
        printf("error allocation \n");
        exit(-1);
    }
    // initialisation
    rdbtn->radio_btn = NULL;
    rdbtn->label = "RadioButton";
    rdbtn->typelabel = 0;
    rdbtn->main_rdb = FALSE;
    rdbtn->mode = TRUE;
    return (radio_button*)rdbtn;
}
/******************************************************************************
 * Nom : make_radio_button;
 * Entrees : radio_button * : la structure d'un radio_button
 *           GList * : le groupe auquel appartient le radio button
 * La fonction permet de creer un radio button passee en parametres
 * */
void make_radio_button(radio_button* radiobtn_info, GSList* groupe)
{
    if (!radiobtn_info) // test d'existance
    {
        printf("\nErreur : radio button n'existe pas!");
        exit(0);
    }
    // Creation du radio button
    if (!radiobtn_info->typelabel) 
        radiobtn_info->radio_btn = gtk_radio_button_new_with_label(groupe, 
                                                        radiobtn_info->label);
    else radiobtn_info->radio_btn = gtk_radio_button_new_with_mnemonic(groupe, 
                                                        radiobtn_info->label);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(radiobtn_info->radio_btn), 
                                                        radiobtn_info->mode);
    groupe = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radiobtn_info->radio_btn));
}
/******************************************************************************
 * Nom : remplir_radio_button;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : radio_button * : la structure radio_button remplit a partir du
 *           fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * radio_button par les valeurs extrait de fichier
 * */
radio_button* remplir_radio_button(FILE* f1)
{
    radio_button* rdb = creer_initialiser_radio_button(); // initialisation 
    obj_att* p = NULL;
    // lire les attributs de radiobutton a partir du fichier
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->attribut, "label"))
        {
            rdb->label = p->valeur;
            rdb->typelabel = 0;
        }
        else if (!strcmp(p->attribut, "mnemonic"))
        {
            rdb->label = p->valeur;
            rdb->typelabel = 1;
        }
        else if (!strcmp(p->attribut, "mode"))
        {
            if (!strcmp(p->valeur, "FALSE")) rdb->mode = FALSE;
        }
        else if (!strcmp(p->attribut, "mainrdb"))
        {
            if (!strcmp(p->valeur, "TRUE")) rdb->main_rdb = TRUE;
        }
        p = p->suiv;
    }
    return (radio_button*)rdb;
}