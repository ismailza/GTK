#include "structures.h"

void intitBox(BoxCaracteristique *props)
{
    props->isHomogeneous = FALSE;   //homogenite FALSE
    props->orientation = GTK_ORIENTATION_VERTICAL; //orientation vertical
    props->spacing = 1; //l'espace entre les fils = 1px
}


//creation de Box
GtkWidget *creerBoxxx(BoxCaracteristique carac)
{
    //creation de box
    carac.box = gtk_box_new(carac.orientation, carac.spacing);
    //definir l'homoginite
    gtk_box_set_homogeneous(GTK_BOX(carac.box ), carac.isHomogeneous);
    //retourner le box
    return ((GtkWidget *)carac.box);
}

void positionnementchild(GtkBox *box, GtkWidget *Child, positionnementFils positions)
{
    //le cas si l'encapsulation sera au debut de box
    if(positions.extremite == 0) //pack_start
        gtk_box_pack_start(box, Child, positions.expand, positions.fill, positions.padding);

    //le cas si l'encapsulation sera a la fin de box
    if(positions.extremite == 1) //pack_end
        gtk_box_pack_end(box, Child, positions.expand, positions.fill, positions.padding);

}
