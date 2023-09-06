/* La structure de button *****************************************************/
typedef struct
{
    // pointeur sur le bouton
    GtkWidget* button;              
    // titre du bouton
    gchar* title;                   
    // indique si le titre est un label, mnemonic ou icon
    gint typetitle;                 
    /*
     * typetitle = 0 : gtk_button_new_with_label ();
     * typetitle = 1 : gtk_button_new_with_mnemonic();
     * typetitle = 2 : gtk_button_new_from_icon_name();
     */
    // la taille de l'icon
    GtkIconSize icon_siz;           
    GtkReliefStyle relief_type;     //gtk_button_set_relief();
    gboolean use_underline;         //gtk_button_set_use_underline ()
    GtkPositionType image_position; //gtk_button_set_image_position ()
}button;
/*******************************************************************************
 * Nom : creer_initialiser_button;
 * Sorties : button * : la structure button initialisee
 * La fonction permet d'allouer un espace memoire pour un button et de
 * l'initialisee par des valeurs par defaut
 * */
button* creer_initialiser_button()
{
    button* btn = (button*)malloc(sizeof(button)); // allocation memoire
    if (!btn)   
    {
        printf("\nErreur  d'allocation memoire de button !");
        exit(0);
    }
    // initialisation des champs de la structure
    btn->button = NULL;
    btn->title = "Button";
    btn->typetitle = 0;
    btn->icon_siz = GTK_ICON_SIZE_MENU;
    btn->relief_type = GTK_RELIEF_NORMAL;
    btn->use_underline = FALSE;
    btn->image_position = GTK_POS_LEFT;
    return (button*)btn;
}
/******************************************************************************
 * Nom : make_button;
 * Entrees : button * : la structure d'un button
 * La fonction permet de creer un button passee en parametres
 * */
void make_button(button* btn_info)
{
    if (!btn_info)  // test d'existance
    {
        printf("\nErreur : button n'existe pas!");
        exit(0);
    }
    // Creation du button
    if (btn_info->typetitle == 0) 
        btn_info->button = gtk_button_new_with_label(btn_info->title);
    else if (btn_info->typetitle == 1) 
        btn_info->button = gtk_button_new_with_mnemonic(btn_info->title);
    else btn_info->button = gtk_button_new_from_icon_name(btn_info->title, 
                                                          btn_info->icon_siz);
    gtk_button_set_relief(GTK_BUTTON(btn_info->button), btn_info->relief_type);
    gtk_button_set_use_underline(GTK_BUTTON(btn_info->button), 
                                btn_info->use_underline);
    gtk_button_set_image_position(GTK_BUTTON(btn_info->button), 
                                btn_info->image_position);
}
/******************************************************************************
 * Nom : remplir_button;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : button * : la structure button remplit a partir du fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * button par les valeurs extrait de fichier
 * */
button* remplir_button(FILE* f1)
{
    // initialisation de la structure button
    button* btns = creer_initialiser_button();
    obj_att* p = NULL;
    // lire les attributs du button a partir du fichier
    p = lire_att(f1);
    while (p)  
    {
        if (!strcmp(p->attribut, "label"))
        {
            btns->title = p->valeur;
            btns->typetitle = 0;
        }
        else if (!strcmp(p->attribut, "mnemonic"))
        {
            btns->title = p->valeur;
            btns->typetitle = 1;
        }
        else if (!strcmp(p->attribut, "icon-name"))
        {
            btns->title = p->valeur;
            btns->typetitle = 2;
        }
        else if (!strcmp(p->attribut, "icon-size"))
        {
            if (!strcmp(p->valeur, "INVALID")) 
                btns->icon_siz = GTK_ICON_SIZE_INVALID;
            else if (!strcmp(p->valeur, "SMALL_TOOLBAR")) 
                btns->icon_siz = GTK_ICON_SIZE_SMALL_TOOLBAR;
            else if (!strcmp(p->valeur, "LARGE_TOOLBAR")) 
                btns->icon_siz = GTK_ICON_SIZE_LARGE_TOOLBAR;
            else if (!strcmp(p->valeur, "BUTTON")) 
                btns->icon_siz = GTK_ICON_SIZE_BUTTON;
            else if (!strcmp(p->valeur, "DND")) 
                btns->icon_siz = GTK_ICON_SIZE_DND;
            else if (!strcmp(p->valeur, "DIALOG")) 
                btns->icon_siz = GTK_ICON_SIZE_DIALOG;
        }
        else if (!strcmp(p->attribut, "relief-type"))
        {
            if (!strcmp(p->valeur, "HALF")) 
                btns->relief_type = GTK_RELIEF_HALF;
            else if (!strcmp(p->valeur, "NONE")) 
                btns->relief_type = GTK_RELIEF_NONE;
        }
        else if (!strcmp(p->attribut, "underline"))
        {
            if (!strcmp(p->valeur, "TRUE")) btns->use_underline = TRUE;
        }
        else if (!strcmp(p->attribut, "image-position"))
        {
            if (!strcmp(p->valeur, "RIGHT")) 
                btns->image_position = GTK_POS_RIGHT;
            else if (!strcmp(p->valeur, "TOP")) 
                btns->image_position = GTK_POS_TOP;
            else if (!strcmp(p->valeur, "BOTTOM")) 
                btns->image_position = GTK_POS_BOTTOM;
        }
        p = p->suiv;
    }
    return (button*)btns;
}