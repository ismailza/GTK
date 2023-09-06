#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <malloc.h>

GtkWidget *im = NULL;
GtkWidget *im1 = NULL;

typedef struct cr
{
    gchar *info;
    int Idelem;
}info;

info *creer_initialiser_info()
{
    info *in;
    in=(info*)malloc(sizeof (info));
    in->Idelem=-1;
    return (info*)in;
}

void remplir_info(info *in,gchar *info_1)
{
    in->info=g_strdup(info_1);
    in->Idelem=5;
}

typedef struct win
{
    GtkWidget *win;
    info *back;//backgrround
    GtkWindowType type_win;
    info *title;            //gtk_window_set_title ()
    gboolean resiasable;    //gtk_window_set_resizable()
    gboolean modal;         //gtk_window_set_modal ()
    int default_size[2];    //gtk_window_set_default_size ()
    int request_size[2];    //gtk_widget_set_size_request()
    info *icon;             //gtk_window_set_icon_from_file ()
    double opacity;         //gtk_widget_set_opacity()
    gboolean fullscreen;    //gtk_window_fullscreen ()
    gboolean keep_below;    //gtk_window_set_keep_below()
    //gtk_window_set_titlebar()
    //gtk_window_set_position()
}window;

void epurer_blan(FILE *f1)
{
    char c=(char) fgetc(f1);
    while (c==' ')c=(char) fgetc(f1);
    ungetc(c,f1);
}


typedef struct iii
{
    info *attribut;
    info *valeur;
    struct iii *suiv;
}obj_att;


obj_att *creer_initialiser_obj_att()
{
    obj_att *ne;
    ne=(obj_att*)malloc(sizeof (obj_att));
    ne->valeur=creer_initialiser_info();
    ne->attribut=creer_initialiser_info();
    ne->suiv=NULL;
    return (obj_att*)ne;
}

obj_att *inserer_obj_att(gchar *att,gchar *val,obj_att *info)
{
    obj_att *ne=creer_initialiser_obj_att();
    remplir_info(ne->attribut,att);
    remplir_info(ne->valeur,val);
    if(!info) return (obj_att*)ne;
    ne->suiv=info;
    info=ne;
    return (obj_att*)info;
}

void str_to_val1_val2(info *in,int *val1,int *val2)
{
    int i=0;
    int val=0,k;
    while (in->info[i]!='\0')
    {
        if(in->info[i]=='|')
        {
            *val1=val;
            val=0;
        }
        else {
            k = (int) in->info[i] - (int) '0';
            val = val * 10 + k;
        }
        i++;
    }
    *val2=val;
}

window *creer_initialiser_win()
{
    window *win;
    win=(window*)malloc(sizeof (window));
    if (!win) { printf("\n Erreur d'allocation"); exit(0); }

    win->title=creer_initialiser_info();
    remplir_info(win->title,"default");
    win->icon=creer_initialiser_info();
    win->back=creer_initialiser_info();

    win->resiasable=TRUE;
    win->modal=FALSE;
    win->type_win=GTK_WINDOW_TOPLEVEL;
    win->default_size[0]=100;
    win->default_size[1]=100;
    win->request_size[0]=-1;
    win->request_size[1]=-1;

    win->opacity=1;
    win->fullscreen=FALSE;
    win->keep_below=FALSE;
    win->win=NULL;
    return (window*)win;
}

obj_att *lire_att(FILE *f1)
{
    char c;
    obj_att *liste=NULL;
    gchar mot[50] = {0};
    gchar mot1[50]= {0};
    c=(char)fgetc(f1);
    if(c=='>')
    {
        fscanf(f1,"\n");
        epurer_blan(f1);
        return (obj_att*)NULL;
    }
    do{
        ungetc(c,f1);
        fscanf(f1,"%50[^=]=%50[^,],",mot,mot1);
        liste=inserer_obj_att(mot,mot1,liste);
        c=(char)fgetc(f1);
    } while (c!='>');
    fscanf(f1,"\n");
    epurer_blan(f1);
    return (obj_att*)liste;
}



window *remplir_window(FILE *f1)
{
    float f;
    char c;
    int val1,val2;
    obj_att *liste=NULL,*p=NULL;
    window *wn_info=creer_initialiser_win();
    liste= lire_att(f1);
    if(!liste)return (window*)wn_info;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"typewin")==0)
        {
            if(strcmp(p->valeur->info,"GTK_WINDOW_TOPLEVEL")==0)wn_info->type_win=GTK_WINDOW_TOPLEVEL;
            else if(strcmp(p->valeur->info,"GTK_WINDOW_POPUP")==0)wn_info->type_win=GTK_WINDOW_POPUP;
        }else if(strcmp(p->attribut->info,"title")==0)
            remplir_info(wn_info->title,p->valeur->info);
        else if(strcmp(p->attribut->info,"resia")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)wn_info->resiasable=TRUE;
            else if(strcmp(p->valeur->info,"FALSE")==0)wn_info->resiasable=FALSE;
        }else if(strcmp(p->attribut->info,"modal")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)wn_info->modal=TRUE;
            else wn_info->modal=FALSE;
        }else if(strcmp(p->attribut->info,"dfs")==0)
        {
            str_to_val1_val2(p->valeur,&val1,&val2);
            wn_info->default_size[0]=val1;
            wn_info->default_size[1]=val2;
        }else if(strcmp(p->attribut->info,"rqs")==0)
        {
            str_to_val1_val2(p->valeur,&val1,&val2);
            wn_info->request_size[0]=val1;
            wn_info->request_size[1]=val2;
            wn_info->default_size[0]=-1;
            wn_info->default_size[1]=-1;
        }else if(strcmp(p->attribut->info,"ico")==0)
            remplir_info(wn_info->icon,p->valeur->info);
        else if(strcmp(p->attribut->info,"opa")==0)
        {
            f=atof(p->valeur->info);
            wn_info->opacity=(float)f;
        }else if(strcmp(p->attribut->info,"fulls")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)wn_info->fullscreen=TRUE;
            else wn_info->fullscreen=FALSE;
        }else if(strcmp(p->attribut->info,"kepb")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)wn_info->keep_below=TRUE;
            else wn_info->keep_below=FALSE;
        }else if(strcmp(p->attribut->info,"back")==0)
            remplir_info(wn_info->back,p->valeur->info);
        p=p->suiv;
    }
    return (window*)wn_info;
}




void make_window(window *wn_info)
{
    GtkWidget *window=gtk_window_new(wn_info->type_win);
    if(wn_info->title->Idelem!=-1)gtk_window_set_title(GTK_WINDOW(window),wn_info->title->info);
    gtk_window_set_resizable(GTK_WINDOW(window),wn_info->resiasable);
    gtk_window_set_modal (GTK_WINDOW(window),wn_info->modal);

    if(wn_info->default_size[0]!=-1)
        gtk_window_set_default_size(GTK_WINDOW(window),
                                    wn_info->default_size[0],wn_info->default_size[1]);
    else if(wn_info->request_size[0]!=-1)
        gtk_window_set_default_size(GTK_WINDOW(window),800,800);//default size

    if(wn_info->icon->Idelem!=-1) gtk_window_set_icon_from_file(GTK_WINDOW(window),wn_info->icon->info,NULL);

    if(wn_info->opacity!=-1) gtk_widget_set_opacity (GTK_WIDGET(window),wn_info->opacity);

    if(wn_info->fullscreen) gtk_window_fullscreen(GTK_WINDOW(window));
    if(wn_info->back->Idelem!=-1)
    {
        GtkStyleContext* context = gtk_widget_get_style_context(window);
        GtkCssProvider* provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, wn_info->back->info, -1, NULL);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    gtk_window_set_keep_below(GTK_WINDOW(window),wn_info->keep_below);
    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    wn_info->win=window;
}


typedef struct hdb
{
    GtkWidget *header_bar;
    info *title;                //gtk_header_bar_set_title ()
    info *sub_title;            //gtk_header_bar_set_subtitle ()
    gboolean close_button_show; //gtk_header_bar_set_show_close_button ()
    // menu:minimize,maximize,close
    info *decoration_layout;    //gtk_header_bar_set_decoration_layout ()

    // gtk_header_bar_pack_start ()
    // gtk_header_bar_pack_end ()
}header_bar;


header_bar *creer_initialiser_header_bar()
{
    header_bar *hdb;
    hdb=(header_bar*) malloc(sizeof (header_bar));
    if(!hdb)
    {
        printf("error allocation \n");
        exit(-1);
    }
    hdb->title=creer_initialiser_info();
    remplir_info(hdb->title,"default");
    hdb->sub_title=creer_initialiser_info();
    remplir_info(hdb->sub_title,"default");
    hdb->decoration_layout=creer_initialiser_info();
    remplir_info(hdb->decoration_layout,"menu:minimize,maximize,close");
    hdb->close_button_show=TRUE;
    hdb->header_bar=NULL;
    return (header_bar*)hdb;
}


header_bar *remplir_header_bar(FILE *f1)
{
    char mot[35];
    header_bar *hdd=creer_initialiser_header_bar();
    obj_att *liste=NULL,*p=NULL;
    liste=lire_att(f1);
    if(!liste)return (header_bar*)hdd;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"title")==0)
            remplir_info(hdd->title,p->valeur->info);
        else if(strcmp(p->attribut->info,"subt")==0)
        {
            remplir_info(hdd->sub_title,p->valeur->info);
        }else if(strcmp(p->attribut->info,"clbsh")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)hdd->close_button_show=TRUE;
            else hdd->close_button_show=FALSE;
        }else if(strcmp(p->attribut->info,"declay")==0)
            remplir_info(hdd->decoration_layout,p->valeur->info);
        p=p->suiv;
    }
    return (header_bar*)hdd;
}


void make_headerbar(header_bar *header_bar_info)
{
    GtkWidget *hd_bar=gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(hd_bar),header_bar_info->title->info);
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR(hd_bar),header_bar_info->sub_title->info);

    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(hd_bar),header_bar_info->close_button_show);
    gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR(hd_bar),header_bar_info->decoration_layout->info);
    header_bar_info->header_bar=hd_bar;
}

