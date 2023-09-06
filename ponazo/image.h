/* La structure d'une image **************************************************/
typedef struct
{
    // pointeur sur le widget image
    GtkWidget* image;
    gchar* img_name; //le chemin d’emplacement de l'image
    gint type_img; // indique si l'image est un file , animation ou icon
    /*
     * 0: gtk_image_new_from_file ();
     * 1: gtk_image_new_from_animation ();et gdk_pixbuf_animation_new_from_file();
     * 2: gtk_image_new_from_icon_name ();
     * */
    GtkIconSize icon_siz; //la taille d'icon
    int pixel_size;         //gtk_image_set_pixel_size ()
}image;
/*******************************************************************************
 * Nom : creer_initialiser_image;
 * Sorties : image * : la structure image initialisee
 * La fonction permet d'allouer un espace memoire pour une image et de
 * l'initialisee par des valeurs par defaut
 * */
image* creer_initialiser_image()
{
    // initialisation de la structure image
    image* im = (image*)malloc(sizeof(image));
    if (!im)
    {
        printf("`\nErreur d'allocation memoire de l'image!");
        exit(0);
    }
    // initialisation des champs de la structure
    im->image = NULL;
    im->img_name = NULL;
    im->type_img = 0;
    im->icon_siz = GTK_ICON_SIZE_MENU;
    im->icon_siz = 100;
    return (image*)im;
}
/******************************************************************************
 * Nom : make_image;
 * Entrees : image * : la structure d'une image
 * La fonction permet de creer une image passee en parametres
 * */
void make_image(image* img_info)
{
    if (!img_info)  // test d'existance
    {
        printf("\nErreur : image n'existe pas");
        exit(0);
    }
    // Creation de l'image
    if (!img_info->type_img) 
        img_info->image = gtk_image_new_from_file(img_info->img_name);
    else if (img_info->type_img == 2) 
        img_info->image = gtk_image_new_from_icon_name(img_info->img_name, 
                                                        img_info->icon_siz);
    else img_info->image = gtk_image_new_from_animation (
                gdk_pixbuf_animation_new_from_file(img_info->img_name, NULL));
    gtk_image_set_pixel_size(GTK_IMAGE(img_info->image), img_info->pixel_size);
}
/******************************************************************************
 * Nom : remplir_image;
 * Entrees : FILE *fl : le fichier qui contient le code source
 * Sorties : image * : la structure image remplit a partir du fichier
 * La fonction permet de modifier les valeurs par defaut de la structure
 * image par les valeurs extrait de fichier
 * */
image* remplir_image(FILE* f1)
{
    // initialisation de la structure image
    image* im = creer_initialiser_image();
    obj_att* p = NULL;
    // lire les attributs de l'image a partir du fichier
    p = lire_att(f1); 
    while (p)
    {
        if (!strcmp(p->attribut, "filename"))
        {
            im->img_name = p->valeur;
            im->type_img = 0;
        }
        else if (!strcmp(p->attribut, "animation-file"))
        {
            im->img_name = p->valeur;
            im->type_img = 1;
        }
        else if (!strcmp(p->attribut, "icon-name"))
        {
            im->img_name = p->valeur;
            im->type_img;
        }
        else if (!strcmp(p->attribut, "icon-size"))
        {
            if (!strcmp(p->valeur, "BUTTON")) 
                im->icon_siz = GTK_ICON_SIZE_BUTTON;
            else if (!strcmp(p->valeur, "DND")) 
                im->icon_siz = GTK_ICON_SIZE_DND;
            else if (!strcmp(p->valeur, "DIALOG")) 
                im->icon_siz = GTK_ICON_SIZE_DIALOG;
            else if (!strcmp(p->valeur, "SMALL_TOOLBAR")) 
                im->icon_siz = GTK_ICON_SIZE_SMALL_TOOLBAR;
            else if (!strcmp(p->valeur, "LARGE_TOOLBAR")) 
                im->icon_siz = GTK_ICON_SIZE_LARGE_TOOLBAR;
        }
        else if (!strcmp(p->attribut, "pixel")) im->pixel_size = atoi(p->valeur);
        p = p->suiv;
    }
    return (image*)im;
}