/* La structure d'une fenetre *************************************************/
typedef struct win
{
    GtkWidget* win;         // pointeur sur la fenetre
    GtkWindowType type_win; // type de la fenetre (POPUP or TOPLEVEL)
    // GtkWidget *gtk_window_new(GtkWindowType );
    gchar* title;           // titre de la fenetre
    // void gtk_window_set_title(GtkWindow *, gchar *);
    gboolean resize;        // indique si la fenetre est resizable ou pas
    // void gtk_window_set_resizable(GtkWindow *, gboolean );
    gboolean modal;         // indique l'interaction avec d'autre fenetre
    // void gtk_window_set_modal (GtkWindow *, gboolean );
    gint def_size[2];
    // void gtk_window_set_default_size (GtkWindow *, gint , gint );
    gint req_size[2];
    // void gtk_widget_set_size_request (GtkWidget *, gint , gint );
    gchar* icon;
    // gboolean gtk_window_set_icon_from_file (GtkWindow *, gchar *, GError **);
    double opacity;        // indique l'ocapacity de la fenetre
    // void gtk_widget_set_opacity (GtkWidget *, double opacity);
    gboolean fullscreen;    // indique si la fenetre est en plein ecran
    // void gtk_window_fullscreen (GtkWindow *);
    gboolean keep_below;    // indique si la fenetre reste en dessous
    // void gtk_window_set_keep_below (GtkWindow *, gboolean );
}window;
/*******************************************************************************
 * Nom : creer_initialiser_win;
 * Sorties : window * : la structure window initialisee
 * La fonction permet d'allouer un espace memoire pour une fenetre et de
 * l'initialisee par des valeurs par defaut
 * */
window* creer_initialiser_win()
{
    window* win = (window*)malloc(sizeof(window)); // allocation memoire
    if (!win)
    {
        printf("\nErreur d'allocation memoire pour la fenetre!");
        exit(0);
    }
    // initialisation des champs de la structure window
    win->win = NULL;
    win->type_win = GTK_WINDOW_TOPLEVEL;
    win->title = "untitled";
    win->resize = TRUE;
    win->modal = FALSE;
    win->def_size[0] = 200;
    win->def_size[1] = 200;
    win->req_size[0] = -1;
    win->req_size[1] = -1;
    win->icon = NULL;
    win->opacity = 1;
    win->fullscreen = FALSE;
    win->keep_below = FALSE;
    return (window*)win;
}
/******************************************************************************
 * Nom : make_window;
 * Entrees : window *wn_info : la structure d'une fenetre
 * La fonction permet de creer la fenetre passee en parametres
 * */
void make_window(window* wn_info)
{
    if (!wn_info)   // test pour verifier l'existance de la structure
    {
        printf("\nErreur : fenetre n'existe pas!!");
        exit(0);
    }
    // Creer la fenetre
    wn_info->win = gtk_window_new(wn_info->type_win);
    // mettre le titre de la fenetre
    gtk_window_set_title(GTK_WINDOW(wn_info->win), wn_info->title);
    // indique si la fenetre est redimentionnable ou pas
    gtk_window_set_resizable(GTK_WINDOW(wn_info->win), wn_info->resize);
    // mettre la taille par default
    if (wn_info->def_size[0] != -1) gtk_window_set_default_size(GTK_WINDOW(wn_info->win),
                                        wn_info->def_size[0], wn_info->def_size[1]);
    // mettre la taille demandee
    else if (wn_info->req_size[0] != -1) gtk_window_set_default_size(GTK_WINDOW(wn_info->win), 
                                        wn_info->req_size[0], wn_info->req_size[1]);
    // indique l'interaction avec d'autre fenetre
    gtk_window_set_modal(GTK_WINDOW(wn_info->win), wn_info->modal);
    // mettre l'icon pour la fenetre
    if (wn_info->icon) gtk_window_set_icon_from_file(GTK_WINDOW(wn_info->win), wn_info->icon, NULL);
    // indique l'opacite de la fenetre
    gtk_widget_set_opacity(GTK_WIDGET(wn_info->win), wn_info->opacity);
    // indique si la fenetre s'ouvre en mode plein ecran
    if (wn_info->fullscreen) gtk_window_fullscreen(GTK_WINDOW(wn_info->win));
    // indique si la fenetre reste en dessous
    gtk_window_set_keep_below(GTK_WINDOW(wn_info->win), wn_info->keep_below);
    g_signal_connect(wn_info->win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
/******************************************************************************
 * Nom : remplir_window;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : window * : la structure de la fenetre remplit a partir du fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * fenentre par les valeurs extrait de fichier et retourne la structure remplit.
 * */
window* remplir_window(FILE* fl)
{
    int val1, val2;
    obj_att* p = NULL;
    // initialiser la structure fenetre
    window* wn_info = creer_initialiser_win(); 
    // lire les attributs de la fenetre a partir du fichier
    p = lire_att(fl); // mettre les attributs dans une liste
    while (p) // tant qu'on a des attributs dans la liste
    {   // modifier les valeurs par 
        if (!strcmp(p->attribut, "type")) 
        {
            if (!strcmp(p->valeur, "POPUP")) wn_info->type_win = GTK_WINDOW_POPUP;
        }
        else if (!strcmp(p->attribut, "title")) wn_info->title = p->valeur;
        else if (!strcmp(p->attribut, "resize"))
        {
            if (!strcmp(p->valeur, "FALSE")) wn_info->resize = FALSE;
        }
        else if (!strcmp(p->attribut, "modal"))
        {
            if (!strcmp(p->valeur, "TRUE")) wn_info->modal = TRUE;
        }
        else if (!strcmp(p->attribut, "def-size"))
        {
            str_to_val1_val2(p->valeur, &val1, &val2);
            wn_info->def_size[0] = val1;
            wn_info->def_size[1] = val2;
        }
        else if (!strcmp(p->attribut, "req-size"))
        {
            str_to_val1_val2(p->valeur, &val1, &val2);
            wn_info->req_size[0] = val1;
            wn_info->req_size[1] = val2;
            wn_info->def_size[0] = -1;
            wn_info->def_size[1] = -1;
        }
        else if (!strcmp(p->attribut, "icon")) wn_info->icon = p->valeur;
        else if (!strcmp(p->attribut, "opacity")) wn_info->opacity = (float)atof(p->valeur);
        else if (!strcmp(p->attribut, "fullscreen"))
        {
            if (!strcmp(p->valeur, "TRUE")) wn_info->fullscreen = TRUE;
        }
        else if (!strcmp(p->attribut, "keep-bellow"))
        {
            if (!strcmp(p->valeur, "TRUE")) wn_info->keep_below = TRUE;
        }
        p = p->suiv;
    }
    return ((window*)wn_info);
}