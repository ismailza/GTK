#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <gtk/gtk.h>
#include "structures.h"

#define __ICON__ "D:\\C projects\\GTK\\Ismail_ZAHIR\\icon.png"
#define __FILE__ "D:\\C projects\\GTK\\Ismail_ZAHIR\\ExamTP.txt"

void Myffunn()
{
    GtkWidget *win, *win1;
    tab_glist *info=creer_intialiser_tab_glist();
    tab_glist *info1=creer_intialiser_tab_glist();
    FILE *f1=NULL;
    f1= fopen(__FILE__,"r");
    if(!f1)
    {
        printf("\nError oppening file!!");
        exit(0);
    }
    win= principale_function(f1,info);
    win1= principale_function(f1,info1);
    gtk_widget_show_all(win);
    gtk_widget_show_all(win1);
}

void OverrideWindowBar(GtkWindow* Window, const char* Title, const char* Layout)
{
    GtkHeaderBar* Bar = GTK_HEADER_BAR(gtk_header_bar_new());
    gtk_header_bar_set_title(Bar, Title);
    if (Layout)
    {
        gtk_header_bar_set_show_close_button(Bar, TRUE);
        gtk_header_bar_set_decoration_layout(Bar, Layout);
    }
    gtk_window_set_titlebar(Window, GTK_WIDGET(Bar));
}

window *create_window()
{
    window *win = creer_initialiser_win();
    remplir_info(win->title, "Test des fonction GTK+");
    remplir_info(win->icon, __ICON__);
    win->default_size[0] = 800;
    win->default_size[1] = 800;
    make_window(win);
    OverrideWindowBar(GTK_WINDOW(win->win), "Test des fonction GTK+", "icon:minimize,maximize,close");
    return win;
}

menu *insert_menu_item(menu *menu, menu_item *item)
{
    if (!menu | !item) return menu;
    menu_item *ptr = menu->info, *tptr = NULL;
    if (!ptr)
    {
        menu->info = item;
        return menu;
    }
    while (ptr->but)
    {
        tptr = ptr;
        ptr = ptr->but;
    }
    ptr->but = item;
    ptr->but->top = tptr;
    return menu;
}

menu_item *insert_submenu(menu_item *m_item, menu *submenu)
{
    if (!m_item | !submenu) return m_item;
    m_item->rght = submenu->info;
    submenu->info->top = m_item;
    return m_item;
}

menu *create_menu()
{
    menu *menu, *m1, *m2;
    menu = crer_initi_menu();
    menu_item *mi1 = creer_intialiser_menu_item();
    remplir_info(mi1->label, "_Fichier");
    remplir_info(mi1->icon, __ICON__);

    menu_item *smi1 = creer_intialiser_menu_item();
    remplir_info(smi1->label, "_Nouveau");
    remplir_info(smi1->icon, __ICON__);

    menu_item *smi11 = creer_intialiser_menu_item();
    remplir_info(smi11->label, "_Dossier");
    remplir_info(smi11->icon, __ICON__);

    menu_item *smi12 = creer_intialiser_menu_item();
    remplir_info(smi12->label, "_Fichier");
    remplir_info(smi12->icon, __ICON__);

    m2 = crer_initi_menu();
    m2 = insert_menu_item(m2, smi11);
    m2 = insert_menu_item(m2, smi12);
    smi1 = insert_submenu(smi1, m2);

    menu_item *smi2 = creer_intialiser_menu_item();
    remplir_info(smi2->label, "_Enregistrer");
    remplir_info(smi2->icon, __ICON__);

    menu_item *smi3 = creer_intialiser_menu_item();
    remplir_info(smi3->label, "_Enregistrer sous");
    remplir_info(smi3->icon, __ICON__);

    menu_item *smi4 = creer_intialiser_menu_item();
    remplir_info(smi4->label, "_Quitter");
    remplir_info(smi4->icon, __ICON__);
    remplir_info(smi4->signal, "quit");

    m1 = crer_initi_menu();
    m1 = insert_menu_item(m1, smi1);
    m1 = insert_menu_item(m1, smi2);
    m1 = insert_menu_item(m1, smi3);
    m1 = insert_menu_item(m1, smi4);

    mi1 = insert_submenu(mi1, m1);

    menu_item *mi2 = creer_intialiser_menu_item();
    remplir_info(mi2->label, "_Accueil");
    remplir_info(mi2->icon, __ICON__);

    menu_item *mi3 = creer_intialiser_menu_item();
    remplir_info(mi3->label, "_Insertion");
    remplir_info(mi3->icon, __ICON__);

    menu = insert_menu_item(menu, mi1);
    menu = insert_menu_item(menu, mi2);
    menu = insert_menu_item(menu, mi3);

    make_menu(menu);
    return menu;
}

