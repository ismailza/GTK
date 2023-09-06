#include"gestionLabel.h"


void pr_image_create(prImage* im)
{
	im->image = gtk_image_new_from_file(im->chemin);
}

void creat_image_xml(prImage*i, Node*racine) 
{
	if (racine) 
	{
		Prop*crt = racine->List;
		while (crt) 
		{
			if (!(strcmp((crt->name), "chemin")))strcpy(i->chemin, (crt->Value));	
			crt = crt->next;
		}
		pr_image_create(i);
	}
}
GtkWidget* creat_Image_from_node( Node*racine) 
{
	prImage  pp;
	creat_image_xml(&pp, racine);
	return((GtkWidget*)pp.image);
}