typedef struct btn
{
    GtkWidget *button;
    info *back;//background
    info *signal;//signal
    info *label;                    //gtk_button_new_with_label ()
    info *mnemonic;                 //gtk_button_new_with_mnemonic ()
    info *button_from_icon_name;    //gtk_button_new_from_icon_name()
    GtkIconSize icon_siz;
    GtkReliefStyle relief_type;     //gtk_button_set_relief();
    gboolean use_underline;         //gtk_button_set_use_underline ()
    GtkPositionType image_position; //gtk_button_set_image_position ()
    //gtk_button_set_image()
    //gtk_button_set_label()
}button;


button *creer_initialiser_button()
{
    button *btn;
    btn=(button*) malloc(sizeof (button));
    if(!btn)
    {
        printf("error allocation \n");
        exit(-1);
    }
    btn->label=creer_initialiser_info();
    btn->back=creer_initialiser_info();
    btn->signal=creer_initialiser_info();
    remplir_info(btn->label,"default");
    btn->mnemonic=creer_initialiser_info();
    btn->icon_siz=GTK_ICON_SIZE_MENU;
    btn->relief_type=GTK_RELIEF_NORMAL;
    btn->use_underline=FALSE;
    btn->image_position=GTK_POS_LEFT;
    btn->button_from_icon_name=creer_initialiser_info();
    btn->button=NULL;
    return (button*)btn;
}

button *remplir_button(FILE *f1)
{
    button *btns=creer_initialiser_button();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (button*)btns;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"lab" )==0)
            remplir_info(btns->label,p->valeur->info);
        else if(strcmp(p->attribut->info,"mnemonic" )==0)
        {
            remplir_info(btns->mnemonic,p->valeur->info);
            btns->label->Idelem=-1;
        }else if(strcmp(p->attribut->info,"bticf" )==0)
        {
            remplir_info(btns->label,p->valeur->info);
            btns->label->Idelem=-1;
        }else if(strcmp(p->attribut->info,"ics" )==0)
        {
            if(strcmp(p->valeur->info,"GTK_ICON_SIZE_INVALID")==0)btns->icon_siz=GTK_ICON_SIZE_INVALID;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_SMALL_TOOLBAR")==0)btns->icon_siz=GTK_ICON_SIZE_SMALL_TOOLBAR;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_LARGE_TOOLBAR")==0)btns->icon_siz=GTK_ICON_SIZE_LARGE_TOOLBAR;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_BUTTON")==0)btns->icon_siz=GTK_ICON_SIZE_BUTTON;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_DND")==0)btns->icon_siz=GTK_ICON_SIZE_DND;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_DIALOG")==0)btns->icon_siz=GTK_ICON_SIZE_DIALOG;
        }else if(strcmp(p->attribut->info,"rlt" )==0)
        {
            if(strcmp(p->valeur->info,"GTK_RELIEF_HALF")==0)btns->relief_type=GTK_RELIEF_HALF;
            else if(strcmp(p->valeur->info,"GTK_RELIEF_NONE")==0)btns->relief_type=GTK_RELIEF_NONE;
        }else if(strcmp(p->attribut->info,"usun" )==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)btns->use_underline=TRUE;
            else btns->use_underline=TRUE;
        }else if(strcmp(p->attribut->info,"impos" )==0)
        {
            if(strcmp(p->valeur->info,"GTK_POS_RIGHT")==0)btns->image_position=GTK_POS_RIGHT;
            else if(strcmp(p->valeur->info,"GTK_POS_TOP")==0)btns->image_position=GTK_POS_TOP;
            else if(strcmp(p->valeur->info,"GTK_POS_BOTTOM")==0)btns->image_position=GTK_POS_BOTTOM;
        }else if(strcmp(p->attribut->info,"back")==0)
        {
            remplir_info(btns->back,p->valeur->info);
        }else if(strcmp(p->attribut->info,"signal")==0)
        {
            remplir_info(btns->signal,p->valeur->info);
        }
        p=p->suiv;
    }
    return (button*)btns;
}

void func()
{
    g_print("button clicked\n");
}
void func1(GtkWidget *button,gpointer data)
{
    gtk_widget_hide(im1);
}


void make_button(button *button_info)
{
    GtkWidget *btn=NULL;
    if(button_info->label->Idelem!=-1) btn=gtk_button_new_with_label(button_info->label->info);
    else if(button_info->mnemonic->Idelem!=-1)
    {
        const gchar *label = button_info->mnemonic->info;
        btn = gtk_button_new_with_label(label);
        GtkWidget *child=gtk_bin_get_child(GTK_BIN(btn));
        gtk_label_set_use_underline(GTK_LABEL(child), TRUE);
    }
    else if(button_info->button_from_icon_name->Idelem !=-1)
        btn=gtk_button_new_from_icon_name(button_info->button_from_icon_name->info,button_info->icon_siz);
    if(button_info->back->Idelem!=-1)
    {
        GtkStyleContext* context = gtk_widget_get_style_context(btn);
        GtkCssProvider* provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, button_info->back->info, -1, NULL);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    if(button_info->signal->Idelem!=-1)
    {
        if(strcmp(button_info->signal->info,"func")==0)g_signal_connect(btn,"clicked",G_CALLBACK(func),NULL);
        else if(strcmp(button_info->signal->info,"destroy")==0)g_signal_connect(btn,"clicked",G_CALLBACK(gtk_main_quit),NULL);
        else if(strcmp(button_info->signal->info,"affi")==0)g_signal_connect(btn,"clicked",G_CALLBACK(func1),im1);
    }
    gtk_button_set_relief(GTK_BUTTON(btn),button_info->relief_type);
    gtk_button_set_use_underline(GTK_BUTTON(btn),button_info->use_underline);
    gtk_button_set_image_position(GTK_BUTTON(btn),button_info->image_position);
    button_info->button=btn;
}


typedef struct lb{
    GtkWidget *label;
    info *label_name;//gtk_label_new ()
    info *label_with_mnemonic;//gtk_label_new_with_mnemonic()
    info *markup_text;//gtk_label_set_markup ()
    info *markup_text_with_mnemonic;//gtk_label_set_markup_with_mnemonic ()
    info *pattern;//gtk_label_set_pattern ()
    GtkJustification justify;//gtk_label_set_justify ()
    gdouble xalign;//gtk_label_set_xalign ()
    gdouble yalign;//gtk_label_set_yalign ()
    PangoEllipsizeMode ellipsis;//gtk_label_set_ellipsize ()
    gboolean selectable;//gtk_label_set_selectable()
}label;





label *crer_initialiser_label()
{
    label *lb;
    lb=(label*)malloc(sizeof (label));
    if(!lb)
    {
        printf("error allocation \n");
        exit(-1);
    }
    lb->label_name=creer_initialiser_info();
    remplir_info(lb->label_name,"default");
    lb->label_with_mnemonic=creer_initialiser_info();
    lb->markup_text=creer_initialiser_info();
    lb->markup_text_with_mnemonic=creer_initialiser_info();
    lb->pattern=creer_initialiser_info();
    lb->justify=GTK_JUSTIFY_LEFT;
    lb->ellipsis=PANGO_ELLIPSIZE_NONE;
    lb->selectable=FALSE;
    lb->xalign=0.5;
    lb->yalign=0.5;
    lb->label=NULL;
    return (label*)lb;
}

label *remplir_label(FILE *f1)
{
    label *lab=crer_initialiser_label();
    double f;
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (label*)lab;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"labna" )==0)
            remplir_info(lab->label_name,p->valeur->info);
        else if(strcmp(p->attribut->info,"labmn" )==0)
        {
            remplir_info(lab->label_with_mnemonic,p->valeur->info);
            lab->label_name->Idelem=-1;
        }else if(strcmp(p->attribut->info,"labmar" )==0)
            remplir_info(lab->markup_text,p->valeur->info);
        else if(strcmp(p->attribut->info,"labmarm" )==0)
            remplir_info(lab->markup_text_with_mnemonic,p->valeur->info);
        else if(strcmp(p->attribut->info,"patt" )==0)
            remplir_info(lab->pattern,p->valeur->info);
        else if(strcmp(p->attribut->info,"just" )==0)
        {
            if(strcmp(p->valeur->info,"GTK_JUSTIFY_RIGHT")==0)lab->justify=GTK_JUSTIFY_RIGHT;
            else if(strcmp(p->valeur->info,"GTK_JUSTIFY_CENTER")==0)lab->justify=GTK_JUSTIFY_CENTER;
            else if(strcmp(p->valeur->info,"GTK_JUSTIFY_FILL")==0)lab->justify=GTK_JUSTIFY_FILL;
        }else if(strcmp(p->attribut->info,"xlig" )==0)
        {
            f=atof(p->valeur->info);
            lab->xalign=f;
        }else if(strcmp(p->attribut->info,"ylig" )==0)
        {
            f=atof(p->valeur->info);
            lab->yalign=f;
        }else if(strcmp(p->attribut->info,"ellip" )==0)
        {
            if(strcmp(p->valeur->info,"PANGO_ELLIPSIZE_START")==0)lab->ellipsis=PANGO_ELLIPSIZE_START;
            else if(strcmp(p->valeur->info,"PANGO_ELLIPSIZE_MIDDLE")==0)lab->ellipsis=PANGO_ELLIPSIZE_MIDDLE;
            else if(strcmp(p->valeur->info,"PANGO_ELLIPSIZE_END")==0)lab->ellipsis=PANGO_ELLIPSIZE_END;
        }else if(strcmp(p->attribut->info,"selec" )==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)lab->selectable=TRUE;
            else lab->selectable=FALSE;
        }
        p=p->suiv;
    }
    return (label*)lab;

}

