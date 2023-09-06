#include <gtk/gtk.h>
#include "checkers.h"
#include "Shader.h"
#include "dyad.h"
#include <pthread.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------MACROS------------------------------------------------------------------------

typedef struct
{
  gint x;
  gint y;
} Size, Pos;

typedef struct
{
  GtkWidget *wind;
  GtkWidget *scrollwind;
  gchar *title; // titre de la fenêtre
  Pos pos;
  Size size;
  gint rouge;
  gint vert;
  gint bleu;
  gboolean isResisable;  // si on peut la redimensionner
  gchar *iconfile;       // l'icône
  gboolean isFullscreen; // si elle est en mode fullscreen
  gboolean isDecorated;  // si elle a des décorations
  gboolean isDeletable;  // s’il est possible de la fermer
  gchar *scroll;

} MaFenetre;

MaFenetre *initF(MaFenetre *wind)
{
  wind = (MaFenetre *)malloc(sizeof(MaFenetre));
  return wind;
}

MaFenetre *Init_default_window_prop(MaFenetre *wind)
{
  wind = initF(wind);
  wind->wind = NULL;
  wind->isResisable = FALSE; // il est possible de changer la taille de la fenêtre.

  return wind;
} // fin de la fonction Init_default_window_prop.

void fenetre(MaFenetre *wind)
{
  wind->wind = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_style_context_add_class(gtk_widget_get_style_context(wind->wind), "fenetre");
  gtk_window_set_default_size(GTK_WINDOW(wind->wind), 1200, 900);
  gtk_window_set_position(GTK_WINDOW(wind->wind), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(wind->wind), TRUE);
}

void connect_headerbar_window(MaFenetre *wind, GtkWidget *hbar)
{
  gtk_window_set_titlebar(GTK_WINDOW(wind->wind), hbar);
}

void add_widget_window(MaFenetre *wind, GtkWidget *widget)
{
  gtk_container_add(GTK_CONTAINER(wind->wind), widget);
}

typedef struct
{
  GtkWidget *hbar;
  gchar *title;
  gboolean showclose;

} Headerbar;

Headerbar *init_headerbar(Headerbar *hbar)
{
  hbar = (Headerbar *)malloc(sizeof(Headerbar));
  hbar->hbar = NULL;
  hbar->showclose = TRUE;
  hbar->title = (char *)"checkers";
  return hbar;
}

void headerbar(Headerbar *hbar)
{
  hbar->hbar = gtk_header_bar_new();
  gtk_header_bar_set_title(GTK_HEADER_BAR(hbar->hbar), hbar->title);
  gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(hbar->hbar), hbar->showclose);
}

void Connect_to_headerbar(GtkWidget *hbar, GtkWidget *widget, gboolean start)
{
  if (start)
    gtk_header_bar_pack_start(GTK_HEADER_BAR(hbar), widget);
  else
    gtk_header_bar_pack_end(GTK_HEADER_BAR(hbar), widget);
}

typedef struct prButton
{
  GtkWidget *button; // l'objet button
  gchar *label;      // label du button
  gchar *stock_icon; // button du stock
  gchar *image;
  Pos pos;
  Size size; // position du button
  gchar *name;
} prSimpleBut;

prSimpleBut *init_button(prSimpleBut *button, gchar *title)
{
  button = (prSimpleBut *)malloc(sizeof(prSimpleBut));
  button->button = NULL;
  button->label = title;
  return button;
}

void create_button(prSimpleBut *button)
{
  button->button = gtk_button_new_with_label(button->label);
}

typedef struct
{
  GtkWidget *combo;
  gint active_index;
} ComboBox;

ComboBox *Init_comobox(ComboBox *combobox)
{
  combobox = (ComboBox *)malloc(sizeof(ComboBox));
  combobox->combo = NULL;
  combobox->active_index = 0;
  return combobox;
}

void create_combobox(ComboBox *combobox)
{
  combobox->combo = gtk_combo_box_text_new();
}

void add_item_combobox(ComboBox *combobox, gchar *title)
{
  gtk_combo_box_text_append_text(GTK_COMBO_BOX(combobox->combo), title);
}

void set_active_item(ComboBox *combobox)
{
  gtk_combo_box_set_active(GTK_COMBO_BOX(combobox->combo), combobox->active_index);
}

typedef struct
{
  GtkWidget *box;
  gchar *orient;
  gint space;
  Pos pos;
  Size size;
  gchar *name;
} Box;

Box *initB(Box *box)
{
  box = (Box *)malloc(sizeof(Box));
  return box;
}

Box *initDefaultbox(Box *box)
{

  box = initB(box);
  box->orient = (char *)"vertical";
  box->space = 0;
  box->size.x = 0;
  box->size.y = 0;
  return box;
}

void BoxC(Box *box)
{
  if (strcmp(box->orient, "vertical") == 0)
    box->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, box->space);
  else if (strcmp(box->orient, "horizontal") == 0)
    box->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, box->space);
  if (box->size.x && box->size.y)
    gtk_widget_set_size_request(box->box, box->size.x, box->size.y);
}

void Connect_to_Box(GtkWidget *box, GtkWidget *widget, gboolean start, gboolean expand, gboolean fill, gint padding)
{
  if (start)
    gtk_box_pack_start(GTK_BOX(box), widget, expand, fill, padding);
  else
    gtk_box_pack_end(GTK_BOX(box), widget, expand, fill, padding);
}

typedef struct
{
  GtkWidget *label;
  gchar *text;
} Label;

Label *initLabel(Label *label)
{
  label = (Label *)malloc(sizeof(Label));
  label->label = NULL;
  label->text = NULL;

  return label;
}

void CreateLabel(Label *label)
{
  label->label = gtk_label_new(label->text);
}

typedef struct
{
  GtkWidget *grid;
  gboolean column_homogeneous;
  gboolean row_homogeneous;
  gint column_spacing;
  gint row_spacing;
  Pos pos;
  Size size;
  gint collumn;
  gint row;
  gchar *name;
} Grid;

Grid *initGrid(Grid *grid)
{
  grid = (Grid *)malloc(sizeof(Grid));
  grid->grid = NULL;
  grid->column_homogeneous = FALSE;
  grid->row_homogeneous = FALSE;
  grid->column_spacing = 10;
  grid->row_spacing = 0;
  grid->size.x = 0;
  grid->size.y = 0;
  grid->collumn = -1;
  grid->row = -1;
  grid->name = (char *)"defaultValue";
  return grid;
}

void CreateGrid(Grid *grid)
{
  grid->grid = gtk_grid_new();
  gtk_grid_set_column_spacing(grid->grid, grid->column_spacing);
}

void LinktoGrid(Grid *grid, GtkWidget *child, gint left, gint top)
{
  gtk_grid_attach(GTK_GRID(grid->grid), child, left, top, 1, 1);
}

void margin_bot(GtkWidget *widget, gint margin)
{
  gtk_widget_set_margin_bottom(widget, margin);
}

