/* La structure d'une boite ***************************************************/
/*
 * structure du widget GTK_BOX
 * Expend: n'est utile que si la GtkBox en question n'est pas définie comme 
 * homogène (homogeneous=FALSE lors de la création).
 * Dans ce cas, tous les widgets qui auront été insérés avec la valeur expand=TRUE
 * se partageront tout l'espace libre de la GtkBox (les widgets avec expand=FALSE
 *  n'utiliseront que l'espace qui leur est nécessaire).
 */
typedef struct
{
    // pointeur sur le widget box
    GtkWidget* box;                     // gtk_box_new()
    // indique l'orientation
    GtkOrientation orientation;
    // indique Si les fils doivent tous avoir la même taille
    gboolean homogeneous;               // gtk_box_set_homogeneous()           
    // indique la taille en pixels entre les fils de la boite
    gint spacing;                       //gtk_box_set_spacing ()
    GtkBaselinePosition basline_position;// gtk_box_set_baseline_position ()
}box;
/*******************************************************************************
 * Nom : creer_initialiser_box;
 * Sorties : box * : la structure box initialisee
 * La fonction permet d'allouer un espace memoire pour une boite et de
 * l'initialisee par des valeurs par defaut
 * */
box* creer_initialiser_box()
{
    box* bx = (box*)malloc(sizeof(box));    // allocation memoire
    if (!bx)
    {
        printf("\nErreur d'allocation memoire pour box");
        exit(0);
    }
    // initialisation des champs de la structure box
    bx->box = NULL;
    bx->basline_position = GTK_BASELINE_POSITION_TOP;
    bx->spacing = 0;
    bx->orientation = GTK_ORIENTATION_HORIZONTAL;
    bx->homogeneous = FALSE;
    return ((box*)bx);
}
/******************************************************************************
 * Nom : make_box;
 * Entrees : box *box_info : la structure d'une boite
 * La fonction permet de creer la box passee en parametres
 * */
void make_box(box* box_info)
{
    if (!box_info)  // test d'existance
    {
        printf("\nErreur: box n'existe pas");
        exit(0);
    }
    // Creation de la box 
    box_info->box = gtk_box_new(box_info->orientation, box_info->spacing);
    gtk_box_set_baseline_position(GTK_BOX(box_info->box), box_info->basline_position);
    gtk_box_set_homogeneous(GTK_BOX(box_info->box), box_info->homogeneous);
}
/******************************************************************************
 * Nom : remplir_box;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : box * : la structure de la box remplit a partir du fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * box par les valeurs extrait de fichier et retourne la structure remplit.
 * */
box* remplir_box(FILE* f1)
{
    // initialiser la structure box
    box* bx = creer_initialiser_box();
    obj_att* p = NULL;
    // lire les attributs de la box a partir du fichier
    p = lire_att(f1); // mettre les attributs dans une liste
    while (p) // tant qu'on a des attributs dans la liste
    {   // modifier les valeurs par defauts
        if (!strcmp(p->attribut, "homogeneous"))
        {
            if (!strcmp(p->valeur, "TRUE")) bx->homogeneous = TRUE;
        }
        else if (!strcmp(p->attribut, "orientation"))
        {
            if (!strcmp(p->valeur, "VERTICAL")) bx->orientation = GTK_ORIENTATION_VERTICAL;
        }
        else if (!strcmp(p->attribut, "spacing")) bx->spacing = atoi(p->valeur);
        else if (!strcmp(p->attribut, "baseline-position"))
        {
            if (!strcmp(p->valeur, "CENTER")) bx->basline_position = GTK_BASELINE_POSITION_CENTER;
            else if (!strcmp(p->valeur, "BOTTOM")) bx->basline_position = GTK_BASELINE_POSITION_BOTTOM;
        }
        p = p->suiv;
    }
    return (box*)bx;
}