void make_label(label *label_info)
{
    GtkWidget *lab=NULL;
    if(label_info->label_name->Idelem!=-1) lab=gtk_label_new (label_info->label_name->info);
    else if(label_info->label_with_mnemonic->Idelem!=-1) lab=gtk_label_new_with_mnemonic(label_info->label_with_mnemonic->info);

    if(label_info->markup_text->Idelem!=-1) gtk_label_set_markup(GTK_LABEL(lab),label_info->markup_text->info);
    if(label_info->markup_text_with_mnemonic->Idelem!=-1) gtk_label_set_markup_with_mnemonic(GTK_LABEL(lab),label_info->label_with_mnemonic->info);

    if(label_info->pattern->Idelem!=-1)gtk_label_set_pattern(GTK_LABEL(lab),label_info->pattern->info);

    gtk_label_set_justify(GTK_LABEL(lab),label_info->justify);

    gtk_label_set_xalign(GTK_LABEL(lab),(gfloat)label_info->xalign);
    gtk_label_set_yalign(GTK_LABEL(lab),(gfloat)label_info->yalign);
    gtk_label_set_ellipsize(GTK_LABEL(lab),label_info->ellipsis);

    gtk_label_set_selectable(GTK_LABEL(lab),label_info->selectable);

    label_info->label=lab;
}


typedef struct im{
    GtkWidget *image;
    int reqsize[2];
    info *file_name;//gtk_image_new_from_file ()
    info *signal;
    info *animation_file;//gtk_image_new_from_animation () et gdk_pixbuf_animation_new_from_file()
    info *icon_name;//gtk_image_new_from_icon_name ()
    GtkIconSize icon_siz;
    int pixel_size;//gtk_image_set_pixel_size ()
}image;

image *creer_initialiser_image()
{
    image *im;
    im=(image*) malloc(sizeof (image));
    if(!im)
    {
        printf("error allocation \n");
        exit(-1);
    }
    im->reqsize[0]=-1;
    im->reqsize[1]=-1;
    im->file_name=creer_initialiser_info();
    im->signal=creer_initialiser_info();
    remplir_info(im->file_name,"C:\\Users\\USER\\Desktop\\zzz.png");
    im->animation_file=creer_initialiser_info();
    im->icon_name=creer_initialiser_info();
    im->icon_siz=GTK_ICON_SIZE_MENU;
    im->icon_siz=100;
    im->image=NULL;
    return (image*)im;
}


image *remplir_image(FILE *f1)
{
    image *im=creer_initialiser_image();
    int f;
    int val1,val2;
    obj_att *liste=NULL,*p=NULL;
    liste=lire_att(f1);
    if(!liste)return (image*)im;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"nmfil")==0)
        {
            remplir_info(im->file_name,p->valeur->info);
        }
        else if(strcmp(p->attribut->info,"anmf" )==0)
        {
            remplir_info(im->animation_file,p->valeur->info);
            im->file_name->Idelem=-1;
        }else if(strcmp(p->attribut->info,"icona" )==0)
        {
            remplir_info(im->icon_name,p->valeur->info);
            im->file_name->Idelem=-1;
        }else if(strcmp(p->attribut->info,"icons" )==0)
        {
            if(strcmp(p->valeur->info,"GTK_ICON_SIZE_BUTTON")==0)im->icon_siz=GTK_ICON_SIZE_BUTTON;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_DND")==0)im->icon_siz=GTK_ICON_SIZE_DND;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_DIALOG")==0)im->icon_siz=GTK_ICON_SIZE_DIALOG;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_SMALL_TOOLBAR")==0)im->icon_siz=GTK_ICON_SIZE_SMALL_TOOLBAR;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_LARGE_TOOLBAR")==0)im->icon_siz=GTK_ICON_SIZE_LARGE_TOOLBAR;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_MENU")==0)im->icon_siz=GTK_ICON_SIZE_MENU;
        }else if(strcmp(p->attribut->info,"pix" )==0)
        {
            f=atoi(p->valeur->info);
            im->pixel_size=f;
        }else if(strcmp(p->attribut->info,"rqs" )==0)
        {
            str_to_val1_val2(p->valeur,&val1,&val2);
            im->reqsize[0]=val1;
            im->reqsize[1]=val2;
        }else if(strcmp(p->attribut->info,"signal" )==0)remplir_info(im->signal,p->valeur->info);
        p=p->suiv;
    }
    return (image*)im;
}


void make_image(image *image_info)
{

    GdkPixbufAnimation *anim;
    GdkPixbuf *pixbuf;
    GdkPixbuf *scaled_pixbuf;
    if(image_info->file_name->Idelem!=-1)
    {
        if(image_info->reqsize[0]!=-1) {
            pixbuf = gdk_pixbuf_new_from_file(image_info->file_name->info, NULL);
            scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, image_info->reqsize[0], image_info->reqsize[1], GDK_INTERP_BILINEAR);
            im = gtk_image_new_from_pixbuf(scaled_pixbuf);
        }else im = gtk_image_new_from_file(image_info->file_name->info);
    }
    else if(image_info->animation_file->Idelem!=-1) {
        anim=gdk_pixbuf_animation_new_from_file(image_info->animation_file->info,NULL);
        im=gtk_image_new_from_animation(anim);
    }else if(image_info->icon_name->Idelem!=-1) im=gtk_image_new_from_icon_name(image_info->icon_name->info,image_info->icon_siz);
    if(image_info->signal->Idelem!=-1)
        im1=im;
    image_info->image=im;
}


typedef struct radbtn
{
    GtkWidget *radio_button;
    info *label;//gtk_radio_button_new_with_label()
    info *label_with_mnemonic;//gtk_radio_button_new_with_mnemonic ()
    gboolean main_rdb;//pour savoir que le radio button est le pere
    gboolean mode;//gtk_toggle_button_set_mode()
    //gtk_radio_button_get_group ()
}radio_button;

radio_button *creer_initialiser_radio_button()
{
    radio_button *rdbt;
    rdbt=(radio_button*) malloc(sizeof (radio_button));
    if(!rdbt)
    {
        printf("error allocation \n");
        exit(-1);
    }
    rdbt->main_rdb=FALSE;
    rdbt->label=creer_initialiser_info();
    remplir_info(rdbt->label,"default");
    rdbt->label_with_mnemonic=creer_initialiser_info();
    rdbt->mode=TRUE;
    rdbt->radio_button=NULL;
    return (radio_button*)rdbt;
}


radio_button *remplir_radio_button(FILE *f1)
{
    radio_button *rdb=creer_initialiser_radio_button();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (radio_button*)rdb;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"label")==0)remplir_info(rdb->label,p->valeur->info);
        else if(strcmp(p->attribut->info,"labmne")==0)
        {
            remplir_info(rdb->label_with_mnemonic,p->valeur->info);
            rdb->label->Idelem=-1;
        }else if(strcmp(p->attribut->info,"mode")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)rdb->mode=TRUE;
            else rdb->mode=FALSE;
        }else if(strcmp(p->attribut->info,"mainrdb")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)rdb->main_rdb=TRUE;
        }
        p=p->suiv;
    }
    return (radio_button*)rdb;
}

void raddd(GtkWidget *radio,gpointer data)
{
    GtkLabel *Label;
    Label =GTK_LABEL(gtk_bin_get_child(GTK_BIN(radio)));
    char *Text=gtk_label_get_text(Label);
//    printf("%s\n",(char *)data);

}
void make_radio_button(radio_button *radio_button_info,GSList *groupe)
{
    GtkWidget *rdb=NULL;
    if(radio_button_info->label->Idelem!=-1)
        rdb=gtk_radio_button_new_with_label(groupe,radio_button_info->label->info);
    else if(radio_button_info->label_with_mnemonic->Idelem!=-1)
        rdb=gtk_radio_button_new_with_mnemonic (groupe,radio_button_info->label_with_mnemonic->info);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(rdb),radio_button_info->mode);
    groupe=gtk_radio_button_get_group (GTK_RADIO_BUTTON(rdb));
//    g_signal_connect(rdb,"clicked",G_CALLBACK(raddd),radio_button_info->label->info);
    radio_button_info->radio_button=rdb;
}


typedef struct chb
{
    GtkWidget *checkbutton;
    info *label;//gtk_check_button_new_with_label ()
    info *label_with_mnemonic;//gtk_check_button_new_with_mnemonic ()
    gboolean mode;//gtk_toggle_button_set_mode()
}check_button;


check_button *creer_initialiser_check_button()
{
    check_button *chkbtn;
    chkbtn=(check_button*) malloc(sizeof (check_button));
    if(!chkbtn)
    {
        printf("error allocation \n");
        exit(-1);
    }
    chkbtn->label=creer_initialiser_info();
    remplir_info(chkbtn->label,"default");
    chkbtn->label_with_mnemonic=creer_initialiser_info();
    chkbtn->mode=TRUE;
    chkbtn->checkbutton=NULL;
    return (check_button*)chkbtn;
}

check_button *remplir_check_button(FILE *f1)
{
    check_button *ckb=creer_initialiser_check_button();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (check_button*)ckb;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"label")==0)remplir_info(ckb->label,p->valeur->info);
        else if(strcmp(p->attribut->info,"labmne")==0)
        {
            remplir_info(ckb->label_with_mnemonic,p->valeur->info);
            ckb->label->Idelem=-1;
        }else if(strcmp(p->attribut->info,"mode")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)ckb->mode=TRUE;
            else ckb->mode=FALSE;
        }
        p=p->suiv;
    }
    return (check_button*)ckb;
}



void make_check_button(check_button *check_button_info)
{
    GtkWidget *chbt=NULL;
    if(check_button_info->label->Idelem!=-1)chbt=gtk_check_button_new_with_label(check_button_info->label->info);
    else if(check_button_info->label_with_mnemonic->Idelem!=-1) chbt=gtk_check_button_new_with_mnemonic(check_button_info->label_with_mnemonic->info);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(chbt),check_button_info->mode);
    check_button_info->checkbutton=chbt;
}