void margin_top(GtkWidget *widget, gint margin)
{
  gtk_widget_set_margin_top(widget, margin);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------Game Structs-------------------------------------------------------------------

/*Définition de la structure "Piece" qui représente une pièce du jeu. Elle contient les champs "x" et "y"
pour les coordonnées de la pièce, ainsi que le champ "role" pour le rôle de la pièce.*/
typedef struct piece
{
  int x;
  int y;
  int role;
} Piece;

/*Définition de la structure "Move" qui représente un mouvement dans le jeu.
Elle contient les champs "role" pour le rôle du mouvement, "x1" et "y1" pour les coordonnées de départ de la pièce à déplacer,
"x2" et "y2" pour les coordonnées d'arrivée de la pièce à déplacer.
La structure inclut également un tableau "jumped_on_pieces" pour stocker les pièces capturées,
le champ "nb" pour le nombre de pièces capturées et le champ "score" pour le score du coup (utilisé par l'algorithme minimaxalgo).*/
typedef struct move
{
  int role;
  int x1;                     // Starting row of the piece to be moved
  int y1;                     // Starting column of the piece to be moved
  int x2;                     // Ending row of the piece to be moved
  int y2;                     // Ending column of the piece to be moved
  Piece jumped_on_pieces[20]; // Captured pieces
  int nb;                     // Number of captured pieces
  int score;                  // Score of the move (used by the minimaxalgo algorithm)
} Move;

/*Définition de l'énumération "Turn" qui représente l'état du tour de jeu.
Les valeurs possibles sont "TURN_NONE" (aucun mouvement), "TURN_MOVE" (mouvement simple) et "TURN_TAKE" (mouvement avec capture).*/
typedef enum
{
  TURN_NONE,
  TURN_MOVE,
  TURN_TAKE,
} Turn;

/*Définition de l'énumération "Owner" qui représente le propriétaire d'une case du jeu.
Les valeurs possibles sont "OWNER_NONE" (aucun propriétaire), "OWNER_WHITE" (propriétaire blanc) et "OWNER_BLACK" (propriétaire noir).*/
typedef enum
{
  OWNER_NONE,
  OWNER_WHITE,
  OWNER_BLACK,
} Owner;

/*Définition de l'énumération "Status" qui représente l'état du jeu.
Les valeurs possibles sont "STATUS_NONE" (jeu terminé),
"STATUS_MOVE" (mouvement en cours) et "STATUS_JUMP_AGAIN" (possibilité de sauter à nouveau).*/
typedef enum
{
  STATUS_NONE,
  STATUS_MOVE,
  STATUS_JUMP_AGAIN,
} Status;

struct field;

typedef struct cel
{
  int laststate[8][8];
  int scoreP1;
  int scoreP2;
  struct cel *svt;
} Pile;

/*Définition de la structure "View" qui représente la vue du jeu.
Elle contient différents widgets (composants graphiques) tels que "grid", "hbar", "difficulty", "mode", "box" et "color".
La structure inclut également un tableau "laststate" pour stocker l'état précédent du jeu,
le champ "nbmoves" pour le nombre de mouvements effectués, le tableau "field" pour représenter la grille du jeu,
le champ "last" pour le dernier mouvement effectué, le champ "status" pour l'état du jeu,
les champs "round", "pieces_one" et "pieces_two" pour le décompte des pièces, et le pointeur "client" pour la communication réseau.*/
typedef struct view
{

  GtkWidget *grid;

  GtkWidget *hbar;
  GtkWidget *difficulty;
  GtkWidget *mode;
  GtkWidget *box;
  GtkWidget *color;
  GtkWidget *time;
  GtkWidget *labelTimerPlayer1;
  GtkWidget *labelTimerPlayer2;
  GtkWidget *wind;
  Pile *lastmoves;
  Pile *nextmoves;
  GtkWidget *scoreP1;
  GtkWidget *scoreP2;

  struct field *field[8][8];
  struct field *last;

  Status status;
  gint round;
  gint pieces_one;
  gint pieces_two;
  dyad_Stream *client;
} View;

/*Définition de la structure "Field" qui représente une case du jeu.
Elle contient un widget "button" pour afficher la case graphiquement,
un pointeur "view" vers la vue du jeu, le champ "owner" pour le propriétaire de la case,
le booléen "is_queen" pour indiquer si la pièce est une dame, et les champs "col" et "row" pour les coordonnées de la case.*/
typedef struct field
{

  GtkWidget *button;

  View *view;

  Owner owner;
  gboolean is_queen;
  gint col;
  gint row;
} Field;

/*Définition de la structure "GameSettings" qui représente les paramètres du jeu.
Elle contient un pointeur "view" vers la vue du jeu,
le champ "difficulty" pour la difficulté du jeu, et le champ "mode" pour le mode de jeu.*/
typedef struct gameparameters
{
  View *view;
  gint difficulty;
  gint mode;
} GameSettings;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------Const and variables------------------------------------------------------------

int Game_Ended = 0;                                 // Variable indiquant si le jeu est terminé ou non
int color = 0;                                      // Variable indiquant la couleur du joueur en cours
#define TITLE_MOVE_ONE "Déplacement : Joueur Blanc" // Titre indiquant le déplacement du Joueur Blanc
#define TITLE_MOVE_TWO "Déplacement : Joueur Noir"  // Titre indiquant le déplacement du Joueur Noir
#define TITLE_TAKE_ONE "Prise : Joueur Blanc"       // Titre indiquant la prise effectuée par le Joueur Blanc
#define TITLE_TAKE_TWO "Prise : Joueur Noir"        // Titre indiquant la prise effectuée par le Joueur Noir
#define TITLE_WON_ONE "Le Joueur Blanc a gagné !"   // Titre indiquant que le Joueur Blanc a gagné
#define TITLE_WON_TWO "Le Joueur Noir a gagné !"    // Titre indiquant que le Joueur Noir a gagné

#define EMPTY 0  // Constante représentant une case vide
#define WHITE 1  // Constante représentant une case occupée par une pièce blanche
#define BLACK 2  // Constante représentant une case occupée par une pièce noire
#define WQUEEN 3 // Constante représentant une case occupée par une dame blanche
#define BQUEEN 4 // Constante représentant une case occupée par une dame noire

guint timeout_source_id = 0; // Identifiant pour la source de temporisation
gint shaders = 1;            // Variable indiquant si les shaders sont activés ou non
View *view;                  // Pointeur vers une structure de vue
int connected = 0;           // Variable indiquant si une connexion est établie
dyad_Stream *server;         // Pointeur vers un flux dyad pour le serveur
dyad_Stream *client;         // Pointeur vers un flux dyad pour le client
dyad_Stream *lokman;         // Pointeur vers un flux dyad pour lokman
gint timerIDPlayer1 = 0;
gint timerIDPlayer2 = 0;
gint secondsElapsedPlayer1 = 0;
gint secondsElapsedPlayer2 = 0;
gint totalTime = 5999; // Total time for each player in seconds
gint ScorePlayer1 = 0;
gint ScorePlayer2 = 0;
int timeended = 0;
int myboard[8][8] = { // Tableau représentant l'état du jeu avec les pièces
    {EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK},
    {BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY},
    {EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {WHITE, EMPTY, WHITE, EMPTY, WHITE, EMPTY, WHITE, EMPTY},
    {EMPTY, WHITE, EMPTY, WHITE, EMPTY, WHITE, EMPTY, WHITE},
    {WHITE, EMPTY, WHITE, EMPTY, WHITE, EMPTY, WHITE, EMPTY}};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------Game interface and logic and Connexion----------------------------------------------------

static void load_style_context(void)
{
  GtkCssProvider *css_prov;

  // Crée une nouvelle instance de GtkCssProvider
  css_prov = gtk_css_provider_new();

  // Charge les données CSS à partir de CSS_DATA dans le GtkCssProvider
  gtk_css_provider_load_from_data(css_prov, CSS_DATA, -1, NULL);

  // Ajoute le fournisseur de style à l'écran par défaut avec une priorité d'application
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(css_prov),
                                            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

/*Cette fonction met à jour le statut du jeu.*/
static void view_set_status(View *view, Status status)
{
  view->status = status;

  GtkHeaderBar *hbar;
  const gchar *title;

  if (!color)
  {
    switch (status)
    {
    case STATUS_NONE:
      title = (view->round % 2 == 0) ? TITLE_WON_ONE : TITLE_WON_TWO;
      break;
    case STATUS_MOVE:
      title = (view->round % 2 == 0) ? TITLE_MOVE_ONE : TITLE_MOVE_TWO;
      break;
    case STATUS_JUMP_AGAIN:
      title = (view->round % 2 == 0) ? TITLE_TAKE_ONE : TITLE_TAKE_TWO;
      break;
    }
  }
  else
  {
    switch (status)
    {
    case STATUS_NONE:
      title = (view->round % 2 == 0) ? TITLE_WON_TWO : TITLE_WON_ONE;
      break;
    case STATUS_MOVE:
      title = (view->round % 2 == 0) ? TITLE_MOVE_TWO : TITLE_MOVE_ONE;
      break;
    case STATUS_JUMP_AGAIN:
      title = (view->round % 2 == 0) ? TITLE_TAKE_TWO : TITLE_TAKE_ONE;
      break;
    }
  }

  gtk_header_bar_set_title(GTK_HEADER_BAR(view->hbar), title);
}

void updatescorelabelplayer1(int score)
{
  ScorePlayer1 = score;
  gchar *scoretext = g_strdup_printf("Player 1 score : %d", score);
  gtk_label_set_text(GTK_LABEL(view->scoreP1), scoretext);
  g_free(scoretext);
}

void updatescorelabelplayer2(int score)
{
  ScorePlayer2 = score;
  gchar *scoretext = g_strdup_printf("Player 2 score : %d", score);
  gtk_label_set_text(GTK_LABEL(view->scoreP2), scoretext);
  g_free(scoretext);
}

void stopTimerPlayer1()
{
  if (timerIDPlayer1 != 0)
  {
    g_source_remove(timerIDPlayer1);
    timerIDPlayer1 = 0;
  }
}

void stopTimerPlayer2()
{
  if (timerIDPlayer2 != 0)
  {
    g_source_remove(timerIDPlayer2);
    timerIDPlayer2 = 0;
  }
}

void updateTimerLabelPlayer1()
{
  gint remainingTime = totalTime - secondsElapsedPlayer1;
  gchar *timerText = g_strdup_printf("Player 1 Time: %02d:%02d",
                                     remainingTime / 60,
                                     remainingTime % 60);
  gtk_label_set_text(GTK_LABEL(view->labelTimerPlayer1), timerText);
  g_free(timerText);
}

void updateTimerLabelPlayer2()
{
  gint remainingTime = totalTime - secondsElapsedPlayer2;
  gchar *timerText = g_strdup_printf("Player 2 Time: %02d:%02d",
                                     remainingTime / 60,
                                     remainingTime % 60);
  gtk_label_set_text(GTK_LABEL(view->labelTimerPlayer2), timerText);
  g_free(timerText);
}

gboolean updateTimerPlayer1(gpointer data)
{
  secondsElapsedPlayer1++;
  updateTimerLabelPlayer1();
  if (secondsElapsedPlayer1 >= totalTime)
  {
    // Player 1 has run out of time, end the game
    timeended = 1;
    Game_Ended = 1;
    stopTimerPlayer1();
    stopTimerPlayer2();
    view->round = 0;
    view_set_status(view, STATUS_NONE);

    return G_SOURCE_REMOVE;
  }
  return G_SOURCE_CONTINUE;
}

gboolean updateTimerPlayer2(gpointer data)
{
  secondsElapsedPlayer2++;
  updateTimerLabelPlayer2();
  if (secondsElapsedPlayer2 >= totalTime)
  {
    // Player 2 has run out of time, end the game
    timeended = 1;
    Game_Ended = 1;
    stopTimerPlayer1();
    stopTimerPlayer2();
    view->round = 1;
    view_set_status(view, STATUS_NONE);

    return G_SOURCE_REMOVE;
  }
  return G_SOURCE_CONTINUE;
}

void startTimerPlayer1()
{
  timerIDPlayer1 = g_timeout_add_seconds(1, updateTimerPlayer1, NULL);
}

void startTimerPlayer2()
{
  timerIDPlayer2 = g_timeout_add_seconds(1, updateTimerPlayer2, NULL);
}

void resetTimerPlayer1()
{
  secondsElapsedPlayer1 = 0;
  updateTimerLabelPlayer1();
}

void resetTimerPlayer2()
{
  secondsElapsedPlayer2 = 0;
  updateTimerLabelPlayer2();
}

/*Cette fonction retourne le propriétaire du champ spécifié en utilisant
les coordonnées du champ et les tableaux statiques white_x, white_y, black_x et black_y.*/
Owner fieldGetOwner(Field *field)
{
  gint col, row;
  gint i, k;

  col = field->col;
  row = field->row;

  static gint white_x[3][4] = {
      {0, 2, 4, 6},
      {1, 3, 5, 7},
      {0, 2, 4, 6},
  };

  static gint white_y[3][4] = {
      {7, 7, 7, 7},
      {6, 6, 6, 6},
      {5, 5, 5, 5},
  };

  static gint black_x[3][4] = {
      {1, 3, 5, 7},
      {0, 2, 4, 6},
      {1, 3, 5, 7},
  };

  static gint black_y[3][4] = {
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {2, 2, 2, 2},
  };

  for (i = 0; i < 3; i++)
  {
    for (k = 0; k < 4; k++)
    {
      if (col == white_x[i][k] &&
          row == white_y[i][k])
        return OWNER_WHITE;

      if (col == black_x[i][k] &&
          row == black_y[i][k])
        return OWNER_BLACK;
    }
  }

  return OWNER_NONE;
}

/*Cette fonction initialise un champ en créant un bouton GTK associé,
en définissant le propriétaire du champ en fonction des coordonnées
et en ajoutant des classes de style au contexte du bouton en fonction du propriétaire.*/
static void field_init(Field *field)
{
  field->button = gtk_button_new();
  field->view = NULL;
  field->owner = fieldGetOwner(field);
  field->is_queen = FALSE;

  /* Add style to button */
  gtk_container_set_border_width(GTK_CONTAINER(field->button), 8);
  GtkStyleContext *context = gtk_widget_get_style_context(field->button);
  gtk_style_context_add_class(context, "field");

  switch (field->owner)
  {
  case OWNER_NONE:
    break;
  case OWNER_BLACK:
    if (!color)
      gtk_style_context_add_class(context, "black");
    else
      gtk_style_context_add_class(context, "white");
    break;
  case OWNER_WHITE:
    if (!color)
      gtk_style_context_add_class(context, "white");
    else
      gtk_style_context_add_class(context, "black");
    break;
  }
}

/*Cette fonction retourne le bouton associé au champ spécifié.*/
GtkWidget *field_get_button(Field *field)
{
  return field->button;
}

/*Cette fonction retourne la vue associée au champ spécifié.*/
View *field_get_view(Field *field)
{
  return field->view;
}

/*Cette fonction associe une vue au champ spécifié en mettant à jour le membre view de la structure Field.*/
void field_set_view(Field *field, View *view)
{
  field->view = view;
}

/*Cette fonction retourne le propriétaire du champ spécifié.*/
Owner field_get_owner(Field *field)
{
  return field->owner;
}

/*Cette fonction met à jour le propriétaire du champ spécifié en mettant à jour
le membre owner de la structure Field et en modifiant les classes de style du bouton en conséquence.*/
void field_set_owner(Field *field, Owner owner)
{
  field->owner = owner;

  GtkStyleContext *context;

  context = gtk_widget_get_style_context(field->button);

  switch (owner)
  {
  case OWNER_NONE:
    gtk_style_context_remove_class(context, "black");
    gtk_style_context_remove_class(context, "white");
    gtk_style_context_remove_class(context, "queen");
    break;
  case OWNER_BLACK:
    if (!color)
    {
      gtk_style_context_remove_class(context, "blackpre");
      gtk_style_context_add_class(context, "black");
    }
    else
    {
      gtk_style_context_remove_class(context, "whitepre");
      gtk_style_context_add_class(context, "white");
    }

    break;
  case OWNER_WHITE:
    if (!color)
    {
      gtk_style_context_remove_class(context, "whitepre");
      gtk_style_context_add_class(context, "white");
    }
    else
    {
      gtk_style_context_remove_class(context, "blackpre");
      gtk_style_context_add_class(context, "black");
    }
    break;
  }
}

/*Cette fonction retourne une valeur booléenne indiquant si le champ spécifié est une "reine" ou non.*/
gboolean field_get_is_queen(Field *field)
{
  return field->is_queen;
}

/*Cette fonction met à jour le pre-propriétaire du champ spécifié en modifiant les classes de style du bouton en conséquence.*/
void field_set_preowner(Field *field, Owner owner)
{
  GtkStyleContext *context;

  context = gtk_widget_get_style_context(field->button);

  switch (owner)
  {
  case OWNER_NONE:
    gtk_style_context_remove_class(context, "blackpre");
    gtk_style_context_remove_class(context, "whitepre");
    break;
  case OWNER_BLACK:
    if (!color)
      gtk_style_context_add_class(context, "blackpre");
    else
      gtk_style_context_add_class(context, "whitepre");
    break;
  case OWNER_WHITE:
    if (!color)
      gtk_style_context_add_class(context, "whitepre");
    else
      gtk_style_context_add_class(context, "blackpre");
    break;
  }
}

/*Cette fonction met à jour le statut "reine" du champ spécifié en modifiant le membre is_queen
de la structure Field et en ajoutant ou supprimant la classe de style "queen" du bouton en conséquence.*/
void field_set_is_queen(Field *field,
                        gboolean is_queen)
{
  field->is_queen = is_queen;
  if (is_queen)
  {
    GtkStyleContext *context = gtk_widget_get_style_context(field->button);
    gtk_style_context_add_class(context, "queen");
  }
}

/*Cette fonction retourne la colonne du champ spécifié.*/
gint field_get_col(Field *field)
{
  return field->col;
}

/*Cette fonction retourne la ligne du champ spécifié.*/
gint field_get_row(Field *field)
{
  return field->row;
}

/*Cette fonction crée et initialise un champ avec les coordonnées spécifiées, en utilisant la fonction field_init pour l'initialisation.*/
Field *init_field_inPos(gint col,
                        gint row)
{
  Field *field;

  field = g_malloc(sizeof(Field));

  field->col = col;
  field->row = row;

  field_init(field);

  return field;
}

//---------------------------------------------------------------Connexion----------------------------------------------------------

/*Cette fonction envoie une matrice de l'état du jeu d'un pair à un autre.*/
void sendarray()
{
  gint i, k;
  Owner owner, isqueen;
  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      owner = field_get_owner(view->field[k][i]);
      isqueen = field_get_is_queen(view->field[k][i]);
      if (owner == OWNER_WHITE)
      {
        if (isqueen)
          myboard[i][k] = WQUEEN;
        else
          myboard[i][k] = WHITE;
      }
      else if (owner == OWNER_BLACK)
      {
        if (isqueen)
          myboard[i][k] = BQUEEN;
        else
          myboard[i][k] = BLACK;
      }
      else if (owner == OWNER_NONE)
        myboard[i][k] = EMPTY;
    }
  }
  int temp[8][8];

  // Create a temporary matrix to store the transformed board
  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      switch (myboard[8 - 1 - i][8 - 1 - k])
      {
      case 0:
        temp[i][k] = 0;
        break;
      case 1:
        temp[i][k] = 2;
        break;
      case 2:
        temp[i][k] = 1;
        break;
      case 3:
        temp[i][k] = 4;
        break;
      case 4:
        temp[i][k] = 3;
        break;
      }
    }
  }
  dyad_write(lokman, temp, sizeof(myboard));
}

