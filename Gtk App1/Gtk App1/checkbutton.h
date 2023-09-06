/* La structure d'un checkbutton **********************************************/
typedef struct
{
    // pointeur sur le widget checkbutton
    GtkWidget* checkbtn;
    // le titre du button
    gchar* title;
    // indique si le titre est un label ou mnemonic
    gint typetitle;
    /*
     * 0: label     gtk_check_button_new_with_label ();
     * 1: mnemonic  gtk_check_button_new_with_mnemonic ();
     * */
    gboolean mode;    //gtk_toggle_button_set_mode()
}check_button;
/*******************************************************************************
 * Nom : creer_initialiser_check_button;
 * Sorties : check_button * : la structure checkbtn initialisee
 * La fonction permet d'allouer un espace memoire pour un checkbutton et de
 * l'initialisee par des valeurs par defaut
 * */
check_button* creer_initialiser_check_button()
{
    // allocation memoire de la structure checkbutton
    check_button* checkbtn = (check_button*)malloc(sizeof(check_button));
    if (!checkbtn)
    {
        printf("\nErreur d'allocation memoire pour checkbutton!");
        exit(0);
    }
    // initialisation des champs de la structure
    checkbtn->checkbtn = NULL;
    checkbtn->title = "CheckButton";
    checkbtn->typetitle = 0;
    checkbtn->mode = TRUE;
    return (check_button*)checkbtn;
}
/******************************************************************************
 * Nom : make_check_button;
 * Entrees : check_button * : la structure d'un checkbutton
 * La fonction permet de creer un checkbutton passee en parametres
 * */
void make_check_button(check_button* checkbtn_info)
{
    if (!checkbtn_info)     // test d'existance
    {
        printf("\nErreur : Checkbutton n'xiste pas!");
        exit(0);
    }
    // Creation du checkbutton
    if (!checkbtn_info->typetitle) 
        checkbtn_info->checkbtn = gtk_check_button_new_with_label (
                                                checkbtn_info->title);
    else checkbtn_info->checkbtn = gtk_check_button_new_with_mnemonic (
                                                checkbtn_info->title);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(checkbtn_info->checkbtn), 
                               checkbtn_info->mode);
}
/******************************************************************************
 * Nom : remplir_check_button;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : check_button * : la structure check_button remplit a partir du
 *           fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * check_button par les valeurs extrait de fichier
 * */
check_button* remplir_check_button(FILE* f1)
{
    check_button* ckb = creer_initialiser_check_button(); // initialisation
    obj_att* p = NULL;
    // lire les attributs de checkbutton a partir du fichier
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->attribut, "label"))
        {
            ckb->title = p->valeur;
            ckb->typetitle = 0;
        }
        else if (!strcmp(p->attribut, "mnemonic"))
        {
            ckb->title, p->valeur;
            ckb->typetitle = 1;
        }
        else if (!strcmp(p->attribut, "mode"))
        {
            if (!strcmp(p->valeur, "FALSE")) ckb->mode = FALSE;
        }
        p = p->suiv;
    }
    return (check_button*)ckb;
}