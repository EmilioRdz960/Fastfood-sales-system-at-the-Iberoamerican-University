/**
 * @file: appKubo.c
 * @brief: Este programa permite atender a los clientes de la cafetería Elkubo.c
 * @details: El programa se encarga de recibir los pedidos de los clientes, los clientes se llegan a formar una sola Cola en Caja (FIFO). 
 * En la caja, los clientes una vez que pagan su pizza o sus tacos (son atendidos) salen de la cola.
 * De ahí sus pedidos son enviados a una Cola de Tacos (FIFO) o a una Cola de Pizzas (LSC).
 * Los clientes llegan a la cola de las pizzas o tacos y una vez que les entregan su comida,
 * salen de la cola sus solicitudes (de pizzas o tacos).
 * El negocio lleva los tickets de facturación en un histórico que es una Pila (stack) deFacturas (LIFO).
 * Todas las colas del sistema están integradas en una Lista Doble Lineal.
 * @author: Rodriguez Díaz
 * @date: 01/04/2025
 */

#include "defKubo.h"
#include "tiposGTK.h"

void menu(void);
void crearLDL(nodoLDL **inicio, char colaAlimento[], int tamCliente, float monto);
void insertarClientes(nodoLDL *inicio, char archClientes[]);
void imprimirLDL(nodoLDL *aux);
void imprimirListaCompleta(nodoLDL *aux);
void atenderCaja(nodoLDL *aux, tipoArbol *raiz);
void atenderTacos(nodoLDL *aux);
void atenderPizzas(nodoLDL *aux);
void atenderColaTacos(nodoLDL *aux);
void atenderColaPizzas(nodoLDL *aux);
void generarFactura(nodoLDL *aux);
void actualizarClientes(nodoLDL *aux, char archClientes[]);
tipoArbol *insertarArbol(tipoArbol *pt, int cuenta, char nombre[], int numTacos, int numPizzas, float saldo);
void imprimirArbAsc(tipoArbol *aux);
void guardarHistorico(tipoArbol *aux, char archArbol[]);
int buscarAlumno(tipoArbol *aux, int cuenta, char nombre[], int *tacos, int *pizzas, float *saldo);

// Prototipos de callbacks para GTK
void CloseTheApp(GtkWidget *widget, gpointer data);

