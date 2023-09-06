#include "WidgetsProps.h"
#include "reader.h"
#include "hash.h"

GtkWidget *createWidget(Node *NdObject, TableInterface *interface);
void positionnementchild(GtkBox *box, GtkWidget *Child, positionnementFils positions);

/////***************Box Traitement***********////


void intitBox(BoxCaracteristique *props)
{
    props->isHomogeneous = FALSE;   //homogenite FALSE 
    props->orientation = GTK_ORIENTATION_VERTICAL; //orientation vertical
    props->spacing = 1; //l'espace entre les fils = 1px
}



//remplir la structure de box 
void remplirStructure(BoxCaracteristique *props, char *nomProp, char *contentProp)
{
    
    //gestion d'orientation 
    if(strcmp(nomProp, "orientation") == 0)
    {  
        if(strcmp(contentProp, "1") == 0)
            props->orientation = GTK_ORIENTATION_HORIZONTAL;
        else if(strcmp(contentProp, "0") == 0)
            props->orientation = GTK_ORIENTATION_VERTICAL;
    }

    //gestion d'homogenite 
    else if(strcmp(nomProp, "homogeneous") == 0)
    {  
        if(strcmp(contentProp, "true") == 0)
            props->isHomogeneous = TRUE;
        else 
            props->isHomogeneous = FALSE;
    }


    //le spacing 
    else if(strcmp(nomProp, "spacing") == 0)
        props->spacing = (int)(atof(contentProp));
    

    else
        printf("\nPardon mais la propriete que vous chercher n'est pas valable, on va adopter celle par defaut\n");
}

//lier les props 
void linkProps(BoxCaracteristique *props, Prop *List)
{
    Prop *crt = List;
    while (crt)
    {
        remplirStructure(props, crt->name, crt->Value);
        crt = crt->next;
    }
}


//creation de Box 
GtkWidget *creerBoxxx(BoxCaracteristique carac)
{
    //creation de box 
    GtkWidget *box = gtk_box_new(carac.orientation, carac.spacing);
    //definir l'homoginite 
    gtk_box_set_homogeneous(GTK_BOX(box), carac.isHomogeneous);
    //retourner le box
    return ((GtkWidget *)box);
}
/**Passer au fils depend de container donc il faut 
 * bien pour chaque container declarer s'il s'agit d'un 
 * box, d'un grid, d'une table, ou window 
 * passerAFilsBox
 * passerAFilsGrid
 * passerAFilsTable
 * ...
 * et les proprietes de packing seront declarees bien evidement
 * dans les props de fils 
 * pour faciliter le travail 
 * et etre independant totalement de fichier XML deja cree

*/

//

void passerAPereRad(Node *rootGtkWidget, GtkWidget *createdBox, TableInterface *interface)
{
    //passer au fils de box 
    Node *crt = rootGtkWidget->fils;
    unsigned short int extremite = 0;
    
    GtkWidget *child = createWidget(crt, interface);
    //*CHERCHER LES PROPS fill et expand dans les props *//
    int padding = 10;

    Prop *list = crt->List;
    //initialisation des types de packing 
    gboolean expand = FALSE, fill = FALSE;
    while(list)
    {
        //expand
        if(strcmp(list->name, "expand") == 0)
        {
            if(strcmp(list->Value, "true") == 0)
                expand = TRUE;
            
            else if(strcmp(list->Value, "false") == 0)
                expand = FALSE;
        }

        //fill
        else if(strcmp(list->name, "fill") == 0)
        {
            if(strcmp(list->Value, "true") == 0)
                fill = TRUE;
            
            else if(strcmp(list->Value, "false") == 0)
                fill = FALSE;
        }

        //padding (nbr de pix entre le box et les frontieres)
        else if(strcmp(list->name, "padding") == 0)
            padding = (int)atoi(list->Value);
        
        //encapsulation au debut ou fin
        else if(strcmp(list->name, "extremite") == 0)
            extremite = (int)atoi(list->Value);
        
        //passer a la prop suivante en cherchant 
        //la prop specifique au box 
        list = list->next;
    }

    //positionner le fils dans le box 
    positionnementFils pos = {extremite, expand, fill, padding};
    positionnementchild(GTK_BOX(createdBox), child, pos);

    
}

// void passerAFrereRad(Node *rootGtkWidget, GtkWidget *createdBox, GtkWidget *pereRad, TableInterface *interface)
// {
    
// }




void passerAFilsBox(Node *rootGtkWidget, GtkWidget *createdBox, TableInterface *interface)
{
    //passer au fils de box 
    Node *crt = rootGtkWidget->fils;
    unsigned short int extremite = 0;
    while (crt)
    {
        GtkWidget *child = createWidget(crt, interface);
        //*CHERCHER LES PROPS fill et expand dans les props *//
        int padding = 10;

        Prop *list = crt->List;
        //initialisation des types de packing 
        gboolean expand = FALSE, fill = FALSE;
        while(list)
        {
            //expand
            if(strcmp(list->name, "expand") == 0)
            {
                if(strcmp(list->Value, "true") == 0)
                    expand = TRUE;
                
                else if(strcmp(list->Value, "false") == 0)
                    expand = FALSE;
            }

            //fill
            else if(strcmp(list->name, "fill") == 0)
            {
                if(strcmp(list->Value, "true") == 0)
                    fill = TRUE;
                
                else if(strcmp(list->Value, "false") == 0)
                    fill = FALSE;
            }

            //padding (nbr de pix entre le box et les frontieres)
            else if(strcmp(list->name, "padding") == 0)
                padding = (int)atoi(list->Value);
            
            //encapsulation au debut ou fin
            else if(strcmp(list->name, "extremite") == 0)
                extremite = (int)atoi(list->Value);
            
            //passer a la prop suivante en cherchant 
            //la prop specifique au box 
            list = list->next;
        }

        //positionner le fils dans le box 
        positionnementFils pos = {extremite, expand, fill, padding};
        positionnementchild(GTK_BOX(createdBox), child, pos);

        //passer au frere s'il existe 
        crt = crt->freres;
    }
}

//d'apres le noued de notre arbre on va creer la widget 
GtkWidget *createBoxNode(Node *root, TableInterface *interface)
{
    //les elements par defaut de Box
	BoxCaracteristique Prop;
    intitBox(&Prop);
    

    //remplissage des valeurs d'apres l arbre 
	linkProps(&Prop, root->List);
    GtkWidget *box = creerBoxxx(Prop);

    passerAFilsBox(root, box, interface);

    //et ici on va avoir le box et leurs fils 
    return ((GtkWidget *)box);
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



////************Box Traitement FIN***********////
