#include <gtk/gtk.h>


//*Window

typedef struct 
{
	GtkWindowType Type;   //type de fenetre	
	GtkWindowPosition Pos;// position 
	gchar title[25];          //titre 
	gint Hauteur;          //Hauteur
	gint Largeur;          //Largeur  
	gboolean isMain;      //if main	
	gboolean isResisable; //if resisable
	gchar iconPlace[100];      //l'icone
	gboolean isModale;    //if Modale
	gboolean isMaximized;  //if maximized
	gboolean isFullscreen; //if fullscreen
	gboolean isDecorated;  //if decorated
	gboolean isDeletable;  
	GtkWidget*child;
}windowProp;

typedef struct
{
	gchar*title;
}buttonProp;


//Frames
typedef struct
{
	gchar label[25];
	gfloat	label_xalign;
	gfloat	label_yalign;
	GtkShadowType	shadow_type;
	gint Hauteur;          //Hauteur
	gint Largeur;          //Largeur  
	GtkWidget*child;
}FrameProp;


//Boxs 
typedef struct { 
    GtkOrientation   orientation; //l'orientation vertical ou Horizontal 
    gint             spacing;   //espace entre les fils de Box 
    gboolean         isHomogeneous;
}BoxCaracteristique; //snom de la structure 


typedef struct { 
    gint        extremite; //0 = pack start or  1 = pack end 
    gboolean    expand;
    gboolean    fill;
    guint       padding;
}positionnementFils;


typedef struct {
    GtkLayout   *Layout;    //un GtkLayout
    GtkWidget   *child;     //le fils de layout
    gint         x;         //la position x pour se deplacer vers 
    gint         y;         //la position y pour se deplacer vers
}LayoutProps;

typedef struct {
	GtkWidget* menu_barre, *menu_champ, *menu_item;
}menu;

typedef struct
{
	int len;///la longueur d'entrée.
	int width;///largeur.
	gboolean visible;///le texte est vistible ou pas.
	char invis_car;///le caractère d'un text invisible.
	gboolean editable;///on peut saisir par clavier.
	char text_to_set[20];///le texte à saisir.
}Entry;

typedef struct toolbarre
{
	GtkWidget *toolbar;             // barre doutil
	GtkToolItem *toolitem;          // les buttons de la barre doutil
	gchar* icon;                    // licon  dapres le stock
	gpointer *signal;               // signal du button
}tool;

typedef struct Position
{
	int x;          // position sur l'abscisse en pixel
	int y;          // position sur l'ordonnée en pixel
} Position;

typedef struct prImage
{
	GtkWidget *image;       //widget image
	GtkWidget *event_box;   // événement relatif à l'image
	char chemin[20];           //chemin de l'image
	Position *iPos;     //position de l'image
} prImage;
typedef struct
{
	char style[10];///0:italic,1:oblique,3:normal
	char font[26];///police d'ecriture
	char taille[4];///size du texte
	char weight[15];///ultralight,light,normal,bold,ultrabold,heavy
	char color[26];///couleur du texte
	char background[26];///couleur du fond
	char underline[10];///single,double,low ou none
	char underline_color[26];///couleur du soulignement
	char barrer_txt[6];///true ou false
	char color_bar[26];///couleur du
	char select;//0:non ou 1:oui
	char alignement;//0:left,1:right,2:center, ou 3:fill
}Format;

typedef struct
{
	GtkWidget* Label;///pointeur sur le label
	char texte[255];///le texte à afficher
	Format lab;//formatage du texte
}Lab_txt;

typedef struct
{
	char background[26];
	char foreground[26];
	int left_margin;
	char style[26];///italic,oblique,normal
	char weight[26];///normal,gras,light,ultrabold
	int size;
}formatTv;

typedef struct
{
	GtkWidget *textview; /// widget d'un textview.
	GtkTextBuffer *buffer; /// le buffer associé au text view.
	GtkTextIter iter; /// l'itération au niveau du buffer.
	char text[100];
	formatTv t;
}TextView;


//button

typedef struct 
{
	int x;
	int y;
}prPos;


typedef struct prButton
{
    GtkWidget   *button;        // l'objet button
    GtkWidget   *racine;        // le conteneur
    gchar       label[20];         // label du button
    gchar       stock_icon[20];    // button du stock
    char        nom[20];           // nom du button
    int         mnemonic ;      // mnemonic
    int         type_special;    // button special
    char        image[20];
    prPos       *pos;              // position du button
} prSimpleBut ;

typedef struct prRadio
{

	GtkWidget   *radioButton;       /// l'objet button radio
    GtkWidget   *racine;            /// Le conteneur
    GtkWidget   *rad2;              /// le pere du group
	GSList      *group;             /// group des buttons creer
	gchar       label[50];             /// label du button
	char        nom[20];               ///Le nom du radio (pour le css)
	int          mnemonic;          ///1 si creation avec mnemonic, 0 sinon
	prPos       *pos;               ///position du bouton
} prRadioButton;


typedef struct prCheck
{
   GtkWidget   *checkButton;    //Le button
   GtkWidget   *racine;         //Le container du
   gchar       label[100];          //le text du prLabel
   char        nom[20];           //Le nom du checkButton (pour le css)
   int         mnemonic ;      //1 si creation avec mnemonic, 0 sinon
} prCheckButton;



//Table 




typedef struct 
{
  GtkWidget *table;               // la table 
  GtkContainer *fatherContainer;  // le conteneur de la table
  guint rows;                     // nombre de lignes
  guint columns;                  // nombre de colonnes
  guint def_row_spacing;          // spacing par defaut entre les lignes
  guint def_col_spacing;          // spacing par defaut entre les colonnes
  gboolean homogeneous;           // homogenete des cellules 
  struct TChildsProps *children;     // listes des options des fils
}tableProps;

typedef struct TChildsProps
{
  // le widget fils
  GtkWidget *childWidget;    

  // le placement dans la table      
  guint left_attach;              
  guint right_attach;
  guint top_attach;
  guint bottom_attach;
  // options d'attachement
  GtkAttachOptions xoptions;
  GtkAttachOptions yoptions;
  // padding du widget
  guint xpadding;
  guint ypadding;
  // le fils suivant 
  struct TChildsProps *next;
  
}tableChildsProps;


//NoteBook

typedef struct 
{
    GtkWidget *noteBook;
    gboolean isScrollable;
    gboolean showTabs; 
    gboolean showBoders;
    GtkPositionType position; //0: Left, 1: Right, 2: Top, 3: bottom
}NoteBookProps;


typedef struct 
{
    GtkWidget *tab_label; //le titre de l'onglet
}noteBookFils; 


//comboBox 

typedef struct str_liste
{
    char *itemName;
    struct str_liste *next; 
}comboList;

typedef struct 
{
    GtkWidget *comboBox;
    struct str_liste *items;
} comboBoxProps;