/*Cette fonction imprime l'ip du serveur auquel le pair est connecté.*/
static void onConnect(dyad_Event *e)
{
  printf("connected: %s  %s\n", e->msg, dyad_getAddress(e->stream));
  connected = 1;
}

/*Cette fonction recoit la matrice est l'applique sur le jeu.*/
static void onData(dyad_Event *e)
{

  if (e->size == sizeof(myboard))
  {
    memcpy(myboard, e->data, sizeof(myboard));

    gint i, k;
    view->pieces_one = 0;
    view->pieces_two = 0;
    for (i = 0; i < 8; i++)
    {
      for (k = 0; k < 8; k++)
      {
        field_set_owner(view->field[k][i], OWNER_NONE);
        field_set_is_queen(view->field[k][i], FALSE);
        switch (myboard[i][k])
        {
        case 1:
          field_set_owner(view->field[k][i], OWNER_WHITE);
          view->pieces_one++;
          break;
        case 2:
          field_set_owner(view->field[k][i], OWNER_BLACK);
          view->pieces_two++;
          break;
        case 3:
          field_set_owner(view->field[k][i], OWNER_WHITE);
          field_set_is_queen(view->field[k][i], TRUE);
          view->pieces_one++;
          break;
        case 4:
          field_set_owner(view->field[k][i], OWNER_BLACK);
          field_set_is_queen(view->field[k][i], TRUE);
          view->pieces_two++;
          break;
        }
      }
    }

    if (view->pieces_one == 0 || view->pieces_two == 0)
    {
      view_set_status(view, STATUS_NONE);
      return;
    }

    view->round++;
    view_set_status(view, STATUS_MOVE);
  }
  else
    printf("[jalal] %s", e->data);
}

static void onData1(dyad_Event *e)
{
  dyad_write(e->stream, e->data, e->size);
}

/*Cette fonction partage avec l'autre pair un message echo server.*/
static void onAccept(dyad_Event *e)
{
  dyad_addListener(e->remote, DYAD_EVENT_DATA, onData1, NULL);
  dyad_writef(e->remote, "echo server Lokman\r\n");
  lokman = e->remote;
}

/*Cette fonction indique un erreur du connexion au server.*/
static void onError(dyad_Event *e)
{
  printf("server error: %s\n", e->msg);
}

//-------------------------------------------------------------------------------------------------------------------------

/*Cette fonction indique est ce que une piece peut se deplacer du champ start vers champ dest et retourne le type du mouvement prevu.*/
Turn check_get_turn(Field *field[8][8], Field *start, Field *dest)
{
  gint start_x, start_y, dest_x, dest_y, diff_x, diff_y, mid_x, mid_y;
  Owner owner, enemy, owner_mid, owner_dest;
  gboolean is_queen;

  start_x = field_get_col(start);
  start_y = field_get_row(start);
  dest_x = field_get_col(dest);
  dest_y = field_get_row(dest);
  diff_x = dest_x - start_x;
  diff_y = dest_y - start_y;
  owner = field_get_owner(start);
  enemy = (owner == OWNER_WHITE) ? OWNER_BLACK : OWNER_WHITE;
  owner_dest = field_get_owner(dest);
  is_queen = field_get_is_queen(start);

  /* Verifier est ce que le champ destination est vide*/
  if (owner_dest != OWNER_NONE)
    return TURN_NONE;

  /* Verifier un mouvement normal */
  if ((diff_x * diff_x == 1))
  {
    if (is_queen && (diff_y * diff_y == 1))
      return TURN_MOVE;

    if (owner == OWNER_WHITE && diff_y == -1)
      return TURN_MOVE;

    if (owner == OWNER_BLACK && diff_y == 1)
      return TURN_MOVE;
  }

  /* Verifier un mouvement capture */
  if ((diff_x * diff_x == 4))
  {
    mid_x = (dest_x + start_x) / 2;
    mid_y = (dest_y + start_y) / 2;
    owner_mid = field_get_owner(field[mid_x][mid_y]);

    if (owner_mid != enemy)
      return TURN_NONE;

    if (is_queen && (diff_y * diff_y) == 4)
      return TURN_TAKE;

    if (owner == OWNER_WHITE && diff_y == -2)
      return TURN_TAKE;

    if (owner == OWNER_BLACK && diff_y == 2)
      return TURN_TAKE;
  }

  return TURN_NONE;
}

// Vérifie si une pièce peut sauter à nouveau (capture en chaîne)
gboolean check_get_can_jump_again(Field *field[8][8],
                                  Field *piece)
{
  gint start_x, start_y, dest_x, dest_y, i, k;
  Turn turn;

  start_x = field_get_col(piece);
  start_y = field_get_row(piece);

  /* Verifier top/bottom left/right pieces */
  for (i = -2; i <= 2; i += 4)
  {
    for (k = -2; k <= 2; k += 4)
    {
      dest_x = start_x + i;
      dest_y = start_y + k;

      /* Verifier si le champ destination est valide */
      if (dest_x < 0 || dest_x > 7 ||
          dest_y < 0 || dest_y > 7)
        continue;

      /* Verifier si c'est possible de sauter vers destination */
      turn = check_get_turn(field, piece, field[dest_x][dest_y]);

      if (turn == TURN_TAKE)
        return TRUE;
    }
  }

  return FALSE;
}

// Vérifie si une pièce peut etre dame
gboolean check_get_becomes_queen(Field *piece)
{
  Owner owner;
  gint row;

  owner = field_get_owner(piece);
  row = field_get_row(piece);

  if ((owner == OWNER_WHITE && row == 0) ||
      (owner == OWNER_BLACK && row == 7))
    return TRUE;
  else
    return FALSE;
}

Pile *empiler(Pile *tete, int table[8][8])
{
  gint i, k;
  Pile *nv = (Pile *)malloc(sizeof(Pile));
  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      nv->laststate[i][k] = table[i][k];
    }
  }
  nv->scoreP1 = ScorePlayer1;
  nv->scoreP2 = ScorePlayer2;
  nv->svt = NULL;
  if (!tete)
    return nv;
  nv->svt = tete;
  return nv;
}

Pile *Depiler(Pile *tete, int table[8][8])
{
  if (!tete)
    return tete;
  memcpy(table, tete->laststate, sizeof(tete->laststate));
  Pile *tmp = tete;
  tete = tete->svt;
  free(tmp);
  return tete;
}

Pile *Clearpile(Pile *tete)
{
  Pile *tmp;
  while (tete)
  {
    tmp = tete;
    tete = tete->svt;
    free(tmp);
  }
  return NULL;
}

// Cette fonction sauvegarde la dernier etat du jeu dans le champ laststate
void recordlaststate(View *view, int lastornext)
{
  gint i, k;
  int table[8][8];
  Owner owner, isqueen;
  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      owner = field_get_owner(view->field[k][i]);
      isqueen = field_get_is_queen(view->field[k][i]);
      if (owner == OWNER_WHITE)
      {
        if (isqueen)
          table[i][k] = WQUEEN;
        else
          table[i][k] = WHITE;
      }
      else if (owner == OWNER_BLACK)
      {
        if (isqueen)
          table[i][k] = BQUEEN;
        else
          table[i][k] = BLACK;
      }
      else if (owner == OWNER_NONE)
        table[i][k] = EMPTY;
    }
  }

  if (!lastornext)
    view->lastmoves = empiler(view->lastmoves, table);
  else
    view->nextmoves = empiler(view->nextmoves, table);
}

