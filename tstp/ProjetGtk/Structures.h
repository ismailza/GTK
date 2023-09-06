#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
///*****************WINDOW*********
typedef struct
{
    GtkWidget *window;
	GtkWindowType Type;   //type de fenetre
	GtkWindowPosition Pos;// position
	gchar title[25];          //titre
	gint Hauteur;          //Hauteur
	gint Largeur;          //Largeur
	gboolean isMain;      //if main
	gboolean isResisable; //if resisable
	gchar iconPlace[100];      //l'icone
	gboolean isModale;    //if Modale
	gboolean isMaximized;  //if maximized
	gboolean isFullscreen; //if fullscreen
	gboolean isDecorated;  //if decorated
	gboolean isDeletable;
	GtkWidget*child;
}windowProp;

void Init_default_window_prop(windowProp*wind);
GtkWidget*Creatwindow(windowProp props);


///**********BOX***********************
typedef struct {
    GtkWidget *box;
    GtkOrientation   orientation; //l'orientation vertical ou Horizontal
    gint             spacing;   //espace entre les fils de Box
    gboolean         isHomogeneous;
}BoxCaracteristique; //snom de la structure


typedef struct {
    gint        extremite; //0 = pack start or  1 = pack end
    gboolean    expand;
    gboolean    fill;
    guint       padding;
}positionnementFils;


#endif // STRUCTURES_H_INCLUDED
