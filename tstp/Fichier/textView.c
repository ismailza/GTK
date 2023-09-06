#include "fichier.h"



void init_format_textbuf(TextView *txt)
{
    strcpy(txt->t.background,"white");
    strcpy(txt->t.foreground,"red");
    strcpy(txt->t.style,"italic");
    strcpy(txt->t.weight,"gras");
    strcpy(txt->text,"Bonjour tout le monde");
    txt->t.size=100;
    txt->t.left_margin=30;
}

void create_textView(TextView *view)
{
    view->buffer=gtk_text_buffer_new(NULL);
    view->textview=gtk_text_view_new_with_buffer(view->buffer);

    gtk_text_buffer_create_tag(view->buffer,"lmarg","left_margin",view->t.left_margin,NULL);
    ///*********choix du style********
    if(!strcmp(view->t.style,"italic"))
    gtk_text_buffer_create_tag(view->buffer,"style","style",PANGO_STYLE_ITALIC,NULL);
    if(!strcmp(view->t.style,"oblique"))
    gtk_text_buffer_create_tag(view->buffer,"style","style",PANGO_STYLE_OBLIQUE,NULL);
    if(!strcmp(view->t.style,"normal"))
    gtk_text_buffer_create_tag(view->buffer,"style","style",PANGO_STYLE_NORMAL,NULL);
    ///*********choix du weight*******
    if(!strcmp(view->t.weight,"gras"))
    gtk_text_buffer_create_tag(view->buffer,"weight","weight",PANGO_WEIGHT_BOLD,NULL);
    if(!strcmp(view->t.weight,"light"))
    gtk_text_buffer_create_tag(view->buffer,"weight","weight",PANGO_WEIGHT_LIGHT,NULL);
    if(!strcmp(view->t.weight,"normal"))
    gtk_text_buffer_create_tag(view->buffer,"weight","weight",PANGO_WEIGHT_NORMAL,NULL);
    if(!strcmp(view->t.weight,"ultrabold"))
    gtk_text_buffer_create_tag(view->buffer,"weight","weight",PANGO_WEIGHT_ULTRABOLD,NULL);
    ///**************size*****************
    gtk_text_buffer_create_tag(view->buffer,"size","size",view->t.size,NULL);
    ///********color***********************
    gtk_text_buffer_create_tag(view->buffer,"foreground","foreground",view->t.foreground,NULL);
    ///*******background******************
    gtk_text_buffer_create_tag(view->buffer,"background","background",view->t.background,NULL);
    gtk_text_buffer_get_iter_at_offset(view->buffer,&view->iter,0);
    //gtk_text_buffer_insert(buffer,&iter,NULL,-1);
    gtk_text_buffer_insert_with_tags_by_name(view->buffer,&view->iter,view->text,-1,"background","foreground","weight","style","lmarg","size");


}
///********************************************

///********************************************
void read_prop_tv(Prop *lst,TextView *view)
{
    Prop *crt;
    crt=lst;
    while(crt)
    {
    if(!strcmp(crt->name,"text"))
    {
        trans_tir_to_blanc(crt->Value);
        strcpy(view->text,crt->Value);
    }
    else
         if(!strcmp(crt->name,"background"))
         strcpy(view->t.background,crt->Value);
         else
            if(!strcmp(crt->name,"foreground"))
            strcpy(view->t.foreground,crt->Value);
            else
              if(!strcmp(crt->name,"style"))
              strcpy(view->t.style,crt->Value);
              else
                  if(!strcmp(crt->name,"weight"))
                  strcpy(view->t.weight,crt->Value);
        crt=crt->next;
    }
}


///********************************************

///********************************************

GtkWidget *node_to_textview(Node *noeud)
{
    TextView view;
    ///*******INITIALISATION************
    init_format_textbuf(&view);
    read_prop_tv(noeud->List,&view);
    create_textView(&view);
    return (GtkWidget*)view.textview;
}







