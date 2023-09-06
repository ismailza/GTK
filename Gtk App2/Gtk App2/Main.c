#include "deplacement.c"
///fonction de signal
static void sortir(GtkWidget* button, gpointer window)
{
    gtk_main_quit();
    gtk_widget_destroy(window);

}

///fonction main

int main(int argc, char* argv[])
{
    //initialisation du main
    gtk_init(&argc, &argv);
    POS p; p.pos = 'B';
    Fenetre* F = NULL;
    MonBox* box1 = NULL, * box3 = NULL, * box = NULL;
    Monfixed* fix = NULL;
    MonImage* bg, * bv, * bv1, * bv2;
    MonBouton* bouton1, * bouton2, * bouton3;
    gint activat = 1;
    dimension d = { 500,500 }, d4 = { 5,10 }, d6 = { 400,400 };
    color_widg couleur1 = { "white",0.7 };
    ///  window
    F = init_window("Fenetre Principale", p, d, 0, NULL, NULL, 0);
    F = creer_Window(F);
    ///fixed
    fix = init_Fixed(d6, "fixed pricipale", F->window, init_crd());
    fix = creer_Fixed(fix);
    ///box
    box1 = Init_box(NULL, "sousbox1", 'v', 0,
        dimAdd(500, 70), 0, 50, NULL, init_crd());
    box1 = creer_Box(box1);

    box3 = Init_box(NULL, "sousbox2", 'H', 0,
        dimAdd(500, 50), 50, 30, NULL, init_crd());
    box3 = creer_Box(box3);

    box = Init_box(NULL, "box1", 'v', 0,
        dimAdd(500, 0), 0, 0, NULL, init_crd());
    box = creer_Box(box);
    bg = init_image("./images/bg1.jpg", "bg", crdAdd(0, 0), NULL);
    creer_image(bg);
    fix = ajouter_Fixed(fix, bg->image.elem, bg->ImCrd);
    box = ajouter_Box(box, box1->box.elem, 0, 0, 0, 1);
    box = ajouter_Box(box, box3->box.elem, 0, 0, 0, 1);
    fix = ajouter_Fixed(fix, box->box.elem, crdAdd(0, 0));

    bv = init_image("./images/bg2.gif", "b", crdAdd(0, 0), NULL);
    creer_image(bv);
    box1 = ajouter_Box(box1, bv->image.elem, 0, 0, 0, 1);
    bv1 = init_image("./images/txt.gif", "b", crdAdd(0, 0), NULL);
    creer_image(bv1);
    box1 = ajouter_Box(box1, bv1->image.elem, 0, 0, 0, 1);
    bv2 = init_image("./images/txt2.gif", "b", crdAdd(0, 0), NULL);
    creer_image(bv2);
    box1 = ajouter_Box(box1, bv2->image.elem, 0, 0, 0, 1);

    ///bouton
    bouton1 = init_Button(NULL, NULL, NULL, d4, "Exit",
        NULL, 4, 0, &couleur1, init_crd());
    bouton1->button.elem = gtk_button_new_from_icon_name("edit-clear-symbolic", GTK_ICON_SIZE_DND);
    add_bgcolor(bouton1->button.elem, bouton1->color->color, bouton1->color->opacity);

    gtk_button_set_relief((GtkButton*)bouton1->button.elem, GTK_RELIEF_NONE);
    bouton2 = init_Button(NULL, NULL, NULL, d4, "Play",
        NULL, 4, 0, &couleur1, init_crd());
    bouton2->button.elem = gtk_button_new_from_icon_name(
        "preferences-desktop-accessibility-symbolic", GTK_ICON_SIZE_DND);
    add_bgcolor(bouton2->button.elem, bouton2->color->color, bouton2->color->opacity);
    gtk_button_set_relief((GtkButton*)bouton2->button.elem, GTK_RELIEF_NONE);

    bouton3 = init_Button(NULL, NULL, NULL, d4, "Continue",
        NULL, 4, 0, &couleur1, init_crd());
    bouton3->button.elem = gtk_button_new_from_icon_name(
        "object-rotate-right-symbolic", GTK_ICON_SIZE_DND);
    add_bgcolor(bouton3->button.elem, bouton3->color->color, bouton3->color->opacity);

    gtk_button_set_relief((GtkButton*)bouton3->button.elem, GTK_RELIEF_NONE);
    box3 = ajouter_Box(box3, bouton1->button.elem, 1, 1, 0, 1);
    box3 = ajouter_Box(box3, bouton2->button.elem, 1, 1, 0, 1);
    box3 = ajouter_Box(box3, bouton3->button.elem, 1, 1, 0, 1);

    g_signal_connect(F->window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(bouton2->button.elem, "clicked", G_CALLBACK(deplacementEspace), NULL);///deplacementEspace
    g_signal_connect(bouton1->button.elem, "clicked", G_CALLBACK(sortir), F->window);
    g_signal_connect(bouton3->button.elem, "clicked", G_CALLBACK(deplacementEspace), &activat);
    Afficher_widget(F->window);
    gtk_main();
    return 0;
}
