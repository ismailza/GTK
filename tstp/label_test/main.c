#include <gtk/gtk.h>
#include<stdio.h>
#include <stdlib.h>
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
//*****************************************
/*Cette fonction permet d'initialiser les champs d'un label.
Elle prend en arguments un pointeur sur le label et elle ne retourne rien*/
//*****************************************
void init_label(Lab_txt* l)
{
    strcpy(l->texte,"\nbonjour tout le monde\ncomment allez vous!!!");
    strcpy(l->lab.background, "white");
    strcpy(l->lab.color, "blue");
    strcpy(l->lab.barrer_txt, "false");
    strcpy(l->lab.color_bar, "black");
    strcpy(l->lab.font, "calibri");
    strcpy(l->lab.taille, "20");
    strcpy(l->lab.underline, "none");
    strcpy(l->lab.underline_color, "black");
    strcpy(l->lab.style, "normal");
    strcpy(l->lab.weight, "normal");
    l->lab.alignement = '3';
    l->lab.select = '1';
}
/*cette fonction permet de lire les caractéristique d'un label*/
void read_label(Lab_txt* l)
{
    printf("\nDonnez le style:normal,oblique ou italic");
    scanf("%s",l->lab.style);
    printf("\nDonnez la police");
    scanf("%s", l->lab.font);
    printf("\nDonnez la taille");
    scanf("%s", l->lab.taille);
    printf("\nDonner la couleur");
    scanf("%s", l->lab.color);
    printf("\nDonnez la couleyr du fond");
    scanf("%s", l->lab.background);
    printf("\nDonnez le type d'underline");
    printf("\nsingle,double,low,none");
    scanf("%s", l->lab.underline);
    printf("\nDonner la couler d'underline");
    scanf("%s", l->lab.underline_color);
    printf("Vous voulez barrer le texte?");
    printf("\n true:si oui, false:si non");
    scanf("%s", l->lab.barrer_txt);
    if (!strcmp(l->lab.barrer_txt, "true"))
    {
        printf("\nAvec quelle couleuur vous voulez le barer");
        scanf("%s", l->lab.color_bar);
    }
    getchar();
    printf("\nalignement du texte:");
    printf("\n0:left,1:right,2:center,3:fill");
    scanf("%c", &l->lab.alignement);getchar();
    printf("\nVous voullez selectionner le texte?");
    printf("\n0:non,1:oui");
    scanf("%c", &l->lab.select);
}


///*****************************************
/*Cette fonction permet de créer un label,
Elle prend en argument un pointeur sur le label qu'on va le créer,
ainsi que ses caractéristique
*/
///*****************************************
void creer_label(Lab_txt* l)
{
    char* format,//ce pointeur est pour le formatage du texte
        * markup;//pour regrouper les caractéristiques et le format
    //initialisation du label
    l->Label = gtk_label_new(NULL);
    //format du texte
    format = "<span style=\"%s\"><span face=\"%s\"><span size=\"%s\">\
             <span weight=\"%s\"><span color=\"%s\">\
              <span background=\"%s\"><span underline=\"%s\">\
              <span underline_color=\"%s\"><span strikethrough=\"%s\">\
              <span strikethrough_color=\"%s\">%s</span></span></span>\
              </span></span></span></span></span></span></span>";
    //association des caractéristique aux codes format
    markup = g_markup_printf_escaped(format, l->lab.style, l->lab.font,
            l->lab.taille, l->lab.weight,l->lab.color, l->lab.background,
             l->lab.underline, l->lab.underline_color,l->lab.barrer_txt,
              l->lab.color_bar, l->texte);
    //choix d'alignement
    if (l->lab.alignement == '0') gtk_label_set_justify((l->Label), GTK_JUSTIFY_LEFT);
    if (l->lab.alignement == '1')  gtk_label_set_justify((l->Label), GTK_JUSTIFY_RIGHT);
    if (l->lab.alignement == '2')  gtk_label_set_justify((l->Label), GTK_JUSTIFY_CENTER);
    if (l->lab.alignement == '3')  gtk_label_set_justify((l->Label), GTK_JUSTIFY_FILL);
    //selection du texte
    if (l->lab.select == '0') gtk_label_set_selectable((l->Label), FALSE);
    if (l->lab.select == '1') gtk_label_set_selectable((l->Label), TRUE);
    //convertir le texte en utf8
    markup=g_locale_to_utf8(markup,-1,NULL,NULL,NULL);
    //association du texte avec le label
    gtk_label_set_markup(GTK_LABEL(l->Label), markup);
    //liber le pointeur markup
    g_free(markup);
}

void creer_box_label(GtkWidget* BOX)
{
    Lab_txt l;
    init_label(&l);
    //read_label(&l);
    creer_label(&l);

    gtk_box_pack_start(GTK_BOX(BOX), l.Label, TRUE, TRUE, 2);
}

GtkWidget *creer_window(GtkWidget *window)
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "hello!");
    gtk_widget_set_size_request(window, 300, 300);
    gtk_window_set_position(window, GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_resizable(window, TRUE);
    return (GtkWidget*)window;
}

void main(int argc, char* argv[])
{
    GtkWidget* window, * BOX;
    gtk_init(&argc, &argv);
    window=creer_window(window);
    BOX = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    creer_box_label(BOX);
    gtk_container_add(GTK_CONTAINER(window), BOX);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_widget_show_all(window);
    gtk_main();
}