int main(int argc, char *argv[])
{
    FILE *fp;
    int opcion, coincidencia = 0;
    char cola[40], nickname[40], archArbol[20];
    char archClientes[20], tacos[20], pizzas[20];
    float monto, saldo;
    int tamClien, idCuenta, tamPizzas, tamTacos;

    //variable GTk
    ref base;
    base.iniArbol = NULL;
    base.comienzo = NULL;


    //0. Defino los objetos de GTK

    GtkWidget *window, *windowArbolBinario, *venataArbolBinario, *hboxPrincipal, *vboxIzquierda, *vboxDerecha, *hboxLDL; //Ventana
    GtkWidget *botonAtenderCaja, *botonAtenderTacos, *botonAtenderPizzas, *botonImprimirSistema, *botonImprimirArbol;
    GtkWidget *botonBuscarCliente, *botonSalir, *botonCargarDatos; //Boton para convertir
    GtkWidget *botonIzqNodoLDL, *botonDerNodoLDL, *botonIzqNodoLSCF, *botonDerNodoLSCF; //Botones para mover el nodo en la lista
    GtkWidget *labelPresentacion,*labelSaldo, *labelQDO,*entryTP, *labelCDO, *entryCantidad, *labelAsignarCola;//Atender clientes en caja
    GtkWidget *labelPresentacion2, *labelmensajeTacos; //Atender clientes en tacos
    GtkWidget *labelPresentacion3, *labelmensajePizzas; //Atender clientes en pizzas
    GtkWidget *labelPresentacion4, *labelIngresarNumeroCuenta, *entryNumCliente, *labelAlumnoEncontrado, *labelnumeroCuenta, *labelNombre, *labeltactos, *labelpizzas, *labelsaldo; //Buscar cliente en el historico (arbol)
    GtkWidget *labelPresentacion6, *botonIzqNodoLDL, *botonDerNodoLDL, *labelReporteCCS, *botonIzqNodoLSCF, *labelCNS, *botonDerNodoLSCF;//Navegar Listas

    GtkWidget *vBox, *hBox, *hBox2, vboxLDL, hboxLDL; //Caja de botones

    GtkWidget *imagen; //Imagen de la cafetería

    GtkWidget *imagenTacos, *imagenPizzas, *imagenCaja;//Imagenes

    GtkWidget *linea;//Separador

    GdkPixbuf *pixbufOriginal, *pixbufEscalado;//Para escalar la imagen

    //Colores de botones
    GdkColor azulFondo, blancoTexto;
    GdkColor verdeFondo, negroTexto;
    GdkColor rojoFondo, negroTexto2;
    GdkColor amarilloFondo, negroTexto3;
    GdkColor moradoFondo, negroTexto4;
    GdkColor naranjaFondo, negroTexto5;
    GdkColor azulOscuroFondo, negroTexto6;
    GdkColor azulTexto;//Texto color azul



    //1. Inicializar el ambiente

    gtk_init(&argc, &argv);

    //2. Creación de objetos y modificación de atributos

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//Ventana principal
    gtk_window_set_title(GTK_WINDOW(window),"Menu");//Titulo de la ventana
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);//Borde de la ventana
    gtk_window_set_default_size(GTK_WINDOW(window), 850, 650);//Tamaño de la ventana

    windowArbolBinario = gtk_window_new(GTK_WINDOW_TOPLEVEL);//Ventana principal
    gtk_window_set_title(GTK_WINDOW(window),"Historico");//Titulo de la ventana
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);//Borde de la ventana
    gtk_window_set_default_size(GTK_WINDOW(window), 850, 650);//Tamaño de la ventana

    // Asignación de imagenes
    imagenTacos = gtk_image_new_from_file("tacos.jpg"); // Carga la imagen desde un archivo tacos.jpg
    imagenPizzas = gtk_image_new_from_file("pizza.jpg"); // Carga la imagen desde un archivo pizza.jpg
    imagenCaja = gtk_image_new_from_file("caja.jpg"); // Carga la imagen desde un archivo caja.jpg

    //Dibujar el Menú principal de la izquierda
    botonAtenderCaja = gtk_button_new_with_label("Atender caja");
    botonAtenderTacos = gtk_button_new_with_label("Atender Tacos");
    botonAtenderPizzas = gtk_button_new_with_label("Atender Pizzas");
    botonImprimirSistema = gtk_button_new_with_label("Buscar cliente en el historico");
    botonImprimirArbol = gtk_button_new_with_label("Estado del árbol.");
    botonBuscarCliente = gtk_button_new_with_label("Navegar Listas.");
    botonSalir = gtk_button_new_with_label("Salir");


    //Botones para mover el nodo en la lista
    botonDerNodoLDL = gtk_button_new_with_label("-->");
    botonIzqNodoLDL = gtk_button_new_with_label("<--");
    botonDerNodoLSCF = gtk_button_new_with_label("|-->|");
    botonIzqNodoLSCF = gtk_button_new_with_label("|<--|");
   /* 3. Cambiar colores de fondo y texto */

    // Para botónAtenderCaja (Azul cielo con texto blanco)
    gdk_color_parse("#87CEEB", &azulFondo);     // Azul cielo
    gdk_color_parse("#FFFFFF", &blancoTexto);   // Blanco
    gtk_widget_modify_bg(botonAtenderCaja, GTK_STATE_NORMAL, &azulFondo);//Cambiar el fondo del botón
    gtk_widget_modify_fg(botonAtenderCaja, GTK_STATE_NORMAL, &blancoTexto);//Cambiar el color del texto del botón

    // Para botónAtenderTacos (Verde brillante con texto negro)
    gdk_color_parse("#00FF00", &verdeFondo);    // Verde
    gdk_color_parse("#000000", &negroTexto);    // Negro
    gtk_widget_modify_bg(botonAtenderTacos, GTK_STATE_NORMAL, &verdeFondo);
    gtk_widget_modify_fg(botonAtenderTacos, GTK_STATE_NORMAL, &negroTexto);

    // Para botónAtenderPizzas (Rojo oscuro con texto negro)
    gdk_color_parse("#FF0000", &rojoFondo);    // Rojo
    gdk_color_parse("#000000", &negroTexto2);    // Negro
    gtk_widget_modify_bg(botonAtenderPizzas, GTK_STATE_NORMAL, &rojoFondo);
    gtk_widget_modify_fg(botonAtenderPizzas, GTK_STATE_NORMAL, &negroTexto2);

    // Para botónImprimirSistema (Amarillo claro con texto negro)
    gdk_color_parse("#FFFF00", &amarilloFondo);    // Amarillo 
    gdk_color_parse("#000000", &negroTexto3);    // Negro
    gtk_widget_modify_bg(botonImprimirSistema, GTK_STATE_NORMAL, &amarilloFondo);
    gtk_widget_modify_fg(botonImprimirSistema, GTK_STATE_NORMAL, &negroTexto3);

    // Para botónImprimirArbol (Morado oscuro con texto negro)
    gdk_color_parse("#800080", &moradoFondo);    // Morado
    gdk_color_parse("#000000", &negroTexto4);    // Negro
    gtk_widget_modify_bg(botonImprimirArbol, GTK_STATE_NORMAL, &moradoFondo);
    gtk_widget_modify_fg(botonImprimirArbol, GTK_STATE_NORMAL, &negroTexto4);

    // Para botónBuscarCliente (Naranja claro con texto negro)
    gdk_color_parse("#FFA500", &naranjaFondo);    // Naranja
    gdk_color_parse("#000000", &negroTexto5);    // Negro
    gtk_widget_modify_bg(botonBuscarCliente, GTK_STATE_NORMAL, &naranjaFondo);    
    gtk_widget_modify_fg(botonBuscarCliente, GTK_STATE_NORMAL, &negroTexto5);

    // Para botónSalir (Azul oscuro con texto negro)
    gdk_color_parse("#0000FF", &azulOscuroFondo);    // Azul
    gdk_color_parse("#000000", &negroTexto6);    // Negro
    gtk_widget_modify_bg(botonSalir, GTK_STATE_NORMAL, &azulOscuroFondo);    
    gtk_widget_modify_fg(botonSalir, GTK_STATE_NORMAL, &negroTexto6);

    // Botones para mover el nodo en la lista



     // 4. Etiquetas y entradas (vbox derecha)

    /* //Atender clientes en caja
    labelPresentacion = gtk_label_new("Atender clientes en caja"); // Etiqueta para la caja
    linea = gtk_hseparator_new();// Separador
    labelSaldo = gtk_label_new("Usted cuenta con un saldo de:"); // Etiqueta para el saldo
    entryTP = gtk_entry_new(); // Entrada para el tipo de pedido
    labelQDO = gtk_label_new("Escriba si desea ordenar tacos o pizzas (tacos/pizzas):"); // Etiqueta para el tipo de pedido
    labelCDO = gtk_label_new("Cuantos desea ordenar?:"); // Etiqueta para el cantidad
    entryCantidad = gtk_entry_new(); // Entrada para el cantidad
    botonCargarDatos = gtk_button_new_with_label("Cargar datos"); // Botón para cargar datos
    labelAsignarCola = gtk_label_new("El cliente ha pasado a la cola de:"); // Etiqueta pasar cliente a la cola del pedido 




    // Para labelPresentacion (Azul fuerte)
    gdk_color_parse("#0000FF", &azulTexto); // Azul fuerte
    gtk_widget_modify_fg(labelPresentacion, GTK_STATE_NORMAL, &azulTexto);*/

    //Para atender clientes en Tacos
    /*labelPresentacion2 = gtk_label_new("Atender clientes en tacos");
    labelmensajeTacos = gtk_label_new("Se atendio correctamente");

    // Para labelPresentacion2 (Azul fuerte)
    gdk_color_parse("#0000FF", &azulTexto); // Azul fuerte
    gtk_widget_modify_fg(labelPresentacion2, GTK_STATE_NORMAL, &azulTexto);

    //Para atender clientes en Pizzas
    labelPresentacion3 = gtk_label_new("Atender clientes en pizzas");
    labelmensajePizzas = gtk_label_new("Se atendio correctamente");

    // Para labelPresentacion3 (Azul fuerte)
    gdk_color_parse("#0000FF", &azulTexto); // Azul fuerte
    gtk_widget_modify_fg(labelPresentacion3, GTK_STATE_NORMAL, &azulTexto);

    //Para buscar cliente en el historico (arbol)
    labelPresentacion4 = gtk_label_new("Buscar cliente en el historico (arbol)");
    labelIngresarNumeroCuenta = gtk_label_new("Ingrese el numero de la cuenta del cliente a buscar:");
    entryTP = gtk_entry_new();
    labelAlumnoEncontrado = gtk_label_new("Alumno encontrado!");
    labelnumeroCuenta = gtk_label_new("Numero de cuenta:");
    labelNombre = gtk_label_new("Nombre:");
    labeltactos = gtk_label_new("Tacos:");
    labelpizzas = gtk_label_new("Pizzas:");
    labelsaldo = gtk_label_new("Saldo:");

    // Para labelPresentacion4 (Azul fuerte)
    gdk_color_parse("#0000FF", &azulTexto); // Azul fuerte
    gtk_widget_modify_fg(labelPresentacion4, GTK_STATE_NORMAL, &azulTexto);

    // Para imprimir estado del árbol
    labelPresentacion5 = gtk_label_new("Imprimir estado del árbol");
    labelResultadoArbol = gtk_label_new("123456, Carlos, 18, 9, 50.000000");

    //Para labelPresentacion5 (Azul fuerte)
    gdk_color_parse("#0000FF", &azulTexto); // Azul fuerte
    gtk_widget_modify_fg(labelPresentacion5, GTK_STATE_NORMAL, &azulTexto);*/

   

    //Para Navegar Listas
    labelPresentacion6 = gtk_label_new("Navegar Listas");
    hBox = gtk_hbox_new(FALSE, 5);
    botonIzqNodoLDL = gtk_button_new_with_label("<--");
    botonDerNodoLDL = gtk_button_new_with_label("-->");
    labelReporteCCS = gtk_label_new("Caja,TamClientes,Saldo");
    hBox2 = gtk_hbox_new(FALSE, 5);
    botonIzqNodoLSCF = gtk_button_new_with_label("<--");
    labelCNS = gtk_label_new("Cuenta,Nombre,Saldo");
    botonDerNodoLSCF = gtk_button_new_with_label("-->");

    

    // Para labelPresentacion6 (Azul fuerte)
    gdk_color_parse("#0000FF", &azulTexto); // Azul fuerte
    gtk_widget_modify_fg(labelPresentacion6, GTK_STATE_NORMAL, &azulTexto);

    

    //3. Conectar los callbacks a los objetos o acciones

    g_signal_connect(GTK_OBJECT(window), "destroy",GTK_SIGNAL_FUNC(CloseTheApp), &base);

    // Botón Atender Caja
    //g_signal_connect(G_OBJECT(botonAtenderCaja), "clicked", G_CALLBACK(callbackAtenderCaja), &base);
    //g_signal_connect(G_OBJECT(botonCargarDatos), "clicked", G_CALLBACK(callbackCargarDatos), &base);


     //4. Define la jerarquía de instancias

     // Caja horizontal principal
    hboxPrincipal = gtk_hbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(window), hboxPrincipal);//Agregar contorno
    


    // Lado izquierdo (Botones principales)
    vboxIzquierda = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hboxPrincipal), vboxIzquierda, FALSE, FALSE, 10);
    gtk_box_pack_start_defaults(GTK_BOX(vboxIzquierda), botonAtenderCaja);
    gtk_box_pack_start_defaults(GTK_BOX(vboxIzquierda), botonAtenderTacos);
    gtk_box_pack_start_defaults(GTK_BOX(vboxIzquierda), botonAtenderPizzas);
    gtk_box_pack_start_defaults(GTK_BOX(vboxIzquierda), botonImprimirSistema);
    gtk_box_pack_start_defaults(GTK_BOX(vboxIzquierda), botonImprimirArbol);
    gtk_box_pack_start_defaults(GTK_BOX(vboxIzquierda), botonBuscarCliente);
    gtk_box_pack_start_defaults(GTK_BOX(vboxIzquierda), botonSalir);

    //Lado derecho
    gtk_box_pack_start(GTK_BOX(hboxLDL), botonIzqNodoLDL, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hboxLDL), imagenTacos, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hboxLDL), botonDerNodoLDL, FALSE, FALSE, 5);

    // Empaquetar el hbox y el label de información en el vbox

    // Finalmente, agrega vboxLDL al contenedor derecho principal (vboxDerecha)
    gtk_box_pack_start(GTK_BOX(vboxDerecha), vboxLDL, FALSE, FALSE, 10);

    //Atender caja
   /* vboxDerecha = gtk_vbox_new(FALSE, 10);
    gtk_box_pack_start(GTK_BOX(hboxPrincipal), vboxDerecha, TRUE, TRUE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelPresentacion);
    gtk_box_pack_start(GTK_BOX(vboxDerecha), linea, FALSE, TRUE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelSaldo);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelQDO);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), entryTP);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelCDO);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), entryCantidad);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), botonCargarDatos);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelAsignarCola);*/

    vboxDerecha = gtk_vbox_new(FALSE, 10);
    hBox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hboxPrincipal), vboxDerecha, TRUE, TRUE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), hBox);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), hBox);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), botonIzqNodoLDL);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelReporteCCS);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), botonDerNodoLDL);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelReporteCCS);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), botonIzqNodoLSCF);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelCNS);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), botonDerNodoLSCF);
    //Atender tacos
   /* gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelPresentacion2);
    gtk_box_pack_start(GTK_BOX(vboxDerecha), linea, FALSE, TRUE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelmensajeTacos);

    //Atender pizzas
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelPresentacion3);
    gtk_box_pack_start(GTK_BOX(vboxDerecha), linea, FALSE, TRUE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelmensajePizzas);

    //Buscar cliente en el historico (arbol)
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelPresentacion4);
    gtk_box_pack_start(GTK_BOX(vboxDerecha), linea, FALSE, TRUE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelIngresarNumeroCuenta);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), entryTP);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelAlumnoEncontrado);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelnumeroCuenta);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelNombre);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labeltactos);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelpizzas);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelsaldo);

    //Imprimir estado del sistema
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelPresentacion6);
    gtk_box_pack_start(GTK_BOX(vboxDerecha), linea, FALSE, TRUE, 5);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelReportePizzas);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelResultadoReportePizzas);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelReporteFacturas);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelResultadoReporteFacturas);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelReporteTacos);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelResultadoReporteTacos);
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelReporteCaja);    
    gtk_box_pack_start_defaults(GTK_BOX(vboxDerecha), labelResultadoReporteCaja);*/


    // Imagen de cafetería
    pixbufOriginal = gdk_pixbuf_new_from_file("cafeteria.jpg", NULL);
    pixbufEscalado = gdk_pixbuf_scale_simple(pixbufOriginal, 100, 100, GDK_INTERP_BILINEAR); // cambia tamaño aquí
    imagen = gtk_image_new_from_pixbuf(pixbufEscalado);
    gtk_box_pack_start_defaults(GTK_BOX(vboxIzquierda), imagen);

    //5. Mostrar los widgets

    gtk_widget_show_all(window);

    //6. Iniciar el loop

    gtk_main();

    //7. No olvides programar el fin de la App 
    

    strcpy(archClientes, argv[1]); // guardamos el nombre del archivo clientes.txt
    strcpy(archArbol, argv[3]); // guardamos el nombre del archivo historico.txt

    fp = fopen(argv[2], "r"); //Abrir archivo negocio.txt
    if (fp == NULL) //Verificamos integridad del archivo
    {
        printf("/nArchivo negocio.txt no disponible.\n");
        exit(1);
    }
    
    while(fscanf(fp, "%s\t%d\t%f", cola, &tamClien, &monto) == 3) 
    {
        crearLDL(&base.comienzo, cola, tamClien, monto);
    }
    fclose(fp); //Cerrar archivo negocio.txt

    imprimirLDL(base.comienzo); //Funcion para imprimir y verificar la integridad de la lista LDL

    insertarClientes(base.comienzo, archClientes); //Funcion para insertar clientes en la cola caja
    
    printf("\nNodo inicial %s\n", base.comienzo->cola);

    fp = fopen(argv[3], "r"); //Abrir archivo historico.txt
    if (fp == NULL) //Verificamos integridad del archivo
    {
        printf("/nArchivo historico.txt no disponible.\n");
        exit(1);
    }

    while(fscanf(fp, "%d\t%[^\t]%d\t%d\t%f", &idCuenta, nickname, &tamTacos, &tamPizzas, &saldo) == 5)
    {
        base.iniArbol = insertarArbol(base.iniArbol, idCuenta, nickname, tamTacos, tamPizzas, saldo);
    }
    fclose(fp); //Cerrar archivo historico.txt

    imprimirArbAsc(base.iniArbol);

    imprimirListaCompleta(base.comienzo); //Funcion para imprimir y verificar la integridad de la lista completa
    do 
    {
        menu();
        scanf("%d", &opcion);

        switch (opcion) 
        {
        case 1:
            printf("Atender cliente en Caja\n");
            atenderCaja(base.comienzo, base.iniArbol);
            break;
        case 2:
            printf("Atender cliente en Tacos\n");
            atenderColaTacos(base.comienzo);
            break;
        case 3:
            printf("Atender cliente en Pizzas\n");
            atenderColaPizzas(base.comienzo);
            break;
        case 4:
            printf("\nBuscar cliente en historico (arbol)\n");
            printf("Ingrese el numero de la cuenta del cliente a buscar: ");
            scanf("%d", &idCuenta);

            if(buscarAlumno(base.iniArbol, idCuenta, nickname, &tamTacos, &tamPizzas, &saldo) == 1)
            {
                printf("\nAlumno encontrado!.\n");
                printf("\nNumero de cuenta: %d\n", idCuenta);
                printf("Nombre: %s\n", nickname);
                printf("Tacos: %d\n", tamTacos);
                printf("Pizzas: %d\n", tamPizzas);
                printf("Saldo: %.2f\n", saldo);
            }
            else
            {
                printf("\nAlumno no encontrado.\n");
            }
            printf("\n\n");
                break;

            break;
        case 5:
            printf("\nImprimir estado del arbol\n");
            imprimirArbAsc(base.iniArbol);
            break;
        case 6:
            printf("\nImprimir estado del sistema\n");
            imprimirListaCompleta(base.comienzo);
            break;
        case 7:
            printf("Salir\n");
            atenderTacos(base.comienzo);
            atenderPizzas(base.comienzo);
            generarFactura(base.comienzo);
            actualizarClientes(base.comienzo, archClientes);
            guardarHistorico(base.iniArbol, archArbol);
            imprimirListaCompleta(base.comienzo);
            break;
        default:
            printf("Opción inválida, por favor ingresa una opción correcta.\n");
            break;
        }
    }while (opcion != 7);
    

    return 0;
}