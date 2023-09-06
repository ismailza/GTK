/* La structure d'une toggle button  ******************************************/
typedef struct
{
    GtkWidget* toogle_btn;      // pointeur sur le widget toggle button
    gchar* title;               // le titre du toggle  button
    gint typetitle;             // indique si le titre est un label ou mnemonic
    gboolean mode;              //gtk_toggle_button_set_mode()
    gboolean inconsitant;       //gtk_toggle_button_set_inconsistent()
}toggle_button;
/*******************************************************************************
 * Nom : creer_initialiser_toggle_button;
 * Sorties : toggle_button * : la structure toggle_button initialisee
 * La fonction permet d'allouer un espace memoire pour un toggle_button et de
 * l'initialisee par des valeurs par defaut
 * */
toggle_button* creer_initialiser_toggle_button()
{
    // allocation memoire pour la structure toggle button
    toggle_button* tglbtn = (toggle_button*)malloc(sizeof(toggle_button));
    if (!tglbtn)
    {
        printf("\nErreur d'allocation memoire pour toggle button!");
        exit(0);
    }
    // initialisation des champs de la structure
    tglbtn->toogle_btn = NULL;
    tglbtn->title = "Toggle Button";
    tglbtn->toogle_btn = 0;
    tglbtn->mode = FALSE;
    tglbtn->inconsitant = FALSE;
    return (toggle_button*)tglbtn;
}
/*******************************************************************************
 * Nom : make_toggle_button;
 * Entrees : toggle_button * : la structure d'un toggle button
 * La fonction permet de creer un toggle button passee en parametres
 * */
void make_toggle_button(toggle_button* tgl_btn_info)
{
    if (!tgl_btn_info) // test d'existance
    {
        printf("\nErreur : toggle button n'existe pas");
        exit(0);
    }
    // Creation du toggle button
    if (!tgl_btn_info->typetitle) 
        tgl_btn_info->toogle_btn = gtk_toggle_button_new_with_label (
                                                            tgl_btn_info->title);
    else tgl_btn_info->toogle_btn = gtk_toggle_button_new_with_mnemonic (
                                                             tgl_btn_info->title);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(tgl_btn_info->toogle_btn),
                               tgl_btn_info->mode);
    gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(tgl_btn_info->toogle_btn), 
                                       tgl_btn_info->inconsitant);
}
/******************************************************************************
 * Nom : remplir_toggle_button;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : toggle_button * : la structure check_button remplit a partir du
 *           fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * toggle_button par les valeurs extrait de fichier
 * */
toggle_button* remplir_toggle_button(FILE* f1)
{
    toggle_button* tglb = creer_initialiser_toggle_button(); // initialisation
    obj_att* p = NULL;
    // lire les attributs de toggle button a partir du fichier
    p = lire_att(f1);
    while (p) 
    {
        if (!strcmp(p->attribut, "label")) 
        {
            tglb->title = p->valeur; 
            tglb->typetitle = 0; 
        }
        else if (!strcmp(p->attribut, "mnemonic"))
        {
            tglb->title = p->valeur;
            tglb->typetitle = 1;
        }
        else if (!strcmp(p->attribut, "mode"))
        {
            if (!strcmp(p->valeur, "TRUE")) tglb->mode = TRUE;
        }
        else if (!strcmp(p->attribut, "inconsistent"))
        {
            if (!strcmp(p->valeur, "TRUE")) tglb->inconsitant = TRUE;
        }
        p = p->suiv;
    }
    return ((toggle_button*)tglb);
}