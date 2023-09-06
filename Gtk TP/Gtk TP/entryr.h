/* La structure d'une entree **************************************************/
typedef struct
{
    GtkWidget* entry;           // pointeur sur le widget entry
    // le texte à saisir.
    gchar* text;                //gtk_entry_set_text ()
    //le texte est visible ou pas.
    gboolean visibilty;         //gtk_entry_set_visibility ()
    //nombre max des widgets d'une entrée.
    gint max_entry;             //gtk_entry_set_max_length ()
    //si une bordure doit être affichée autour de l'entrée de texte ou non.
    gboolean frame;             //gtk_entry_set_has_frame ()
    //l'alignement horizontal du texte dans l'entrée de texte.
    gfloat alignement;          //gtk_entry_set_alignment ()
    //un texte qui est affiché dans l'entrée de texte lorsque celle-ci est vide.
    gchar* placeholder;         //gtk_entry_set_placeholder_text ()
    //si le mode d'écriture doit être en mode d'insertion ou en mode d'écrasement.
    gboolean overwriten;        //gtk_entry_set_overwrite_mode ()
    // une icône à afficher dans l'entrée de texte à partir de son nom.
    gchar* icon_name;           //gtk_entry_set_icon_from_icon_name()
    // position d'un icon
    GtkEntryIconPosition icon_position;
    //le texte de l'info-bulle qui apparaît lorsque l'utilisateur survole une icône
    gchar* tooltip_text_icon;  //gtk_entry_set_icon_tooltip_text ()
    //le but de l'entrée de texte
    GtkInputPurpose purpose;    //gtk_entry_set_input_purpose ()
    //fournir des indications sur le type de données que l'utilisateur est censé saisir.
    GtkInputHints inputhints;   //gtk_entry_set_input_hints()
}entry;
/*******************************************************************************
 * Nom : crrer_initialser_entry;
 * Sorties : enty * : la structure d'une entree initialisee
 * La fonction permet d'allouer un espace memoire pour une entree et de 
 * l'initialise
 * */
entry* crrer_initialser_entry()
{
    // allocation memoire pour la structure d'entree
    entry* ent = (entry*)malloc(sizeof(entry));
    if (!ent)
    {
        printf("\nErreur d'allocation memoire pour entry");
        exit(0);
    }
    // initialisation des champs de la structure d'entree
    ent->entry = NULL;
    ent->text = NULL;
    ent->placeholder = NULL;
    ent->icon_name = NULL;
    ent->tooltip_text_icon = NULL;
    ent->icon_position = GTK_ENTRY_ICON_PRIMARY;
    ent->max_entry = 80;
    ent->frame = TRUE;
    ent->alignement = 0;
    ent->overwriten = TRUE;
    ent->purpose = GTK_INPUT_PURPOSE_EMAIL;
    ent->inputhints = GTK_INPUT_HINT_NONE;
    ent->visibilty = TRUE;
    return (entry*)ent;
}
/*******************************************************************************
 * Nom : make_entry;
 * Entrees : button * : la structure d'une entree
 * La fonction permet de creer une entree passee en parametres
 * */
