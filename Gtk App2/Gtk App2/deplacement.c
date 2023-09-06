//
// Created by Brahim Vall on 4/18/22.
//

#include "Formule_pls_virus.c"
#define maxV 200

/////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//used to initialize and alocate memory for a mov struct
mov*initmov()
{
    mov*fix=(mov*) malloc(sizeof (mov));
    //j is used to add images
    fix->j=0;
    //play and pause
    fix->i=1;
    fix->population=0;
    //g_source tag
    fix->t=0;
    //element list
    fix->elem=NULL;
    //initialiese the counter
    fix->count=0;
    for (int i = 0; i < Nbcase; ++i) {
        for (int j = 0; j < Nbcase; ++j) {
            fix->Map[i][j]=0;
        }
    }
    return fix;
}
///////////////////////////////////LIST////////////
/////////////////////////////////

void pos(gint *x,gint *y,gint vitesse)
{
    //the step
    if(vitesse==0) return;
    gint a=g_random_int_range(1,5);
    printf("\na=%d\n",a);
    switch (a) {
        case 1:(*x)++;break;
        case 2: (*x)--;break;
        case 3:(*y)++;break;
        default:(*y)--;break;
    }
    //bountries control
    if((*x)<1) (*x)++;
    if((*y)<1) (*y)++;
    if((*x)>(Nbcase-1))(*x)--;
    if((*y)>(Nbcase-1))(*y)--;
    //crossing the boundaries
    if((*x)==(Nbcase-1)) (*x)=0;
    if((*y)==(Nbcase-1)) (*y)=0;
    //tracing coordinates generated
    printf("\nx=%d y=%d\n",*x,*y);

}

gint virus_photo_num(Virus v[4]){
    gint taille=0,i,vir=1;

    for(i=0;i<4;i++){
        if(v[i].id==0) break;
        taille++;
        vir*=v[i].id;
    }
    switch (taille) {
        case 1:return vir;// 1, 2 ,3 ,4
        case 2:
            return vir+20;// 2->22, 3->23, 4->24, 6->26, 8->28, 12->32
        case 3:return vir+30;//6->36 , 8->38, 12->42, 24->54
        default:return 10;// les 4 virus
    }
}
char*photo(Virus v[4])
{
    gchar entier[5];
    gchar *chemin=(gchar*) malloc(30*sizeof (gchar));
   int pt= virus_photo_num(v);
    sprintf(entier,"%d",pt);
                                    printf("\nsprint : %d\n",pt);
    strcpy(chemin,"./images/");
    strcat(chemin,entier);
    strcat(chemin,".png");
    return chemin;
}
/////////////////////////////////////   Ajouter

static void retour(GtkWidget *btn , gpointer elem)
{
    Dliste *L=(Dliste*)elem;
// fermer la premiere fenetre
    gtk_widget_destroy((GtkWidget *) L->Lval);
    L=L->suiv;
    // visioner la fenetre
    gtk_widget_show((GtkWidget *) L->Lval);
}

static void virus_Info(GtkWidget *btn,gpointer liste){
    Dliste*L=(Dliste*)liste;
    Dliste *q=L->suiv;
    etat *etatInd=(etat *)L->Lval;
    gchar* virulance;
    gchar* distance;
    gchar* letalite;
    gchar* contagion;
    virulance=g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    if(virulance){
        if(strcmp(virulance,"faible")==0)  etatInd->virus[0].virulance=1;
        else  if(strcmp(virulance,"modere")==0) etatInd->virus[0].virulance=2;
        else if(strcmp(virulance,"moyenne")==0)  etatInd->virus[0].virulance=3;
        else if(strcmp(virulance,"forte")==0)  etatInd->virus[0].virulance=4;
    }

    q=q->suiv;
    distance=g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    if(distance){
        etatInd->virus[0].distanceCont=atoi(distance);
    }
    q=q->suiv;
    letalite=g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    if(letalite){
        if(strcmp(letalite,"faible")==0) etatInd->virus[0].letalite[0]=1;
        else  if(strcmp(letalite,"modere")==0)  etatInd->virus[0].letalite[0]=2;
        else if(strcmp(letalite,"moyenne")==0)  etatInd->virus[0].letalite[0]=3;
        else if(strcmp(letalite,"forte")==0)  etatInd->virus[0].letalite[0]=4;
    }
    q=q->suiv;
    contagion=g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    if(contagion){
        etatInd->virus[0].contagion=atoi(contagion);
    }
    q=q->suiv;
    gtk_widget_destroy((GtkWidget *) q->Lval);
}
char*set_text(Individu ind,gint x,gint y){
    gchar*text=(gchar*) malloc(700*sizeof (char ));
    gchar element[60];
    strcpy(text,"Genre: ");
    if(ind.genre==1) strcat(text,"Homme \n");
    else strcat(text,"Femme \n");
    strcat(text,"Age: ");
    sprintf(element, "%d\n", ind.age);
    strcat(text,element);
    strcat(text,"Vitesse: ");
    sprintf(element, "%d km/h\n", ind.vitesse);
    strcat(text,element);
    strcat(text,"Bilan de sante: \n");
    strcat(text,"\tGénétiquement: ");
    switch (ind.bl.gn) {
        case 1:strcat(text,"faible \n");break;
        case 2:strcat(text,"fragile \n");break;
        case 3:strcat(text,"moyen \n");break;
        default:strcat(text,"fort \n");break;
    }
    strcat(text,"\tTension artérielle: ");
    switch (ind.bl.ta) {
        case 1:strcat(text,"normale\n");break;
        case 2:strcat(text,"hypertendu\n");break;
        default:strcat(text,"hypertension forte\n");break;
    }
    strcat(text,"\tDiabète: ");
    switch (ind.bl.db) {
        case 1:strcat(text,"normal\n");break;
        case 2:strcat(text,"modéré\n");break;
        default:strcat(text,"avancé\n");break;
    }
    strcat(text,"\tCardiaque: ");
    switch (ind.bl.cr) {
        case 1:strcat(text,"normal\n");break;
        case 2:strcat(text,"malade\n");break;
        default:strcat(text,"sévère\n");break;
    }
    strcat(text,"\tPoumons: ");
    switch (ind.bl.pm) {
        case 1:strcat(text,"sein\n");break;
        case 2:strcat(text,"malade\n");break;
        default:strcat(text,"gravement touché\n");break;
    }
    sprintf(element, "\tPosition: (%d ,%d ) \n",x,y);
    strcat(text,element);
    if(ind.etatInd.sein=='S') strcat(text,"Etat santé: Sein\n");
    else{
        for(int i=0;i<4;i++){
            if(ind.etatInd.virus[i].id==0) break;
            strcat(text,"Virus: ");
            strcat(text,ind.etatInd.virus[i].nomVirus);
            strcat(text,"\n\tvirulance: ");
            switch (ind.etatInd.virus[i].virulance) {
                case 1:strcat(text,"faible\n");break;
                case 2:strcat(text,"modere\n");break;
                case 3:strcat(text,"moyenne\n");break;
                default:strcat(text,"forte\n");break;
            }
            strcat(text,"\tLétalité: ");
            switch (ind.etatInd.virus[i].id) {
                case 1:strcat(text,"moyenne\n");break;
                case 2:strcat(text,"forte\n");break;
                default:strcat(text,"modere\n");break;
            }
            sprintf(element,"\tDistance de contamination:%d m\n", ind.etatInd.virus[i].distanceCont);
            strcat(text,element);
            sprintf(element, "\tContagions: %d %% \n",ind.etatInd.virus[i].contagion);
            strcat(text,element);
        }
    }
    sprintf(element, "PV:  %d %% \n",ind.vivant);
    strcat(text,element);
    return text;
}
///fonction de signal
void ajouter_element( mov*fixe,Individu ind, coordonnees *C){

    printf("\n%d et %d\n",C->x,C->y);
    gchar element[100];
    fixe->elem= inseretDeb(fixe->elem,C->x,C->y,ind);
    fixe->population++;
    sprintf(element,"\t\t\t\t\t\t\t\t\t\t\tPopulation: %d\n", fixe->population);
    gtk_label_set_text((GtkLabel *) fixe->lablePop, element);
    //l'infection ou pas
    if(ind.etatInd.sein=='S')fixe->elem->im=gtk_image_new_from_file("./images/0.png");
    else fixe->elem->im=gtk_image_new_from_file(photo(fixe->elem->ind.etatInd.virus));

    gtk_widget_set_tooltip_text(fixe->elem->im, set_text(fixe->elem->ind,fixe->elem->x,fixe->elem->y));
                                printf("nameOf: %d",ind.etatInd.virus[0].id);
                                printf(" ind SEX:%d\n",ind.genre);
    g_object_set_data((GObject *) fixe->elem->im, "info", fixe->elem);
    if((fixe->population-1)<2000){
        int i=0;
        while (gtk_container_get_children(GTK_CONTAINER(fixe->B[fixe->elem->x][fixe->elem->y]->eventbox.elem))){
            fixe->elem->x= g_random_int_range(C->x,C->x+i+1);
            fixe->elem->y= g_random_int_range(C->y,C->y+i+1);
            i++;
        }
        fixe->Map[fixe->elem->x][fixe->elem->y]=1;
        ajouter_Event_Box(fixe->B[fixe->elem->x][fixe->elem->y]->eventbox.elem,fixe->elem->im);
        gtk_widget_show(fixe->elem->im);
    }
}

