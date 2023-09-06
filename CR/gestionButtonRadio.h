#include "gestionNoteBook.h"




///propriété fichier
// <object class=radios id=groupradio1 >
//         <property name=nemonic > 1 </property>
// 		<property name=nom >  radiobtn </property>
// 		<property name=label > option1 </property>
// </object >
// <object class=radios id=groupradio2 >
//         <property name=nemonic > 1 </property>
// 		<property name=nom >  radiobtn </property>
// 		<property name=label > option2 </property>
// </object >
// <object class=radios id=groupradio3 >
//         <property name=nemonic > 1 </property>
// 		<property name=nom >  radiobtn </property>
//         <property name=label > option3 </property>
// </object >



void initialiser_btnRadio(prRadioButton *radios)
{
	
	radios->racine = NULL;
	strcpy(radios->label, "rd1");
	strcpy(radios->nom, "rdbtn");
	radios->mnemonic = 0;
	radios->pos = NULL;
}///fin

///lire prop
void lire_prop_radioBtn(Prop*pr, prRadioButton *radios)
{
	Prop *crt = pr;
	while(crt)
	{
		if (!strcmp(crt->name, "label"))strcpy(radios->label, crt->Value);
		else if (!strcmp(crt->name, "nom"))strcpy(radios->nom, crt->Value);
		else if (!strcmp(crt->name, "mnemonic"))radios->mnemonic = atoi(crt->Value);

		crt = crt->next;
	}
	
}///fin

///creer button radios
void create_radio_button(prRadioButton *btn, prRadioButton *btn1)
{
    GtkWidget *group = NULL;
    if (btn1)
        group = btn1->radioButton;

    if ((btn->mnemonic) && (btn->label))
        btn->radioButton = gtk_radio_button_new_with_mnemonic_from_widget (GTK_RADIO_BUTTON (group),
																			(btn->label));
    else if ((btn->label))
      	btn->radioButton = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (group),
                                        								btn->label);
    else
		btn->radioButton = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (group),
                                        								"prRadioButton");
    gtk_widget_set_name (btn->radioButton, btn->nom);

    if (btn->nom) gtk_widget_set_name(btn->radioButton, btn->nom);
}///fin

///pour creer un boutton radio
prRadioButton Create_RadioButton_from_node(Node *Racine,prRadioButton*Pere)
{
	prRadioButton radioBTN;

	initialiser_btnRadio(&radioBTN);
	lire_prop_radioBtn(Racine->List, &radioBTN);
	create_radio_button(&radioBTN, Pere);
	return(radioBTN);
}///fin


		/* <object class=groupRadioBox id=box1 >
				<property name=homogeneous > false </property>
				<property name=orientation > 0 </property>
				<property name=x > 0 </property>
				<property name=y > 0 </property>
				<child >
					<object class=radios id=radio1 >
						<property name=mnemonic > 1 </property>
						<property name=nom >  radiobtn </property>
 						<property name=label > option1 </property>
					</object >
				</child >
				<child >
					<object class=radios id=radio2 >
						<property name=mnemonic > 1 </property>
						<property name=nom >  radiobtn </property>
 						<property name=label > option1 </property>
					</object >
				</child >
				<child >
					<object class=radios id=radio3 >
						<property name=mnemonic > 1 </property>
						<property name=nom >  radiobtn </property>
 						<property name=label > option1 </property>
					</object >
				</child >
			</object >*/

void Create_All_Radios(Node *Racine, GtkWidget*Container,TableInterface*inter)
{
	Node*crt = Racine->freres;
	prRadioButton RadPere, RadFils;
	RadPere=Create_RadioButton_from_node(Racine, NULL);
	gtk_box_pack_start(GTK_BOX(Container), RadPere.radioButton, TRUE, TRUE, 0);
	insertionEntite(inter, Racine->id, RadPere.radioButton);
	while (crt)
	{
		RadFils= Create_RadioButton_from_node(crt, &RadPere);
		gtk_box_pack_start(GTK_BOX(Container), RadFils.radioButton, TRUE, TRUE, 0);
		insertionEntite(inter, crt->id, RadFils.radioButton);
		crt = crt->freres;
	}
}


GtkWidget *Create_GroupRadioButton_from_node(Node *Racine,TableInterface*inter)
{
	BoxCaracteristique prop = { GTK_ORIENTATION_VERTICAL, 1, FALSE };
	linkProps(&prop, Racine->List);
	GtkWidget *box = creerBoxxx(prop);
	Create_All_Radios(Racine->fils, box, inter);
	return((GtkWidget*)box);
}///fin