// Cette fonction retire le pre-owner du champ en retirant le style
static void field_leave(GtkWidget *button, Field *piece)
{
  Owner owner;

  owner = field_get_owner(piece);

  if (owner != OWNER_NONE)
    return;

  field_set_preowner(piece, OWNER_NONE);
}

// Cette fonction ajoute un pre-owner pour le champ en ajoutant le style
static void field_enter(GtkWidget *button, Field *piece)
{
  View *view;
  Owner owner, owner_now;
  Turn turn;

  view = field_get_view(piece);
  owner = field_get_owner(piece);
  owner_now = (view->round % 2 == 0) ? OWNER_WHITE : OWNER_BLACK;

  /* Vérifier si le jeu est en cours */
  if (view->status == STATUS_NONE)
    return;

  /* Vérifier le propriétaire du champ de destination */
  if (owner != OWNER_NONE)
    return;

  /* Vérifier si un champ est sélectionné */
  if (!view->last)
    return;

  turn = check_get_turn(view->field, view->last, piece);

  if (turn == TURN_NONE)
    return;

  field_set_preowner(piece, owner_now);
}

// Cette fonction verifie si c'est possible de sauter et applique le saute
static void field_jump(View *view, Field *field[8][8], Field *piece)
{
  Owner owner_now, owner_dest;
  gint start_x, start_y, dest_x, dest_y, mid_x, mid_y;
  gint score;
  if (view->round % 2 == 0)
    score = ScorePlayer2;
  else
    score = ScorePlayer1;

  owner_now = (view->round % 2 == 0) ? OWNER_WHITE : OWNER_BLACK;
  owner_dest = field_get_owner(piece);
  start_x = field_get_col(view->last);
  start_y = field_get_row(view->last);
  dest_x = field_get_col(piece);
  dest_y = field_get_row(piece);
  mid_x = (start_x + dest_x) / 2;
  mid_y = (start_y + dest_y) / 2;

  /* Vérifier le champ de destination */
  if (owner_dest != OWNER_NONE)
    return;

  /* Vérifier le type de mouvement prévu */
  if (check_get_turn(field, view->last, piece) != TURN_TAKE)
    return;

  recordlaststate(view, 0);
  view->nextmoves = Clearpile(view->nextmoves);
  /* Retirer la pièce du milieu */
  field_set_owner(field[mid_x][mid_y], OWNER_NONE);

  /* Déplacer les pièces */
  PlaySound(TEXT("C:\\Users\\Adhaim\\Desktop\\gtk\\test\\move_sound_effect.wav"), NULL, SND_FILENAME | SND_ASYNC);
  field_set_owner(view->last, OWNER_NONE);
  field_set_owner(piece, owner_now);
  field_set_is_queen(piece, field_get_is_queen(view->last));
  field_set_is_queen(view->last, FALSE);

  /* Vérifier si la pièce est maintenant une dame */
  if (check_get_becomes_queen(piece))
  {
    if (!field_get_is_queen(piece))
      score += 10;
    field_set_is_queen(piece, TRUE);
  }

  /* Soustraire une pièce */
  if (owner_now == OWNER_WHITE)
    view->pieces_two--;
  else
    view->pieces_one--;

  /* Vérifier si on peut encore sauter */
  if (check_get_can_jump_again(field, piece))
  {
    view->last = piece;
    view_set_status(view, STATUS_JUMP_AGAIN);
    return;
  }

  /* Vérifier si quelqu'un a gagné */
  if (view->pieces_one == 0 ||
      view->pieces_two == 0)
  {
    if (connected)
      CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&sendarray, NULL, 0, NULL);
    view_set_status(view, STATUS_NONE);
    return;
  }

  view->last = NULL;

  if (view->round % 2 == 0)
    updatescorelabelplayer2(score + 30);
  else
    updatescorelabelplayer1(score + 30);

  view->round++;
  if (view->round % 2 == 0)
  {
    stopTimerPlayer1();
    startTimerPlayer2();
  }
  else
  {
    stopTimerPlayer2();
    startTimerPlayer1();
  }
  if (connected)
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&sendarray, NULL, 0, NULL);
  view_set_status(view, STATUS_MOVE);
}

// Cette fonction verifie si c'est possible de mover et applique le mouvement
static void field_move(View *view, Field *field[8][8], Field *piece)
{
  Owner owner, owner_now;
  Turn turn;
  gint score;
  if (view->round % 2 == 0)
    score = ScorePlayer2;
  else
    score = ScorePlayer1;

  owner = field_get_owner(piece);
  owner_now = (view->round % 2 == 0) ? OWNER_WHITE : OWNER_BLACK;

  if (owner == owner_now)
  {
    view->last = piece;
    return;
  }

  if (!view->last)
    return;

  /* Vérifier le type de mouvement prévu */
  turn = check_get_turn(field, view->last, piece);

  switch (turn)
  {
  case TURN_NONE:
    return;
  case TURN_MOVE:
    break;
  case TURN_TAKE:
    field_jump(view, field, piece);
    return;
  }

  recordlaststate(view, 0);
  view->nextmoves = Clearpile(view->nextmoves);
  /* Déplacer le champ */
  PlaySound(TEXT("C:\\Users\\Adhaim\\Desktop\\gtk\\test\\move_sound_effect.wav"), NULL, SND_FILENAME | SND_ASYNC);
  field_set_owner(view->last, OWNER_NONE);
  field_set_owner(piece, owner_now);
  field_set_is_queen(piece, field_get_is_queen(view->last));
  field_set_is_queen(view->last, FALSE);
  view->last = NULL;

  /* Vérifier si la pièce est maintenant une dame */
  if (check_get_becomes_queen(piece))
  {
    if (!field_get_is_queen(piece))
      score += 10;
    field_set_is_queen(piece, TRUE);
  }

  if (view->round % 2 == 0)
    updatescorelabelplayer2(score + 10);
  else
    updatescorelabelplayer1(score + 10);

  view->round++;
  if (view->round % 2 == 0)
  {
    stopTimerPlayer1();
    startTimerPlayer2();
  }
  else
  {
    stopTimerPlayer2();
    startTimerPlayer1();
  }
  if (connected)
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&sendarray, NULL, 0, NULL);
  view_set_status(view, STATUS_MOVE);
}

// Cette fonction verifie si il y'a des mouvements et des sautes possibles d'apres le champ clicked
static void field_clicked(GtkWidget *button, Field *piece)
{
  View *view;

  view = field_get_view(piece);

  switch (view->status)
  {
  case STATUS_NONE:
    return;
  case STATUS_MOVE:
    field_move(view, view->field, piece);
    break;
  case STATUS_JUMP_AGAIN:
    field_jump(view, view->field, piece);
    break;
  }
}

// Cette fonction ajoute les buttons du champs dans le grid
static void view_add_fields(View *view, Field *field[8][8])
{
  gint i, k;
  GtkGrid *grid;
  GtkWidget *button;

  grid = GTK_GRID(view->grid);

  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      field[i][k] = init_field_inPos(i, k);
      button = field_get_button(field[i][k]);
      field_set_view(field[i][k], view);
      gtk_grid_attach(grid, button, i, k, 1, 1);
      g_signal_connect(button, "clicked", G_CALLBACK(field_clicked), field[i][k]);
      g_signal_connect(button, "enter", G_CALLBACK(field_enter), field[i][k]);
      g_signal_connect(button, "leave", G_CALLBACK(field_leave), field[i][k]);
    }
  }
}

// Cette fonction initialise le view
static void init_view(View *view)
{
  view->grid = gtk_grid_new();
  view->hbar = NULL;
  view->last = NULL;
  view->status = STATUS_MOVE;
  view->round = 0;
  view->pieces_one = 12;
  view->pieces_two = 12;

  /* Grid */
  GtkStyleContext *context = gtk_widget_get_style_context(view->grid);
  gtk_style_context_add_class(context, "tableau");
  gtk_grid_set_column_homogeneous(GTK_GRID(view->grid), TRUE);
  gtk_grid_set_row_homogeneous(GTK_GRID(view->grid), TRUE);
  view_add_fields(view, view->field);
}

//----------------------------------------------------------Ai implemented with minimax algorithm------------------------------------------------------

// Cette fonction évalue le tableau et renvoie un score
int Boardevaluate()
{
  int i, k, score = 0;
  // Parcourir le tableau
  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      // Calculer le score en fonction des pièces du joueur
      if (myboard[i][k] == BLACK)
        score = score + 1;
      if (myboard[i][k] == WHITE)
        score = score - 1;
      if (myboard[i][k] == WQUEEN)
        score = score - 3;
      if (myboard[i][k] == BQUEEN)
        score = score + 3;
    }
  }
  return score;
}

// Vérifie si la pièce à une position donnée appartient au joueur actuel
gboolean is_myown_piece(int x, int y, int player)
{
  if (player == BLACK)
  {
    return myboard[x][y] == BLACK || myboard[x][y] == BQUEEN;
  }
  else
  {
    return myboard[x][y] == WHITE || myboard[x][y] == WQUEEN;
  }
}

// Applique un mouvement sur le plateau (myboard)
void make_a_move(Move *move)
{
  // Déplace la pièce
  move->role = myboard[move->x1][move->y1];
  myboard[move->x2][move->y2] = myboard[move->x1][move->y1];
  myboard[move->x1][move->y1] = EMPTY;

  // Supprime les pièces sautées
  for (int i = 0; i < move->nb; i++)
  {
    int x = move->jumped_on_pieces[i].x;
    int y = move->jumped_on_pieces[i].y;
    move->jumped_on_pieces[i].role = myboard[x][y];
    myboard[x][y] = EMPTY;
  }

  // Promouvoir en dame si nécessaire
  if (move->x2 == 0 && myboard[move->x2][move->y2] == WHITE)
  {
    myboard[move->x2][move->y2] = WQUEEN;
  }
  if (move->x2 == 7 && myboard[move->x2][move->y2] == BLACK)
  {
    myboard[move->x2][move->y2] = BQUEEN;
  }
}

// Annule un mouvement sur le plateau (myboard)
void undo_a_move(Move *move)
{
  // Replacer la pièce
  myboard[move->x1][move->y1] = move->role;
  myboard[move->x2][move->y2] = EMPTY;

  // Restaurer les pièces sautées
  for (int i = 0; i < move->nb; i++)
  {
    int x = move->jumped_on_pieces[i].x;
    int y = move->jumped_on_pieces[i].y;
    myboard[x][y] = move->jumped_on_pieces[i].role;
  }
}