gboolean
virusChoix (gpointer user_data)
{
    etat *V=(etat *)user_data;
    gchar malade[20];
    dimension dim5 = {20, 40};
    MonFrame *frame = init_Frame(NULL, walo2(), 0, 10, " PERSONALISER UN VIRUS", 1, init_crd());
    frame = creer_Frame(frame);
    Monlabel *label[4];
    label[0] = init_label("label1", "Virulance", NULL, walo2(), init_crd());
    label[1] = init_label("label2", "Distance de contamination", NULL, walo2(), init_crd()),
            label[2] = init_label("label3", "letalite", NULL, walo2(), init_crd());
    label[3] = init_label("label4", "contagion(%)", NULL, walo2(), init_crd());
    label[0] = creer_label(label[0]);
    label[1] = creer_label(label[1]);
    label[2] = creer_label(label[2]);
    label[3] = creer_label(label[3]);
    MonComboBox *Virus[4];
    Virus[0] = init_comboBox(NULL, "combo virulance", dim5, init_crd(), 1);
    Virus[0] = creer_comboBox(Virus[0]);
    Virus[0] = ajouter_string_comboBox(Virus[0], "faible");
    Virus[0] = ajouter_string_comboBox(Virus[0], "modere");
    Virus[0] = ajouter_string_comboBox(Virus[0], "moyenne");
    Virus[0] = ajouter_string_comboBox(Virus[0], "forte");


    Virus[1] = init_comboBox(NULL, "combo_distance", dim5, init_crd(), 1);
    Virus[1] = creer_comboBox(Virus[1]);
    for (int i = 1; i <= 5; i++)
        Virus[1] = ajouter_number_comboBox(Virus[1], i);

    Virus[2] = init_comboBox(NULL, "letalite", dim5, init_crd(), 1);
    Virus[2] = creer_comboBox(Virus[2]);
    Virus[2] = ajouter_string_comboBox(Virus[2], "faible");
    Virus[2] = ajouter_string_comboBox(Virus[2], "modere");
    Virus[2] = ajouter_string_comboBox(Virus[2], "moyenne");
    Virus[2] = ajouter_string_comboBox(Virus[2], "forte");


    Virus[3] = init_comboBox(NULL, "contagion", dim5, init_crd(), 1);
    Virus[3] = creer_comboBox(Virus[3]);
    for (int i = 1; i <= 10; i++)
        Virus[3] = ajouter_number_comboBox(Virus[3], i * 10);

    MonFrame *frameBox = init_Frame(NULL, walo2(), 0, 10, "Information sur le virus", 1, init_crd());
    frameBox = creer_Frame(frameBox);
    MonBox *box7 = Init_box(frameBox->frame.elem, NULL, 'v', 0, dimAdd(30, 160), 0, 10, NULL, init_crd());
    box7 = creer_Box(box7);
    box7 = ajouter_Box(box7, label[0]->label.elem, 0, 0, 6, 1);
    box7 = ajouter_Box(box7, Virus[0]->combo.elem, 0, 0, 11, 1);
    box7 = ajouter_Box(box7, label[1]->label.elem, 0, 0, 11, 1);
    box7 = ajouter_Box(box7, Virus[1]->combo.elem, 0, 0, 11, 1);
    box7 = ajouter_Box(box7, label[2]->label.elem, 0, 0, 11, 1);
    box7 = ajouter_Box(box7, Virus[2]->combo.elem, 0, 0, 11, 1);
    box7 = ajouter_Box(box7, label[3]->label.elem, 0, 0, 11, 1);
    box7 = ajouter_Box(box7, Virus[3]->combo.elem, 0, 0, 11, 1);
    MonDialog *dialogchoix;
    dialogchoix = init_dialog(NULL, "choix_virus", NULL, 1, walo2(), NULL, init_crd());
    creer_dialog(dialogchoix);
    gtk_window_set_resizable((GtkWindow *) dialogchoix->dialog, FALSE);
    MonBouton *bouton;
    bouton = init_Button(NULL, NULL, NULL, walo2(), " ", NULL, 1, 1, NULL, init_crd());
    bouton->button.elem = ajouter_button_dialog(dialogchoix, "\t\t\t\t\tOK\t\t\t\t\t", 2);

    Dliste *L = cree_Dliste((gpointer *)V);
    L = inserer_fin(L, (gpointer *)Virus[0]->combo.elem);
    L = inserer_fin(L, (gpointer *) Virus[1]->combo.elem);
    L = inserer_fin(L, (gpointer *) Virus[2]->combo.elem);
    L = inserer_fin(L, (gpointer *) Virus[3]->combo.elem);
    L = inserer_fin(L, (gpointer *)  dialogchoix->dialog);
    gtk_combo_box_set_active((GtkComboBox *) Virus[0]->combo.elem, 0);
    gtk_combo_box_set_active((GtkComboBox *) Virus[1]->combo.elem, 0);
    gtk_combo_box_set_active((GtkComboBox *) Virus[2]->combo.elem, 0);
    gtk_combo_box_set_active((GtkComboBox *) Virus[3]->combo.elem, 0);

    ///  ajouter frame
    ajouter_content_area(dialogchoix, frameBox->frame.elem);
    g_signal_connect(bouton->button.elem, "clicked", G_CALLBACK(virus_Info), L);

    Afficher_widget(dialogchoix->dialog);

    return FALSE;
}

