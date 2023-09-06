/* La structure de grid *******************************************************/
typedef struct
{
    GtkWidget* grid;    // pointeur sur le widget gerid
    // l'espacement vertical entre les rangées d'une gride
    gint rowspacing;    //gtk_grid_set_row_spacing ()
    // l'espacement horizontal entre les deux colonnes de la gride
    gint columnspacing; //gtk_grid_set_column_spacing ()
    // la ligne de base d'une gride
    gint row_baseline;  //gtk_grid_set_baseline_row()
    //gtk_grid_attach()
}grid;
/*******************************************************************************
 * Nom : creer_initialiser_grid;
 * Sorties : grid * : la structure d'une gride initialisee
 * La fonction permet d'allouer un espace memoire pour une gride et de
 * l'initialise
 * */
grid* creer_initialiser_grid()
{
    // allocation memoire pour la gride
    grid* gr = (grid*)malloc(sizeof(grid));
    if (!gr)
    {
        printf("\nErreur d'allocation memoire pour grid");
        exit(0);
    }
    // initialisation des champs de la structure grid
    gr->grid = NULL;
    gr->rowspacing = 20;
    gr->columnspacing = 20;
    gr->row_baseline = 0;
    return ((grid*)gr);
}
/*******************************************************************************
 * Nom : make_grid;
 * Entrees : grid * : la structure d'une gride
 * La fonction permet de creer une gride passee en parametres
 * */
void make_grid(grid* info_grid)
{
    if (!info_grid) // test d'existance
    {
        printf("\nErreur : grid n'existe pas");
        exit(0);
    }
    // cration de grid
    info_grid->grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(info_grid->grid), info_grid->rowspacing);
    gtk_grid_set_column_spacing(GTK_GRID(info_grid->grid), 
                                info_grid->columnspacing);
    gtk_grid_set_baseline_row(GTK_GRID(info_grid->grid), 
                                info_grid->row_baseline);
}
grid* remplir_grid(FILE* f1)
{
    grid* gr = creer_initialiser_grid();
    obj_att* p = NULL;
    p = lire_att(f1);
    while (p)
    {
        if (!strcmp(p->attribut, "rowspacing")) gr->rowspacing = atoi(p->valeur);
        else if (!strcmp(p->attribut, "columnspacing")) 
            gr->columnspacing = atoi(p->valeur);
        else if (!strcmp(p->attribut, "row_baseline")) 
            gr->row_baseline = atoi(p->valeur);
        p = p->suiv;
    }
    return (grid*)gr;
}