// Fonction auxiliaire pour vérifier si une position donnée est dans les limites du plateau (myboard)
int valid_position(int x, int y)
{
  return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// Fonction auxiliaire pour vérifier si une pièce donnée est une dame
int queen(int piece)
{
  return piece == WQUEEN || piece == BQUEEN;
}

// Fonction auxiliaire pour ajouter un mouvement à la liste des mouvements possibles
void move_add(Move *moves, int *nb_moves, int x1, int y1, int x2, int y2)
{
  moves[*nb_moves].x1 = x1;
  moves[*nb_moves].y1 = y1;
  moves[*nb_moves].x2 = x2;
  moves[*nb_moves].y2 = y2;
  moves[*nb_moves].nb = 0;
  (*nb_moves)++;
}

// Renvoie tous les mouvements réguliers possibles pour la pièce à la position (x, y)
void get_regular_moves(int x, int y, int *nb_moves, Move *moves)
{
  int piece = myboard[x][y];

  if (piece == WHITE || piece == WQUEEN)
  {
    // Vérifie s'il y a un mouvement régulier en haut à gauche
    if (valid_position(x - 1, y - 1) && myboard[x - 1][y - 1] == EMPTY)
    {
      move_add(moves, nb_moves, x, y, x - 1, y - 1);
    }
    // Vérifie s'il y a un mouvement régulier en haut à droite
    if (valid_position(x - 1, y + 1) && myboard[x - 1][y + 1] == EMPTY)
    {
      move_add(moves, nb_moves, x, y, x - 1, y + 1);
    }
    // Vérifie si la pièce est une reine et peut se déplacer en diagonale dans toutes les directions
    if (queen(piece))
    {
      // Vérifie s'il y a un mouvement régulier en bas à gauche
      if (valid_position(x + 1, y - 1) && myboard[x + 1][y - 1] == EMPTY)
      {
        move_add(moves, nb_moves, x, y, x + 1, y - 1);
      }
      // Vérifie s'il y a un mouvement régulier en bas à droite
      if (valid_position(x + 1, y + 1) && myboard[x + 1][y + 1] == EMPTY)
      {
        move_add(moves, nb_moves, x, y, x + 1, y + 1);
      }
    }
  }
  else if (piece == BLACK || piece == BQUEEN)
  {
    // Vérifie s'il y a un mouvement régulier en bas à gauche
    if (valid_position(x + 1, y - 1) && myboard[x + 1][y - 1] == EMPTY)
    {
      move_add(moves, nb_moves, x, y, x + 1, y - 1);
    }
    // Vérifie s'il y a un mouvement régulier en bas à droite
    if (valid_position(x + 1, y + 1) && myboard[x + 1][y + 1] == EMPTY)
    {
      move_add(moves, nb_moves, x, y, x + 1, y + 1);
    }
    // Vérifie si la pièce est une reine et peut se déplacer en diagonale dans toutes les directions
    if (queen(piece))
    {
      // Vérifie s'il y a un mouvement régulier en haut à gauche
      if (valid_position(x - 1, y - 1) && myboard[x - 1][y - 1] == EMPTY)
      {
        move_add(moves, nb_moves, x, y, x - 1, y - 1);
      }
      // Vérifie s'il y a un mouvement régulier en haut à droite
      if (valid_position(x - 1, y + 1) && myboard[x - 1][y + 1] == EMPTY)
      {
        move_add(moves, nb_moves, x, y, x - 1, y + 1);
      }
    }
  }
}

// Renvoie tous les mouvements de capture possibles pour la pièce à la position (x, y)
void get_captures_moves(int x, int y, int *nb_moves, Move *moves, int piece)
{
  Move submove[20];
  int subnb = 0, i, j;

  if (piece == WHITE || piece == WQUEEN)
  {
    // Check if there's a capture move to the top-left
    if (valid_position(x - 1, y - 1) && myboard[x - 1][y - 1] == BLACK || myboard[x - 1][y - 1] == BQUEEN)
    {
      // Check if the next diagonal space in that direction is empty
      if (valid_position(x - 2, y - 2) && myboard[x - 2][y - 2] == EMPTY)
      {

        Move *test = (Move *)malloc(sizeof(Move));
        test->x1 = x;
        test->y1 = y;
        test->x2 = x - 2;
        test->y2 = y - 2;
        test->jumped_on_pieces[0].x = x - 1;
        test->jumped_on_pieces[0].y = y - 1;
        test->nb = 1;
        make_a_move(test);

        get_captures_moves(x - 2, y - 2, &subnb, submove, piece);
        undo_a_move(test);
        if (subnb == 0)
        {
          moves[*nb_moves].x1 = x;
          moves[*nb_moves].y1 = y;
          moves[*nb_moves].x2 = x - 2;
          moves[*nb_moves].y2 = y - 2;
          moves[*nb_moves].jumped_on_pieces[0].x = x - 1;
          moves[*nb_moves].jumped_on_pieces[0].y = y - 1;
          moves[*nb_moves].nb = 1;
          (*nb_moves)++;
        }
        else
        {

          for (i = 0; i < subnb; i++)
          {
            moves[*nb_moves].x1 = x;
            moves[*nb_moves].y1 = y;
            moves[*nb_moves].x2 = submove[i].x2;
            moves[*nb_moves].y2 = submove[i].y2;
            moves[*nb_moves].jumped_on_pieces[0].x = x - 1;
            moves[*nb_moves].jumped_on_pieces[0].y = y - 1;
            moves[*nb_moves].nb = 1;
            for (j = 0; j < submove[i].nb; j++)
            {
              moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].x = submove[i].jumped_on_pieces[j].x;
              moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].y = submove[i].jumped_on_pieces[j].y;
              moves[*nb_moves].nb++;
            }
            (*nb_moves)++;
          }
        }
      }
    }
    // Check if there's a capture move to the top-right
    if (valid_position(x - 1, y + 1) && myboard[x - 1][y + 1] == BLACK || myboard[x - 1][y + 1] == BQUEEN)
    {
      // Check if the next diagonal space in that direction is empty
      if (valid_position(x - 2, y + 2) && myboard[x - 2][y + 2] == EMPTY)
      {
        Move *test = (Move *)malloc(sizeof(Move));
        test->x1 = x;
        test->y1 = y;
        test->x2 = x - 2;
        test->y2 = y + 2;
        test->jumped_on_pieces[0].x = x - 1;
        test->jumped_on_pieces[0].y = y + 1;
        test->nb = 1;
        make_a_move(test);

        get_captures_moves(x - 2, y + 2, &subnb, submove, piece);
        undo_a_move(test);
        if (subnb == 0)
        {
          moves[*nb_moves].x1 = x;
          moves[*nb_moves].y1 = y;
          moves[*nb_moves].x2 = x - 2;
          moves[*nb_moves].y2 = y + 2;
          moves[*nb_moves].jumped_on_pieces[0].x = x - 1;
          moves[*nb_moves].jumped_on_pieces[0].y = y + 1;
          moves[*nb_moves].nb = 1;
          (*nb_moves)++;
        }
        else
        {

          for (i = 0; i < subnb; i++)
          {
            moves[*nb_moves].x1 = x;
            moves[*nb_moves].y1 = y;
            moves[*nb_moves].x2 = submove[i].x2;
            moves[*nb_moves].y2 = submove[i].y2;
            moves[*nb_moves].jumped_on_pieces[0].x = x - 1;
            moves[*nb_moves].jumped_on_pieces[0].y = y + 1;
            moves[*nb_moves].nb = 1;
            for (j = 0; j < submove[i].nb; j++)
            {
              moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].x = submove[i].jumped_on_pieces[j].x;
              moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].y = submove[i].jumped_on_pieces[j].y;
              moves[*nb_moves].nb++;
            }
            (*nb_moves)++;
          }
        }
      }
    }
    // Check if the piece is a queen and can capture diagonally in all directions
    if (queen(piece))
    {
      // Check if there's a capture move to the bottom-left
      if (valid_position(x + 1, y - 1) && (myboard[x + 1][y - 1] == BLACK || myboard[x + 1][y - 1] == BQUEEN))
      {
        // Check if the next diagonal space in that direction is empty
        if (valid_position(x + 2, y - 2) && myboard[x + 2][y - 2] == EMPTY)
        {
          Move *test = (Move *)malloc(sizeof(Move));
          test->x1 = x;
          test->y1 = y;
          test->x2 = x + 2;
          test->y2 = y - 2;
          test->jumped_on_pieces[0].x = x + 1;
          test->jumped_on_pieces[0].y = y - 1;
          test->nb = 1;
          make_a_move(test);

          get_captures_moves(x + 2, y - 2, &subnb, submove, piece);
          undo_a_move(test);
          if (subnb == 0)
          {
            moves[*nb_moves].x1 = x;
            moves[*nb_moves].y1 = y;
            moves[*nb_moves].x2 = x + 2;
            moves[*nb_moves].y2 = y - 2;
            moves[*nb_moves].jumped_on_pieces[0].x = x + 1;
            moves[*nb_moves].jumped_on_pieces[0].y = y - 1;
            moves[*nb_moves].nb = 1;
            (*nb_moves)++;
          }
          else
          {

            for (i = 0; i < subnb; i++)
            {
              moves[*nb_moves].x1 = x;
              moves[*nb_moves].y1 = y;
              moves[*nb_moves].x2 = submove[i].x2;
              moves[*nb_moves].y2 = submove[i].y2;
              moves[*nb_moves].jumped_on_pieces[0].x = x + 1;
              moves[*nb_moves].jumped_on_pieces[0].y = y - 1;
              moves[*nb_moves].nb = 1;
              for (j = 0; j < submove[i].nb; j++)
              {
                moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].x = submove[i].jumped_on_pieces[j].x;
                moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].y = submove[i].jumped_on_pieces[j].y;
                moves[*nb_moves].nb++;
              }
              (*nb_moves)++;
            }
          }
        }
      }
      // Check if there's a capture move to the bottom-right
      if (valid_position(x + 1, y + 1) && (myboard[x + 1][y + 1] == BLACK || myboard[x + 1][y + 1] == BQUEEN))
      {
        // Check if the next diagonal space in that direction is empty
        if (valid_position(x + 2, y + 2) && myboard[x + 2][y + 2] == EMPTY)
        {
          Move *test = (Move *)malloc(sizeof(Move));
          test->x1 = x;
          test->y1 = y;
          test->x2 = x + 2;
          test->y2 = y + 2;
          test->jumped_on_pieces[0].x = x + 1;
          test->jumped_on_pieces[0].y = y + 1;
          test->nb = 1;
          make_a_move(test);

          get_captures_moves(x + 2, y + 2, &subnb, submove, piece);
          undo_a_move(test);
          if (subnb == 0)
          {
            moves[*nb_moves].x1 = x;
            moves[*nb_moves].y1 = y;
            moves[*nb_moves].x2 = x + 2;
            moves[*nb_moves].y2 = y + 2;
            moves[*nb_moves].jumped_on_pieces[0].x = x + 1;
            moves[*nb_moves].jumped_on_pieces[0].y = y + 1;
            moves[*nb_moves].nb = 1;
            (*nb_moves)++;
          }
          else
          {

            for (i = 0; i < subnb; i++)
            {
              moves[*nb_moves].x1 = x;
              moves[*nb_moves].y1 = y;
              moves[*nb_moves].x2 = submove[i].x2;
              moves[*nb_moves].y2 = submove[i].y2;
              moves[*nb_moves].jumped_on_pieces[0].x = x + 1;
              moves[*nb_moves].jumped_on_pieces[0].y = y + 1;
              moves[*nb_moves].nb = 1;
              for (j = 0; j < submove[i].nb; j++)
              {
                moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].x = submove[i].jumped_on_pieces[j].x;
                moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].y = submove[i].jumped_on_pieces[j].y;
                moves[*nb_moves].nb++;
              }
              (*nb_moves)++;
            }
          }
        }
      }
    }
  }
  else if (piece == BLACK || piece == BQUEEN)
  {
    // Check if there's a capture move to the bottom-left
    if (valid_position(x + 1, y - 1) && (myboard[x + 1][y - 1] == WHITE || myboard[x + 1][y - 1] == WQUEEN))
    {
      // Check if the next diagonal space in that direction is empty
      if (valid_position(x + 2, y - 2) && myboard[x + 2][y - 2] == EMPTY)
      {
        Move *test = (Move *)malloc(sizeof(Move));
        test->x1 = x;
        test->y1 = y;
        test->x2 = x + 2;
        test->y2 = y - 2;
        test->jumped_on_pieces[0].x = x + 1;
        test->jumped_on_pieces[0].y = y - 1;
        test->nb = 1;
        make_a_move(test);

        get_captures_moves(x + 2, y - 2, &subnb, submove, piece);
        undo_a_move(test);
        if (subnb == 0)
        {
          moves[*nb_moves].x1 = x;
          moves[*nb_moves].y1 = y;
          moves[*nb_moves].x2 = x + 2;
          moves[*nb_moves].y2 = y - 2;
          moves[*nb_moves].jumped_on_pieces[0].x = x + 1;
          moves[*nb_moves].jumped_on_pieces[0].y = y - 1;
          moves[*nb_moves].nb = 1;
          (*nb_moves)++;
        }
        else
        {

          for (i = 0; i < subnb; i++)
          {
            moves[*nb_moves].x1 = x;
            moves[*nb_moves].y1 = y;
            moves[*nb_moves].x2 = submove[i].x2;
            moves[*nb_moves].y2 = submove[i].y2;
            moves[*nb_moves].jumped_on_pieces[0].x = x + 1;
            moves[*nb_moves].jumped_on_pieces[0].y = y - 1;
            moves[*nb_moves].nb = 1;
            for (j = 0; j < submove[i].nb; j++)
            {
              moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].x = submove[i].jumped_on_pieces[j].x;
              moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].y = submove[i].jumped_on_pieces[j].y;
              moves[*nb_moves].nb++;
            }
            (*nb_moves)++;
          }
        }
      }
    }
    // Check if there's a capture move to the bottom-right
    if (valid_position(x + 1, y + 1) && (myboard[x + 1][y + 1] == WHITE || myboard[x + 1][y + 1] == WQUEEN))
    {
      // Check if the next diagonal space in that direction is empty
      if (valid_position(x + 2, y + 2) && myboard[x + 2][y + 2] == EMPTY)
      {
        Move *test = (Move *)malloc(sizeof(Move));
        test->x1 = x;
        test->y1 = y;
        test->x2 = x + 2;
        test->y2 = y + 2;
        test->jumped_on_pieces[0].x = x + 1;
        test->jumped_on_pieces[0].y = y + 1;
        test->nb = 1;
        make_a_move(test);

        get_captures_moves(x + 2, y + 2, &subnb, submove, piece);
        undo_a_move(test);
        if (subnb == 0)
        {

          moves[*nb_moves].x1 = x;
          moves[*nb_moves].y1 = y;
          moves[*nb_moves].x2 = x + 2;
          moves[*nb_moves].y2 = y + 2;
          moves[*nb_moves].jumped_on_pieces[0].x = x + 1;
          moves[*nb_moves].jumped_on_pieces[0].y = y + 1;
          moves[*nb_moves].nb = 1;
          (*nb_moves)++;
        }
        else
        {

          for (i = 0; i < subnb; i++)
          {
            moves[*nb_moves].x1 = x;
            moves[*nb_moves].y1 = y;
            moves[*nb_moves].x2 = submove[i].x2;
            moves[*nb_moves].y2 = submove[i].y2;
            moves[*nb_moves].jumped_on_pieces[0].x = x + 1;
            moves[*nb_moves].jumped_on_pieces[0].y = y + 1;
            moves[*nb_moves].nb = 1;
            for (j = 0; j < submove[i].nb; j++)
            {
              moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].x = submove[i].jumped_on_pieces[j].x;
              moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].y = submove[i].jumped_on_pieces[j].y;
              moves[*nb_moves].nb++;
            }
            (*nb_moves)++;
          }
        }
      }
    }
    // Check if the piece is a queen and can capture diagonally in all directions
    if (queen(piece))
    {
      // Check if there's a capture move to the top-left
      if (valid_position(x - 1, y - 1) && (myboard[x - 1][y - 1] == WHITE || myboard[x - 1][y - 1] == WQUEEN))
      {
        // Check if the next diagonal space in that direction is empty
        if (valid_position(x - 2, y - 2) && myboard[x - 2][y - 2] == EMPTY)
        {
          Move *test = (Move *)malloc(sizeof(Move));
          test->x1 = x;
          test->y1 = y;
          test->x2 = x - 2;
          test->y2 = y - 2;
          test->jumped_on_pieces[0].x = x - 1;
          test->jumped_on_pieces[0].y = y - 1;
          test->nb = 1;
          make_a_move(test);

          get_captures_moves(x - 2, y - 2, &subnb, submove, piece);
          undo_a_move(test);
          if (subnb == 0)
          {
            moves[*nb_moves].x1 = x;
            moves[*nb_moves].y1 = y;
            moves[*nb_moves].x2 = x - 2;
            moves[*nb_moves].y2 = y - 2;
            moves[*nb_moves].jumped_on_pieces[0].x = x - 1;
            moves[*nb_moves].jumped_on_pieces[0].y = y - 1;
            moves[*nb_moves].nb = 1;
            (*nb_moves)++;
          }
          else
          {

            for (i = 0; i < subnb; i++)
            {
              moves[*nb_moves].x1 = x;
              moves[*nb_moves].y1 = y;
              moves[*nb_moves].x2 = submove[i].x2;
              moves[*nb_moves].y2 = submove[i].y2;
              moves[*nb_moves].jumped_on_pieces[0].x = x - 1;
              moves[*nb_moves].jumped_on_pieces[0].y = y - 1;
              moves[*nb_moves].nb = 1;
              for (j = 0; j < submove[i].nb; j++)
              {
                moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].x = submove[i].jumped_on_pieces[j].x;
                moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].y = submove[i].jumped_on_pieces[j].y;
                moves[*nb_moves].nb++;
              }
              (*nb_moves)++;
            }
          }
        }
      }
      // Check if there's a capture move to the top-right
      if (valid_position(x - 1, y + 1) && (myboard[x - 1][y + 1] == WHITE || myboard[x - 1][y + 1] == WQUEEN))
      {
        // Check if the next diagonal space in that direction is empty
        if (valid_position(x - 2, y + 2) && myboard[x - 2][y + 2] == EMPTY)
        {
          Move *test = (Move *)malloc(sizeof(Move));
          test->x1 = x;
          test->y1 = y;
          test->x2 = x - 2;
          test->y2 = y + 2;
          test->jumped_on_pieces[0].x = x - 1;
          test->jumped_on_pieces[0].y = y + 1;
          test->nb = 1;
          make_a_move(test);

          get_captures_moves(x - 2, y + 2, &subnb, submove, piece);
          undo_a_move(test);
          if (subnb == 0)
          {
            moves[*nb_moves].x1 = x;
            moves[*nb_moves].y1 = y;
            moves[*nb_moves].x2 = x - 2;
            moves[*nb_moves].y2 = y + 2;
            moves[*nb_moves].jumped_on_pieces[0].x = x - 1;
            moves[*nb_moves].jumped_on_pieces[0].y = y + 1;
            moves[*nb_moves].nb = 1;
            (*nb_moves)++;
          }
          else
          {

            for (i = 0; i < subnb; i++)
            {
              moves[*nb_moves].x1 = x;
              moves[*nb_moves].y1 = y;
              moves[*nb_moves].x2 = submove[i].x2;
              moves[*nb_moves].y2 = submove[i].y2;
              moves[*nb_moves].jumped_on_pieces[0].x = x - 1;
              moves[*nb_moves].jumped_on_pieces[0].y = y + 1;
              moves[*nb_moves].nb = 1;
              for (j = 0; j < submove[i].nb; j++)
              {
                moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].x = submove[i].jumped_on_pieces[j].x;
                moves[*nb_moves].jumped_on_pieces[moves[*nb_moves].nb].y = submove[i].jumped_on_pieces[j].y;
                moves[*nb_moves].nb++;
              }
              (*nb_moves)++;
            }
          }
        }
      }
    }
  }
}

