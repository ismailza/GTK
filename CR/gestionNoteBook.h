#include "gestionTable.h"

NoteBookProps* initPropsNoteBook()
{
    NoteBookProps* props = (NoteBookProps *)malloc(sizeof(NoteBookProps));
    if(props)
    {
        props->noteBook = gtk_notebook_new();
        props->isScrollable = TRUE; 
        props->showBoders = TRUE;
        props->showTabs = TRUE;

        return props;
    }
    return NULL;
}



GtkPositionType associatePos(char *pos)
{
    GtkPositionType tabPos[4] = {GTK_POS_LEFT, GTK_POS_RIGHT, GTK_POS_TOP, GTK_POS_BOTTOM};

    if(0 <= (int)atoi(pos) && (int)atoi(pos) <= 3)
        return tabPos[(int)atoi(pos)];
    
    else
    {
        printf("\nChoix invalide alors on va adopter la valeur par defaut\n");
        return tabPos[2];
    }
}



void assoociatePropsNoteBook(NoteBookProps *props, Prop *List)
{
    Prop *crt = List;
    while (crt)
    {
        if(strcmp(crt->name, "scrollable") == 0)
            gtk_notebook_set_scrollable(props->noteBook, (int)atoi(crt->Value));
        
        else if(strcmp(crt->name, "borders") == 0)
            gtk_notebook_set_show_border(props->noteBook, (int)atoi(crt->Value));

        else if(strcmp(crt->name, "tabs") == 0)
            gtk_notebook_set_show_tabs(props->noteBook, (int)atoi(crt->Value));
        
        else if(strcmp(crt->name, "position") == 0)
            gtk_notebook_set_tab_pos(props->noteBook, associatePos(crt->Value));
        crt = crt->next;
    } 
}

void posFilsOng(GtkNotebook* notebook, GtkWidget *child, noteBookFils *cara)
{

    gtk_notebook_append_page(notebook, child, cara->tab_label);

}

noteBookFils* initStructureOnglet()
{
    return (noteBookFils *)malloc(sizeof(noteBookFils));
}

void passerAfilsNoteBook(Node *filsNode, NoteBookProps *noteBookCreated, TableInterface *interface)
{

    Node *onglet = filsNode->fils;
    
    while (onglet)
    {
        if(strcmp(onglet->classBalise, "onglet") == 0)
        {
            noteBookFils *cara = initStructureOnglet();

            Node *fils;

            if(strcmp(onglet->fils->classBalise, "label") == 0)
            {
                cara->tab_label = node_to_label(onglet->fils);
                fils = onglet->fils->freres;
            }
            
            else
            {   
                cara->tab_label = gtk_label_new(onglet->List->Value);
                fils = onglet->fils;
            }
                
            GtkWidget *child = createWidget(fils, interface);
            posFilsOng(GTK_NOTEBOOK(noteBookCreated->noteBook), child, cara);
        }
        else 
        {
            printf("\nle premier fils de NoteBook il faut qu'il soit un onglet\n");
            exit(-1);
        }
        onglet = onglet->freres;
    }

}

GtkWidget *createNoteBookFromNode(Node *NodeTree, TableInterface *interface)
{
    NoteBookProps *props = initPropsNoteBook();
    assoociatePropsNoteBook(props, NodeTree->List);
    passerAfilsNoteBook(NodeTree, props, interface);
    return props->noteBook;
}

