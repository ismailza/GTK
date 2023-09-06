#include "ILISIBuilder.h"


int main(int argc, char const *argv[])
{

    gtk_init(&argc, &argv);
    ILISIBuilder *builder; 

   
    builder = ilisiBuilderReadFromFile("ILISI.xml");

    GtkWidget *bx = chercherRef(builder->table, "window1");
    g_signal_connect(G_OBJECT(bx), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *quited = chercherRef(builder->table, "button2");
    g_signal_connect(G_OBJECT(quited), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    

    gtk_widget_show_all(builder->parentWidget);
    gtk_main();
    return 0;
}