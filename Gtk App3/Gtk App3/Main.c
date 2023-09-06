#include<stdlib.h>  
#include<gtk/gtk.h>  

void demande_confirmation(GtkWidget* widget, gpointer boitededialogue)
{
    GtkWidget* maboitededialog;
    maboitededialog = gtk_message_dialog_new(GTK_WINDOW(boitededialogue),
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Voulez-vous reelement quitter?");
    gtk_window_set_title(GTK_WINDOW(maboitededialog), "Demande de confirmation");
    switch (gtk_dialog_run(GTK_DIALOG(maboitededialog)))
    {
    case GTK_RESPONSE_YES://GTK_RESPONSE_YES valeur de retour du bouton YES  
        /*Si on clique le bouton YES, on quitte l'application*/
        gtk_main_quit();
        break;
    case GTK_RESPONSE_NO://GTK_RESPONSE_NO valeur de retour du bouton NO  
        /* Si on clique No, on ferme la boite de dialogue seulement */
        gtk_widget_destroy(maboitededialog);
        break;
    }
}

int main(int argc, char* argv[])
{
    GtkWidget* mafenetre;
    GtkWidget* montableau;
    GtkWidget* montexte;
    GtkWidget* monbouton;
    gchar* codage;
    gtk_init(&argc, &argv);
    mafenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    montableau = gtk_table_new(1, 2, TRUE);
    monbouton = gtk_button_new_with_label("Quitter");
    gtk_container_add(GTK_CONTAINER(mafenetre), GTK_WIDGET(montableau));
    gtk_window_set_title(GTK_WINDOW(mafenetre), "Samomoi");
    gtk_window_set_position(GTK_WINDOW(mafenetre), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(mafenetre), 400, 100);
    gtk_window_set_icon_from_file(GTK_WINDOW(mafenetre), "icone.gif", NULL);
    codage = g_locale_to_utf8("Marseille! À la bien cousin.La famille...",
        -1,
        NULL,
        NULL,
        NULL);
    montexte = gtk_label_new(codage);

    g_free(codage);
    /* insertion de deux widgets (le texte et le bouton) sur le tableau (table)*/
    gtk_table_attach(GTK_TABLE(montableau), montexte,
        0, 1, 0, 1,
        GTK_EXPAND | GTK_FILL, GTK_EXPAND,
        0, 0);
    gtk_table_attach(GTK_TABLE(montableau), monbouton,
        1, 2, 0, 1,
        GTK_EXPAND | GTK_FILL, GTK_EXPAND,
        0, 0);

    g_signal_connect(G_OBJECT(monbouton), "clicked",
        G_CALLBACK(demande_confirmation), (gpointer)mafenetre);
    gtk_widget_show_all(mafenetre);
    gtk_main();
    return 0;
}
