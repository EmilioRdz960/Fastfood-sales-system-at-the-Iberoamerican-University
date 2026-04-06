#include "defKubo.h"
#include "tiposGTK.h"

// Cierra la aplicación
extern void CloseTheApp(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
    return;
}