// Renvoie tous les mouvements possibles pour la pièce à la position (x, y)
Move *get_moves(int x, int y, int *nb_moves)
{
  int piece = myboard[x][y];
  // Alloue de la mémoire pour les mouvements possibles (maximum de 50 mouvements)
  Move *moves = (Move *)malloc(sizeof(Move) * 50);
  *nb_moves = 0;
  get_regular_moves(x, y, nb_moves, moves);
  // if(x==3 && y == 2)g_print("%d %d %d",myboard[1][0],myboard[2][1],myboard[3][2]);
  get_captures_moves(x, y, nb_moves, moves, piece);
  return moves;
}

// Verifie si le jeu est termine (si il n'ya plus du pieces blanches ou noirs)
int game_overr()
{
  int white_pieces = 0;
  int black_pieces = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (myboard[i][j] == WHITE || myboard[i][j] == WQUEEN)
      {
        white_pieces++;
      }
      else if (myboard[i][j] == BLACK || myboard[i][j] == BQUEEN)
      {
        black_pieces++;
      }
    }
  }
  if (white_pieces == 0)
  {
    return BLACK;
  }
  else if (black_pieces == 0)
  {
    return WHITE;
  }
  else
  {
    return 0;
  }
}

// algorithme Minimax retourne le meilleur mouvement
Move minimaxalgo(int depth, int player)
{
  Move bestMove;
  if (depth == 0 || game_overr() != 0)
  {
    bestMove.score = Boardevaluate();
    return bestMove;
  }
  if (player == BLACK)
  {
    bestMove.score = INT_MIN;
    Move *moves;
    int n_moves = 0;
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        if (is_myown_piece(i, j, player))
        {
          moves = get_moves(i, j, &n_moves);
          for (int k = 0; k < n_moves; k++)
          {
            make_a_move(moves + k);
            // g_print(" BLACK ROLE  x : %d y : %d x1 : %d y1 : %d  piece_captured : %d\n",moves[k].x1,moves[k].y1,moves[k].x2,moves[k].y2,moves[k].nb);
            moves[k].score = minimaxalgo(depth - 1, WHITE).score;
            // g_print(" score : %d \n",moves[k].score);
            undo_a_move(moves + k);
            if (moves[k].score > bestMove.score)
            {
              int l;
              bestMove.x1 = moves[k].x1;
              bestMove.y1 = moves[k].y1;
              bestMove.x2 = moves[k].x2;
              bestMove.y2 = moves[k].y2;
              bestMove.nb = moves[k].nb;
              bestMove.role = moves[k].role;
              bestMove.score = moves[k].score;
              for (l = 0; l < moves[k].nb; l++)
              {
                bestMove.jumped_on_pieces[l].x = moves[k].jumped_on_pieces[l].x;
                bestMove.jumped_on_pieces[l].y = moves[k].jumped_on_pieces[l].y;
                bestMove.jumped_on_pieces[l].role = moves[k].jumped_on_pieces[l].role;
              }
            }
          }
          free(moves);
        }
      }
    }
    if (bestMove.score == INT_MIN)
    {
      bestMove.score = INT_MAX;
    }
    return bestMove;
  }
  else
  {
    bestMove.score = INT_MAX;
    Move *moves;
    int n_moves = 0;
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {

        if (is_myown_piece(i, j, player))
        {
          moves = get_moves(i, j, &n_moves);
          for (int k = 0; k < n_moves; k++)
          {
            make_a_move(moves + k);
            // g_print(" WHITE ROLE  x : %d y : %d x1 : %d y1 : %d  piece_captured : %d",moves[k].x1,moves[k].y1,moves[k].x2,moves[k].y2,moves[k].nb);
            moves[k].score = minimaxalgo(depth - 1, BLACK).score;
            // g_print(" score : %d \n",moves[k].score);
            undo_a_move(moves + k);
            if (moves[k].score < bestMove.score)
            {
              int l;
              bestMove.x1 = moves[k].x1;
              bestMove.y1 = moves[k].y1;
              bestMove.x2 = moves[k].x2;
              bestMove.y2 = moves[k].y2;
              bestMove.nb = moves[k].nb;
              bestMove.role = moves[k].role;
              bestMove.score = moves[k].score;
              for (l = 0; l < moves[k].nb; l++)
              {
                bestMove.jumped_on_pieces[l].x = moves[k].jumped_on_pieces[l].x;
                bestMove.jumped_on_pieces[l].y = moves[k].jumped_on_pieces[l].y;
                bestMove.jumped_on_pieces[l].role = moves[k].jumped_on_pieces[l].role;
              }
            }
          }
          free(moves);
        }
      }
    }
    if (bestMove.score == INT_MAX)
    {
      bestMove.score = INT_MIN;
    }
    return bestMove;
  }
}

