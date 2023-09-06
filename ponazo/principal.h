
typedef struct fvb
{
    GSList* gg;
    struct fvb* suiv;
}glist;

typedef struct
{
    glist* gliste_t;
    GtkWidget* menu_button;
}tab_glist;


glist* crer_glist(GSList* group)
{
    glist* ne = (glist*)malloc(sizeof(glist));
    ne->gg = group;
    ne->suiv = NULL;
    return (glist*)ne;
}


tab_glist* creer_intialiser_tab_glist()
{
    tab_glist* li;
    glist* ne = crer_glist(NULL);
    li = (tab_glist*)malloc(sizeof(tab_glist));
    li->gliste_t = ne;
    li->menu_button = NULL;
    return (tab_glist*)li;
}

int inserer_tab_glist(tab_glist* ll, GSList* group)
{
    glist* ne = crer_glist(group);
    if (!ll->gliste_t)
    {
        ll->gliste_t = ne;
        return (int)1;
    }
    ne->suiv = ll->gliste_t;
    ll->gliste_t = ne;
    return (int)1;
}
// **************************

/******************************************************************************
 * Nom : principale_function;
 * Entrees : File *: le fichier source
 * Sorties :
 *
 **/
GtkWidget* principale_function(FILE* f1, tab_glist* info)
{
    char mot[35];   // pour lire le tag du fichier
    char mot1[35];
    char mot2[35];
    fscanf(f1, "<%35[^ ] ", mot);
    int k, j, w, h;
    if (!strcmp(mot, "window")) // fenetre
    {
        window* stwin = remplir_window(f1); // remplir la fennetre 
        make_window(stwin);                 // creer la fenetre
        fscanf(f1, "<%35[^ ] ", mot);
        fscanf(f1, "%35[^>]>\n", mot1);
        epurer_blan(f1);
        do {
            if (!strcmp(mot1, "headerbar")) // l'entete
                gtk_window_set_titlebar(GTK_WINDOW(stwin->win), principale_function(f1, info));
            else if (!strcmp(mot1, "container-add"))
                gtk_container_add(GTK_CONTAINER(stwin->win), principale_function(f1, info));
            fscanf(f1, "<%35[^>]>\n", mot2); // lire la balise fermante
            epurer_blan(f1);
            fscanf(f1, "<%35[^ ] ", mot);   // lire une balise
            if (!strcmp(mot, "child"))
            {
                fscanf(f1, "%35[^>]>\n", mot1);
                epurer_blan(f1);
            }
        } while (!strcmp(mot, "child"));
        fscanf(f1, ">\n");
        epurer_blan(f1);
        return ((GtkWidget*)stwin->win);
    }
    else if (!strcmp(mot, "headbar"))   // l'entete
    {
        header_bar* shdb = remplir_header_bar(f1); // remplir l'entete
        make_headerbar(shdb);                       // creer l'entete
        fscanf(f1, "%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)shdb->header_bar);
    }
    else if (!strcmp(mot, "box"))
    {
        box* sbx = remplir_box(f1);     // remplir la boite
        make_box(sbx);                  // creer la boite
        fscanf(f1, "<%35[^ ] ", mot);
        fscanf(f1, "%35[^>]>\n", mot1);
        epurer_blan(f1);
        do {
            if (!strcmp(mot1, "pack-start"))
                gtk_box_pack_start(GTK_BOX(sbx->box), principale_function(f1, info), FALSE, FALSE, 0);
            else if (!strcmp(mot1, "pack-end"))
                gtk_box_pack_end(GTK_BOX(sbx->box), principale_function(f1, info), FALSE, FALSE, 0);
            fscanf(f1, "<%35[^>]>\n", mot2);
            epurer_blan(f1);
            fscanf(f1, "<%35[^ ] ", mot);
            if (!strcmp(mot, "child"))
            {
                fscanf(f1, "%35[^>]>\n", mot1);
                epurer_blan(f1);
            }
        } while (!strcmp(mot, "child"));
        fscanf(f1, ">\n");
        epurer_blan(f1);
        return ((GtkWidget*)sbx->box);
    }
    else if (!strcmp(mot, "toolbar"))
    {
        tool_bar* stlb = remplir_tool_bar(f1);
        make_tool_bar(stlb);
        return ((GtkWidget*)stlb->toolbar);
    }
    else if (!strcmp(mot, "entry"))
    {
        entry* sent = remplir_entry(f1);
        make_entry(sent);
        fscanf(f1, "%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)sent->entry);
    }
    else if (!strcmp(mot, "button"))
    {
        button* sbtn = remplir_button(f1);
        make_button(sbtn);
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)sbtn->button);
    }
    else if (!strcmp(mot, "menu"))
    {
        menu* smn = remplir_menu(f1);
        make_menu(smn);
        gtk_widget_show_all(smn->menu);
        info->menu_button = smn->menu;
        return (GtkWidget*)smn->menu;
    }
    else if (!strcmp(mot, "combotext"))
    {
        combo_box* cb = creer_initialiser_combo_box();
        fscanf(f1, ">\n");
        cb->info = remplir_com_liste(f1);
        make_combo_box_text(cb);
        fscanf(f1, ">\n");
        epurer_blan(f1);
        return ((GtkWidget*)cb->combobox);
    }
    else if (!strcmp(mot, "linkbutton"))
    {
        link_button* slkb = remplir_link_button(f1);
        make_link_button(slkb);
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)slkb->link_btn);
    }
    else if (!strcmp(mot, "entry"))
    {
        entry* sent = remplir_entry(f1);
        make_entry(sent);
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)sent->entry);
    }
    else if (!strcmp(mot, "togglebutton"))
    {
        toggle_button* stglb = remplir_toggle_button(f1);
        make_toggle_button(stglb);
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)stglb->toogle_btn);
    }
    else if (!strcmp(mot, "radiobutton"))
    {
        radio_button* srdb = remplir_radio_button(f1);
        GSList* group;
        if (srdb->main_rdb) make_radio_button(srdb, NULL);
        else make_radio_button(srdb, info->gliste_t->gg);
        group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(srdb->radio_btn));
        inserer_tab_glist(info, group);
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)srdb->radio_btn);
    }
    else if (!strcmp(mot, "checkbutton"))
    {
        check_button* schk = remplir_check_button(f1);
        make_check_button(schk);
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)schk->checkbtn);
    }
    else if (!strcmp(mot, "image"))
    {
        image* sim = remplir_image(f1);
        make_image(sim);
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)sim->image);
    }
    else if (!strcmp(mot, "label"))
    {
        label* slb = remplir_label(f1);
        make_label(slb);
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)slb->label);
    }
    else if (!strcmp(mot, "grid"))
    {
        grid* sgr = remplir_grid(f1);
        make_grid(sgr);
        fscanf(f1, "<%35[^ ] ", mot);
        fscanf(f1, "%35[^,],%d,%d,%d,%d>\n", mot1, &k, &j, &w, &h);
        epurer_blan(f1);
        do {
            gtk_grid_attach(GTK_GRID(sgr->grid), principale_function(f1, info), k, j, w, h);
            fscanf(f1, "<%35[^>]>\n", mot2);
            epurer_blan(f1);
            fscanf(f1, "<%35[^ ] ", mot);
            if (!strcmp(mot, "child"))
            {
                fscanf(f1, "%35[^,],%d,%d,%d,%d>\n", mot1, &k, &j, &w, &h);
                epurer_blan(f1);
            }
        } while (!strcmp(mot, "child"));
        fscanf(f1, ">\n");
        epurer_blan(f1);
        return ((GtkWidget*)sgr->grid);
    }
    else if (!strcmp(mot, "search_bar"))
    {
        search_bar* ssrch = remplir_search_bar(f1);
        make_search_bar(ssrch);
        gtk_search_bar_connect_entry(GTK_SEARCH_BAR(ssrch->serch_bar), GTK_ENTRY(principale_function(f1, info)));
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)ssrch->serch_bar);
    }
    else if (!strcmp(mot, "menubutton"))
    {
        menu_button* smnbt = remplir_menu_button(f1);
        make_menu_button(smnbt);
        gtk_menu_button_set_popup(GTK_MENU_BUTTON(smnbt->menu_btn), principale_function(f1, info));
        fscanf(f1, "<%35[^>]>\n", mot1);
        epurer_blan(f1);
        return ((GtkWidget*)smnbt->menu_btn);
    }
    else if (!strcmp(mot, "fixed")) {
        int val1, val2;
        gchar* kl = NULL;
        GtkWidget* fix = gtk_fixed_new();
        fscanf(f1, ">\n");
        epurer_blan(f1);
        fscanf(f1, "<%35[^ ] ", mot);
        fscanf(f1, "%35[^>]>\n", mot1);
        sscanf(mot1, "%d|%d", &val1, &val2);
        epurer_blan(f1);
        do {
            gtk_fixed_put(GTK_FIXED(fix), principale_function(f1, info), val1, val2);
            fscanf(f1, "<%35[^>]>\n", mot2);
            epurer_blan(f1);
            fscanf(f1, "<%35[^ ] ", mot);
            if (strcmp(mot, "child") == 0)
            {
                fscanf(f1, "%35[^>]>\n", mot1);
                epurer_blan(f1);
                strcpy(kl, mot1);
                str_to_val1_val2(kl, &val1, &val2);
            }
        } while (!strcmp(mot, "child"));
        fscanf(f1, ">\n");
        epurer_blan(f1);
        gtk_widget_show_all(fix);
        return (GtkWidget*)fix;
    }
    else if (!strcmp(mot, "scrollbar"))
    {
        GtkWidget* scrol = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrol), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
        fscanf(f1, ">\n");
        epurer_blan(f1);
        fscanf(f1, "<%35[^ ] ", mot);
        fscanf(f1, "%35[^>]>\n", mot1);
        if (!strcmp(mot1, "container-add")) gtk_container_add(GTK_CONTAINER(scrol), principale_function(f1, info));
        fscanf(f1, "%35[^>]>\n", mot1);
        epurer_blan(f1);
        return (GtkWidget*)scrol;
    }
    return NULL;
}