typedef struct tgb{
    GtkWidget *toogle_button;
    info *label;//gtk_toggle_button_new_with_label ()
    info *label_with_mnemonic;//gtk_toggle_button_new_with_mnemonic ()
    gboolean mode;//gtk_toggle_button_set_mode()
    gboolean inconsitant;//gtk_toggle_button_set_inconsistent()
}toggle_button;


toggle_button *creer_initialiser_toggle_button()
{
    toggle_button *tglbtn;
    tglbtn=(toggle_button*) malloc(sizeof (toggle_button));
    if(!tglbtn)
    {
        printf("error allocation \n");
        exit(-1);
    }
    tglbtn->label=creer_initialiser_info();
    remplir_info(tglbtn->label,"default");
    tglbtn->label_with_mnemonic=creer_initialiser_info();
    tglbtn->mode=FALSE;
    tglbtn->inconsitant=FALSE;
    tglbtn->toogle_button=NULL;
    return (toggle_button*)tglbtn;
}


toggle_button *remplir_toggle_button(FILE *f1)
{
    toggle_button *tglb=creer_initialiser_toggle_button();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (toggle_button*)tglb;
    p=liste;

    while (p)
    {
        if(strcmp(p->attribut->info,"label")==0)
        {

            remplir_info(tglb->label,p->valeur->info);
        }
        else if(strcmp(p->attribut->info,"labmne")==0)
        {
            remplir_info(tglb->label_with_mnemonic,p->valeur->info);
            tglb->label->Idelem=-1;
        }else if(strcmp(p->attribut->info,"mode")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)tglb->mode=TRUE;
            else tglb->mode=FALSE;
        }else if(strcmp(p->attribut->info,"incon")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)tglb->inconsitant=TRUE;
            else tglb->inconsitant=FALSE;
        }
        p=p->suiv;
    }
    return (toggle_button*)tglb;
}

void make_toggle_button(toggle_button *toggle_button_info)
{
    GtkWidget *tgb=NULL;
    if(toggle_button_info->label->Idelem!=-1) tgb=gtk_toggle_button_new_with_label(toggle_button_info->label->info);
    else if(toggle_button_info->label_with_mnemonic->Idelem!=-1) tgb=gtk_toggle_button_new_with_mnemonic(toggle_button_info->label_with_mnemonic->info);
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(tgb),toggle_button_info->mode);
    gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(tgb),toggle_button_info->inconsitant);
    gtk_widget_set_size_request(tgb,40,40);
    gtk_widget_set_halign(tgb,  GTK_ALIGN_CENTER);
    toggle_button_info->toogle_button=tgb;
}


typedef struct linb
{
    GtkWidget *link_button;
    info *label;//gtk_link_button_new_with_label ()
    info *url;//gtk_link_button_set_uri ()
    gboolean visited;//gtk_link_button_set_visited ()
}link_button;

link_button *creer_initialiser_link_button()
{
    link_button *linbtn;
    linbtn=(link_button*) malloc(sizeof (link_button));
    if(!linbtn)
    {
        printf("error allocation \n");
        exit(-1);
    }
    linbtn->label=creer_initialiser_info();
    remplir_info(linbtn->label,"link");
    linbtn->url=creer_initialiser_info();
    remplir_info(linbtn->url,"https://www.google.com/?hl=fr");
    linbtn->visited=FALSE;
    linbtn->link_button=NULL;
    return (link_button*)linbtn;
}

link_button *remplir_link_button(FILE *f1)
{
    link_button *linkb=creer_initialiser_link_button();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (link_button*)linkb;
    p=liste;
    while (p)
    {
        if(strcmp(p->valeur->info,"label")==0)remplir_info(linkb->label,p->valeur->info);
        else if(strcmp(p->valeur->info,"url")==0)remplir_info(linkb->url,p->valeur->info);
        else if(strcmp(p->valeur->info,"vise")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)linkb->visited=TRUE;
            else linkb->visited=FALSE;
        }
        p=p->suiv;
    }
    return (link_button*)linkb;
}


void make_link_button(link_button *link_button_info)
{
    GtkWidget *lkbt=NULL;
    lkbt=gtk_link_button_new_with_label(link_button_info->url->info,link_button_info->label->info);
    gtk_link_button_set_visited(GTK_LINK_BUTTON(lkbt),link_button_info->visited);
    gtk_widget_set_size_request(lkbt,80,40);
    gtk_widget_set_halign(lkbt,  GTK_ALIGN_CENTER);
    link_button_info->link_button=lkbt;
}

typedef struct mnb{
    GtkWidget *menu_butt;
    //gtk_menu_button_set_popup ()
    //gtk_menu_button_set_popover ()

    GtkArrowType direction;//gtk_menu_button_set_direction ()
    //gtk_menu_button_set_align_widget ()
}menu_button;

menu_button *creer_intialiser_menu_button()
{
    menu_button *mnbtn;
    mnbtn=(menu_button*) malloc(sizeof (menu_button));
    if(!mnbtn)
    {
        printf("error allocation \n");
        exit(-1);
    }
    mnbtn->direction=GTK_ARROW_DOWN;
    mnbtn->menu_butt=NULL;
    return (menu_button*)mnbtn;
}

menu_button *remplir_menu_button(FILE *f1)
{
    menu_button *mnb=creer_intialiser_menu_button();
    obj_att *liste=NULL;
    liste= lire_att(f1);
    if(!liste)return (menu_button*)mnb;
    if(strcmp(liste->valeur->info,"GTK_ARROW_UP")==0)mnb->direction=GTK_ARROW_UP;
    else if(strcmp(liste->valeur->info,"GTK_ARROW_LEFT")==0)mnb->direction=GTK_ARROW_LEFT;
    else if(strcmp(liste->valeur->info,"GTK_ARROW_RIGHT")==0)mnb->direction=GTK_ARROW_RIGHT;
    else if(strcmp(liste->valeur->info,"GTK_ARROW_NONE")==0)mnb->direction=GTK_ARROW_NONE;
    return (menu_button*)mnb;
}


void make_menu_button(menu_button *menu_button_info)
{
    GtkWidget *mnbtn=gtk_menu_button_new ();
    gtk_menu_button_set_direction(GTK_MENU_BUTTON(mnbtn),menu_button_info->direction);
    gtk_widget_set_size_request(mnbtn,40,40);
    gtk_widget_set_halign(mnbtn,  GTK_ALIGN_CENTER);
    menu_button_info->menu_butt=mnbtn;
}


typedef struct mnit
{
    info *label;//gtk_menu_item_new_with_label ()
    info *mnemonic;//gtk_menu_item_new_with_mnemonic ()
    info *shor;
    info *icon;
    int cpt;
    struct mnit *but;
    struct mnit *top;
    struct mnit *rght;
}menu_item;


menu_item *creer_intialiser_menu_item()
{
    menu_item *mnit;
    mnit=(menu_item*)malloc(sizeof(menu_item));
    if(!mnit)
    {
        printf("error allocation \n");
        exit(-1);
    }
    mnit->label=creer_initialiser_info();
    mnit->icon=creer_initialiser_info();
    mnit->shor=creer_initialiser_info();
    remplir_info(mnit->label,"default");
    mnit->mnemonic=creer_initialiser_info();
    mnit->cpt=0;
    mnit->but=NULL;
    mnit->rght=NULL;
    mnit->top=NULL;
    return (menu_item*)mnit;
}


menu_item *remplir_menu_item(FILE *f1)
{
    char mot[18],c;
    int k=1;
    menu_item *in,*p,*ne;
    obj_att *liste=NULL,*q;
    fscanf(f1,"<%30[^ ] ",mot);
    liste= lire_att(f1);
    in=creer_intialiser_menu_item();
    q=liste;
    while(q)
    {
        if(strcmp(q->attribut->info,"label")==0)remplir_info(in->label,q->valeur->info);
        else if(strcmp(q->attribut->info,"mnemo")==0){
            remplir_info(in->mnemonic,q->valeur->info);
            in->label->Idelem=-1;
        }else if(strcmp(q->attribut->info,"short")==0)remplir_info(in->shor,q->valeur->info);
        else if(strcmp(q->attribut->info,"icon")==0)remplir_info(in->icon,q->valeur->info);
        q=q->suiv;
    }
    p=in;
    do {
        fscanf(f1,"<%30[^ ] ",mot);
        if(strcmp(mot,"item")==0)
        {
            liste= lire_att(f1);
            ne=creer_intialiser_menu_item();
            q=liste;
            while(q)
            {
                if(strcmp(q->attribut->info,"label")==0)remplir_info(ne->label,q->valeur->info);
                else if(strcmp(q->attribut->info,"mnemo")==0){
                    remplir_info(ne->mnemonic,q->valeur->info);
                    ne->label->Idelem=-1;
                }else if(strcmp(q->attribut->info,"short")==0)remplir_info(ne->shor,q->valeur->info);
                else if(strcmp(q->attribut->info,"icon")==0)remplir_info(ne->icon,q->valeur->info);
                q=q->suiv;
            }
            if(k==0)
            {
                p->but=ne;
                ne->top=p;
                p=ne;
                k=1;
            }else{
                p->rght=ne;
                ne->top=p;
                p=ne;
            }
        }else if(strcmp(mot,"/item")==0)
        {
            fscanf(f1,">\n");
            epurer_blan(f1);
            while (p->cpt==1)p=p->top;
            p->cpt=1;
            k=0;
        }
    } while (strcmp(mot,"/menu")!=0);
    fscanf(f1,">\n");
    epurer_blan(f1);
    return (menu_item*)in;
}



