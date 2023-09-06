#include <gtk\gtk.h>

gboolean fin(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void saisie(GtkButton *button, GtkWidget *view);

int main(int argc, char* argv[])
{
    GtkWidget* window;
    GtkWidget* box;
    GtkWidget* text_view;
    GtkWidget* button;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 200);
    gtk_window_set_title(GTK_WINDOW(window), "XIII. Les zones de texte.");
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),0);

    box=gtk_vbox_new(FALSE,5);
    gtk_container_add(GTK_CONTAINER(window),box);

    text_view=gtk_text_view_new();
    gtk_box_pack_start(GTK_BOX(box),text_view,TRUE,TRUE,0);

    button=gtk_button_new_with_label("Afficher");
    gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(saisie),text_view);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

gboolean fin(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    gtk_widget_destroy(widget);
    gtk_main_quit();

    return 0;
}

void saisie(GtkButton *button, GtkWidget *view)
{
    GtkWidget *dialog;
    GtkTextBuffer* text_buffer=0;
    GtkTextIter start;
    GtkTextIter end;
    gchar* buf=0;

    //On récupère le buffer
    text_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    //On récupère l'origine du buffer
    gtk_text_buffer_get_start_iter(text_buffer,&start);
    //On récupère la fin du buffer
    gtk_text_buffer_get_end_iter(text_buffer,&end);

    //On copie le contenu du buffer dans une variable
    buf=gtk_text_buffer_get_text(text_buffer,&start, &end,TRUE);

    //On affiche le texte dans une boite de dialogue.
    dialog = gtk_message_dialog_new(NULL,
    GTK_DIALOG_MODAL,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "Votre texte :\n%s", buf);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    //On libere la memoire
    g_free(buf);
}