// Cette function cherche le meilleur mouvement et l'applique au jeu en utilisant minimax
void findandapply(View *view, int player, int difficulty)
{
  gint i, k, t;
  Owner owner, isqueen;
  Move bestmove;
  gint score;
  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      owner = field_get_owner(view->field[k][i]);
      isqueen = field_get_is_queen(view->field[k][i]);
      if (owner == OWNER_WHITE)
      {
        if (isqueen)
          myboard[i][k] = WQUEEN;
        else
          myboard[i][k] = WHITE;
      }
      else if (owner == OWNER_BLACK)
      {
        if (isqueen)
          myboard[i][k] = BQUEEN;
        else
          myboard[i][k] = BLACK;
      }
      else if (owner == OWNER_NONE)
        myboard[i][k] = EMPTY;
    }
  }

  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      g_print("%d     ", myboard[i][k]);
    }
    g_print("\n");
  }

  bestmove = minimaxalgo(difficulty, player);

  g_print("x : %d \ny : %d \nx1 : %d \ny1 : %d \n", bestmove.x1, bestmove.y1, bestmove.x2, bestmove.y2);

  recordlaststate(view, 0);
  view->nextmoves = Clearpile(view->nextmoves);

  if (view->round % 2 == 0)
    score = ScorePlayer2;
  else
    score = ScorePlayer1;

  if (bestmove.nb == 0)
    score += 10;
  else
    score = 10 * bestmove.nb + score;
  for (t = 0; t < bestmove.nb; t++)
  {

    score += 20;
    field_set_owner(view->field[bestmove.jumped_on_pieces[t].y][bestmove.jumped_on_pieces[t].x], OWNER_NONE);
    field_set_is_queen(view->field[bestmove.jumped_on_pieces[t].y][bestmove.jumped_on_pieces[t].x], FALSE);
  }

  if (player == BLACK)
  {
    field_set_owner(view->field[bestmove.y1][bestmove.x1], OWNER_NONE);
    field_set_owner(view->field[bestmove.y2][bestmove.x2], OWNER_BLACK);
  }
  else
  {
    field_set_owner(view->field[bestmove.y1][bestmove.x1], OWNER_NONE);
    field_set_owner(view->field[bestmove.y2][bestmove.x2], OWNER_WHITE);
  }

  field_set_is_queen(view->field[bestmove.y2][bestmove.x2], field_get_is_queen(view->field[bestmove.y1][bestmove.x1]));
  field_set_is_queen(view->field[bestmove.y1][bestmove.x1], FALSE);
  if (check_get_becomes_queen(view->field[bestmove.y2][bestmove.x2]))
  {
    if (!field_get_is_queen(view->field[bestmove.y1][bestmove.x1]))
      score += 10;
    field_set_is_queen(view->field[bestmove.y2][bestmove.x2], TRUE);
  }

  PlaySound(TEXT("C:\\Users\\Adhaim\\Desktop\\gtk\\test\\move_sound_effect.wav"), NULL, SND_FILENAME | SND_ASYNC);
  make_a_move(&bestmove);
  if (view->round % 2 == 0)
    updatescorelabelplayer2(score);
  else
    updatescorelabelplayer1(score);
  view->round++;
  if (view->round % 2 == 0)
  {
    stopTimerPlayer1();
    startTimerPlayer2();
  }
  else
  {
    stopTimerPlayer2();
    startTimerPlayer1();
  }
  if (game_overr() == 0)
    view_set_status(view, STATUS_MOVE);
  else
  {
    view->round--;
    view_set_status(view, STATUS_NONE);
    Game_Ended = 1;
  }
}

// Cette fonction identifie le mode et la difficulte du jeu
gboolean whichturn(gpointer data)
{

  GameSettings *settings = (GameSettings *)data;
  gint diff;
  g_print("%d %d", settings->difficulty, settings->mode);
  g_print("\n%d\n", Game_Ended);

  switch (settings->difficulty)
  {
  case 0:
    diff = 2;
    break;
  case 1:
    diff = 4;
    break;
  case 2:
    diff = 6;
    break;
  }
  if (!Game_Ended)
  {
    if (settings->mode == 2)
    {
      if (settings->view->round % 2 != 0)
      {
        g_print("ai's turn\n");
        findandapply(settings->view, BLACK, diff);
      }
      else
      {
        g_print("ai's turn\n");
        findandapply(settings->view, WHITE, diff);
      }
      return TRUE;
    }
    else if (settings->mode == 1)
    {
      if (settings->view->round % 2 != 0)
      {
        g_print("ai's turn\n");
        findandapply(settings->view, BLACK, diff);
      }
      return TRUE;
    }
  }

  return FALSE;
}

// Cette fonction fait un reset sur le view (retourne le view a l'etat initial)
void reset_view(View *view, gint difficulty, gint mode)
{
  gint i, k;
  Owner owner_init;
  Game_Ended = 0;
  for (i = 0; i < 8; i++)
  {
    for (k = 0; k < 8; k++)
    {
      owner_init = fieldGetOwner(view->field[i][k]);
      field_set_owner(view->field[i][k], OWNER_NONE);
      field_set_owner(view->field[i][k], owner_init);
      field_set_is_queen(view->field[i][k], FALSE);
    }
  }

  view->last = NULL;
  view->round = color;
  view->pieces_one = 12;
  view->pieces_two = 12;

  view->lastmoves = NULL;
  view->nextmoves = NULL;
  GameSettings *settings = (GameSettings *)malloc(sizeof(GameSettings));
  settings->view = view;
  settings->difficulty = difficulty;
  settings->mode = mode;
  timeended = 0;
  updatescorelabelplayer1(0);
  updatescorelabelplayer2(0);
  stopTimerPlayer1();
  stopTimerPlayer2();
  resetTimerPlayer1();
  resetTimerPlayer2();
  if (!color)
    startTimerPlayer2();
  else
    stopTimerPlayer2();

  view_set_status(view, STATUS_MOVE);
  if (timeout_source_id != 0)
    g_source_remove(timeout_source_id);
  timeout_source_id = g_timeout_add(2000, whichturn, settings);
}

// Cette fonction reset le jeu a l'etat derniere
static void last_move_button_clicked(GtkWidget *button, View *view)
{

  if (!timeended && view->lastmoves)
  {
    int lasttable[8][8];
    recordlaststate(view, 1);
    updatescorelabelplayer2(view->lastmoves->scoreP2);
    updatescorelabelplayer1(view->lastmoves->scoreP1);
    view->lastmoves = Depiler(view->lastmoves, lasttable);
    gint i, k;
    for (i = 0; i < 8; i++)
    {
      for (k = 0; k < 8; k++)
      {
        field_set_owner(view->field[k][i], OWNER_NONE);
        field_set_is_queen(view->field[k][i], FALSE);
        switch (lasttable[i][k])
        {
        case 1:
          field_set_owner(view->field[k][i], OWNER_WHITE);
          break;
        case 2:
          field_set_owner(view->field[k][i], OWNER_BLACK);
          break;
        case 3:
          field_set_owner(view->field[k][i], OWNER_WHITE);
          field_set_is_queen(view->field[k][i], TRUE);
          break;
        case 4:
          field_set_owner(view->field[k][i], OWNER_BLACK);
          field_set_is_queen(view->field[k][i], TRUE);
          break;
        }
      }
    }

    view->round--;

    if (view->round % 2 == 0)
    {
      stopTimerPlayer1();
      startTimerPlayer2();
    }
    else
    {
      stopTimerPlayer2();
      startTimerPlayer1();
    }
    view_set_status(view, STATUS_MOVE);
  }
  return;
}

// Cette fonction reset le jeu a l'etat derniere
static void next_move_button_clicked(GtkWidget *button, View *view)
{

  if (!timeended && view->nextmoves)
  {
    int nexttable[8][8];
    recordlaststate(view, 0);
    updatescorelabelplayer2(view->nextmoves->scoreP2);
    updatescorelabelplayer1(view->nextmoves->scoreP1);
    view->nextmoves = Depiler(view->nextmoves, nexttable);

    gint i, k;
    for (i = 0; i < 8; i++)
    {
      for (k = 0; k < 8; k++)
      {
        field_set_owner(view->field[k][i], OWNER_NONE);
        field_set_is_queen(view->field[k][i], FALSE);
        switch (nexttable[i][k])
        {
        case 1:
          field_set_owner(view->field[k][i], OWNER_WHITE);
          break;
        case 2:
          field_set_owner(view->field[k][i], OWNER_BLACK);
          break;
        case 3:
          field_set_owner(view->field[k][i], OWNER_WHITE);
          field_set_is_queen(view->field[k][i], TRUE);
          break;
        case 4:
          field_set_owner(view->field[k][i], OWNER_BLACK);
          field_set_is_queen(view->field[k][i], TRUE);
          break;
        }
      }
    }

    view->round++;
    if (view->round % 2 == 0)
    {
      stopTimerPlayer1();
      startTimerPlayer2();
    }
    else
    {
      stopTimerPlayer2();
      startTimerPlayer1();
    }
    view_set_status(view, STATUS_MOVE);
  }
  return;
}

// Cette fonction applique un signal sur le button
void last_move_signal(prSimpleBut *lastmove, View *view)
{
  g_signal_connect(lastmove->button, "clicked", G_CALLBACK(last_move_button_clicked), view);
}

// Cette fonction applique un signal sur le button
void next_move_signal(prSimpleBut *nextmove, View *view)
{
  g_signal_connect(nextmove->button, "clicked", G_CALLBACK(next_move_button_clicked), view);
}