/// fonction principal du signal

gboolean fct(GtkWidget *btn,gpointer liste)
{
    Dliste*L=(Dliste*)liste;
    mov *fixe= g_object_get_data((GObject *) L->Lval, "mov");
    Dliste *q=L;
    Individu indiv;
    gchar* valsexe;
    gchar* age;
    gchar* tension;
    gchar* diabetique;
    gchar* cardiaque;
    gchar* genetique;
    gchar* poumon;
    gint vit;

    gchar *malade;
    malade= g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    if(strcmp(malade,"Sein")==0){
        indiv.etatInd.sein='S';
    }
    else if(strcmp(malade,"CoronaVirus")==0){
        indiv.etatInd.virus[0].id=1;
        strcpy(indiv.etatInd.virus[0].nomVirus,"CoronaVirus");
        indiv.etatInd.virus[0].contagion=70;
        indiv.etatInd.virus[0].distanceCont=5;
        indiv.etatInd.virus[0].virulance=3;
        printf("\ncorona: Vnom:%s et Vdist:%d\n\n",indiv.etatInd.virus[0].nomVirus,
               indiv.etatInd.virus[0].distanceCont);

    }else if(strcmp(malade,"EbolaVirus")==0){
        indiv.etatInd.virus[0].id=2;
        strcpy(indiv.etatInd.virus[0].nomVirus,"EbolaVirus");
        indiv.etatInd.virus[0].contagion=45;
        indiv.etatInd.virus[0].distanceCont=4;
        indiv.etatInd. virus[0].virulance=4;
        printf("\nebol\n\n");
    }else if(strcmp(malade,"RotaVirus")==0){
        strcpy(indiv.etatInd.virus[0].nomVirus,"RotaVirus");
        indiv.etatInd.virus[0].contagion=2;
        indiv.etatInd.virus[0].distanceCont=3;
        indiv.etatInd.virus[0].virulance=3;
        indiv.etatInd.virus[0].id=3;
        printf("\nrota\n\n");
    }else{
        virusChoix(q->Lval);
        indiv.etatInd.virus[0].virulance= g_random_int_range(1,5);
        indiv.etatInd.virus[0].distanceCont= g_random_int_range(1,6);
        indiv.etatInd.virus[0].contagion= g_random_int_range(1,10)*10;
        letalite_cat(indiv.etatInd.virus[0].letalite,4);
        strcpy(indiv.etatInd.virus[0].nomVirus,"Autre");
        indiv.etatInd.virus[0].id=4;
    }
    letalite_cat(indiv.etatInd.virus[0].letalite,indiv.etatInd.virus[0].id);
    for(int i=1;i<4;indiv.etatInd.virus[i++].id=0);
    q=q->suiv;
    gpointer G=q->Lval;
    q=q->suiv;
    vit=(gint)gtk_range_get_value(GTK_RANGE(q->Lval));
    indiv.vitesse=vit;
    q=q->suiv;

    valsexe= g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    indiv.genre=0;
    if(valsexe){if(strcmp( valsexe,"HOMME")==0) indiv.genre=1;}

    q=q->suiv;
    age=g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    indiv.age=10;
    if(age)
    {
        if( strcmp(age,"<12")==0) indiv.age= g_random_int_range(1,13);
        else  if( strcmp(age,"[12-25]")==0)indiv.age=g_random_int_range(13,26);
        else if( strcmp(age,"[25-40]")==0) indiv.age=g_random_int_range(26,41);
        else if( strcmp(age,"[40-65]")==0)indiv.age=g_random_int_range(41,66);
        else  indiv.age=g_random_int_range(66,108);// if( strcmp(age,">65")==0)
    }

    q=q->suiv;
    genetique = g_strdup( gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    indiv.bl.gn=1;
    if(genetique)
    {

        if(strcmp(genetique,"faible")==0) indiv.bl.gn=1;
        else  if(strcmp(genetique,"fragile")==0) indiv.bl.gn=2;
        else if(strcmp(genetique,"moyenne")==0) indiv.bl.gn=3;
        else if(strcmp(genetique,"forte")==0) indiv.bl.gn=4;
    }

    q=q->suiv;
    tension =g_strdup( gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    indiv.bl.ta=1;
    if(tension)
    {
        if(strcmp(tension,"normal")==0) indiv.bl.ta=1;
        else if(strcmp(tension,"hypertendu")==0) indiv.bl.ta=2;
        else if(strcmp(tension,"hypertension forte")==0) indiv.bl.ta=3;

    }
    q=q->suiv;
    diabetique = g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    indiv.bl.db=1;
    if(diabetique)
    {
        if(strcmp(diabetique,"normal")==0) indiv.bl.db=1;
        else if(strcmp(diabetique,"modere")==0) indiv.bl.db=2;
        else if(strcmp(diabetique,"severe")==0) indiv.bl.db=3;
    }
    q=q->suiv;

    cardiaque = g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
    indiv.bl.cr=1;
    if(cardiaque)
    {
        if(strcmp(cardiaque,"normal")==0) indiv.bl.cr=1;
        else if(strcmp(cardiaque,"modere")==0) indiv.bl.cr=2;
        else if(strcmp(cardiaque,"severe")==0) indiv.bl.cr=3;
    }
    q=q->suiv;
     poumon = g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(q->Lval)));
     indiv.bl.pm=1;
     if(poumon)
     {
         if(strcmp(poumon,"sein")==0) indiv.bl.pm=1;
         else if(strcmp(poumon,"touche")==0) indiv.bl.pm=2;
         else if(strcmp(poumon,"gravement touche")==0) indiv.bl.pm=3;
     }
    q=q->suiv;
    indiv.vivant=maxV;
    ajouter_element(fixe,indiv, (coordonnees*)g_object_get_data(G,"crdBox"));
    gtk_widget_destroy((GtkWidget *) q->Lval);
    return FALSE;
}


