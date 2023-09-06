#include "gestionButtonRadio.h"



comboList * creerNodeCombo(char *itemNam)
{
    comboList *NvNode;
    NvNode = (comboList *) malloc(sizeof(comboList));
    NvNode->next = NULL;
    NvNode->itemName = (char *)malloc(sizeof(char)*strlen(itemNam));
    strcpy(NvNode->itemName,itemNam);
    return (comboList *)NvNode;
}


void addComboItems(comboBoxProps *ourCombo,Prop *Propr)
{
    // pointeur pour prcourir les proprietes
    comboList *noeud;
    Prop *crt = Propr;
    while (crt)
    {
        // tester dur le nom de l'attribut
        if(strcmp(crt->name, "item") == 0)  
        {
            noeud = creerNodeCombo(crt->Value);
            noeud->next = ourCombo->items;
            ourCombo->items = noeud;
            printf("affectation de %s\n",noeud->itemName);
        }
        crt = crt->next;
    }
}



void creatComboBox(comboBoxProps *props)
{
    props->comboBox = gtk_combo_box_text_new();
    comboList *crt = props->items;
    printf("combo created\n");
    // test = gtk_combo_box_text_new();
    
    while (crt)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(props->comboBox),crt->itemName);
        crt = crt->next;
    }
}


GtkWidget *createComboBox_fromNode(Node *root)
{
    // declaration de la structure des proprietes de la table
    comboBoxProps ourCombo;
    // initialisation de la structure
    ourCombo.comboBox = NULL;
    ourCombo.items = NULL;
    printf("fin chapitre\n");
    // recuperation des proprietes depuis l'arbre
    addComboItems(&ourCombo,root->List);
    printf("fin chapitre\n");
    // creation de la table selon les proprietes
    creatComboBox(&ourCombo);
    printf("fin chapitre\n");
    return ((GtkWidget *)ourCombo.comboBox);
}