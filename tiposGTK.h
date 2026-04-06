/*struct referencias
{
    GtkWidget *entryPesos; //Campo de entrada
    GtkWidget *labelResultado; //Etiquetas de texto
};

typedef struct referencias ref;*/


struct referencias
{
    GtkWidget *entryTP;
    GtkWidget *entryCantidad;
    GtkWidget *labelAsignarCola;
    // ...otros campos...
    tipoArbol *iniArbol;
    nodoLDL *comienzo;
    // ...otros labels si los necesitas...
};
typedef struct referencias ref;