box *create_checkbtn(box *bx, gchar *label)
{
    check_button *ckbtn1 = creer_initialiser_check_button();
    remplir_info(ckbtn1->label, label);
    make_check_button(ckbtn1);
    gtk_box_pack_start(GTK_BOX(bx->box1), ckbtn1->checkbutton, FALSE, TRUE, 0);
    return bx;
}

box *create_list_multiple_choice_1()
{
    box *vbox = creer_initialiser_box();
    vbox->orientation = GTK_ORIENTATION_VERTICAL;
    vbox->spacing = 2;
    make_box(vbox);

    label *lb = crer_initialiser_label();
    remplir_info(lb->label_name, "Liste Choix Multiples");
    make_label(lb);

    gtk_box_pack_start(GTK_BOX(vbox->box1), lb->label, FALSE, TRUE, 0);

    box *svbox = creer_initialiser_box();
    svbox->orientation = GTK_ORIENTATION_VERTICAL;
    svbox->spacing = 2;
    remplir_info(svbox->bord, "* { border: black solid 2px;}");
    make_box(svbox);
    gtk_box_pack_start(GTK_BOX(vbox->box1), svbox->box1, FALSE, TRUE, 0);
    svbox = create_checkbtn(svbox, "Mon 1er Choix");
    svbox = create_checkbtn(svbox, "Mon 2d Choix");
    svbox = create_checkbtn(svbox, "Mon 3e Choix");
    return vbox;
}

box *create_combo_box()
{
    GtkWidget *cmbox1 = gtk_combo_box_text_new ();
    GtkWidget *cmbox2 = gtk_combo_box_text_new ();

    box *vbox = creer_initialiser_box();
    vbox->orientation = GTK_ORIENTATION_VERTICAL;
    vbox->spacing = 2;
    make_box(vbox);

    label *lb = crer_initialiser_label();
    remplir_info(lb->label_name, "Liste Deroulante: Multi Choix");
    make_label(lb);

    gtk_box_pack_start(GTK_BOX(vbox->box1), lb->label, FALSE, TRUE, 0);

    box *svbox = creer_initialiser_box();
    svbox->orientation = GTK_ORIENTATION_VERTICAL;
    svbox->spacing = 2;
    remplir_info(svbox->bord, "* { border: black solid 2px;}");
    make_box(svbox);
    gtk_box_pack_start(GTK_BOX(vbox->box1), svbox->box1, FALSE, TRUE, 0);

    box *hbox1 = creer_initialiser_box();
    hbox1->orientation = GTK_ORIENTATION_HORIZONTAL;
    hbox1->spacing = 2;
    make_box(hbox1);

    box *hbox2 = creer_initialiser_box();
    hbox2->orientation = GTK_ORIENTATION_HORIZONTAL;
    hbox2->spacing = 2;
    make_box(hbox2);

    label *lb1 = crer_initialiser_label();
    remplir_info(lb1->label_name, "Choix parmis 5 : ");
    make_label(lb1);
    gtk_box_pack_start(GTK_BOX(hbox1->box1), lb1->label, FALSE, TRUE, 0);

    label *lb2 = crer_initialiser_label();
    remplir_info(lb2->label_name, "Choix parmis 7 : ");
    make_label(lb2);
    gtk_box_pack_start(GTK_BOX(hbox2->box1), lb2->label, FALSE, TRUE, 0);

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox1),"Faites votre choix");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox1),"Choix 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox1),"Choix 2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox1),"Choix 3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox1),"Choix 4");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox1),"Choix 5");
    gtk_combo_box_set_active(GTK_COMBO_BOX(cmbox1),0);

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox2),"Faites votre choix");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox2),"Choix 1");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox2),"Choix 2");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox2),"Choix 3");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox2),"Choix 4");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox2),"Choix 5");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox2),"Choix 6");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmbox2),"Choix 7");
    gtk_combo_box_set_active(GTK_COMBO_BOX(cmbox2),0);

    gtk_box_pack_start(GTK_BOX(hbox1->box1), cmbox1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox2->box1), cmbox2, FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(svbox->box1), hbox1->box1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(svbox->box1), hbox2->box1, FALSE, TRUE, 0);

    return vbox;
}

