#include "gestionCheckButton.h"


/********************************************************************
  
  >_Nom:		      initTableProps.
  >_Entree:		    La structure des proprietes de la table .
  >_Sortie:	      La structure initialisee.
  >_Description:	La fonction permet d'initialiser la structure 
                  par des valeurs condideres des valeur par defauts.

********************************************************************/
void initTableProps(tableProps *ourTable)
{
    ourTable->table=NULL;
    ourTable->fatherContainer=NULL;  
    ourTable->rows=0;                     
    ourTable->columns=0;                  
    ourTable->def_row_spacing=0;          
    ourTable->def_col_spacing=0;          
    ourTable->homogeneous=TRUE;           
    ourTable->children=NULL; 
}
/********************************************************************
  
  >_Nom:		      initTableChildsProps.
  >_Entree:		    La structure des proprietes des fils de la table .
  >_Sortie:	      La structure initialisee.
  >_Description:	La fonction permet d'initialiser la structure 
                  par des valeurs condideres des valeur par defauts
                  qui cincerne lattachement des fils a la table mere.

********************************************************************/
void initTableChildsProps(tableChildsProps *child)
{
    child->childWidget = NULL;
    child->left_attach = 0;
    child->right_attach = 0;
    child->top_attach = 0;
    child->bottom_attach = 0;
    child->xoptions = GTK_FILL;
    child->yoptions = GTK_FILL;
    child->xpadding = 0;
    child->ypadding = 0;
    child->next = NULL;
}


/********************************************************************
  
  >_Nom:		      createTableProps.
  >_Entree:		    La structure des proprietes de la table a creer.
  >_Sortie:	      La table.
  >_Description:	La fonction permet de creer une table suivant
                  les proprietes stcokes dans la structure.

********************************************************************/
GtkWidget* createTableProps(tableProps *ourTable)
{
    // la table a creer
    GtkWidget* table;
    // creation de la table 
    table = gtk_table_new(ourTable->rows,ourTable->columns, ourTable->homogeneous);
    // specefication du spacing entre les lignes
    gtk_table_set_row_spacings(GTK_TABLE(table), ourTable->def_row_spacing);
    // specefication du spacing entre les colonnes
    gtk_table_set_col_spacings(GTK_TABLE(table), ourTable->def_col_spacing); 
    return((GtkWidget *)table);
}

void changeTableChildsProps_fromNode(tableChildsProps *ourChild,Prop *Propr)
{
    Prop *crt = Propr;
    while (crt)
    {

        if(strcmp(crt->name, "left_attach") == 0)  
            ourChild->left_attach = (int)(atof(crt->Value));

        else if(strcmp(crt->name, "right_attach") == 0)  
            ourChild->right_attach = (int)(atof(crt->Value));

        else if(strcmp(crt->name, "top_attach") == 0)  
            ourChild->top_attach = (int)(atof(crt->Value));

        else if(strcmp(crt->name, "bottom_attach") == 0)  
            ourChild->bottom_attach = (int)(atof(crt->Value));

        else if(strcmp(crt->name, "xpadding") == 0)  
            ourChild->xpadding = (int)(atof(crt->Value));
        
        else if(strcmp(crt->name, "ypadding") == 0)  
            ourChild->ypadding = (int)(atof(crt->Value));

        else if(strcmp(crt->name, "xoptions") == 0)
        {  
            if(strcmp(crt->Value, "expand") == 0)
                ourChild->xoptions = GTK_EXPAND;
            else if (strcmp(crt->Value, "shrink") == 0)
                ourChild->xoptions = GTK_SHRINK;
            else if (strcmp(crt->Value, "fill") == 0)
                ourChild->xoptions = GTK_FILL;
        }

        else if(strcmp(crt->name, "yoptions") == 0)
        {  
            if(strcmp(crt->Value, "expand") == 0)
                ourChild->yoptions = GTK_EXPAND;
            else if (strcmp(crt->Value, "shrink") == 0)
                ourChild->yoptions = GTK_SHRINK;
            else if (strcmp(crt->Value, "fill") == 0)
                ourChild->yoptions = GTK_FILL;
        }
        crt = crt->next;
    }
}

void changeTableProps_fromNode(tableProps *ourTable,Prop *Propr)
{
    Prop *crt = Propr;
    while (crt)
    {

        if(strcmp(crt->name, "rows") == 0)  
            ourTable->rows = (int)(atof(crt->Value));

        else if(strcmp(crt->name, "columns") == 0)  
            ourTable->columns = (int)(atof(crt->Value));

        else if(strcmp(crt->name, "homogeneous") == 0)
        {  
            if(strcmp(crt->Value, "true") == 0)
                ourTable->homogeneous = TRUE;
            else 
                ourTable->homogeneous = FALSE;
        }

        else if(strcmp(crt->name, "row_spacing") == 0)
            ourTable->def_row_spacing = (int)(atof(crt->Value));
        
        else if(strcmp(crt->name, "col_spacing") == 0)
            ourTable->def_col_spacing = (int)(atof(crt->Value));

        else
            printf("\nPardon mais la propriete que vous chercher n'est pas valable, on va adopter celle par defaut\n");
        crt = crt->next;
    }
}

tableChildsProps* createTableChildsProps()
{
    tableChildsProps* CProps;
    CProps = (tableChildsProps*)malloc(sizeof(tableChildsProps));
    if(!CProps) 
    {
        printf("espace memoire insuffisant");
        exit(-1);
    }
    initTableChildsProps(CProps);
    CProps->next = NULL;
    return((tableChildsProps*) CProps);
}



void addTableChilds_fromNode(tableProps *ourTable,Node *root,TableInterface *tableInt)
{
    Node *crt = root->fils;
    
    while (crt)
    {
        GtkWidget *child = createWidget(crt, tableInt);
        tableChildsProps *childProps;
        childProps = createTableChildsProps();
        changeTableChildsProps_fromNode(childProps,crt->List);
        

        gtk_table_attach(GTK_TABLE(ourTable->table), child,childProps->left_attach
                    , childProps->right_attach, childProps->top_attach
                    ,childProps->bottom_attach,childProps->xoptions
                    ,childProps->yoptions,childProps->xpadding
                    ,childProps->ypadding);


        crt = crt->freres;
    }
  
}



GtkWidget *createTable_fromNode(Node *root,TableInterface *tableInt)
{
    
    tableProps ourTable;
    
    initTableProps(&ourTable);
    changeTableProps_fromNode(&ourTable,root->List);
    ourTable.table = createTableProps(&ourTable);
    
    addTableChilds_fromNode(&ourTable, root, tableInt);

    return ((GtkWidget *)ourTable.table);
}