GtkWidget *make_menu_bar(menu_item *menu_item_info,GtkWidget *mn_bar)
{
    GtkWidget *item=NULL;
    if(!menu_item_info)return (GtkWidget*)NULL;
    if(menu_item_info->icon->Idelem!=-1 || menu_item_info->shor->Idelem!=-1 )
    {
        item=gtk_menu_item_new();
        GtkWidget *bxx= gtk_box_new(GTK_ORIENTATION_HORIZONTAL,15);
        if(menu_item_info->icon->Idelem!=-1)
        {
            GtkWidget *im= gtk_image_new_from_icon_name(menu_item_info->icon->info,GTK_ICON_SIZE_MENU);
            gtk_box_pack_start(GTK_BOX(bxx),im,FALSE,FALSE,0);
        }
        GtkWidget *label1=NULL;
        if(menu_item_info->label->Idelem!=-1)label1=gtk_label_new(menu_item_info->label->info);
        else if(menu_item_info->mnemonic->Idelem!=-1)label1=gtk_label_new(menu_item_info->mnemonic->info);
        gtk_box_pack_start(GTK_BOX(bxx),label1,FALSE,FALSE,0);
        if(menu_item_info->shor->Idelem!=-1)
        {
            GtkWidget *label= gtk_label_new(menu_item_info->shor->info);
            gtk_box_pack_start(GTK_BOX(bxx),label,FALSE,FALSE,0);
        }
        gtk_container_add(GTK_CONTAINER(item),bxx);
    }else {
        if(menu_item_info->label->Idelem!=-1)item=gtk_menu_item_new_with_label(menu_item_info->label->info);
        else if(menu_item_info->mnemonic->Idelem!=-1)item=gtk_menu_item_new_with_mnemonic(menu_item_info->mnemonic->info);
    }
    gtk_menu_shell_append(GTK_MENU_SHELL(mn_bar),item);
    if(menu_item_info->but)
        make_menu_bar(menu_item_info->but,mn_bar);
    if(menu_item_info->rght)
    {
        GtkWidget *submenu=gtk_menu_new();
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),submenu);
        make_menu_bar(menu_item_info->rght,submenu);
    }
    return (GtkWidget*)mn_bar;
}


typedef struct mnn
{
    GtkWidget *menu;
    GtkPackDirection direction;//gtk_menu_bar_set_pack_direction()
    menu_item *info;
}menu;



menu *crer_initi_menu()
{
    menu *mn;
    mn=(menu*) malloc(sizeof (menu));
    if(!mn)
    {
        printf("error allocation \n");
        exit(-1);
    }
    mn->direction=GTK_PACK_DIRECTION_LTR;
    mn->info=NULL;
    mn->menu=NULL;
    return (menu*)mn;
}

menu *remplir_menu(FILE *f1)
{
    menu *mn=crer_initi_menu();
    obj_att *liste=NULL;
    liste= lire_att(f1);
    if(!liste)return (menu*)mn;
    if(strcmp(liste->valeur->info,"GTK_PACK_DIRECTION_RTL")==0)mn->direction=GTK_PACK_DIRECTION_RTL;
    else if(strcmp(liste->valeur->info,"GTK_PACK_DIRECTION_TTB")==0)mn->direction=GTK_PACK_DIRECTION_TTB;
    else if(strcmp(liste->valeur->info,"GTK_PACK_DIRECTION_BTT")==0)mn->direction=GTK_PACK_DIRECTION_BTT;
    mn->info=remplir_menu_item(f1);
    return (menu*)mn;
}

void make_menu(menu *menu_info)
{
    GtkWidget *mn_bar=gtk_menu_bar_new();
    gtk_menu_bar_set_pack_direction(GTK_MENU_BAR(mn_bar),menu_info->direction);
    mn_bar=make_menu_bar(menu_info->info,mn_bar);
    menu_info->menu=mn_bar;
}




typedef struct tli
{
    info *label;//gtk_tool_button_set_label()
    gboolean underline;//gtk_tool_button_set_use_underline()
    info *icon_name;//gtk_tool_button_set_icon_name ()
    //gtk_tool_button_set_icon_widget ()
    info *tooltip_markup;//gtk_tool_item_set_tooltip_markup ()
    info *tooltip_text;//gtk_tool_item_set_tooltip_text ()
    struct tli *suiv;
}tool_item;

tool_item *creer_intialiser_tool_item()
{
    tool_item *ne;
    ne=(tool_item*)malloc(sizeof (tool_item));
    if(!ne)
    {
        printf("error allocation \n");
        exit(-1);
    }
    ne->label=creer_initialiser_info();
    remplir_info(ne->label,"default");
    ne->icon_name=creer_initialiser_info();
    remplir_info(ne->icon_name,"document-open");
    ne->tooltip_markup=creer_initialiser_info();
    ne->tooltip_text=creer_initialiser_info();
    ne->suiv=NULL;
    return (tool_item*)ne;
}

tool_item *remplir_tool_item(FILE *f1)
{
    tool_item *toit=creer_intialiser_tool_item();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (tool_item*)toit;
    p=liste;
    while (p)
    {
        if(strcmp(p->valeur->info,"label")==0)remplir_info(toit->label,p->valeur->info);
        else if(strcmp(p->valeur->info,"icnam")==0)
        {
            remplir_info(toit->icon_name,p->valeur->info);
            toit->label->Idelem=-1;
        }else if(strcmp(p->valeur->info,"tolmar")==0)remplir_info(toit->tooltip_markup,p->valeur->info);
        else if(strcmp(p->valeur->info,"toltex")==0)remplir_info(toit->tooltip_text,p->valeur->info);
        p=p->suiv;
    }
    return (tool_item*)toit;
}


tool_item *remplir_tool_item_liste(FILE *f1)
{
    char mot[30];
    char c;
    tool_item *liste=NULL;
    tool_item *ne=NULL;
    fscanf(f1,"<%35[^ ] ",mot);
    liste=remplir_tool_item(f1);
    fscanf(f1,"<%35[^>]>\n",mot);
    epurer_blan(f1);
    fscanf(f1,"<%35[^ ] ",mot);
    while (strcmp(mot,"/toolbar")!=0)
    {
        ne=remplir_tool_item(f1);
        ne->suiv=liste;
        liste=ne;
        fscanf(f1,"<%35[^>]>\n",mot);
        epurer_blan(f1);
        fscanf(f1,"<%35[^ ] ",mot);
    }
    fscanf(f1,">\n");
    epurer_blan(f1);
    return (tool_item*)liste;
}


typedef struct tlb
{
    GtkWidget *toolbar;
    gboolean show_arrow;//gtk_toolbar_set_show_arrow ()
    GtkIconSize icon_size;//gtk_toolbar_set_icon_size ()
    GtkToolbarStyle tolbar_style;//gtk_toolbar_set_style ()
    tool_item *info;
    //gtk_toolbar_insert ()
}tool_bar;



tool_bar *creer_initilaiser_tool_bar()
{
    tool_bar *tlb;
    tlb=(tool_bar*)malloc(sizeof (tool_bar));
    if(!tlb)
    {
        printf("error allocation \n");
        exit(-1);
    }
    tlb->show_arrow=TRUE;
    tlb->icon_size=GTK_ICON_SIZE_MENU;
    tlb->tolbar_style=GTK_TOOLBAR_TEXT;
    tlb->info=NULL;
    tlb->toolbar=NULL;
    return (tool_bar*)tlb;
}

tool_bar *remplir_tool_bar(FILE *f1)
{
    tool_bar *tlbar=creer_initilaiser_tool_bar();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (tool_bar*)tlbar;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"shoar")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)tlbar->show_arrow=TRUE;
            else tlbar->show_arrow=FALSE;
        }else if(strcmp(p->attribut->info,"icsiz")==0)
        {
            if(strcmp(p->valeur->info,"GTK_ICON_SIZE_INVALID")==0)tlbar->icon_size=GTK_ICON_SIZE_INVALID;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_SMALL_TOOLBAR")==0)tlbar->icon_size=GTK_ICON_SIZE_SMALL_TOOLBAR;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_LARGE_TOOLBAR")==0)tlbar->icon_size=GTK_ICON_SIZE_LARGE_TOOLBAR;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_BUTTON")==0)tlbar->icon_size=GTK_ICON_SIZE_BUTTON;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_DND")==0)tlbar->icon_size=GTK_ICON_SIZE_DND;
            else if(strcmp(p->valeur->info,"GTK_ICON_SIZE_DIALOG")==0)tlbar->icon_size=GTK_ICON_SIZE_DIALOG;
        }else if(strcmp(p->attribut->info,"styl")==0)
        {
            if(strcmp(p->valeur->info,"GTK_TOOLBAR_ICONS")==0)tlbar->tolbar_style=GTK_TOOLBAR_ICONS;
            else if(strcmp(p->valeur->info,"GTK_TOOLBAR_BOTH")==0)tlbar->tolbar_style=GTK_TOOLBAR_BOTH;
            else if(strcmp(p->valeur->info,"GTK_TOOLBAR_BOTH_HORIZ")==0)tlbar->tolbar_style=GTK_TOOLBAR_BOTH_HORIZ;
        }
        p=p->suiv;
    }
    tlbar->info=remplir_tool_item_liste(f1);
    return (tool_bar*)tlbar;
}


void make_tool_bar(tool_bar *tlb)
{
    GtkWidget *tlbr=gtk_toolbar_new();
    GtkToolItem *tooli;
    gtk_toolbar_set_show_arrow(GTK_TOOLBAR(tlbr),tlb->show_arrow);
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(tlbr),tlb->icon_size);
    gtk_toolbar_set_style(GTK_TOOLBAR(tlbr),tlb->tolbar_style);
    tool_item *p=tlb->info;
    while (p)
    {
        tooli= gtk_tool_button_new(NULL,NULL);
        gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(tooli),p->underline);
        if(p->label->Idelem!=-1)
        {
            if(strcmp(p->label->info,"separtor")==0)tooli=gtk_separator_tool_item_new();
            else gtk_tool_button_set_label(GTK_TOOL_BUTTON(tooli),p->label->info);
        }else if(p->icon_name->Idelem!=-1)gtk_tool_button_set_icon_name(GTK_TOOL_BUTTON(tooli),p->icon_name->info);

        if(p->tooltip_markup->Idelem!=-1)gtk_tool_item_set_tooltip_markup(tooli,p->tooltip_markup->info);
        else if(p->tooltip_text->Idelem!=-1)gtk_tool_item_set_tooltip_text(tooli,p->tooltip_text->info);
        gtk_toolbar_insert(GTK_TOOLBAR(tlbr),tooli,-1);
        p=p->suiv;
    }
    tlb->toolbar=tlbr;
}