// Cette fonction lance un nouveu jeu et fait un reset sur le view (retourne le view a l'etat initial)
static void new_game_button_clicked(GtkWidget *button, View *view)
{
  gint diff, lmode;
  Game_Ended = 1;

  if (shaders == 1)
  {
    GList *children = gtk_container_get_children(GTK_CONTAINER(view->box));
    GList *iter;

    for (iter = children; iter != NULL; iter = g_list_next(iter))
    {
      gtk_container_remove(GTK_CONTAINER(view->box), GTK_WIDGET(iter->data));
    }

    g_list_free(children);

    Label *difficulte = initLabel(difficulte);
    difficulte->text = "Difficulty :";
    CreateLabel(difficulte);

    Label *mode = initLabel(mode);
    mode->text = "Mode :";
    CreateLabel(mode);

    Label *time = initLabel(time);
    time->text = "Time :";
    CreateLabel(time);

    Label *color = initLabel(color);
    color->text = "Color :";
    CreateLabel(color);

    // Initialisation du bouton "Dernier mouvement"
    prSimpleBut *lastmove = init_button(lastmove, "Last Move");
    create_button(lastmove);

    // Initialisation du bouton "Prochain mouvement"
    prSimpleBut *nextmove = init_button(nextmove, "Next Move");
    create_button(nextmove);

    Connect_to_headerbar(view->hbar, lastmove->button, FALSE);
    Connect_to_headerbar(view->hbar, nextmove->button, FALSE);
    last_move_signal(lastmove, view);
    next_move_signal(nextmove, view);

    Grid *grid = initGrid(grid);
    CreateGrid(grid);
    Connect_to_Box(view->box, grid->grid, TRUE, TRUE, TRUE, 0);

    Box *boxright = initDefaultbox(boxright);
    boxright->size.x = 800;
    boxright->size.y = 900;
    BoxC(boxright);
    Box *boxleft = initDefaultbox(boxleft);
    boxleft->size.x = 400;
    boxleft->size.y = 900;
    BoxC(boxleft);

    Connect_to_Box(boxleft->box, view->scoreP1, TRUE, FALSE, FALSE, 15);
    margin_bot(view->scoreP1, 70);

    Connect_to_Box(boxleft->box, difficulte->label, TRUE, FALSE, FALSE, 15);
    Connect_to_Box(boxleft->box, view->difficulty, TRUE, FALSE, FALSE, 15);
    margin_bot(view->difficulty, 70);

    Connect_to_Box(boxleft->box, mode->label, TRUE, FALSE, FALSE, 15);
    Connect_to_Box(boxleft->box, view->mode, TRUE, FALSE, FALSE, 15);
    margin_bot(view->mode, 70);

    Connect_to_Box(boxleft->box, time->label, TRUE, FALSE, FALSE, 15);
    Connect_to_Box(boxleft->box, view->time, TRUE, FALSE, FALSE, 15);
    margin_bot(view->time, 70);

    Connect_to_Box(boxleft->box, color->label, TRUE, FALSE, FALSE, 15);
    Connect_to_Box(boxleft->box, view->color, TRUE, FALSE, FALSE, 15);
    margin_bot(view->color, 50);

    Connect_to_Box(boxleft->box, view->scoreP2, TRUE, FALSE, FALSE, 15);

    Connect_to_Box(boxright->box, view->labelTimerPlayer1, TRUE, FALSE, FALSE, 15);
    Connect_to_Box(boxright->box, view->grid, TRUE, TRUE, TRUE, 0);
    Connect_to_Box(boxright->box, view->labelTimerPlayer2, FALSE, FALSE, FALSE, 15);

    LinktoGrid(grid, boxleft->box, 0, 0);
    LinktoGrid(grid, boxright->box, 1, 0);

    color = 0;
    reset_view(view, 0, 0);

    gtk_widget_show_all(view->hbar);
    gtk_widget_show_all(view->box);
    shaders = 0;
  }
  else
  {
    diff = gtk_combo_box_get_active(GTK_COMBO_BOX(view->difficulty));
    lmode = gtk_combo_box_get_active(GTK_COMBO_BOX(view->mode));
    color = gtk_combo_box_get_active(GTK_COMBO_BOX(view->color));
    switch (gtk_combo_box_get_active(GTK_COMBO_BOX(view->time)))
    {
    case 0:
      totalTime = 60;
      break;
    case 1:
      totalTime = 300;
      break;
    case 2:
      totalTime = 5999;
      break;
    }
    reset_view(view, diff, lmode);
  }
}

void on_quit_button_clicked(GtkWidget *widget, gpointer data)
{
  gtk_main_quit(); // Exit the GTK main loop
}

void quitsignal(prSimpleBut *quitbutton)
{
  g_signal_connect(quitbutton->button, "clicked", G_CALLBACK(on_quit_button_clicked), NULL);
}

// Cette fonction applique un signal sur le button
void new_game_signal(prSimpleBut *newbutton, View *view)
{
  g_signal_connect(newbutton->button, "clicked", G_CALLBACK(new_game_button_clicked), view);
}

// Cette fonction retourne le widget grid de la vue.
GtkWidget *view_get_grid(View *view)
{
  return view->grid;
}

// Cette fonction permet de définir le widget hbar de la vue.
void view_set_header_bar(View *view, GtkWidget *hbar)
{
  view->hbar = hbar;
}

// Cette fonction permet de définir le widget difficulty de la vue.
void view_set_difficulty(View *view, GtkWidget *difficulty)
{
  view->difficulty = difficulty;
}

// Cette fonction permet de définir le widget mode de la vue.
void view_set_mode(View *view, GtkWidget *mode)
{
  view->mode = mode;
}

// Cette fonction permet de définir le widget box de la vue.
void view_set_box(View *view, GtkWidget *box)
{
  view->box = box;
}

// Cette fonction permet de définir le widget color de la vue.
void view_set_color(View *view, GtkWidget *color)
{
  view->color = color;
}

void view_set_time(View *view, GtkWidget *time)
{
  view->time = time;
}

void view_set_player1timelabel(View *view, GtkWidget *player1timelabel)
{
  view->labelTimerPlayer1 = player1timelabel;
}

void view_set_player2timelabel(View *view, GtkWidget *player2timelabel)
{
  view->labelTimerPlayer2 = player2timelabel;
}

void view_set_scoreP1(View *view, GtkWidget *scoreP1)
{
  view->scoreP1 = scoreP1;
}

void view_set_scoreP2(View *view, GtkWidget *scoreP2)
{
  view->scoreP2 = scoreP2;
}

void view_set_window(View *view, GtkWidget *window)
{
  view->wind = window;
}

// Cette fonction crée une nouvelle instance de la structure View, alloue la mémoire nécessaire et initialise la vue en appelant la fonction init_view.
View *view_new(void)
{
  View *view;

  view = g_malloc(sizeof(View));

  init_view(view);

  return view;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------MAIN--------------------------------------------------------------------------

void *gtkThread(gint argc, gchar *argv[])
{

  // Initialisation de GTK
  gtk_init(&argc, &argv);

  load_style_context();

  // Initialisation de la fenêtre
  MaFenetre *win = Init_default_window_prop(win);
  fenetre(win);

  // Initialisation de la barre d'en-tête
  Headerbar *hbar = init_headerbar(hbar);
  headerbar(hbar);

  Label *player1time = initLabel(player1time);
  CreateLabel(player1time);

  Label *player2time = initLabel(player2time);
  CreateLabel(player2time);

  Label *scoreP1 = initLabel(scoreP1);
  scoreP1->text = "Player 1 score :";
  CreateLabel(scoreP1);

  Label *scoreP2 = initLabel(scoreP2);
  scoreP2->text = "Player 2 score :";
  CreateLabel(scoreP2);

  // Initialisation du bouton "Nouvelle partie"
  prSimpleBut *newbutton = init_button(newbutton, "New Game");
  create_button(newbutton);

  // Initialisation du bouton "Quit"
  prSimpleBut *quitbutton = init_button(quitbutton, "Quit");
  create_button(quitbutton);

  // Initialisation de la combobox "Difficulté"
  ComboBox *difficulty = Init_comobox(difficulty);
  create_combobox(difficulty);
  add_item_combobox(difficulty, "Easy");
  add_item_combobox(difficulty, "Medium");
  add_item_combobox(difficulty, "Hard");
  set_active_item(difficulty);

  // Initialisation de la combobox "Mode"
  ComboBox *mode = Init_comobox(mode);
  create_combobox(mode);
  add_item_combobox(mode, "Human vs Human");
  add_item_combobox(mode, "AI vs Human");
  add_item_combobox(mode, "AI vs AI");
  set_active_item(mode);

  // Initialisation de la combobox "Couleur"
  ComboBox *color = Init_comobox(color);
  create_combobox(color);
  add_item_combobox(color, "White");
  add_item_combobox(color, "Black");
  set_active_item(color);

  // Initialisation de la combobox "time"
  ComboBox *time = Init_comobox(time);
  create_combobox(time);
  add_item_combobox(time, "1 min");
  add_item_combobox(time, "5 min");
  add_item_combobox(time, "OO");
  set_active_item(time);

  // Connexion de la barre d'en-tête à la fenêtre
  connect_headerbar_window(win, hbar->hbar);

  // Connexion des boutons à la barre d'en-tête
  Connect_to_headerbar(hbar->hbar, newbutton->button, FALSE);
  Connect_to_headerbar(hbar->hbar, quitbutton->button, TRUE);

  // Initialisation du box
  Box *box = initDefaultbox(box);
  BoxC(box);

  // Création de la vue
  view = view_new();
  view_set_window(view, win->wind);
  view_set_header_bar(view, hbar->hbar);
  view_set_difficulty(view, difficulty->combo);
  view_set_mode(view, mode->combo);
  view_set_box(view, box->box);
  view_set_color(view, color->combo);
  view_set_time(view, time->combo);
  view_set_player1timelabel(view, player1time->label);
  view_set_player2timelabel(view, player2time->label);
  view_set_scoreP1(view, scoreP1->label);
  view_set_scoreP2(view, scoreP2->label);
  view->client = client;
  new_game_signal(newbutton, view);
  quitsignal(quitbutton);

  updateTimerLabelPlayer1();
  updateTimerLabelPlayer2();

  // Initialisation des données du shader
  ShaderData Data;
  ShaderData_Default(&Data);
  Data.VertexShader = "vs.glsl";
  Data.PixelShader = "fs.glsl";
  Data.UpdateSpeed = 0.01;

  // Connexion de box au shader créé
  Connect_to_Box(box->box, Shader_Create(&Data, win->wind), TRUE, TRUE, TRUE, 0);

  // Ajout box à la fenêtre
  add_widget_window(win, box->box);

  // Affichage de la fenêtre et de tous ses éléments
  gtk_widget_show_all(win->wind);

  // Boucle principale GTK
  gtk_main();
}

void *dyadThread(void *arg)
{
  dyad_init();

  server = dyad_newStream();
  client = dyad_newStream();

  dyad_addListener(client, DYAD_EVENT_CONNECT, onConnect, NULL);
  dyad_addListener(client, DYAD_EVENT_DATA, onData, NULL);

  dyad_addListener(server, DYAD_EVENT_ERROR, onError, NULL);
  dyad_addListener(server, DYAD_EVENT_ACCEPT, onAccept, NULL);

  dyad_listenEx(server, "192.168.31.21", 8888, 1);

  printf("Waiting for peer to connect...\n");
  dyad_connect(client, "192.168.31.67", 9999);

  while (1)
  {
    dyad_update();
  }

  dyad_shutdown();
  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t gtkThreadID, dyadThreadID;

  // Création du thread GTK
  if (pthread_create(&gtkThreadID, NULL, gtkThread, NULL) != 0)
  {
    fprintf(stderr, "Failed to create GTK thread.\n");
    return 1;
  }

  // Création du thread dyad
  if (pthread_create(&dyadThreadID, NULL, dyadThread, NULL) != 0)
  {
    fprintf(stderr, "Failed to create dyad thread.\n");
    return 1;
  }

  // Attente de la fin des deux threads
  pthread_join(gtkThreadID, NULL);
  pthread_join(dyadThreadID, NULL);

  return 0;
}