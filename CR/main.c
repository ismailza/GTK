#include "ILISIBuilder.h"


int main(int argc, char const *argv[])
{
    //initialisation des arguments GTK
    gtk_init(&argc, &argv);

    //declaration de nouveau buildes 
    ILISIBuilder *builder; 

    //traiter le fichier
    builder = ilisiBuilderReadFromFile("notebook.xml");

    //associer le singnal destroy au fenetre
    GtkWidget *bx = chercherRef(builder->table, "window1");
    g_signal_connect(G_OBJECT(bx), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //associer un 2eme signal au button1
    GtkWidget *quited = chercherRef(builder->table, "button2");
    g_signal_connect(G_OBJECT(quited), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    


    gtk_widget_show_all(builder->parentWidget);
    gtk_main();
    return 0;
}