typedef struct cmb
{
    info *text;//gtk_combo_box_text_append_text()
    info *visible;
    struct cmb *suiv;
}combo_box_text;

combo_box_text *creer_initaliser_combo_box_text()
{
    combo_box_text *ne;
    ne=(combo_box_text*) malloc(sizeof (combo_box_text));
    if(!ne)
    {
        printf("error allocation \n");
        exit(-1);
    }
    ne->text=creer_initialiser_info();
    ne->visible=creer_initialiser_info();
    ne->suiv=NULL;
    return (combo_box_text*)ne;
}

combo_box_text *remplir_com_liste(FILE *f1)
{
    char mot[30];
    char text[30];
    obj_att *liste1=NULL,*p=NULL;
    char c;
    combo_box_text *liste=NULL;
    combo_box_text *ne=NULL;
    fscanf(f1,"%35[^ ] ",mot);
    liste1=lire_att(f1);
    liste=creer_initaliser_combo_box_text();
    p=liste1;
    while (p)
    {
        if(strcmp(p->attribut->info,"value")==0)remplir_info(liste->text,p->valeur->info);
        else if(strcmp(p->attribut->info,"visib")==0)remplir_info(liste->visible,p->valeur->info);
        p=p->suiv;
    }
    fscanf(f1,"%35[^>]>\n",mot);
    epurer_blan(f1);
    fscanf(f1,"<%35[^ ] ",mot);
    while (strcmp(mot,"/combotext")!=0)
    {
        liste1=lire_att(f1);
        ne=creer_initaliser_combo_box_text();
        p=liste1;
        while (p)
        {
            if(strcmp(p->valeur->info,"value")==0)remplir_info(ne->text,liste1->suiv->valeur->info);
            else if(strcmp(p->valeur->info,"visib")==0)remplir_info(ne->visible,liste1->valeur->info);
            p=p->suiv;
        }
        ne->suiv=liste;
        liste=ne;
        fscanf(f1,"%35[^>]>\n",mot);
        epurer_blan(f1);
        fscanf(f1,"<%35[^ ] ",mot);
    }
    fscanf(f1,">\n");
    epurer_blan(f1);
    return (combo_box_text*)liste;
}


GtkWidget *make_combo_box_text(combo_box_text *info)
{
    GtkWidget *cmtext=gtk_combo_box_text_new ();
    combo_box_text *p=info;
    while (p)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(cmtext),p->text->info);
        if(p->visible->Idelem!=-1)
        {
            gtk_combo_box_set_active_id(GTK_COMBO_BOX(cmtext),p->text->info);
            gtk_combo_box_set_active(GTK_COMBO_BOX(cmtext),0);
        }
        p=p->suiv;
    }
    gtk_widget_set_size_request(cmtext,100,40);
    return (GtkWidget *)cmtext;
}

typedef struct ent
{
    GtkWidget *entry;
    info *text;//gtk_entry_set_text ()
    gboolean visibilty;//gtk_entry_set_visibility ()
    gint max_entry;//gtk_entry_set_max_length ()
    gboolean frame;//gtk_entry_set_has_frame ()
    gfloat alignement;//gtk_entry_set_alignment ()
    info *placeholder;//gtk_entry_set_placeholder_text ()
    gboolean overwriten;//gtk_entry_set_overwrite_mode ()
    info *icon_name;//gtk_entry_set_icon_from_icon_name()
    GtkEntryIconPosition icon_position;
    info *tool_tip_text_icon;//gtk_entry_set_icon_tooltip_text ()
    GtkInputPurpose purpose;//gtk_entry_set_input_purpose ()
    GtkInputHints inputhints;//gtk_entry_set_input_hints()
}entry;

entry *crrer_initialser_entry()
{
    entry *ent;
    ent=(entry*)malloc(sizeof (entry));
    if(!ent)
    {
        printf("error allocation \n");
        exit(-1);
    }
    ent->text=creer_initialiser_info();
    ent->placeholder=creer_initialiser_info();
    ent->icon_name=creer_initialiser_info();
    ent->tool_tip_text_icon=creer_initialiser_info();
    ent->max_entry=80;
    ent->frame=TRUE;
    ent->alignement=0;
    ent->overwriten=TRUE;
    ent->purpose=GTK_INPUT_PURPOSE_EMAIL;
    ent->inputhints=GTK_INPUT_HINT_NONE;
    ent->entry=NULL;
    ent->icon_position=GTK_ENTRY_ICON_PRIMARY;
    ent->visibilty=TRUE;
    return (entry*)ent;
}


entry *remplir_entry(FILE *f1)
{
    entry *ent=crrer_initialser_entry();
    int val;
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (entry*)ent;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"text")==0)remplir_info(ent->text,p->valeur->info);
        else if(strcmp(p->attribut->info,"vis")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)ent->visibilty=TRUE;
            else ent->visibilty=FALSE;
        }else if(strcmp(p->attribut->info,"mxen")==0)ent->max_entry=atoi(p->valeur->info);
        else if(strcmp(p->attribut->info,"fram")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)ent->frame=TRUE;
            else ent->frame=FALSE;
        }else if(strcmp(p->valeur->info,"alig")==0)ent->alignement= atof(p->valeur->info);
        else if(strcmp(p->valeur->info,"plah")==0)remplir_info(ent->placeholder,p->valeur->info);
        else if(strcmp(p->valeur->info,"ovew")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)ent->overwriten=TRUE;
            else ent->overwriten=FALSE;
        }else if(strcmp(p->attribut->info,"icpos")==0)
        {
            if(strcmp(p->valeur->info,"GTK_ENTRY_ICON_SECONDARY")==0)ent->icon_position=GTK_ENTRY_ICON_SECONDARY;
            else ent->icon_position=GTK_ENTRY_ICON_PRIMARY;
        }else if(strcmp(p->attribut->info,"icnam")==0)remplir_info(ent->icon_name,p->valeur->info);
        else if(strcmp(p->attribut->info,"tooltex")==0)remplir_info(ent->tool_tip_text_icon,p->valeur->info);
        else if(strcmp(p->attribut->info,"purp")==0)
        {
            if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_FREE_FORM")==0)ent->purpose=GTK_INPUT_PURPOSE_FREE_FORM;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_DIGITS")==0)ent->purpose=GTK_INPUT_PURPOSE_DIGITS;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_NUMBER")==0)ent->purpose=GTK_INPUT_PURPOSE_NUMBER;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_PHONE")==0)ent->purpose=GTK_INPUT_PURPOSE_PHONE;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_URL")==0)ent->purpose=GTK_INPUT_PURPOSE_URL;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_EMAIL")==0)ent->purpose=GTK_INPUT_PURPOSE_EMAIL;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_NAME")==0)ent->purpose=GTK_INPUT_PURPOSE_NAME;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_PASSWORD")==0)ent->purpose=GTK_INPUT_PURPOSE_PASSWORD;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_PIN")==0)ent->purpose=GTK_INPUT_PURPOSE_PIN;
            else if(strcmp(p->valeur->info,"GTK_INPUT_PURPOSE_TERMINAL")==0)ent->purpose=GTK_INPUT_PURPOSE_TERMINAL;
        }else if(strcmp(p->attribut->info,"hints")==0)
        {
            if(strcmp(p->valeur->info,"GTK_INPUT_HINT_LOWERCASE")==0)ent->inputhints=GTK_INPUT_HINT_LOWERCASE;
            else if(strcmp(p->valeur->info,"GTK_INPUT_HINT_UPPERCASE_CHARS")==0)ent->inputhints=GTK_INPUT_HINT_UPPERCASE_CHARS;
            else if(strcmp(p->valeur->info,"GTK_INPUT_HINT_UPPERCASE_WORDS")==0)ent->inputhints=GTK_INPUT_HINT_UPPERCASE_WORDS;
            else if(strcmp(p->valeur->info,"GTK_INPUT_HINT_UPPERCASE_SENTENCES")==0)ent->inputhints=GTK_INPUT_HINT_UPPERCASE_SENTENCES;
            else if(strcmp(p->valeur->info,"GTK_INPUT_HINT_INHIBIT_OSK")==0)ent->inputhints=GTK_INPUT_HINT_INHIBIT_OSK;
            else if(strcmp(p->valeur->info,"GTK_INPUT_HINT_SPELLCHECK")==0)ent->inputhints=GTK_INPUT_HINT_SPELLCHECK;
            else if(strcmp(p->valeur->info,"GTK_INPUT_HINT_NO_EMOJI")==0)ent->inputhints=GTK_INPUT_HINT_NO_EMOJI;
        }
        p=p->suiv;
    }
    return (entry*)ent;
}

void make_entry(entry *entry_info)
{
    GtkWidget *ent=gtk_entry_new ();
    if(entry_info->text->Idelem!=-1)gtk_entry_set_text(GTK_ENTRY(ent),entry_info->text->info);
    gtk_entry_set_visibility(GTK_ENTRY(ent),entry_info->visibilty);
    gtk_entry_set_max_length(GTK_ENTRY(ent),entry_info->max_entry);
    gtk_entry_set_has_frame(GTK_ENTRY(ent),entry_info->frame);
    gtk_entry_set_alignment(GTK_ENTRY(ent),entry_info->alignement);
    if(entry_info->placeholder->Idelem!=-1)gtk_entry_set_placeholder_text(GTK_ENTRY(ent),entry_info->placeholder->info);
    gtk_entry_set_overwrite_mode(GTK_ENTRY(ent),entry_info->overwriten);
    if(entry_info->icon_name->Idelem!=-1)
        gtk_entry_set_icon_from_icon_name(GTK_ENTRY(ent),entry_info->icon_position,entry_info->icon_name->info);
    if(entry_info->tool_tip_text_icon->Idelem!=-1)gtk_entry_set_icon_tooltip_text(GTK_ENTRY(ent),entry_info->icon_position,entry_info->tool_tip_text_icon->info);
    gtk_entry_set_input_purpose(GTK_ENTRY(ent),entry_info->purpose);
    gtk_entry_set_input_hints(GTK_ENTRY(ent),entry_info->inputhints);
//    gtk_widget_set_size_request(ent,40,40);
//    gtk_widget_set_halign(ent,  GTK_ALIGN_CENTER);
//    gtk_widget_set_size_request(ent,200,40);
//    gtk_widget_set_halign(ent,  GTK_ALIGN_CENTER);
    entry_info->entry=ent;
}



