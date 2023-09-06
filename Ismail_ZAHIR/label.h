/* La structure d'un label ***************************************************/
typedef struct
{
    // pointeur sur le widget label
    GtkWidget* label;
    gchar* text;    //texte du label
    gint typetext;
    /*
     * 0 : GtkWidget *gtk_label_new (gchar *);
     * 1 : GtkWidget *gtk_label_new_with_mnemonic(gchar *);
     */
    /* le texte d'un widget d'etiquette en utilisant des balises Pango Markup
    pour formater le texte */
    gchar* markup_text;              //gtk_label_set_markup ()
    gchar* markup_text_with_mnemonic;//gtk_label_set_markup_with_mnemonic ()
    //un motif pour le texte d'un widget d'étiquette
    gchar* pattern;                  //gtk_label_set_pattern ()
    /* l'alignement horizontal du texte dans un widget d'etiquette.
    GTK_JUSTIFY_LEFT : pour aligner le texte à gauche du widget d'étiquette.
    GTK_JUSTIFY_RIGHT : pour aligner le texte à droite du widget d'étiquette.
    GTK_JUSTIFY_CENTER : pour aligner le texte au centre du widget d'étiquette.
    GTK_JUSTIFY_FILL : pour étirer le texte pour remplir tout l'espace disponible
                        dans le widget d'étiquette.
    */
    GtkJustification justify;       //gtk_label_set_justify ()
    //position de de la zone texte
    gdouble xalign;                 //gtk_label_set_xalign ()
    gdouble yalign;                 //gtk_label_set_yalign ()
    //un texte  tronque pour s'adapter a la taille d'un widget d'etiquette
    PangoEllipsizeMode ellipsis;    //gtk_label_set_ellipsize ()
    /* si le texte affiche dans un widget d'étiquette peut etre selectionné ou 
    non par l'utilisateur. */
    gboolean selectable;            //gtk_label_set_selectable()
}label;
/*******************************************************************************
 * Nom : crer_initialiser_label;
 * Sorties : label * : la structure label initialisee
 * La fonction permet d'allouer un espace memoire pour un label et de
 * l'initialisee par des valeurs par defaut
 * */
label* crer_initialiser_label()
{
    label* lb = (label*)malloc(sizeof(label)); // allocation memoire
    if (!lb)
    {
        printf("\nErreur d'allocation memoire pour label!");
        exit(0);
    }
    // initialisation des champs de la structure
    lb->label = NULL;
    lb->text = "";
    lb->typetext = 0;
    lb->markup_text = NULL;
    lb->markup_text_with_mnemonic = NULL;
    lb->pattern = NULL;
    lb->justify = GTK_JUSTIFY_LEFT;
    lb->ellipsis = PANGO_ELLIPSIZE_NONE;
    lb->selectable = FALSE;
    lb->xalign = 0.5;
    lb->yalign = 0.5;
    return (label*)lb;
}
/******************************************************************************
 * Nom : make_label;
 * Entrees : label * : la structure d'un label
 * La fonction permet de creer un label passee en parametres
 * */
void make_label(label* label_info)
{
    if (!label_info) // test d'existance
    {
        printf("\nErreur : label n'existe pas");
        exit(0);
    }
    // Creation du label
    if (!label_info->typetext) 
        label_info->label = gtk_label_new(label_info->text); 
    else label_info->label = gtk_label_new_with_mnemonic(label_info->text); 
    if (label_info->markup_text) 
        gtk_label_set_markup(GTK_LABEL(label_info->label),
                             label_info->markup_text);
    if (label_info->markup_text_with_mnemonic) 
        gtk_label_set_markup_with_mnemonic(GTK_LABEL(label_info->label), 
                                        label_info->markup_text_with_mnemonic);
    if (label_info->pattern) gtk_label_set_pattern(GTK_LABEL(label_info->label), 
                                                   label_info->pattern);
    gtk_label_set_justify(GTK_LABEL(label_info->label), label_info->justify);
    gtk_label_set_xalign(GTK_LABEL(label_info->label), 
                         (gfloat)label_info->xalign);
    gtk_label_set_yalign(GTK_LABEL(label_info->label), 
                         (gfloat)label_info->yalign);
    gtk_label_set_ellipsize(GTK_LABEL(label_info->label), 
                            label_info->ellipsis);
    gtk_label_set_selectable(GTK_LABEL(label_info->label),
                             label_info->selectable);
}
/******************************************************************************
 * Nom : remplir_label;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : label * : la structure label remplit a partir du fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * label par les valeurs extrait de fichier
 * */
label* remplir_label(FILE* f1)
{
    // initialisation de la structure label
    label* lab = crer_initialiser_label();
    obj_att* p = NULL;
    // lire les attributs de la structure label a partir du fichier
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->attribut, "label"))
        {
            lab->text = p->valeur;
            lab->typetext = 0;
        }
        else if (!strcmp(p->attribut, "mnemonic"))
        {
            lab->text = p->valeur;
            lab->typetext = 1;
        }
        else if (!strcmp(p->attribut, "label-markup")) 
            lab->markup_text = p->valeur;
        else if (!strcmp(p->attribut, "label-markup-mnemonic")) 
            lab->markup_text_with_mnemonic = p->valeur;
        else if (!strcmp(p->attribut, "pattern")) lab->pattern = p->valeur;
        else if (!strcmp(p->attribut, "justify"))
        {
            if (!strcmp(p->valeur, "RIGHT")) lab->justify = GTK_JUSTIFY_RIGHT;
            else if (!strcmp(p->valeur, "CENTER")) 
                lab->justify = GTK_JUSTIFY_CENTER;
            else if (!strcmp(p->valeur, "FILL")) 
                lab->justify = GTK_JUSTIFY_FILL;
        }
        else if (!strcmp(p->attribut, "xalign")) lab->xalign = atof(p->valeur);
        else if (!strcmp(p->attribut, "yalign")) lab->yalign = atof(p->valeur);
        else if (!strcmp(p->attribut, "ellipsis"))
        {
            if (!strcmp(p->valeur, "START")) 
                lab->ellipsis = PANGO_ELLIPSIZE_START;
            else if (!strcmp(p->valeur, "MIDDLE")) 
                lab->ellipsis = PANGO_ELLIPSIZE_MIDDLE;
            else if (!strcmp(p->valeur, "END")) 
                lab->ellipsis = PANGO_ELLIPSIZE_END;
        }
        else if (!strcmp(p->attribut, "selectable"))
        {
            if (!strcmp(p->valeur, "TRUE"))lab->selectable = TRUE;
        }
        p = p->suiv;
    }
    return (label*)lab;
}