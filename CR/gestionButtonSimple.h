#include "gestionTextView.h"



void initialise_str_simple_button(prSimpleBut *butt)
{
	butt->racine = NULL;
	
	strcpy(butt->label, "default");
	strcpy(butt->image, "");
	strcpy(butt->stock_icon, "");
	strcpy(butt->nom, "default");
	butt->mnemonic = 0;
	butt->pos = NULL;
	butt->type_special = 0;
}

void create_simple_button(prSimpleBut *btn)
{
	GtkImage *img;
	if (btn->mnemonic)
		btn->button = gtk_button_new_with_mnemonic((btn->label));
	else if (strcmp(btn->stock_icon,""))
			btn->button = gtk_button_new_from_stock(btn->stock_icon);
	else if (strcmp(btn->label,""))
			btn->button = gtk_button_new_with_label(btn->label);
	else btn->button = gtk_button_new_with_label("");
	gtk_widget_set_name(btn->button, btn->nom);
	if (strcmp(btn->image,""))
	{
		img = (GtkImage*)gtk_image_new_from_file(btn->image);
		gtk_image_set_pixel_size(img, 15);
		gtk_button_set_image(GTK_BUTTON(btn->button), (GtkWidget*)img);
	}
	switch (btn->type_special)
	{
	case 1:
		btn->button = gtk_file_chooser_button_new(btn->label, GTK_FILE_CHOOSER_ACTION_OPEN);
		break;
	case 2:
		btn->button = gtk_color_button_new();
		break;
	case 3:
		btn->button = gtk_font_button_new();
		break;
	default: break;
	}
}
void One_button_prop_read(Prop*pr, prSimpleBut* prop)
{
	if (!strcmp(pr->name, "label"))strcpy(prop->label, pr->Value);
	else if (!strcmp(pr->name, "nom"))strcpy(prop->nom, pr->Value);
	else if (!strcmp(pr->name, "stock_icon"))strcpy(prop->stock_icon,pr->Value);
	else if (!strcmp(pr->name, "image"))strcpy(prop->image, pr->Value);
	else if (!strcmp(pr->name, "mnemonic"))prop->mnemonic = (int)atoi(pr->Value);
	else if (!strcmp(pr->name, "type_special"))prop->type_special = atoi(pr->Value);
}

void ButtonPropReader(Prop*List, prSimpleBut* prop)
{
	Prop*crt = List;
	while (crt)
	{
		One_button_prop_read(crt, prop);
		crt = crt->next;
	}
}
GtkWidget *Create_Button_from_node(Node*Racine)
{
	prSimpleBut prop;
	initialise_str_simple_button(&prop);
	ButtonPropReader(Racine->List, &prop);
	create_simple_button(&prop);
	return((GtkWidget*)prop.button);
}