typedef struct srb
{
    GtkWidget *serch_bar;
    gboolean search_mod;//gtk_search_bar_set_search_mode ()
    gboolean shwo_close_button;//gtk_search_bar_set_show_close_button ()
    //gtk_search_bar_connect_entry ()
}search_bar;

search_bar *creer_intialiser_search_bar()
{
    search_bar *srchb;
    srchb=(search_bar*)malloc(sizeof (search_bar));
    if(!srchb)
    {
        printf("error allocation \n");
        exit(-1);
    }
    srchb->search_mod=TRUE;
    srchb->shwo_close_button=TRUE;
    srchb->serch_bar=NULL;
    return (search_bar*)srchb;
}


search_bar *remplir_search_bar(FILE *f1)
{
    char mot[30];
    search_bar *srb=creer_intialiser_search_bar();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (search_bar*)srb;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"seamod")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)srb->search_mod=TRUE;
            else srb->search_mod=FALSE;
        }else if(strcmp(p->attribut->info,"shclb")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)srb->shwo_close_button=TRUE;
            else srb->shwo_close_button=FALSE;
        }
        p=p->suiv;
    }
    return (search_bar*)srb;
}

void make_search_bar(search_bar *searchBar_info)
{
    GtkWidget *srch=gtk_search_bar_new();
    gtk_search_bar_set_search_mode(GTK_SEARCH_BAR(srch),searchBar_info->search_mod);
    gtk_search_bar_set_show_close_button(GTK_SEARCH_BAR(srch),searchBar_info->shwo_close_button);
    gtk_widget_set_size_request(srch,200,40);
    gtk_widget_set_halign(srch,  GTK_ALIGN_CENTER);
    searchBar_info->serch_bar=srch;
}


typedef struct bx
{
    GtkWidget *box1;
    info *bord;//border
    gboolean homogeneous;//gtk_box_set_homogeneous()
    GtkOrientation orientation;
    gint spacing;//gtk_box_set_spacing ()
    GtkBaselinePosition basline_position;//gtk_box_set_baseline_position ()
    //gtk_box_set_center_widget()
    //gtk_box_pack_start ()
    //gtk_box_pack_end ()
}box;

box *creer_initialiser_box()
{
    box *bx;
    bx=(box*)malloc(sizeof (box));
    if(!bx)
    {
        printf("error allocation \n");
        exit(-1);
    }
    bx->bord=creer_initialiser_info();
    bx->basline_position=GTK_BASELINE_POSITION_TOP;
    bx->spacing=0;
    bx->orientation=GTK_ORIENTATION_HORIZONTAL;
    bx->homogeneous=FALSE;
    bx->box1=NULL;
    return (box*)bx;
}

box *remplir_box(FILE *f1)
{
    char mot[30];
    int val;
    box *bx=creer_initialiser_box();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (box*)bx;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"hom")==0)
        {
            if(strcmp(p->valeur->info,"TRUE")==0)bx->homogeneous=TRUE;
            else bx->homogeneous=FALSE;
        }else if(strcmp(p->attribut->info,"orien")==0)
        {
            if(strcmp(p->valeur->info,"GTK_ORIENTATION_VERTICAL")==0)bx->orientation=GTK_ORIENTATION_VERTICAL;
            else if(strcmp(p->valeur->info,"GTK_ORIENTATION_HORIZONTAL")==0)bx->orientation=GTK_ORIENTATION_HORIZONTAL;
        }else if(strcmp(p->attribut->info,"spac")==0)bx->spacing= atoi(p->valeur->info);
        else if(strcmp(p->attribut->info,"baslp")==0)
        {
            if(strcmp(p->valeur->info,"GTK_BASELINE_POSITION_CENTER")==0)bx->basline_position=GTK_BASELINE_POSITION_CENTER;
            else if(strcmp(p->valeur->info,"GTK_BASELINE_POSITION_BOTTOM")==0)bx->basline_position=GTK_BASELINE_POSITION_BOTTOM;
        }else if(strcmp(p->attribut->info,"border")==0)remplir_info(bx->bord,p->valeur->info);

        p=p->suiv;
    }
    return (box*)bx;
}

void make_box(box *box_info)
{
    GtkWidget *bxx=gtk_box_new (box_info->orientation,box_info->spacing);
    gtk_box_set_baseline_position(GTK_BOX(bxx),box_info->basline_position);
    gtk_box_set_homogeneous(GTK_BOX(bxx),box_info->homogeneous);
    if(box_info->bord->Idelem!=-1)
    {
        GtkStyleContext* context = gtk_widget_get_style_context(bxx);
        GtkCssProvider* provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, box_info->bord->info, -1, NULL);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    box_info->box1=bxx;
}


typedef struct gr
{
    GtkWidget *gri;
    gint rowspacing;//gtk_grid_set_row_spacing ()
    gint colimnspacing;//gtk_grid_set_column_spacing ()
    gint row_baseline;//gtk_grid_set_baseline_row()
    //gtk_grid_attach()
}grid;


grid *creer_initialiser_grid()
{
    grid *gr;
    gr=(grid*)malloc(sizeof(grid));
    if(!gr)
    {
        printf("error allocation \n");
        exit(-1);
    }
    gr->rowspacing=20;
    gr->colimnspacing=20;
    gr->row_baseline=0;
    gr->gri=NULL;
    return (grid*)gr;
}

grid *remplir_grid(FILE *f1)
{
    char mot[30];
    grid *gr=creer_initialiser_grid();
    obj_att *liste=NULL,*p=NULL;
    liste= lire_att(f1);
    if(!liste)return (grid *)gr;
    p=liste;
    while (p)
    {
        if(strcmp(p->attribut->info,"rowspa")==0)gr->rowspacing= atoi(p->valeur->info);
        else if(strcmp(p->attribut->info,"colspa")==0)gr->colimnspacing= atoi(p->valeur->info);
        else if(strcmp(p->attribut->info,"spac")==0)gr->row_baseline=atoi(p->valeur->info);
        p=p->suiv;
    }
    return (grid *)gr;
}


void make_grid(grid *info_grid)
{
    GtkWidget *gr=gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gr),info_grid->rowspacing);
    gtk_grid_set_column_spacing(GTK_GRID(gr),info_grid->colimnspacing);
    gtk_grid_set_baseline_row(GTK_GRID(gr),info_grid->row_baseline);
    info_grid->gri=gr;
}

typedef struct fvb
{
    GSList *gg;
    struct fvb *suiv;
}glist;

typedef struct gmlt
{
    glist *gliste_t;
    GtkWidget *menu_button;
}tab_glist;


glist *crer_glist(GSList *group)
{
    glist *ne;
    ne=(glist*)malloc(sizeof (glist));
    ne->gg=group;
    ne->suiv=NULL;
    return (glist*)ne;
}

tab_glist *creer_intialiser_tab_glist()
{
    tab_glist *li;
    glist *ne= crer_glist(NULL);
    li=(tab_glist*)malloc(sizeof(tab_glist));
    li->gliste_t=ne;
    li->menu_button=NULL;
    return (tab_glist*)li;
}

int inserer_tab_glist(tab_glist *ll,GSList *group)
{
    glist *ne= crer_glist(group);
    if(!ll->gliste_t)
    {
        ll->gliste_t=ne;
        return (int)1;
    }
    ne->suiv=ll->gliste_t;
    ll->gliste_t=ne;
    return (int)1;
}





