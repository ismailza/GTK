#include "gestionWindow.h"


GtkShadowType read_frame_shad_type(int ind)
{
	if (ind == 1)return GTK_SHADOW_NONE;
	if (ind == 2)return GTK_SHADOW_IN;
	if (ind == 3)return GTK_SHADOW_OUT;
	if (ind == 4)return GTK_SHADOW_ETCHED_IN;
	if (ind == 5)return GTK_SHADOW_ETCHED_OUT;
    printf("\nErreur \n");
}

void One_frame_prop_read(Prop* pr, FrameProp* props)
{
	if (!strcmp(pr->name, "label"))strcpy(props->label, pr->Value);
	else if (!strcmp(pr->name, "label_xalign"))props->label_xalign = atof(pr->Value);
	else if (!strcmp(pr->name, "label_yalign"))props->label_yalign = atof(pr->Value);
	else if (!strcmp(pr->name, "shadow_type"))props->shadow_type = read_frame_shad_type((int)atof(pr->Value));
	else if (!strcmp(pr->name, "hauteur"))props->Hauteur = (int)atof(pr->Value);
	else if (!strcmp(pr->name, "largeur"))props->Largeur = (int)atof(pr->Value);
}

void FramePropReader(Prop* list, FrameProp* props)
{
	Prop* crt = list;
	while (crt)
	{
		One_frame_prop_read(crt, props);
		crt = crt->next;
	}

}


GtkWidget*CreatFrame(FrameProp prop)
{
	GtkWidget*frame = gtk_frame_new(prop.label);
	gtk_frame_set_label_align(GTK_FRAME(frame), prop.label_xalign, prop.label_yalign);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), prop.shadow_type);
	if(prop.Hauteur>0 && prop.Largeur>0)gtk_widget_set_size_request(frame, prop.Hauteur, prop.Largeur);
	if (prop.child)gtk_container_add(frame, prop.child);
	return((GtkWidget*)frame);
}


void init_default_frame_prop(FrameProp*props)
{
	strcpy(props->label, "default");
	props->Largeur = props->Hauteur = 250;
	props->label_xalign = props->label_yalign = 0.5;
	props->shadow_type = GTK_SHADOW_IN;
	props->child = NULL;
}

GtkWidget* Create_frame_from_node(Node* Racine, TableInterface *table)
{
	FrameProp prop;
	init_default_frame_prop(&prop);
	FramePropReader(Racine->List, &prop);
	prop.child = createWidget(Racine->fils, table);
	return((GtkWidget*)CreatFrame(prop));
}