/**    si l utulisateur click sur le bouton play   ***/
static void play(GtkWidget *btn,GdkEventButton *Event,mov*fixe) // gpointer
{
    color_widg couleur3={"white",0.3};
/// frame 2:choix du caracteristique
    MonBouton *bouton7,*bouton8;
    bouton7=init_Button(NULL,NULL,NULL,walo2(),"Sauvegarder",NULL,1,0,&couleur3,init_crd());
    bouton7=creer_Bouton(bouton7);
    bouton8=init_Button(NULL,NULL,NULL,walo2(),"Retour",NULL,1,0,&couleur3,init_crd());
    bouton8=creer_Bouton(bouton8);
    MonFrame*frame2=init_Frame(NULL,walo2(),0,10," ",1,init_crd());
    frame2=creer_Frame(frame2);
///label  ajouter label au frame
    Monlabel*label4=init_label("label4","\tPouvez-vous choisir les caracteristiques d un individu?",frame2->frame.elem,walo2(),init_crd());
    label4=creer_label(label4);


///ajouter frame au fixed
    dimension dim5={20,20},dim6={20,50};
    MonBox *Box[2];
    color_widg col1={"rgb(134,57,34)",0.2};
    Mongrid *grid= init_creer_Grid("grid",NULL,init_crd(),walo2(),&col1);
    Box[0]= Init_box(NULL,"debut",'h',1, dimAdd(200,40),
                     40,30,NULL,init_crd());
    creer_Box(Box[0]);


    Box[1]= Init_box(NULL,"fin",'h',1,walo2(),10,
                     0,NULL,init_crd());
    creer_Box(Box[1]);

    ajouter_Box(Box[0],frame2->frame.elem,1,1,0,1);

    ajouter_Box(Box[1],bouton7->button.elem,1,1,40,1);
    ajouter_Box(Box[1],bouton8->button.elem,1,1,40,1);


/// box qui contient les boutons...
    MonFrame*frameBox[3];

    frameBox[0]=init_Frame(NULL,walo2(),0,10,
                           "Information personelle",1,init_crd());
    frameBox[0]=creer_Frame(frameBox[0]);



    frameBox[2]=init_Frame(NULL,walo2(),0,10,
                           "Bilan de sante",1,init_crd());
    frameBox[2]=creer_Frame(frameBox[2]);
    MonBox*box6=Init_box(frameBox[0]->frame.elem,NULL,
                         'v',0, dimAdd(25,130),0,10,NULL,init_crd()),
            *box8=Init_box(frameBox[2]->frame.elem,NULL,
                           'v',0,dimAdd(25,160),0,10,NULL,init_crd());
    box6=creer_Box(box6);
    box8=creer_Box(box8);
    MonBox *boxGeneral= Init_box(NULL,"box des box",
                                 'h',1,walo2(),0,5,NULL,init_crd());
    creer_Box(boxGeneral);



    ajouter_Box(boxGeneral,frameBox[0]->frame.elem,1,1,0,1);
    ajouter_Box(boxGeneral,frameBox[2]->frame.elem,1,1,0,1);

    ajouter_Grid_pos(grid,boxGeneral->box.elem, crdAdd(1,1), dimAdd(70,1));
    ajouter_Grid_cote_de(grid,Box[1]->box.elem,boxGeneral->box.elem,GTK_POS_BOTTOM, dimAdd(1,1));
    ajouter_Grid_cote_de(grid,Box[0]->box.elem,boxGeneral->box.elem,GTK_POS_TOP, dimAdd(1,1));


///declarer les  labels et les comboboxs
    Monlabel*label5=init_label("label5","Sexe",NULL,walo2(),init_crd()),
            *label6=init_label("label6","Age",NULL,walo2(),init_crd()),
            *label7=init_label("label7","Vitesse",NULL,walo2(),init_crd()) ,
            *label8=init_label("label8","Etat",NULL,walo2(),init_crd()) ,
            *label9=init_label("label9","Poumons",NULL,walo2(),init_crd()) ,
            *label14=init_label("label14","Genetiquement",NULL,walo2(),init_crd()),
            *label15=init_label("label15","Tension arterielle",NULL,walo2(),init_crd()),
            *label16=init_label("label16","Diabetique",NULL,walo2(),init_crd()),
            *label17=init_label("label17","Cardiaque",NULL,walo2(),init_crd());
    label5=creer_label(label5);
    label6=creer_label(label6);
    label7=creer_label(label7);
    label8=creer_label(label8);
    label9=creer_label(label9);
    label14=creer_label(label14);
    label15=creer_label(label15);
    label16=creer_label(label16);
    label17=creer_label(label17);

    MonComboBox*combo[7];
    combo[0]=init_comboBox(NULL,"combo_sexe",dim5,init_crd(),1);
    combo[0]=creer_comboBox(combo[0]);
    combo[0]=ajouter_string_comboBox(combo[0],"HOMME");
    combo[0]=ajouter_string_comboBox(combo[0],"FEMME");
    gtk_combo_box_set_active((GtkComboBox *) combo[0]->combo.elem, 0);
    combo[1]=init_comboBox(NULL,"combo_age",dim5,init_crd(),1);
    combo[1]=creer_comboBox(combo[1]);
    combo[1]=ajouter_string_comboBox(combo[1],"<12");
    combo[1]=ajouter_string_comboBox(combo[1],"[12-25]");
    combo[1]=ajouter_string_comboBox(combo[1],"[25-40]");
    combo[1]=ajouter_string_comboBox(combo[1],"[40-65]");
    combo[1]=ajouter_string_comboBox(combo[1],">65");
    gtk_combo_box_set_active((GtkComboBox *) combo[1]->combo.elem, 0);

    MonComboBox *Virus=init_comboBox(NULL,"combo_age",dim5,init_crd(),1);
    Virus=creer_comboBox(Virus);
    Virus=ajouter_string_comboBox(Virus,"Sein");
    Virus=ajouter_string_comboBox(Virus,"CoronaVirus");
    Virus=ajouter_string_comboBox(Virus,"EbolaVirus");
    Virus=ajouter_string_comboBox(Virus,"RotaVirus");
    Virus=ajouter_string_comboBox(Virus,"Autre");
    gtk_combo_box_set_active((GtkComboBox *) Virus->combo.elem, 0);

    combo[2]=init_comboBox(NULL,"genetiquement",dim5,init_crd(),1);
    combo[2]=creer_comboBox(combo[2]);
    combo[2]=ajouter_string_comboBox(combo[2],"faible");
    combo[2]=ajouter_string_comboBox(combo[2],"fragile");
    combo[2]=ajouter_string_comboBox(combo[2],"moyenne");
    combo[2]=ajouter_string_comboBox(combo[2],"forte");
    gtk_combo_box_set_active((GtkComboBox *) combo[2]->combo.elem, 0);

    combo[3]=init_comboBox(NULL,"Tension",dim5,init_crd(),1);
    combo[3]=creer_comboBox( combo[3]);
    combo[3]=ajouter_string_comboBox( combo[3],"normal");
    combo[3]=ajouter_string_comboBox( combo[3],"hypertendu");
    combo[3]=ajouter_string_comboBox( combo[3],"hypertension forte");
    gtk_combo_box_set_active((GtkComboBox *) combo[3]->combo.elem, 0);

    combo[4]=init_comboBox(NULL,"Diabetique",dim5,init_crd(),1);
    combo[4]=creer_comboBox(combo[4]);
    combo[4]=ajouter_string_comboBox(combo[4],"normal");
    combo[4]=ajouter_string_comboBox(combo[4],"modere");
    combo[4]=ajouter_string_comboBox(combo[4],"severe");
    gtk_combo_box_set_active((GtkComboBox *) combo[4]->combo.elem, 0);

    combo[5]=init_comboBox(NULL,"Cardiaque",dim5,init_crd(),1);
    combo[5]=creer_comboBox(combo[5]);
    combo[5]=ajouter_string_comboBox(combo[5],"normal");
    combo[5]=ajouter_string_comboBox(combo[5],"modere");
    combo[5]=ajouter_string_comboBox(combo[5],"severe");
    gtk_combo_box_set_active((GtkComboBox *) combo[5]->combo.elem, 0);
    combo[6]=init_comboBox(NULL,"Poumon",dim5,init_crd(),1);
    combo[6]=creer_comboBox(combo[6]);
    combo[6]=ajouter_string_comboBox(combo[6],"sein");
    combo[6]=ajouter_string_comboBox(combo[6],"touche");
    combo[6]=ajouter_string_comboBox(combo[6],"gravement touche");
    gtk_combo_box_set_active((GtkComboBox *) combo[6]->combo.elem, 0);
///scale pour la vitesse
    MonScale*scale1=init_scale(NULL,"scale1",0,3,1.0,
                               5.0,1.0,0,init_crd(),dim6);
    scale1=creer_scale(scale1);


    box6=ajouter_Box(box6,label5->label.elem,0,0,5,1);
    box6=ajouter_Box(box6,combo[0]->combo.elem,0,0,5,1);
    box6=ajouter_Box(box6,label6->label.elem,0,0,5,1);
    box6=ajouter_Box(box6,combo[1]->combo.elem,0,0,5,1);
    box6=ajouter_Box(box6,label7->label.elem,0,0,5,1);
    box6=ajouter_Box(box6,scale1->scale.elem,0,0,5,1);
    box6=ajouter_Box(box6,label8->label.elem,0,0,5,1);
    box6=ajouter_Box(box6,Virus->combo.elem,0,0,10,1);



    box8=ajouter_Box(box8,label14->label.elem,0,0,6,1);
    box8=ajouter_Box(box8,combo[2]->combo.elem,0,0,6,1);
    box8=ajouter_Box(box8,label15->label.elem,0,0,6,1);
    box8=ajouter_Box(box8,combo[3]->combo.elem,0,0,6,1);
    box8=ajouter_Box(box8,label16->label.elem,0,0,6,1);
    box8=ajouter_Box(box8,combo[4]->combo.elem,0,0,6,1);
    box8=ajouter_Box(box8,label17->label.elem,0,0,6,1);
    box8=ajouter_Box(box8,combo[5]->combo.elem,0,0,6,1);
    box8=ajouter_Box(box8,label9->label.elem,0,0,6,1);
    box8=ajouter_Box(box8,combo[6]->combo.elem,0,0,6,1);

    POS P;
    P.cord= crdAdd(1200,0);//wind
    Fenetre *seconWind= init_window("Choix individu",P,walo2(),0,NULL,NULL,0);
    creer_Window(seconWind);
    gtk_container_add((GtkContainer *) seconWind->window, grid->Grid.elem);
    Afficher_widget(seconWind->window);

    Dliste *L=cree_Dliste((gpointer *) Virus->combo.elem);
    g_object_set_data((GObject *) Virus->combo.elem, "mov", fixe);

    L=inserer_fin(L, (gpointer *)btn);
    L=inserer_fin(L, (gpointer *) scale1->scale.elem);
    L=inserer_fin(L, (gpointer *) combo[0]->combo.elem);
    L=inserer_fin(L, (gpointer *) combo[1]->combo.elem);
    L=inserer_fin(L, (gpointer *) combo[2]->combo.elem);
    L=inserer_fin(L, (gpointer *) combo[3]->combo.elem);
    L=inserer_fin(L, (gpointer *) combo[4]->combo.elem);
    L=inserer_fin(L, (gpointer *) combo[5]->combo.elem);
    L=inserer_fin(L, (gpointer *) combo[6]->combo.elem);
    L=inserer_fin(L, (gpointer *) seconWind->window);

    g_signal_connect(bouton7->button.elem, "clicked", G_CALLBACK(fct), L);
    g_signal_connect_swapped(bouton8->button.elem,"clicked",G_CALLBACK(gtk_widget_destroy),seconWind->window);
}
//Mortalite-------------
void mortalite(gdouble mort, Individu* ind){
    gint calcule;
    if(ind->vitesse==0) return;
    if(ind->vivant<15){
        ind->vivant=14;
        ind->vitesse=0;
    }else{
        calcule=maxV-ind->vivant;//TODO::hna
        calcule=((gint)mort)-calcule;
        if(calcule>0) ind->vivant-=calcule;
    }
}
//infecter ----------------
void Infecter(gint x,gint y,Virus virs,mov *fixe)
{
    coordonnees C={x,y};
    GtkWidget *im;
    listObj *indiv=NULL;
    GList *element=NULL;
    cellulePliste *L;
    L=listeEntour(fixe->Map,virs.distanceCont,C);
    printf("\nha ana x=%d y=%d\n",C.x,C.y);
    afficher_pliste(L);
    while(L){
        if(L->val.x>=0&&L->val.y>=0){
            element=gtk_container_get_children((GtkContainer *) fixe->B[L->val.x][L->val.y]->eventbox.elem);
            if(element){
                im= GTK_WIDGET(element->data);
                indiv=(listObj*)(g_object_get_data((GObject *) im, "info"));
                if(indiv->ind.etatInd.sein=='S'){
                    for(int j=0;j<4;j++) {
                        indiv->ind.etatInd.virus[j].id=0;
                    }
                    indiv->ind.etatInd.virus[0]=virs;
                    gtk_image_clear((GtkImage *) indiv->im);
                    gtk_image_set_from_file((GtkImage *) indiv->im, photo(indiv->ind.etatInd.virus));
                }else{
                    for(int i=0;i<4;i++){
                        if(indiv->ind.etatInd.virus[i].id==virs.id)break;
                        if(indiv->ind.etatInd.virus[i].id==0){
                            indiv->ind.etatInd.virus[i]=virs;
                            gtk_image_clear((GtkImage *) indiv->im);
                            gtk_image_set_from_file((GtkImage *) indiv->im, photo(indiv->ind.etatInd.virus));
                            break;
                        }
                    }
                }

            }
        }

        L=L->suivant;
    }
}
int fct_hosp(coordonnees crd)
{
    coordonnees tab[5]={{2,0},{14,24},{50,0},{44,22},{42,48}};
    int i;
    for(i=0;i<5;i++)
    {
        if((tab[i].x==crd.x && tab[i].y==crd.y  ) || ((tab[i].x+1==crd.x) && (tab[i].y==crd.y) ))
            return 1;
        if((crd.x<=tab[i].x+3) && (crd.x>=tab[i].x-2)&&(crd.y<=tab[i].y+3 )&& (crd.y>=tab[i].y+1))
            return 1;
    }
    return 0;
}
gint deplacer(mov*fixe )
{

    //update the counter
    fixe->count++;
    listObj *L=fixe->elem;
    coordonnees D;
    gchar element[100];
    while (L)
    {
        printf("\nvivant ======= %d\n",L->ind.vivant);
        printf("\n %d t %d\n",L->x,L->y);
        D.x=L->x;D.y=L->y;
        //individu mort et disparu <10
        if(L->ind.vivant<15)//TODO:: let me check
            {
                fixe->Map[L->x][L->y]=0;
                if(L->ind.vivant>=1){
                    gtk_image_clear((GtkImage *) L->im);
                    L->ind.vivant--;
                    gtk_image_set_from_file((GtkImage *) L->im, "./images/dead.png");
                }else{
                    fixe->population--;
                    sprintf(element,"\t\t\t\t\t\t\t\t\t\t\tPopulation: %d\n", fixe->population);
                    gtk_label_set_text((GtkLabel *) fixe->lablePop, element);
                    gtk_widget_destroy(L->im);
                    L=L->prev;
                    fixe->elem=supp(fixe->elem, crdAdd(D.x,D.y));
                }
            }
        else{// vivant
            if(L->ind.etatInd.sein!='S'){//malade
                for(int i=0;i<4;i++){
                    if(L->ind.etatInd.virus[i].id==0) break;
                    Infecter(L->x,L->y,L->ind.etatInd.virus[i],fixe);//infecter les autres
                }
                if(L->ind.vitesse)//s'il n'est pas mort
                    mortalite(dead_or_not(L->ind),&L->ind);
                L->ind.vivant--;
                if(fct_hosp(crdAdd(L->x,L->y))&&(L->ind.vivant>25)){//dans un hopital
                    L->ind.etatInd.sein='S';
                    gtk_image_clear((GtkImage *) L->im);
                    gtk_image_set_from_file((GtkImage *) L->im, "./images/0.png");
                }

            }
            pos(&(D.x),&( D.y),L->ind.vitesse);
            if (!gtk_container_get_children(GTK_CONTAINER(fixe->B[D.x][D.y]->eventbox.elem)))
            {
                g_object_ref(L->im);
                fixe->Map[L->x][L->y]=0;
                fixe->Map[D.x][D.y]=1;
                gtk_container_remove(GTK_CONTAINER(fixe->B[L->x][L->y]->eventbox.elem), L->im);
                L->x = D.x;
                L->y = D.y;
                gtk_widget_set_tooltip_text(L->im, set_text(L->ind,L->x,L->y));
                ajouter_Event_Box(fixe->B[L->x][L->y]->eventbox.elem, L->im);
            }
        }
        if(L)
            L=L->next;
        else break;
    }
    if(!fixe->elem) printf("tous est mort\n");
    //if counter is less than 10000ms return true
    return (fixe->count<10000);
}
gboolean on_update (GtkWidget * but, gpointer *data)
{
    mov*fixe=(mov*)data;
    GtkWidget *pp=gtk_image_new_from_file("./images/play.png");
    if(fixe->i==1)//if i=1 then call deplacer() to move the objects
    {
        gtk_image_clear((GtkImage *) pp);
       pp= gtk_image_new_from_file("./images/pause.png");
        gtk_button_set_image((GtkButton *) but,pp);

        fixe->i=0;
        //call deplacer every 120ms to move the objects,
        //note that while t=true deplacer() will be called every 10ms
        fixe->t=g_timeout_add(120, G_SOURCE_FUNC(deplacer), fixe);

    }
    else //if i=0 stop g_timeout_add()//if(fixe->i==0)
    {
        gtk_button_set_image((GtkButton *) but,pp);
        fixe->i=1;//update i
        //this will give t the false value and stop g_timeout_add()
        g_source_remove(fixe->t);
    }
    return FALSE;
}


