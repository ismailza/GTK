#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "structures.h"


void Myffunn()

{
    GtkWidget *win;
    GtkWidget *win1;
    tab_glist *info=creer_intialiser_tab_glist();
    tab_glist *info1=creer_intialiser_tab_glist();
    FILE *f1=NULL;
    f1= fopen("D:\\C projects\\GTK\\exam_tp\\text3.txt","r");
    if(!f1)
    {
        printf("error oppening file \n");
        exit(-1);
    }
    win= principale_function(f1,info);
    win1= principale_function(f1,info1);
    gtk_widget_show_all(win);
    gtk_widget_show_all(win1);
}


int main(int argc,char *argv[])
{
    gtk_init(&argc, &argv);
    Myffunn();
    gtk_main();
    return 0;
}