GtkWidget *principale_function(FILE *f1,tab_glist *infoo)
{
    char mot[35];
    char mot1[35];
    char mot2[35];
    fscanf(f1,"<%35[^ ] ",mot);
//    printf("%s\n",mot);
    int k,j,w,h;
    if(strcmp(mot,"window")==0)
    {
        window *stwin;
        stwin=remplir_window(f1);
        make_window(stwin);
        fscanf(f1,"<%35[^ ] ",mot);
        fscanf(f1,"%35[^>]>\n",mot1);
        epurer_blan(f1);
        do {
            if(strcmp(mot1,"headerbar")==0){

                gtk_window_set_titlebar(GTK_WINDOW(stwin->win), principale_function(f1,infoo));
            }
            else if(strcmp(mot1,"container-add")==0)
            {
                gtk_container_add(GTK_CONTAINER(stwin->win),principale_function(f1,infoo));
            }
            fscanf(f1,"<%35[^>]>\n",mot2);
            epurer_blan(f1);
            fscanf(f1,"<%35[^ ] ",mot);
            if(strcmp(mot,"child")==0)
            {
                fscanf(f1,"%35[^>]>\n",mot1);
                epurer_blan(f1);
            }
        }while(strcmp(mot,"child")==0);
        fscanf(f1,">\n");
        epurer_blan(f1);
        return (GtkWidget*)stwin->win;
    }else if(strcmp(mot,"headbar")==0)
    {
        header_bar *shdb;
        shdb= remplir_header_bar(f1);
        make_headerbar(shdb);
        fscanf(f1,"%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)shdb->header_bar;
    }else if(strcmp(mot,"box")==0)
    {
        box *sbx;
        sbx=remplir_box(f1);
        make_box(sbx);
        fscanf(f1,"<%35[^ ] ",mot);
        fscanf(f1,"%35[^>]>\n",mot1);
        epurer_blan(f1);
        do {
            if(strcmp(mot1,"pack-start")==0){
                gtk_box_pack_start(GTK_BOX(sbx->box1),principale_function(f1,infoo),FALSE,TRUE,0);
            }
            else if(strcmp(mot1,"pack-end")==0)
            {
                gtk_box_pack_end(GTK_BOX(sbx->box1),principale_function(f1,infoo),FALSE,FALSE,0);
            }
            fscanf(f1,"<%35[^>]>\n",mot2);
            epurer_blan(f1);
            fscanf(f1,"<%35[^ ] ",mot);
            if(strcmp(mot,"child")==0)
            {
                fscanf(f1,"%35[^>]>\n",mot1);
                epurer_blan(f1);
            }
        }while(strcmp(mot,"child")==0);
        fscanf(f1,">\n");
        epurer_blan(f1);
        gtk_widget_show_all(sbx->box1);
        return (GtkWidget*)sbx->box1;
    }else if(strcmp(mot,"layout")==0){
        int val1;
        info * kl=creer_initialiser_info();
        int val2;
        GtkWidget *layout= gtk_layout_new(NULL,NULL);
        fscanf(f1,">\n");
        epurer_blan(f1);
        fscanf(f1,"<%35[^ ] ",mot);
        fscanf(f1,"%35[^>]>\n",mot1);
        sscanf(mot1,"%d|%d",&val1,&val2);
        epurer_blan(f1);
        do {
            gtk_layout_put(GTK_LAYOUT(layout),principale_function(f1,infoo),val1,val2);
            fscanf(f1,"<%35[^>]>\n",mot2);
            epurer_blan(f1);
            fscanf(f1,"<%35[^ ] ",mot);
            if(strcmp(mot,"child")==0)
            {
                fscanf(f1,"%35[^>]>\n",mot1);
                epurer_blan(f1);
                remplir_info(kl,mot1);
                str_to_val1_val2(kl,&val1,&val2);
            }
        }while(strcmp(mot,"child")==0);
        fscanf(f1,">\n");
        epurer_blan(f1);
        gtk_widget_show_all(layout);
        return (GtkWidget*)layout;
    }else if(strcmp(mot,"notebook")==0){
        fpos_t p;
        GtkWidget *notbk=gtk_notebook_new();
        fscanf(f1,">\n");
        epurer_blan(f1);
        do {
            gtk_notebook_append_page(GTK_NOTEBOOK(notbk),principale_function(f1,infoo),principale_function(f1,infoo));
            fgetpos(f1,&p);
            fscanf(f1,"<%35[^ ] ",mot);
            if(strcmp(mot,"/notebook")!=0)
            {
                fsetpos(f1,&p);
            }
        }while(strcmp(mot,"/notebook")!=0);
        fscanf(f1,">\n");
        epurer_blan(f1);
        gtk_widget_set_size_request(notbk,400,400);
        return (GtkWidget*)notbk;
    }else if(strcmp(mot,"fixed")==0){
        int val1;
        info * kl=creer_initialiser_info();
        int val2;
        GtkWidget *fix= gtk_fixed_new();
        fscanf(f1,">\n");
        epurer_blan(f1);
        fscanf(f1,"<%35[^ ] ",mot);
        fscanf(f1,"%35[^>]>\n",mot1);
        sscanf(mot1,"%d|%d",&val1,&val2);
        epurer_blan(f1);
        do {
            gtk_fixed_put(GTK_FIXED(fix),principale_function(f1,infoo),val1,val2);
            fscanf(f1,"<%35[^>]>\n",mot2);
            epurer_blan(f1);
            fscanf(f1,"<%35[^ ] ",mot);
            if(strcmp(mot,"child")==0)
            {
                fscanf(f1,"%35[^>]>\n",mot1);
                epurer_blan(f1);
                remplir_info(kl,mot1);
                str_to_val1_val2(kl,&val1,&val2);
            }
        }while(strcmp(mot,"child")==0);
        fscanf(f1,">\n");
        epurer_blan(f1);
        gtk_widget_show_all(fix);
        return (GtkWidget*)fix;
    }else if(strcmp(mot,"toolbar")==0)
    {
        tool_bar *stlb;
        stlb= remplir_tool_bar(f1);
        make_tool_bar(stlb);
        return (GtkWidget*)stlb->toolbar;
    }else if(strcmp(mot,"entry")==0)
    {
        entry *sent;
        sent= remplir_entry(f1);
        make_entry(sent);
        fscanf(f1,"%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)sent->entry;
    }else if(strcmp(mot,"btn")==0)
    {
        button *sbtn;
        sbtn= remplir_button(f1);
        make_button(sbtn);
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)sbtn->button;
    }else if(strcmp(mot,"menu")==0)
    {
        menu *smn;
        smn= remplir_menu(f1);
        make_menu(smn);
        gtk_widget_show_all(smn->menu);
        infoo->menu_button=smn->menu;
        return (GtkWidget*)smn->menu;
    }else if(strcmp(mot,"combotext")==0)
    {
        GtkWidget *cmt;
        combo_box_text *scmt=NULL;
        fscanf(f1,">\n");
        scmt=remplir_com_liste(f1);
        cmt=make_combo_box_text(scmt);
        return (GtkWidget*)cmt;
    }else if(strcmp(mot,"linkbutton")==0)
    {
        link_button *slkb;
        slkb= remplir_link_button(f1);
        make_link_button(slkb);
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)slkb->link_button;
    }else if(strcmp(mot,"entry")==0)
    {
        entry *sent= remplir_entry(f1);
        make_entry(sent);
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)sent->entry;
    }else if(strcmp(mot,"tglbutton")==0)
    {
        toggle_button *stglb= remplir_toggle_button(f1);
        make_toggle_button(stglb);
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)stglb->toogle_button;
    }else if(strcmp(mot,"rdbutton")==0)
    {
        radio_button *srdb;
        GSList *group;
        srdb= remplir_radio_button(f1);
        if(srdb->main_rdb)make_radio_button(srdb,NULL);
        else make_radio_button(srdb,infoo->gliste_t->gg);
        group=gtk_radio_button_get_group (GTK_RADIO_BUTTON(srdb->radio_button));
        inserer_tab_glist(infoo,group);
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)srdb->radio_button;
    }else if(strcmp(mot,"ckbutton")==0)
    {
        check_button *schk;
        schk= remplir_check_button(f1);
        make_check_button(schk);
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)schk->checkbutton;
    }else if(strcmp(mot,"image")==0)
    {
        image *sim;
        sim= remplir_image(f1);
        make_image(sim);
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)sim->image;
    }else if(strcmp(mot,"label")==0)
    {
        label *slb;
        slb= remplir_label(f1);
        make_label(slb);
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)slb->label;
    }else if(strcmp(mot,"grid")==0)
    {
        grid *sgr;
        sgr= remplir_grid(f1);
        make_grid(sgr);
        fscanf(f1,"<%35[^ ] ",mot);
        fscanf(f1,"%35[^,],%d,%d,%d,%d>\n",mot1,&k,&j,&w,&h);
        epurer_blan(f1);
        do {
            gtk_grid_attach(GTK_GRID(sgr->gri), principale_function(f1,infoo),k,j,w,h);
            fscanf(f1,"<%35[^>]>\n",mot2);
            epurer_blan(f1);
            fscanf(f1,"<%35[^ ] ",mot);
            if(strcmp(mot,"child")==0)
            {
                fscanf(f1,"%35[^,],%d,%d,%d,%d>\n",mot1,&k,&j,&w,&h);
                epurer_blan(f1);
            }
        }while(strcmp(mot,"child")==0);
        fscanf(f1,">\n");
        epurer_blan(f1);
        return (GtkWidget*)sgr->gri;
    }else if(strcmp(mot,"search_bar")==0)
    {
        search_bar *ssrch;
        ssrch= remplir_search_bar(f1);
        make_search_bar(ssrch);
        gtk_search_bar_connect_entry(GTK_SEARCH_BAR(ssrch->serch_bar), GTK_ENTRY(principale_function(f1,infoo)));
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)ssrch->serch_bar;
    }else if(strcmp(mot,"mnbutton")==0)
    {
        menu_button *smnbt;
        smnbt= remplir_menu_button(f1);
        make_menu_button(smnbt);
        gtk_menu_button_set_popup(GTK_MENU_BUTTON(smnbt->menu_butt), principale_function(f1,infoo));
        fscanf(f1,"<%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)smnbt->menu_butt;
    }else if(strcmp(mot,"scrollwin")==0)
    {
        GtkWidget *scrol= gtk_scrolled_window_new(NULL,NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrol), GTK_POLICY_ALWAYS,GTK_POLICY_ALWAYS);
        fscanf(f1,">\n");
        epurer_blan(f1);
        fscanf(f1,"<%35[^ ] ",mot);
        fscanf(f1,"%35[^>]>\n",mot1);
        epurer_blan(f1);
        if(strcmp(mot1,"container-add")==0)gtk_container_add(GTK_CONTAINER(scrol), principale_function(f1,infoo));
        fscanf(f1,"%35[^>]>\n",mot1);
        epurer_blan(f1);
        fscanf(f1,"%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)scrol;
    }else if(strcmp(mot,"text")==0)
    {
        GtkWidget *tex=gtk_text_view_new();
        fscanf(f1,">\n");
        epurer_blan(f1);
        fscanf(f1,"%35[^>]>\n",mot1);
        epurer_blan(f1);
        return (GtkWidget*)tex;
    }
    return NULL;
}



void Myffunn()

{
    GtkWidget *win;
    GtkWidget *win1;
    tab_glist *info=creer_intialiser_tab_glist();
    tab_glist *info1=creer_intialiser_tab_glist();
    FILE *f1=NULL;
    f1= fopen("D:\\C projects\\GTK\\exam_tp\\text3.txt","r");
    if(!f1)
    {
        printf("error oppening file \n");
        exit(-1);
    }
    win= principale_function(f1,info);
    win1= principale_function(f1,info1);
    gtk_widget_show_all(win);
    gtk_widget_show_all(win1);
}


int main(int argc,char *argv[])
{
    gtk_init(&argc, &argv);
    Myffunn();
    gtk_main();
    return 0;
}