gboolean sauvegardage(char *text,mov *inter)
{

    listObj *tmp = inter->elem;
    FILE *Mon_fichier ;
    //Ouverture du fichier
    Mon_fichier=fopen(text,"w");
    if(!Mon_fichier) exit(-1);
    if(tmp==NULL) fprintf(Mon_fichier,"1\n");
    else {
        fprintf(Mon_fichier, "0\n");
        while (tmp) {
            fprintf(Mon_fichier, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                    tmp->ind.genre, tmp->ind.age, tmp->ind.vitesse, tmp->ind.bl.gn, tmp->ind.bl.cr, tmp->ind.bl.db,
                    tmp->ind.bl.ta, tmp->ind.bl.pm, tmp->ind.vivant, tmp->x, tmp->y);
            if (tmp->ind.etatInd.sein == 'S') fprintf(Mon_fichier, "1\n");
            else {
                fprintf(Mon_fichier, "%d\n", 0);
                for (int i = 0; i < 4; i++) {
                    if (tmp->ind.etatInd.virus[i].id == 0) { fprintf(Mon_fichier, "0\t0\t0\t0\n"); }
                    else {
                        fprintf(Mon_fichier, "%d\t%d\t%d\t%d\n", tmp->ind.etatInd.virus[i].id,
                                tmp->ind.etatInd.virus[i].virulance,
                                tmp->ind.etatInd.virus[i].distanceCont, tmp->ind.etatInd.virus[i].contagion);
                    }
                }
            }
            tmp = tmp->next;
        }
    }
    fclose(Mon_fichier); // Fermeture du fichier
    return FALSE;
}
void virusnom(gchar*nom,gint id){
    switch (id) {
        case 1:
            strcpy(nom,"CoronaVirus");break;
        case 2:
            strcpy(nom,"EbolaVirus");break;
        case 3:
            strcpy(nom,"RotaVirus");break;
        default:
            strcpy(nom,"Autre");break;
    }
}
gboolean recuperer_data(mov *inter,char*text)
{
    Individu ind;
    FILE *Mon_fichier ;
    //Ouverture du fichier
    Mon_fichier=fopen(text,"r");
    if(!Mon_fichier){
        Mon_fichier=fopen("vide.txt","r");
    }

    /*- Tant que la fin du fichier texte n'est pas atteint */
    int state;
    gint vide;
    coordonnees *C= (coordonnees*)malloc(sizeof (coordonnees));
    fscanf(Mon_fichier,"%d\n",&vide);
    if(!vide)
    {

        while( !feof(Mon_fichier))
        {
            fscanf(Mon_fichier,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                   &ind.genre,&ind.age,&ind.vitesse,&ind.bl.gn,&ind.bl.cr,&ind.bl.db,
                   &ind.bl.ta,&ind.bl.pm,&ind.vivant,&C->x,&C->y);
            fscanf(Mon_fichier,"%d\n",&state);
            if(state==1) ind.etatInd.sein='S';
            else{
                for(int i=0;i<4;i++){
                    fscanf(Mon_fichier,"%d\t%d\t%d\t%d\n",&ind.etatInd.virus[i].id,
                           &ind.etatInd.virus[i].virulance,&ind.etatInd.virus[i].distanceCont,
                           &ind.etatInd.virus[i].contagion);
                    virusnom(ind.etatInd.virus[i].nomVirus,ind.etatInd.virus[i].id);
                    letalite_cat(ind.etatInd.virus[i].letalite,ind.etatInd.virus[i].id);
                }

            }
            printf("%d hahaha %d\n",C->x,C->y);
            ajouter_element(inter,ind,C);
        }
    }
    fclose(Mon_fichier); // Fermeture du fichier
    free(C);
    return FALSE;
}
int EndsWithTxt(gchar*test){
    gint i= (int)strlen(test);
    gint j;
    if(test[i-4]=='.'&&test[i-3]=='t'&&test[i-2]=='x'&&test[i-1]=='t')return 1;
    return 0;
}
void recup(mov *inter,GtkWidget *wind)
{
    GtkWidget* F1 = gtk_file_chooser_dialog_new("Ouvrir", (GtkWindow *) wind
            , GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_OPEN, GTK_RESPONSE_OK
            , GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

    GtkFileFilter *filter;
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter , ".txt");
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(F1), filter);
    gtk_file_chooser_set_current_folder((GtkFileChooser *) F1, "./sauvegarder/");

    gtk_widget_show_all(F1);
    gint resp = gtk_dialog_run((GtkDialog *) F1);

    if(resp == GTK_RESPONSE_OK)
    {
        char test[250];
        strcpy(test,gtk_file_chooser_get_filename((GtkFileChooser *) F1));
        recuperer_data(inter,test);
    }
    gtk_widget_destroy(F1);
}
void Save_clbk(GtkWidget btn,gpointer data)
{   mov* inter=(mov*)data;
    GtkWidget* F1 = gtk_file_chooser_dialog_new("Enregistrer",NULL
            ,GTK_FILE_CHOOSER_ACTION_SAVE,GTK_STOCK_SAVE,GTK_RESPONSE_OK
            ,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,NULL);

    GtkFileFilter *filter;
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter , ".txt");
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(F1), filter);
    gtk_file_chooser_set_current_folder((GtkFileChooser *) F1, "./sauvegarder/");

    gtk_widget_show_all(F1);
    gint resp = gtk_dialog_run((GtkDialog *) F1);

    if(resp == GTK_RESPONSE_OK)
    {
        char test[250];
        strcpy(test,gtk_file_chooser_get_filename((GtkFileChooser *) F1));
        if(!EndsWithTxt(test))// strrchr()
            strcat(test,".txt");
        sauvegardage(test,inter);
    }

    gtk_widget_destroy(F1);
}


