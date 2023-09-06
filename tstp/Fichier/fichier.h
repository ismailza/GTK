#ifndef FICHIER_H_INCLUDED
#define FICHIER_H_INCLUDED
#include <gtk/gtk.h>
#include<string.h>
#include <stdlib.h>
typedef struct Pr
{
	char name[20];
	char *Value;
	struct Pr*next;
}Prop;
typedef struct Nd
{
	char id[20];
	char classBalise[20];
	Prop*List;
	struct Nd*fils;
	struct Nd*freres;
}Node;
typedef struct
{
    char style[10];///0:italic,1:oblique,3:normal
    char font[26];///police d'ecriture
    char taille[4];///size du texte
    char weight[15];///ultralight,light,normal,bold,ultrabold,heavy
    char color[26];///couleur du texte
    char background[26];///couleur du fond
    char underline[10];///single,double,low ou none
    char underline_color[26];///couleur du soulignement
    char barrer_txt[6];///true ou false
    char color_bar[26];///couleur du
    char select;//0:non ou 1:oui
    char alignement;//0:left,1:right,2:center, ou 3:fill
}Format;

typedef struct
{
    GtkWidget* Label;///pointeur sur le label
    char texte[255];///le texte à afficher
    Format lab;//formatage du texte
}Lab_txt;


typedef struct
{
    char background[26];
    char foreground[26];
    int left_margin;
    char style[26];///italic,oblique,normal
    char weight[26];///normal,gras,light,ultrabold
    int size;

}formatTV;
typedef struct
{
GtkWidget *textview; /// widget d'un textview.
GtkTextBuffer *buffer; /// le buffer associé au text view.
GtkTextIter iter; /// l'itération au niveau du buffer.
char text[100];
formatTV t;
}TextView;

void trans_tir_to_blanc(char *val);
void trans_propr(Prop *lstpr,Lab_txt *label);
GtkWidget *node_to_label(Node *noeud);
void init_label(Lab_txt* l);
void creer_label(Lab_txt* l);
void init_format_textbuf(TextView *txt);
void create_textView(TextView *view);
void read_prop_tv(Prop *lst,TextView *view);
GtkWidget *node_to_textview(Node *noeud);

#endif // FICHIER_H_INCLUDED