box *create_hlist_radio_btn()
{
    box *vbox = creer_initialiser_box();
    vbox->orientation = GTK_ORIENTATION_VERTICAL;
    vbox->spacing = 2;
    make_box(vbox);

    label *lb = crer_initialiser_label();
    remplir_info(lb->label_name, "Buttons Radio Horizontales");
    make_label(lb);
    gtk_box_pack_start(GTK_BOX(vbox->box1), lb->label, FALSE, TRUE, 0);

    box *hbox = creer_initialiser_box();
    hbox->orientation = GTK_ORIENTATION_HORIZONTAL;
    hbox->spacing = 2;
    remplir_info(hbox->bord, "* { border: black solid 2px;}");
    make_box(hbox);
    gtk_box_pack_start(GTK_BOX(vbox->box1), hbox->box1, FALSE, TRUE, 0);

    GSList *group;
    tab_glist *infoo = creer_intialiser_tab_glist();
    radio_button *btn1 = creer_initialiser_radio_button();
    btn1->main_rdb = TRUE;
    remplir_info(btn1->label,"Premier");
    make_radio_button(btn1, NULL);
    group = gtk_radio_button_get_group (GTK_RADIO_BUTTON(btn1->radio_button));
    inserer_tab_glist(infoo,group);
    gtk_box_pack_start(GTK_BOX(hbox->box1), btn1->radio_button, FALSE, TRUE, 0);

    radio_button *btn2 = creer_initialiser_radio_button();
    remplir_info(btn2->label,"Deuxieme");
    make_radio_button(btn2, infoo->gliste_t->gg);
    group = gtk_radio_button_get_group (GTK_RADIO_BUTTON(btn2->radio_button));
    inserer_tab_glist(infoo,group);
    gtk_box_pack_start(GTK_BOX(hbox->box1), btn2->radio_button, FALSE, TRUE, 0);

    radio_button *btn3 = creer_initialiser_radio_button();
    remplir_info(btn3->label,"Dernier");
    make_radio_button(btn3, infoo->gliste_t->gg);
    group = gtk_radio_button_get_group (GTK_RADIO_BUTTON(btn3->radio_button));
    inserer_tab_glist(infoo,group);
    gtk_box_pack_start(GTK_BOX(hbox->box1), btn3->radio_button, FALSE, TRUE, 0);

    return vbox;
}

box *create_buttons()
{
    box *hbox = creer_initialiser_box();
    hbox->orientation = GTK_ORIENTATION_HORIZONTAL;
    hbox->spacing = 2;
    make_box(hbox);

    button *btn1 = creer_initialiser_button();
    remplir_info(btn1->mnemonic, "_OK");
    remplir_info(btn1->button_from_icon_name, __ICON__);
    remplir_info(btn1->signal, "func");
    remplir_info(btn1->back, "* { background-image:url(\"redblackback.jpg\");}");
    make_button(btn1);

    button *btn2 = creer_initialiser_button();
    remplir_info(btn2->mnemonic, "_Quitter");
    remplir_info(btn2->signal, "destroy");
    make_button(btn2);

    gtk_box_pack_start(GTK_BOX(hbox->box1), btn1->button, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox->box1), btn2->button, FALSE, TRUE, 0);

    return hbox;
}

box *create_radio_btn(box *bx, gchar *label, GSList *group[1], tab_glist *infoo[1])
{
    radio_button *btn1 = creer_initialiser_radio_button();
    btn1->main_rdb = TRUE;
    remplir_info(btn1->label,label);
    make_radio_button(btn1, group[0]);
    group[0] = gtk_radio_button_get_group (GTK_RADIO_BUTTON(btn1->radio_button));
    inserer_tab_glist(infoo[0],group[0]);
    gtk_box_pack_start(GTK_BOX(bx->box1), btn1->radio_button, FALSE, TRUE, 0);
    return bx;
}

box *create_vlist_radio_btn2()
{
    box *vbox = creer_initialiser_box();
    vbox->orientation = GTK_ORIENTATION_VERTICAL;
    vbox->spacing = 2;
    make_box(vbox);

    label *lb = crer_initialiser_label();
    remplir_info(lb->label_name, "Buttons Radio Verticale");
    make_label(lb);
    gtk_box_pack_start(GTK_BOX(vbox->box1), lb->label, FALSE, TRUE, 0);

    box *hbox = creer_initialiser_box();
    hbox->orientation = GTK_ORIENTATION_VERTICAL;
    hbox->spacing = 2;
    remplir_info(hbox->bord, "* { border: black solid 2px;}");
    make_box(hbox);
    gtk_box_pack_start(GTK_BOX(vbox->box1), hbox->box1, FALSE, TRUE, 0);

    GSList *group = NULL;
    tab_glist *infoo = creer_intialiser_tab_glist();

    hbox = create_radio_btn(hbox, "Faible", &group, &infoo);
    hbox = create_radio_btn(hbox, "Moyen", &group, &infoo);
    hbox = create_radio_btn(hbox, "Assez Bien", &group, &infoo);
    hbox = create_radio_btn(hbox, "Bien", &group, &infoo);
    hbox = create_radio_btn(hbox, "Tres Bien", &group, &infoo);
    return vbox;
}

