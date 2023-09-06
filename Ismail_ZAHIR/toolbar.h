/* La structure d'une barre d'outils  *****************************************/
typedef struct tli  // la structure d'un element de la barre d'outils
{
    GtkToolItem* item;      // pointeur sur le widget tool item
    // titre d'un tool item
    gchar* label;           //gtk_tool_button_set_label()
    // si le texte du bouton d'outil doit etre souligne 
    gboolean underline;     //gtk_tool_button_set_use_underline()
    // l'icone du bouton d'outil
    gchar* icon_name;       //gtk_tool_button_set_icon_name ()
    // le texte de l'info-bulle du bouton d'outil
    gchar *tooltip_markup;  //gtk_tool_item_set_tooltip_markup ()
    gchar *tooltip_text;    //gtk_tool_item_set_tooltip_text ()
    struct tli* suiv;
}tool_item;     
typedef struct  // structure de la barre d'outils
{
    GtkWidget* toolbar;         // pointeur sur le barre d'outils
    /* si les fleches doivent etre affichees pour les elements qui ne peuvent
    pas tenir sur la barre d'outils. */
    gboolean show_arrow;            //gtk_toolbar_set_show_arrow ()
    // taille de l'icon
    GtkIconSize icon_size;          //gtk_toolbar_set_icon_size ()
    // le style de la barre d'outils
    GtkToolbarStyle tolbar_style;   //gtk_toolbar_set_style ()
    // un pointeur de type sous structure tool_item
    tool_item* info;    //gtk_toolbar_insert ()
}tool_bar;
/*******************************************************************************
 * Nom : creer_intialiser_tool_item;
 * Sorties : tool_item * : la structure tool_item initialisee
 * La fonction permet d'allouer un espace memoire pour un element de la barre 
 * d'outils  et de l'initialisee par des valeurs par defaut
 * */
tool_item* creer_intialiser_tool_item()
{
    // allocation memoire de la structure
    tool_item* ne = (tool_item*)malloc(sizeof(tool_item));
    if (!ne)
    {
        printf("\nErreur d'allocation memoire pour tool item");
        exit(0);
    }
    // Initialisation des champs de la structure
    ne->item = NULL;
    ne->label = NULL;
    ne->icon_name = NULL;
    ne->tooltip_markup = NULL;
    ne->tooltip_text = NULL;
    ne->suiv = NULL;
    return (tool_item*)ne;
}
/*******************************************************************************
 * Nom : creer_initilaiser_tool_bar;
 * Sorties : tool_bar * : la structure tool_bar initialisee
 * La fonction permet d'allouer un espace memoire pour une barre d'outils et de
 * l'initialisee par des valeurs par defaut
 * */
tool_bar* creer_initilaiser_tool_bar()
{
    // allocation memoire 
    tool_bar* tlb = (tool_bar*)malloc(sizeof(tool_bar));
    if (!tlb)
    {
        printf("\nErreur d'allocation memoire pour toolbar!");
        exit(0);
    }
    // initialisation des champs de la structure
    tlb->toolbar = NULL;
    tlb->show_arrow = TRUE;
    tlb->icon_size = GTK_ICON_SIZE_MENU;
    tlb->tolbar_style = GTK_TOOLBAR_TEXT;
    tlb->info = NULL;
    return (tool_bar*)tlb;
}
/*******************************************************************************
 * Nom : make_tool_bar;
 * Entrees : tool_bar * : la structure d'une barre d'outils
 * La fonction permet de creer une barre d'outils passee en parametres
 * */
