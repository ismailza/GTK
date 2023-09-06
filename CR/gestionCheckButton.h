#include "gestionButtonSimple.h"


///propriété fichier
// <object class=chekButton id=chek1 >
// 		<property name=label > choix1 </property>
// 		<property name=nemonic > 1 </property>
// 		<property name=nom >  </property>
// </object >




void initialiser_chekbutton(prCheckButton *btn)
{
	btn->racine = NULL;
	strcpy(btn->label, "label");
	strcpy(btn->nom, "nom");
	btn->mnemonic = 0;
}///fin

///lire une propriété
void lire_prop_chekbutton(Prop *pr, prCheckButton *chekbtn)
{
	if (!strcmp(pr->name, "label"))strcpy(chekbtn->label, pr->Value);
	else if (!strcmp(pr->name, "nom"))strcpy(chekbtn->nom, pr->Value);
	else if (!strcmp(pr->name, "nemonic"))chekbtn->mnemonic = (int)atoi(pr->Value);
}///fin

void lire_Allprop_chekbutton(Prop *List, prCheckButton *btnchek)
{
	Prop *crt = List;
	while (crt)
	{
		lire_prop_chekbutton(crt, btnchek);
		crt = crt->next;
	}
}///fin

/**** Fonction pour creation ***/
void create_check_bouton(prCheckButton *btn)
{
    if (!btn) btn = (prCheckButton*)malloc(sizeof (prCheckButton));

    if ((btn->mnemonic) && (btn->label))
        btn->checkButton = gtk_check_button_new_with_mnemonic(btn->label);

    else if (btn->label)
        btn->checkButton = gtk_check_button_new_with_label(btn->label);
    // else
    //     btn->checkButton = gtk_check_button_new_with_label("prCheckButton");
    if (btn->nom) gtk_widget_set_name(btn->checkButton, btn->nom);
}///fin


/*creer chekbutton*/
GtkWidget *Create_chekButton_from_node(Node*Racine)
{
    prCheckButton chekbtn;

   

    //initialisation des champs
	initialiser_chekbutton(&chekbtn);
	///lire les propriétés
	lire_Allprop_chekbutton(Racine->List, &chekbtn);
	///creer le buttton
	create_check_bouton(&chekbtn);
	///recuperer le
	return((GtkWidget*)chekbtn.checkButton);
}