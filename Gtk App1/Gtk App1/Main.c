#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "functions.h"
#include "fenetre.h"
#include "headerbar.h"
#include "label.h"
#include "menu.h"
#include "button.h"
#include "radiobutton.h"
#include "linkbutton.h"
#include "toggleutton.h"
#include "checkbutton.h"
#include "image.h"
#include "toolbar.h"
#include "combobox.h"
#include "entryr.h"
#include "searchbar.h"
#include "box.h"
#include "grid.h"
#include "principal.h"
#include "testttt.h"

void hello()
{
    printf("hello");
}

void interprete_file(FILE* file)
{
    GtkWidget* win;
    tab_glist* info = creer_intialiser_tab_glist();
    win = principale_function(file, info);
    gtk_widget_show_all(win);
}

/******************************************************************************
* Programme principal
*/
int main(int argc, char** argv)
{
    gtk_init(&argc, &argv); 
    FILE* f1 = fopen("TP.txt", "r"); 
    if (!f1) 
    {
        printf("\nError oppening file"); 
        exit(0); 
    }
    interprete_file(f1);
    gtk_main(); 
    return 0;
}