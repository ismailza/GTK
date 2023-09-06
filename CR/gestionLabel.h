#include "gestionMenu.h"


void init_label(Lab_txt* l)
{
    strcpy(l->texte,"\nbonjour tout le monde\n");
    strcpy(l->lab.background, "white");
    strcpy(l->lab.color, "red");
    strcpy(l->lab.barrer_txt, "false");
    strcpy(l->lab.color_bar, "black");
    strcpy(l->lab.font, "calibri");
    strcpy(l->lab.taille, "12");
    strcpy(l->lab.underline, "double");
    strcpy(l->lab.underline_color, "black");
    strcpy(l->lab.style, "normal");
    strcpy(l->lab.weight, "normal");
    l->lab.alignement = '3';
    l->lab.select = '1';
}
///**************************************************************
/**Cette fonction permet de créer un label,
Elle prend en argument un pointeur sur le label qu'on va le créer,
ainsi que ses caractéristiques
*/
///**************************************************************
void creer_label(Lab_txt* l)
{
    //ce pointeur est pour le formatage du texte
    char *format, *markup;//
    //initialisation du label
    l->Label = gtk_label_new(NULL);
    
    //format du texte
    format = "<span style=\"%s\"><span face=\"%s\"><span color=\"%s\">\
              <span background=\"%s\"><span underline=\"%s\"><span underline_color=\"%s\"><span strikethrough=\"%s\">\
              <span strikethrough_color=\"%s\">%s</span></span></span></span></span></span></span></span>";
    //association des caractéristique aux codes format
    markup = g_markup_printf_escaped(   format, l->lab.style, l->lab.font,
                                        l->lab.color, l->lab.background, l->lab.underline, l->lab.underline_color,
                                        l->lab.barrer_txt, l->lab.color_bar, l->texte);
        
    //choix d'alignement
    if (l->lab.alignement == '0')   gtk_label_set_justify((l->Label), GTK_JUSTIFY_LEFT);
    if (l->lab.alignement == '1')   gtk_label_set_justify((l->Label), GTK_JUSTIFY_RIGHT);
    if (l->lab.alignement == '2')   gtk_label_set_justify((l->Label), GTK_JUSTIFY_CENTER);
    if (l->lab.alignement == '3')   gtk_label_set_justify((l->Label), GTK_JUSTIFY_FILL);
    //selection du texte
    if (l->lab.select == '0')       gtk_label_set_selectable((l->Label), FALSE);
    if (l->lab.select == '1')       gtk_label_set_selectable((l->Label), TRUE);
    //
    markup = g_locale_to_utf8(markup,-1,NULL,NULL,NULL);
    //association du texte avec le label
    gtk_label_set_markup(GTK_LABEL(l->Label), markup);
    //liber le pointeur markup
    g_free(markup);
}


///**********************************

///**********************************

///*************************************
void trans_propr(Prop *lstpr,Lab_txt *label)
{
    Prop *crt = lstpr;

    while(crt)
    {
        if(!strcmp(crt->name, "texte"))                 strcpy(label->texte,crt->Value);
        else if(!strcmp(crt->name, "style"))            strcpy(label->lab.style, crt->Value);
        else if(!strcmp(crt->name, "font"))             strcpy(label->lab.font, crt->Value);
        else if(!strcmp(crt->name, "size"))             strcpy(label->lab.taille, crt->Value);
        else if(!strcmp(crt->name, "weight"))           strcpy(label->lab.weight, crt->Value);
        else if(!strcmp(crt->name, "color"))            strcpy(label->lab.color, crt->Value);
        else if(!strcmp(crt->name, "background"))       strcpy(label->lab.background, crt->Value);
        else if(!strcmp(crt->name, "underline"))        strcpy(label->lab.underline, crt->Value);
        else if(!strcmp(crt->name, "underline_color"))  strcpy(label->lab.underline_color, crt->Value);
        else if(!strcmp(crt->name, "barrer"))           strcpy(label->lab.barrer_txt, crt->Value);
        else if(!strcmp(crt->name, "color_bar"))        strcpy(label->lab.color_bar, crt->Value);
        else if(!strcmp(crt->name, "select"))           label->lab.select = crt->Value;
        else if(!strcmp(crt->name, "alignement"))       label->lab.alignement = crt->Value;
        crt=crt->next;
    }
}

///*************************************


///*************************************
GtkWidget *node_to_label(Node *noeud)
{
    Lab_txt l;
    //initialisation les caractéristique
    init_label(&l);
    trans_propr(noeud->List, &l);
    creer_label(&l);

    return (GtkWidget*)l.Label;
}