gboolean retourMain(GtkWidget*btn,gpointer data){
    gtk_widget_show_all(GTK_WIDGET(data));
    gtk_widget_destroy(pere(pere(pere(btn))));
    return FALSE;
}
typedef struct {
    int width;
    int height;
    gchar  text[60];
    gdouble percent;
    gint nbr;
    gdouble red;
    gdouble green;
    gdouble blue;

}RectStat;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,gpointer user_data)
{
    mov *fixe=g_object_get_data(G_OBJECT(widget),"mov");
    listObj *tmp=fixe->elem;
    int tab[5]={0,0,0,0,0};
    RectStat  StatVirus[5]={
            {0,0,"Sein :           ",0,0,0,0.5,0},
            {0,0,"Coronavirus :",0,0,1,0,0},
            {0,0,"EbolaVirus : ",0,0,1,1,0},
            {0,0,"RotaVirus :  ",0,0,204.0/255.0,51.0/255.0,1},
            {0,0,"Autres :     ",0,0,0,0.5,1},

    };
    while(tmp)
    {
        if(tmp->ind.etatInd.sein=='S') tab[0]++;
        else{
            for(int i=0;i<4;i++)
            {
                if (tmp->ind.etatInd.virus[i].id==0) break;
                else tab[tmp->ind.etatInd.virus[i].id]++;
            }
        }
        tmp=tmp->next;
    }
    for (int i=0;i<5;i++)
    {
        StatVirus[i].percent=(gdouble)((tab[i]*100.0)/(gdouble)fixe->population);
        StatVirus[i].width=(((tab[i]*100)/fixe->population)*200)/100;
        StatVirus[i].height=20;
        StatVirus[i].nbr=tab[i];
        gchar *nbrpercent = g_strdup_printf("%2.f%% ----> ", StatVirus[i].percent);
        strcat(StatVirus[i].text,nbrpercent);
        gchar *nbr = g_strdup_printf("%d\n", tab[i]);
        strcat(StatVirus[i].text,nbr);
        strcat(StatVirus[i].text," individus ");
    }
    gdouble high=10;
    for(int i=0;i<5;i++)
    {
        if (StatVirus[i].nbr>=0)
        {
            high+=20;
            cairo_set_source_rgb(cr, 0, 0, 0);
            cairo_select_font_face(cr, "Poppins", CAIRO_FONT_SLANT_NORMAL,
                                   CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, 14.0);
            cairo_move_to(cr, 10.0, high);
            cairo_show_text(cr, StatVirus[i].text);
            high+=10;
            cairo_rectangle(cr, 10, high, StatVirus[i].width, StatVirus[i].height);     /* set rectangle */
            cairo_set_source_rgb(cr, StatVirus[i].red, StatVirus[i].green, StatVirus[i].blue);   /* set fill color */
            cairo_fill(cr);                                            /* fill rectangle */
            high+=30;
        }

    }
    return FALSE;
}