void make_tool_bar(tool_bar* tlb)
{
    if (!tlb)   // test d'existance
    {
        printf("\nErreur: toolbar n'existe pas");
        exit(0);
    }
    // Creation de la barre d'outils
    tlb->toolbar = gtk_toolbar_new();
    gtk_toolbar_set_show_arrow(GTK_TOOLBAR(tlb->toolbar), tlb->show_arrow);
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(tlb->toolbar), tlb->icon_size);
    gtk_toolbar_set_style(GTK_TOOLBAR(tlb->toolbar), tlb->tolbar_style);
    tool_item* p = tlb->info;
    while (p)   // tant qu'on a des tool items
    {
        p->item = gtk_tool_button_new(NULL, NULL);
        gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(p->item), p->underline);
        if (p->label)
        {
            if (strcmp(p->label, "separtor") == 0) 
                p->item = gtk_separator_tool_item_new();
            else gtk_tool_button_set_label(GTK_TOOL_BUTTON(p->item), p->label);
        }
        else if (p->icon_name) 
            gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(p->item), p->icon_name);
        if (p->tooltip_markup) 
            gtk_tool_item_set_tooltip_markup(p->item, p->tooltip_markup);
        else if (p->tooltip_text) 
            gtk_tool_item_set_tooltip_text(p->item, p->tooltip_text);
        gtk_toolbar_insert(GTK_TOOLBAR(tlb->toolbar), p->item, -1);
        p = p->suiv;
    }
}
/******************************************************************************
 * Nom : remplir_tool_item;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : tool_item * : la structure menu_button remplit a partir du fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * tool_item par les valeurs extrait de fichier
 * */
tool_item* remplir_tool_item(FILE* f1)
{
    tool_item* toit = creer_intialiser_tool_item(); // initialisation
    obj_att* p = NULL;
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->attribut, "label")) toit->label = p->valeur;
        else if (!strcmp(p->attribut, "icon-name")) toit->icon_name = p->valeur;
        else if (!strcmp(p->attribut, "tooltip-markup")) toit->tooltip_markup, p->valeur;
        else if (!strcmp(p->attribut, "tooltip-text")) toit->tooltip_text, p->valeur;
        p = p->suiv;
    }
    return (tool_item*)toit;
}
tool_item* remplir_tool_item_liste(FILE* f1)
{
    char mot[30];
    tool_item* liste = NULL;
    tool_item* ne = NULL;
    fscanf(f1, "<%35[^ ] ", mot);
    liste = remplir_tool_item(f1);
    fscanf(f1, "<%35[^>]>\n", mot);
    epurer_blan(f1);
    fscanf(f1, "<%35[^ ] ", mot);
    while (strcmp(mot, "/toolbar") != 0)
    {
        ne = remplir_tool_item(f1);
        ne->suiv = liste;
        liste = ne;
        fscanf(f1, "<%35[^>]>\n", mot);
        epurer_blan(f1);
        fscanf(f1, "<%35[^ ] ", mot);
    }
    fscanf(f1, ">\n");
    epurer_blan(f1);
    return (tool_item*)liste;
}
tool_bar* remplir_tool_bar(FILE* f1)
{
    tool_bar* tlbar = creer_initilaiser_tool_bar();
    obj_att* p = NULL;
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->attribut, "show-arrow"))
        {
            if (!strcmp(p->valeur, "FALSE")) tlbar->show_arrow = FALSE;
        }
        else if (!strcmp(p->attribut, "icon-size"))
        {
            if (!strcmp(p->valeur, "INVALID")) tlbar->icon_size = GTK_ICON_SIZE_INVALID;
            else if (!strcmp(p->valeur, "SMALL_TOOLBAR")) tlbar->icon_size = GTK_ICON_SIZE_SMALL_TOOLBAR;
            else if (!strcmp(p->valeur, "LARGE_TOOLBAR")) tlbar->icon_size = GTK_ICON_SIZE_LARGE_TOOLBAR;
            else if (!strcmp(p->valeur, "BUTTON")) tlbar->icon_size = GTK_ICON_SIZE_BUTTON;
            else if (!strcmp(p->valeur, "DND")) tlbar->icon_size = GTK_ICON_SIZE_DND;
            else if (!strcmp(p->valeur, "DIALOG")) tlbar->icon_size = GTK_ICON_SIZE_DIALOG;
        }
        else if (!strcmp(p->attribut, "style"))
        {
            if (!strcmp(p->valeur, "ICONS")) tlbar->tolbar_style = GTK_TOOLBAR_ICONS;
            else if (!strcmp(p->valeur, "BOTH")) tlbar->tolbar_style = GTK_TOOLBAR_BOTH;
            else if (!strcmp(p->valeur, "BOTH_HORIZ")) tlbar->tolbar_style = GTK_TOOLBAR_BOTH_HORIZ;
        }
        p = p->suiv;
    }
    tlbar->info = remplir_tool_item_liste(f1);
    return (tool_bar*)tlbar;
}