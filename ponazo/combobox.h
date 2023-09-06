/* La structure de boite de selection *****************************************/
typedef struct cmb  // structure d'un element de la boite
{
    // le text de l'element
    gchar* text;        //gtk_combo_box_text_append_text()
    struct cmb* suiv;   // pointeur sur le suivant
}combo_box_text;
typedef struct      // structure de la boite
{
    GtkWidget* combobox;    // pointeur sur la boite de selection
    // pointeur sur la liste des elements de la boite de selection
    combo_box_text* info;   
}combo_box;
/*******************************************************************************
 * Nom : creer_initaliser_combo_box_text;
 * Sorties : combo_box_text * : la structure combo_box_text initialisee
 * La fonction permet d'allouer un espace memoire pour un element de boite de
 * selection et de l'initialise
 * */
combo_box_text* creer_initaliser_combo_box_text()
{
    // allocation memoire
    combo_box_text* ne = (combo_box_text*)malloc(sizeof(combo_box_text));
    if (!ne)
    {
        printf("\nErreur d'allocation memoire pour combo box text");
        exit(0);
    }
    // initialisation des champs de la structure
    ne->text = NULL;
    ne->suiv = NULL;
    return (combo_box_text*)ne;
}
/*******************************************************************************
 * Nom : creer_initialiser_combo_box;
 * Sorties : combo_box * : la structure combo_box initialisee
 * La fonction permet d'allouer un espace memoire pour une de boite de
 * selection et de l'initialise
 * */
combo_box* creer_initialiser_combo_box()
{
    // allocation memoire pour la boite
    combo_box* cb = (combo_box*)malloc(sizeof(combo_box));
    if (!cb)
    {
        printf("\nErreur d'allocation memoire pour combo box");
        exit(0);
    }
    // initialisation des champs de la structure
    cb->combobox = NULL;
    cb->info = NULL;
    return ((combo_box*)cb);
}
/******************************************************************************
 * Nom : make_combo_box_text;
 * Entrees : combo_box * : la structure d'une combo_box
 * La fonction permet de creer une boite de selection passee en parametres
 * */
void make_combo_box_text(combo_box* cb)
{
    if (!cb)    // test d'existance
    {
        printf("\nErreur: combo box n'existe pas");
        exit(0);
    }
    // Creation de la bite
    GtkWidget* cmtext = gtk_combo_box_text_new();
    combo_box_text* ptr = cb->info;
    while (ptr)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cb->combobox), 
                                        ptr->text);
        ptr = ptr->suiv;
    }
}
combo_box_text* remplir_com_liste(FILE* f1)
{
    char mot[30];
    char text[30];
    char c;
    combo_box_text* liste = NULL;
    combo_box_text* ne = NULL;
    fscanf(f1, "<%35[^>]>", mot);
    fscanf(f1, "%35[^<]<", text);
    fscanf(f1, "%35[^>]>\n", mot);
    fscanf(f1, "<%35[^ ] ", mot);
    liste = creer_initaliser_combo_box_text();
    liste->text = text;
    while (strcmp(mot, "/combotext") != 0)
    {

        c = (char)fgetc(f1);
        fscanf(f1, "%35[^<]<", text);
        ne = creer_initaliser_combo_box_text();
        ne->text = text;
        ne->suiv = liste;
        liste = ne;
        fscanf(f1, "%35[^>]>\n", mot);
        fscanf(f1, "<%35[^ ] ", mot);
    }
    return (combo_box_text*)liste;
}