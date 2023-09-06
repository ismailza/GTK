#include"gestionEntry.h"


void init_format_textbuf(TextView *txt)
{
	strcpy(txt->t.background, "black");
	strcpy(txt->t.foreground, "red");
	strcpy(txt->t.style, "italic");
	strcpy(txt->t.weight, "gras");
	strcpy(txt->text, "Bonjour tout le monde");
	txt->t.size = 30;
	txt->t.left_margin = 30;
}

GtkWidget *create_textView(TextView view)
{
	GtkWidget *tv;
	view.buffer = gtk_text_buffer_new(NULL);
	tv = gtk_text_view_new_with_buffer(view.buffer);
	///*******INITIALISATION************
	//init_format_textbuf(&view);
	gtk_text_buffer_create_tag(view.buffer, "lmarg", "left_margin", view.t.left_margin, NULL);
	///*********choix du style********
	if (!strcmp(view.t.style, "italic"))
		gtk_text_buffer_create_tag(view.buffer, "style", "style", PANGO_STYLE_ITALIC, NULL);
	if (!strcmp(view.t.style, "oblique"))
		gtk_text_buffer_create_tag(view.buffer, "style", "style", PANGO_STYLE_OBLIQUE, NULL);
	if (!strcmp(view.t.style, "normal"))
		gtk_text_buffer_create_tag(view.buffer, "style", "style", PANGO_STYLE_NORMAL, NULL);
	///*********choix du weight*******
	if (!strcmp(view.t.weight, "gras"))
		gtk_text_buffer_create_tag(view.buffer, "weight", "weight", PANGO_WEIGHT_BOLD, NULL);
	if (!strcmp(view.t.weight, "light"))
		gtk_text_buffer_create_tag(view.buffer, "weight", "weight", PANGO_WEIGHT_LIGHT, NULL);
	if (!strcmp(view.t.weight, "normal"))
		gtk_text_buffer_create_tag(view.buffer, "weight", "weight", PANGO_WEIGHT_NORMAL, NULL);
	if (!strcmp(view.t.weight, "ultrabold"))
		gtk_text_buffer_create_tag(view.buffer, "weight", "weight", PANGO_WEIGHT_ULTRABOLD, NULL);
	///**************size*****************
	gtk_text_buffer_create_tag(view.buffer, "size", "size", view.t.size, NULL);
	///********color***********************
	gtk_text_buffer_create_tag(view.buffer, "foreground", "foreground", view.t.foreground, NULL);
	///*******background******************
	gtk_text_buffer_create_tag(view.buffer, "background", "background", view.t.background, NULL);
	gtk_text_buffer_get_iter_at_offset(view.buffer, &view.iter, 0);
	//gtk_text_buffer_insert(buffer,&iter,NULL,-1);
	gtk_text_buffer_insert_with_tags_by_name(view.buffer, &view.iter, view.text, -1, "background", "foreground", "weight", "style", "lmarg", "size", NULL);

	return (GtkWidget*)tv;
}

void remplirStructTv(TextView *view, char *nomProp, char *contentProp)
{
	if (!strcmp(nomProp, "size"))
		view->t.size = (int)(atof(contentProp));

	else if (!strcmp(nomProp, "left-margin"))
		view->t.left_margin = (int)(atof(contentProp));

	else if (!strcmp(nomProp, "background"))
		strcpy(view->t.background, contentProp);

	else if (!strcmp(nomProp, "foreground"))
		strcpy(view->t.foreground, contentProp);

	else if (!strcmp(nomProp, "style"))
		strcpy(view->t.style, contentProp);

	else if (!strcmp(nomProp, "weight"))
		strcpy(view->t.weight, contentProp);

	else if (!strcmp(nomProp, "text"))
		strcpy(view->text, contentProp);
	
		
}

void linkPropsTv(TextView *props, Prop *List)
{
	Prop *crt = List;
	while (crt)
	{
		remplirStructTv(props, crt->name, crt->Value);
		crt = crt->next;
	}
}

GtkWidget *create_textview_from_node(Node *noeud)
{
	TextView props;
	init_format_textbuf(&props);
	linkPropsTv(&props, noeud->List);
	return((GtkWidget*)create_textView(props));
}