void make_entry(entry* en_info)
{
    if (!en_info)       // test d'existance
    {
        printf("\nErreur : entry n'existe pas");
        exit(0);
    }
    // creation de l'entree
    en_info->entry = gtk_entry_new();
    if (en_info->text) gtk_entry_set_text(GTK_ENTRY(en_info->entry), 
                                                    en_info->text);
    gtk_entry_set_visibility(GTK_ENTRY(en_info->entry), en_info->visibilty);
    gtk_entry_set_max_length(GTK_ENTRY(en_info->entry), en_info->max_entry);
    gtk_entry_set_has_frame(GTK_ENTRY(en_info->entry), en_info->frame);
    gtk_entry_set_alignment(GTK_ENTRY(en_info->entry), en_info->alignement);
    if (en_info->placeholder) 
        gtk_entry_set_placeholder_text(GTK_ENTRY(en_info->entry),
                                       en_info->placeholder);
    gtk_entry_set_overwrite_mode(GTK_ENTRY(en_info->entry),en_info->overwriten);
    if (en_info->icon_name)
        gtk_entry_set_icon_from_icon_name(GTK_ENTRY(en_info->entry),
                                            en_info->icon_position,
                                            en_info->icon_name);
    if (en_info->tooltip_text_icon) 
        gtk_entry_set_icon_tooltip_text(GTK_ENTRY(en_info->entry), 
                                        en_info->icon_position,
                                        en_info->tooltip_text_icon);
    gtk_entry_set_input_purpose(GTK_ENTRY(en_info->entry), en_info->purpose);
    gtk_entry_set_input_hints(GTK_ENTRY(en_info->entry), en_info->inputhints);
}
entry* remplir_entry(FILE* f1)
{
    entry* ent = crrer_initialser_entry();
    obj_att* p = NULL;
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->attribut, "text")) ent->text = p->valeur;
        else if (!strcmp(p->attribut, "visibility"))
        {
            if (!strcmp(p->valeur, "FALSE")) ent->visibilty = FALSE;
        }
        else if (!strcmp(p->attribut, "max")) ent->max_entry = atoi(p->valeur);
        else if (!strcmp(p->attribut, "frame"))
        {
            if (!strcmp(p->valeur, "FALSE")) ent->frame = FALSE;
        }
        else if (!strcmp(p->attribut, "alignement")) ent->alignement = atof(p->valeur);
        else if (!strcmp(p->attribut, "placeholder")) ent->placeholder = p->valeur;
        else if (!strcmp(p->attribut, "overwriten"))
        {
            if (!strcmp(p->valeur, "FALSE")) ent->overwriten = FALSE;
        }
        else if (!strcmp(p->attribut, "icon-position"))
        {
            if (!strcmp(p->valeur, "SECONDARY")) ent->icon_position = GTK_ENTRY_ICON_SECONDARY;
            else ent->icon_position = GTK_ENTRY_ICON_PRIMARY;
        }
        else if (!strcmp(p->attribut, "icon-name")) ent->icon_name = p->valeur;
        else if (!strcmp(p->attribut, "tool-type-text")) ent->tooltip_text_icon = p->valeur;
        else if (!strcmp(p->attribut, "purpose"))
        {
            if (!strcmp(p->valeur, "FREE_FORM")) ent->purpose = GTK_INPUT_PURPOSE_FREE_FORM;
            else if (!strcmp(p->valeur, "DIGITS")) ent->purpose = GTK_INPUT_PURPOSE_DIGITS;
            else if (!strcmp(p->valeur, "NUMBER")) ent->purpose = GTK_INPUT_PURPOSE_NUMBER;
            else if (!strcmp(p->valeur, "PHONE")) ent->purpose = GTK_INPUT_PURPOSE_PHONE;
            else if (!strcmp(p->valeur, "URL")) ent->purpose = GTK_INPUT_PURPOSE_URL;
            else if (!strcmp(p->valeur, "EMAIL")) ent->purpose = GTK_INPUT_PURPOSE_EMAIL;
            else if (!strcmp(p->valeur, "NAME")) ent->purpose = GTK_INPUT_PURPOSE_NAME;
            else if (!strcmp(p->valeur, "PASSWORD")) ent->purpose = GTK_INPUT_PURPOSE_PASSWORD;
            else if (!strcmp(p->valeur, "PIN")) ent->purpose = GTK_INPUT_PURPOSE_PIN;
            else if (!strcmp(p->valeur, "TERMINAL")) ent->purpose = GTK_INPUT_PURPOSE_TERMINAL;
        }
        else if (!strcmp(p->attribut, "input-hints"))
        {
            if (!strcmp(p->valeur, "LOWERCASE")) ent->inputhints = GTK_INPUT_HINT_LOWERCASE;
            else if (!strcmp(p->valeur, "UPPERCASE_CHARS")) ent->inputhints = GTK_INPUT_HINT_UPPERCASE_CHARS;
            else if (!strcmp(p->valeur, "UPPERCASE_WORDS")) ent->inputhints = GTK_INPUT_HINT_UPPERCASE_WORDS;
            else if (!strcmp(p->valeur, "UPPERCASE_SENTENCES")) ent->inputhints = GTK_INPUT_HINT_UPPERCASE_SENTENCES;
            else if (!strcmp(p->valeur, "INHIBIT_OSK")) ent->inputhints = GTK_INPUT_HINT_INHIBIT_OSK;
            else if (!strcmp(p->valeur, "SPELLCHECK")) ent->inputhints = GTK_INPUT_HINT_SPELLCHECK;
            else if (!strcmp(p->valeur, "NO_EMOJI")) ent->inputhints = GTK_INPUT_HINT_NO_EMOJI;
        }
        p = p->suiv;
    }
    return (entry*)ent;
}