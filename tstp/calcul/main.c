#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

const char *tab[5][4] = {"_Q","C","D","/",
                    "7","8","9","*",
                    "4","5","6","-",
                    "1","2","3","+",
                    "a","0",".","="};
    int i, j, k = 0;
    GtkWidget* boutton[20];
    GtkWidget* hbox[5];
    GtkWidget* entry;
GtkWidget* creer_window(GtkWidget* window)
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "calcul");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_icon_from_file(GTK_WINDOW(window), "calc.JPG", NULL);
    return(GtkWidget*)window;
}
GtkWidget* creer_box_calcul(GtkWidget* box)
{

    box=gtk_vbox_new(FALSE,2);
    entry=gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry),"entrez votre expression");
    gtk_box_pack_start(GTK_BOX(box),entry, TRUE, TRUE, 1);
    for (i = 0; i < 5; i++){
        for (j = 0; j < 4; j++)
            boutton[k++] = gtk_button_new_with_mnemonic(tab[i][j]);
    //for (i = 0; i < 5; i++)
        hbox[i] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,1);}
    k = 0;
    j = 0;
    for (i = 0; i < 20; i++)
    {
        k++;
        gtk_box_pack_start(GTK_BOX(hbox[j]), boutton[i], TRUE, TRUE, 1);
        if (k % 4 == 0) j++;
    }
    for (i = 0; i < 5; i++)
        gtk_box_pack_start(GTK_BOX(box), hbox[i], TRUE, TRUE, 1);
    return (GtkWidget*)box;

}

void main(int* argc, char* argv[])
{
    GtkWidget* window, * Box;
    gtk_init(&argc, &argv);
    window = creer_window(window);
    Box = creer_box_calcul(Box);
    gtk_container_add(GTK_CONTAINER(window), Box);
    gtk_widget_show_all(window);
    //for (i = 0; i < 5; i++)
    g_signal_connect(G_OBJECT(boutton[0]),"clicked",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
}