box *create_form_login()
{
    box *vbox = creer_initialiser_box();
    vbox->orientation = GTK_ORIENTATION_VERTICAL;
    remplir_info(vbox->bord, "* { border: black solid 2px;}");
    vbox->spacing = 2;
    make_box(vbox);

    label *lb = crer_initialiser_label();
    remplir_info(lb->label_name, "Authentification");
    make_label(lb);
    gtk_box_pack_start(GTK_BOX(vbox->box1), lb->label, FALSE, TRUE, 0);

    box *hbox1 = creer_initialiser_box();
    hbox1->orientation = GTK_ORIENTATION_HORIZONTAL;
    hbox1->spacing = 20;
    make_box(hbox1);
    box *hbox2 = creer_initialiser_box();
    hbox2->orientation = GTK_ORIENTATION_HORIZONTAL;
    hbox2->spacing = 20;
    make_box(hbox2);

    gtk_box_pack_start(GTK_BOX(vbox->box1), hbox1->box1, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox->box1), hbox2->box1, FALSE, TRUE, 0);

    label *lb1 = crer_initialiser_label();
    remplir_info(lb1->label_name, "Login");
    make_label(lb1);
    entry *login = crrer_initialser_entry();
    make_entry(login);
    gtk_box_pack_start(GTK_BOX(hbox1->box1), lb1->label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox1->box1), login->entry, FALSE, TRUE, 0);
    label *lb2 = crer_initialiser_label();
    remplir_info(lb2->label_name, "Password");
    make_label(lb2);
    entry *pass = crrer_initialser_entry();
    pass->visibilty = FALSE;
    make_entry(pass);
    gtk_box_pack_start(GTK_BOX(hbox2->box1), lb2->label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox2->box1), pass->entry, FALSE, TRUE, 0);
    button *btn = creer_initialiser_button();
    remplir_info(btn->label, "Connecter");
    make_button(btn);
    gtk_box_pack_start(GTK_BOX(vbox->box1), btn->button, FALSE, TRUE, 0);


    return vbox;
}

window *alert()
{
    window *win = creer_initialiser_win();
    win->default_size[0] = 600;
    win->default_size[1] = 300;
    make_window(win);
    OverrideWindowBar(GTK_WINDOW(win->win), "", "icon:close");
    return win;

}

void create_interface_TP()
{
    // creation de la fenetre
    window *win = create_window();
    // fixe
    GtkWidget *fix= gtk_fixed_new();
    // creation de menu
    menu *m = create_menu();
    gtk_fixed_put(GTK_FIXED(fix),m->menu,0,0);
    // vertical box multiples choices
    box *box1 = create_list_multiple_choice_1();
    gtk_fixed_put(GTK_FIXED(fix),box1->box1,500,60);
    // creation d'une liste deroulante
    box *box2 = create_combo_box();
    gtk_fixed_put(GTK_FIXED(fix),box2->box1,5,200);
    // creation butons radio hor
    box *box3 = create_hlist_radio_btn();
    gtk_fixed_put(GTK_FIXED(fix),box3->box1,500,200);
    // Creation des buttons
    box *box4 = create_buttons();
    gtk_fixed_put(GTK_FIXED(fix),box4->box1,15,300);
    // Creation button radio ver
    box *box5 = create_vlist_radio_btn2();
    gtk_fixed_put(GTK_FIXED(fix),box5->box1,5,360);

    // creation de formulaire
    box *box6 = create_form_login();
    gtk_fixed_put(GTK_FIXED(fix),box6->box1,500,360);


    gtk_container_add(GTK_CONTAINER(win->win), fix);
    gtk_widget_show_all(fix);
    gtk_widget_show_all(win->win);
}

int main(int argc,char *argv[])
{
    gtk_init(&argc, &argv);
    // Myffunn();
    create_interface_TP();
    gtk_main();
    return 0;
}