gboolean finish(GtkWidget *widget,gpointer data){
    int*j=(int*)data;
    (*j)=1;
    gtk_widget_destroy(widget);
    return FALSE;
}
gboolean state_func(GtkWidget *widget,gpointer data){
    GtkWidget*da;
    POS P;
    P.cord= crdAdd(0,0);
    color_widg Col={"rgb(255, 230, 230)",1};

    Fenetre *F= init_window("statistiques",P, dimAdd(240,330),
                            1,NULL,&Col,0);
    creer_Window(F);
    mov *fixe=g_object_get_data(G_OBJECT(widget),"mov");
    if(fixe->j==1){
        fixe->j=0;
        da=gtk_drawing_area_new();
        gtk_container_add(GTK_CONTAINER(F->window),da);
        g_object_set_data(G_OBJECT(da),"mov",fixe);
        gtk_widget_show_all(F->window);
        g_signal_connect (G_OBJECT(F->window), "destroy",
                          G_CALLBACK(finish), &fixe->j);
        g_signal_connect (G_OBJECT(da), "draw",G_CALLBACK(on_draw_event), NULL);
    }
    return FALSE;
}
void deplacementEspace(GtkWidget*but,gpointer data){
    dimension dim={700,600}, D={24,24};
    GtkWidget *per=pere(pere(pere(pere(but))));
    gtk_widget_hide(per);
    GtkWidget *btn,*exit,*statis,*lable;
    MonBouton *update;
    MonBox *boxGeneral,*box1, *boxScroll;
    MonScroll *scroll;
    gint i, j,state=1;
    color_widg Col={"rgb(55,55,255)",0.2};
    mov *inter=initmov();
    POS P;P.cord= crdAdd(250,0);
    Fenetre *F= init_window("Fenetre de Simulation",P,
                            dimAdd(800,700),1,NULL,NULL,0);
    creer_Window(F);
    g_signal_connect (G_OBJECT (F->window), "destroy",
                      G_CALLBACK (gtk_main_quit), NULL);

    inter->grid=gtk_grid_new();
    add_bgcolor(inter->grid,"rgb(255,255,255)",1);
    gtk_widget_set_size_request(inter->grid,1000,800);
   //gtk_grid_set_row_spacing(GTK_GRID(inter->grid), 1);
  // gtk_grid_set_column_spacing(GTK_GRID(inter->grid), 1);

    ///Pack the grid  with 100 event box.
    for (i = 0; i < Nbcase; i++)
    {
        for (j = 0; j < Nbcase; j++)
        {
            inter->B[i][j]=Init_Event_box(inter->grid,D,&Col, crdAdd(i,j));
            inter->B[i][j]= creer_Event_Box(inter->B[i][j],0,i,j);
            g_object_set_data((GObject*)inter->B[i][j]->eventbox.elem,"crdBox",&inter->B[i][j]->Crd);
            gtk_widget_add_events(inter->B[i][j]->eventbox.elem, GDK_BUTTON_PRESS_MASK);
            g_signal_connect(inter->B[i][j]->eventbox.elem, "button_press_event", G_CALLBACK(play), (gpointer *)inter);
        }
    }

    boxGeneral= Init_box(F->window,"boxGen",'v',0,
                                 dimAdd(600,700),0,0,NULL,init_crd());
    creer_Box(boxGeneral);
    box1= Init_box(NULL,"box1",'h',0,
                         walo2(),25,0,NULL,init_crd());
    creer_Box(box1);
    update= init_Button(NULL,"./images/play.png",NULL,walo2(),NULL,NULL,1,0,NULL,init_crd());
    creer_Bouton(update);
    gtk_button_set_relief((GtkButton *)update->button.elem, GTK_RELIEF_NONE);
    ajouter_Box(box1,update->button.elem,0,0,0,1);
    btn=gtk_button_new_from_icon_name("media-floppy",GTK_ICON_SIZE_DND);
    gtk_button_set_relief((GtkButton *)btn, GTK_RELIEF_NONE);
    ajouter_Box(box1,btn,0,0,0,1);
    statis=gtk_button_new_from_icon_name("x-office-presentation",GTK_ICON_SIZE_DND);
    gtk_button_set_relief((GtkButton *)statis, GTK_RELIEF_NONE);
    ajouter_Box(box1,statis,0,0,0,1);

    exit=gtk_button_new_from_icon_name("edit-undo-symbolic",GTK_ICON_SIZE_DND);
    gtk_button_set_relief((GtkButton *)exit, GTK_RELIEF_NONE);
    ajouter_Box(box1,exit,0,0,0,1);

    inter->lablePop= gtk_label_new("\t\t\t\t\t\t\t\t\t\t\tPopulation:0 ");
    ajouter_Box(box1,inter->lablePop,0,0,0,1);

    ajouter_Box(boxGeneral,box1->box.elem,0,0,0,1);


    boxScroll= Init_box(NULL,"boxGen",'v',0,
                         walo2(),10,0,NULL,init_crd());
    creer_Box(boxScroll);
    //box general

    ///fixed
    MonImage *hp[5];
    Monfixed *fix=init_Fixed(D,"fixed pricipale",NULL,init_crd());
    fix=creer_Fixed(fix);
    ajouter_Fixed(fix,inter->grid, crdAdd(0,0));
    scroll= init_Scroll(boxScroll->box.elem,fix->fix.elem,0,dim);
    hp[0]= init_image("./images/sbitar.png","b", crdAdd(1,0),NULL);
    creer_image(hp[0]);
    ajouter_Fixed(fix,  hp[0]->image.elem,   hp[0]->ImCrd);
    hp[1]= init_image("./images/sbitar.png","b", crdAdd(300,600),NULL);
    creer_image(hp[1]);
    ajouter_Fixed(fix,  hp[1]->image.elem,   hp[1]->ImCrd);
    hp[2]= init_image("./images/sbitar.png","b", crdAdd(1050,550),NULL);
    creer_image(hp[2]);
    ajouter_Fixed(fix,  hp[2]->image.elem,   hp[2]->ImCrd);
    hp[3]= init_image("./images/sbitar.png","b", crdAdd(1200,0),NULL);
    creer_image(hp[3]);
    ajouter_Fixed(fix,  hp[3]->image.elem,   hp[3]->ImCrd);
    hp[4]= init_image("./images/sbitar.png","b", crdAdd(1000,1200),NULL);
    creer_image(hp[4]);
    ajouter_Fixed(fix,  hp[4]->image.elem,   hp[4]->ImCrd);

    creer_Scroll(scroll);
    ajouter_Box(boxGeneral,boxScroll->box.elem,1,1,0,1);

    if(data) recup(inter,F->window);
    else{
        recuperer_data(inter,"default.txt");
    }
    inter->j=1;
    g_object_set_data(G_OBJECT(statis),"mov",inter);
    g_signal_connect(statis, "clicked", G_CALLBACK(state_func), (gpointer *) &state);
    g_signal_connect(update->button.elem, "clicked", G_CALLBACK(on_update),(gpointer *) inter);
    g_signal_connect(btn, "clicked", G_CALLBACK(Save_clbk),(gpointer *) inter);
    g_signal_connect(exit, "clicked", G_CALLBACK(retourMain),(gpointer *) per);
    gtk_widget_show_all (F->window);

}

