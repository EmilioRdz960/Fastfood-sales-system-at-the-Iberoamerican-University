/** 
 * @file funElkubo.c
 * @brief Funciones del programa Elkubo.c
 * @author Emilio Rodriguez Díaz
 * @date 01/04/2025
*/

#include "defKubo.h"

void cajaLSF(nodoLSCF **ini, nodoLSCF **fin, int idCuenta, char nickname[], float saldo);
void imprimirLSF(nodoLSCF *aux);
void imprimirLSC(nodoLSCF *ini, nodoLSCF *fin);
void imprimirFacturas(nodoLSL *inicio);
void atenderColaCaja(nodoLDL *aux, float monedero);
void insertarColaNueva(nodoLSCF **ini, nodoLSCF **fin ,char alimento[], float tempMonedero, char tempNombre[], int tempNumCuenta);
void facturar(nodoLDL *aux, int tamAlimento, char tempNombre[], char alimento[], float tempMonedero);
void insertarFactura(nodoLSL **inicio, int tamAlimento, char tempNombre[], char alimento[], float tempMonedero);
void guardarArchivo(char archArbol[], int cuenta, char nombre[], int numTacos, int numPizzas, float saldo);
tipoArbol *adicionarArbol(tipoArbol *raiz, int cuenta, char Nombre[], char alimento[], int tamAlimento, float saldo);

tipoArbol *adicionarArbol(tipoArbol *raiz, int cuenta, char Nombre[], char alimento[], int tamAlimento, float saldo)
{
    tipoArbol *nuevo, *aux;
    int salir = 1;
    int numPizzas = 0;
    int numTacos = 0;

    aux = raiz;

    nuevo = (tipoArbol *)malloc(sizeof(tipoArbol)); //Asigna memoria para el nuevo nodo tipoArbol
    if(nuevo == NULL) // Verifica si hay memoria disponible
    {
        printf("\nNo hay memoria disponible.\n");
        exit(1);
    }
    // Asigna valores a los campos del nuevo nodo
    nuevo->digitosCuenta = cuenta;
    strcpy(nuevo->cliente, Nombre);
    nuevo->gasto = saldo;
    nuevo->izq = NULL;
    nuevo->der = NULL;

    if(strcmp(alimento, "tacos") == 0) // Si el pedido es tacos
    {
        nuevo->tamTacos = tamAlimento;
        nuevo->tamPizzas = numPizzas;
    }
    else // Si el pedido es pizzas
    {
        nuevo->tamTacos = numTacos;
        nuevo->tamPizzas = tamAlimento;
    }


  if(aux == NULL) // Si el árbol está vacío
    {
      raiz = nuevo; // Se inserta el primer dato
    }
    else
    {
        while((aux != NULL) && (salir != 0)) // Si hay un dato o más en el arbol
        {
            if(cuenta > aux->digitosCuenta) // mover a la derecha
	        {
	            if(aux->der != NULL) //se mueve a la derecha
	            {
	                aux = aux->der;
	            }
	            else // si aux->der == NULL se inserta el nodo nuevo
	            {
	                aux->der = nuevo;
	                salir = 0;
	            }
	  
	        }
            else if(cuenta <= aux->digitosCuenta) // mover a la izquierda
	        {
	            if(aux->izq != NULL) // Se mueve  a la izquierda
	            {
	                aux = aux->izq;
	            }
	            else // Si aux->izq == NULL se inserta el nodo nuevo
	            {
	                aux->izq = nuevo;
	                salir = 0;
	            }
	        }
            else
	        {
	            salir = 0;
	        }
        }
    }

  return raiz;
}

void guardarArchivo(char archArbol[], int cuenta, char nombre[], int numTacos, int numPizzas, float saldo)
{
    FILE *fp;

    fp = fopen(archArbol, "a"); //Anexa datos en el archivo historico.txt, en caso de que no exista, lo crea
    if (fp == NULL)
    {
        printf("\nArchivo no disponible.\n");
        exit(1);
    }
    fprintf(fp, "%d\t%s\t%d\t%d\t%.2f\n", cuenta, nombre, numTacos, numPizzas, saldo);
    fclose(fp); //Cerrar archivo historico.txt

    return ;
}

 void guardarHistorico(tipoArbol *aux, char archArbol[])
 {
    if(aux != NULL) 
    {
        guardarHistorico(aux->izq, archArbol);
        guardarArchivo(archArbol, aux->digitosCuenta, aux->cliente, aux->tamTacos, aux->tamPizzas, aux->gasto);
        guardarHistorico(aux->der, archArbol);
    }

    return;
 }

extern int buscarAlumno(tipoArbol *aux, int cuenta, char nombre[], int *tacos, int *pizzas, float *saldo)
{
    int exito = 0;

    while(aux != NULL) // Recorre el árbol
    {
        if(cuenta == aux->digitosCuenta) // Si el numero de cuenta coincide
        {
            strcpy(nombre, aux->cliente);
            *tacos = aux->tamTacos;
            *pizzas = aux->tamPizzas;
            *saldo = aux->gasto;
            exito = 1;
            break;
        }
        else // Si el numero de cuenta no coincide sigue buscando
        {
            if(cuenta > aux->digitosCuenta)
            {
                aux = aux->der; // Se mueve a la derecha
            }
            else
            {
                aux = aux->izq; // Se mueve a la izquierda
            }
        }
    }

    return exito; // Devuelve el valor de la variable exito

}

extern void imprimirArbAsc(tipoArbol *aux)
{

  if(aux != NULL)
    {
      imprimirArbAsc(aux->izq);
      printf("%d, %s, %d, %d, %f\n", aux->digitosCuenta, aux->cliente, aux->tamTacos, aux->tamPizzas, aux->gasto);
      imprimirArbAsc(aux->der);
    }
  
  return;
}

tipoArbol *insertarArbol(tipoArbol *pt, int cuenta, char nombre[], int numTacos, int numPizzas, float saldo)
{
    tipoArbol *nuevo, *aux;
    int salir = 1;

    aux = pt;

    nuevo = (tipoArbol *)malloc(sizeof(tipoArbol)); //Asigna memoria para el nuevo nodo tipoArbol
    if(nuevo == NULL)
    {
        printf("\nNo hay memoria disponible.\n");
        exit(1);
    }
    // Asigna valores a los campos del nuevo nodo
    nuevo->digitosCuenta = cuenta;
    strcpy(nuevo->cliente, nombre);
    nuevo->tamTacos = numTacos;
    nuevo->tamPizzas = numPizzas;
    nuevo->gasto = saldo;
    nuevo->izq = NULL;
    nuevo->der = NULL;

  if(aux == NULL) // Si el árbol está vacío
    {
      pt = nuevo; // Se inserta el primer dato
    }
    else
    {
        while((aux != NULL) && (salir != 0)) // Si hay un dato o más en el arbol
        {
            if(cuenta > aux->digitosCuenta) // mover a la derecha
	        {
	            if(aux->der != NULL) //se mueve a la derecha
	            {
	                aux = aux->der;
	            }
	            else // si aux->der == NULL se inserta el nodo nuevo
	            {
	                aux->der = nuevo;
	                salir = 0;
	            }
	  
	        }
            else if(cuenta <= aux->digitosCuenta) // mover a la izquierda
	        {
	            if(aux->izq != NULL) // Se mueve  a la izquierda
	            {
	                aux = aux->izq;
	            }
	            else // Si aux->izq == NULL se inserta el nodo nuevo
	            {
	                aux->izq = nuevo;
	                salir = 0;
	            }
	        }
            else
	        {
	            salir = 0;
	        }
        }
    }

  return pt;
}

extern void actualizarClientes(nodoLDL *aux, char archClientes[])
{
    FILE *fp;

    fp = fopen(archClientes, "w"); //Reescribe el archivo clientes.txt
    if (fp == NULL)
    {
        printf("\nArchivo no disponible.\n");
        exit(1);
    }

    while(aux != NULL) // Recorre la lista doble LIFO (principal)
    {
        if(strcmp(aux->cola, "caja") == 0) // Si es la cola de caja
        {
            while(aux->ini != NULL) // Recorre la lista FIFO (sublista)
            {
                fprintf(fp, "%d\t%s\t%.2f\n", aux->ini->numCuenta, aux->ini->cliente, aux->ini->monedero);
                aux->ini = aux->ini->next;
            }
        }
        aux = aux->der;
    }

    return;
}

/**
 * @details Esta función se encarga de guardar las facturas generadas en facturas.txt ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void generarFactura(nodoLDL *aux)
{
    FILE *fp;
    nodoLSL *temp;

    temp = aux->start; //El nodo temporal toma la de el inicio de la lista de facturas

    fp = fopen("facturas.txt", "a");//Anexa datos en el archivo facturas.txt, en caso de que no exista, lo crea
    if (fp == NULL) //Verificamos integridad del archivo
    {
        printf("\nArchivo no disponible.\n");
        exit(1);
    }

    while(aux != NULL) //Recorre la lista completa
    {
        if(strcmp(aux->cola, "facturacion") == 0) //Si es la cola de facturación
        {
            while(temp != NULL) // Recorre la lista de facturas
            {
                fprintf(fp, "Factura #%d, Cliente: %s, Compra: %s, Monto: %.2f\n", temp->numFactura, temp->NombreTicket, temp->compra, temp->montoAcumulado); 
                temp = temp->sig;
            }           
        }
        aux = aux->der;
    }
     

    return;
}

/**
 * @details Esta función se encarga de atender la cola de tacos. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void atenderColaTacos(nodoLDL *aux, tipoArbol *raiz)
{
    nodoLSCF *borra; 
    int saldo;


        while(aux != NULL) // Recorre la lista LIFO (principal)
        {
            if(strcmp(aux->cola, "tacos") == 0) // Si es la cola de tacos
            {

                if((aux->ini == NULL) && (aux->fin == NULL)) // Si la cola esta vacia
                {
                    printf("\nNo hay clientes en la cola de tacos\n");
                }
                else // Si hay un cliente o mas en la cola
                {
                    saldo = aux->ini->monedero; // Obtenemos el saldo de la cola
                    if(aux->ini == aux->fin) // Si solo hay un cliente en la cola
                    {
                        borra = aux->ini;
                        aux->ini = NULL;
                        aux->fin = NULL;
                        free(borra);
                        aux->montoTotal = aux->montoTotal - saldo;
                        aux->tamClientes = aux->tamClientes - 1;
                    }       
                    else // Si hay mas de un cliente en la cola
                    {
                        borra = aux->ini;
                        aux->ini = aux->ini->next;
                        free(borra);
                        aux->montoTotal = aux->montoTotal - saldo;
                        aux->tamClientes = aux->tamClientes - 1;
                    }
                }
            }
            aux = aux->der;
        }
    

    
    return;
}

/**
 * @details Esta función se encarga de atender la cola de pizzas. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void atenderColaPizzas(nodoLDL *aux, tipoArbol *raiz)
{
    nodoLSCF *borra; 
    int saldo;

    while(aux != NULL) // Recorre la lista LIFO (principal)
    {
        if(strcmp(aux->cola, "pizzas") == 0) // Si la cola es de pizzas
        {
            printf("\n%p\t %p\n", aux->ini, aux->fin); // Muestra las direcciones de los nodos
            if((aux->ini == NULL) && (aux->fin == NULL)) // Si la cola esta vacia
            {
                printf("\nNo hay clientes en la cola de tacos\n");
            }
            else // Si hay un cliente o mas en la cola
            {
                saldo = aux->ini->monedero;
                borra = aux->ini;
                if(aux->ini == aux->fin) // Si solo hay un cliente en la cola
                {
                    aux->ini = NULL;
                    aux->fin = NULL;
                    free(borra);
                    aux->montoTotal = aux->montoTotal - saldo;
                    aux->tamClientes = aux->tamClientes - 1;
                }
                else // Si hay mas de un cliente en la cola
                {    
                    aux->ini = aux->ini->next;
                    aux->fin->next = aux->ini;
                    free(borra);
                    aux->montoTotal = aux->montoTotal - saldo;
                    aux->tamClientes = aux->tamClientes - 1;
                }
            }

        
        }
        aux = aux->der;
    }
    
    return;
}

/**
 * @details Esta función se encarga de atender pizzas. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void atenderPizzas(nodoLDL *aux)
{
    nodoLSCF *borra; 
    int saldo;

    while(aux != NULL)
    {
        if(strcmp(aux->cola, "pizzas") == 0) // Si es la cola de pizzas
        {
            while(aux->ini != NULL) // Recorre la lista FIFO (sublista)
            {
                //saldo = aux->ini->monedero;
                if(aux->ini == NULL) // Si la cola esta vacia
                {
                    printf("\nNo hay clientes en la cola de tacos\n");
                }
                else  // Si hay un cliente o mas en la cola
                {
                    borra = aux->ini;
                    if(aux->ini == aux->fin) // Si solo hay un cliente en la cola
                    {
                        aux->ini = NULL;
                        aux->fin = NULL;
                        free(borra);
                    }
                    else // Si hay mas de un cliente en la cola
                    {     
                        aux->ini = aux->ini->next;
                        aux->fin->next = aux->ini;
                        free(borra);
                    }
                    aux->montoTotal = aux->montoTotal - saldo;
                    aux->tamClientes = aux->tamClientes - 1;
                }

            }
        }
        aux = aux->der;
    }

    return;
}

/**
 * @details Esta función se encarga de atender tacos. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void atenderTacos(nodoLDL *aux)
{
    nodoLSCF *borra; 
    int saldo;



    while(aux != NULL) // Recorre la lista LIFO (principal)
    {
        if(strcmp(aux->cola, "tacos") == 0) // Si es la cola de tacos
        {
            while(aux->ini != NULL) // Recorre la lista FIFO (sublista)
            {
                saldo = aux->ini->monedero;
                if(aux->ini == NULL) // Si la cola esta vacia
                {
                    printf("\nNo hay clientes en la cola de tacos\n");
                }
                else // Si hay un cliente o mas en la cola
                {
                    if(aux->ini == aux->fin) // Si solo hay un cliente en la cola
                    {
                        borra = aux->ini;
                        aux->ini = NULL;
                        aux->fin = NULL;
                        free(borra);
                        aux->montoTotal = aux->montoTotal - saldo;
                        aux->tamClientes = aux->tamClientes - 1;
                    }      
                    else // Si hay mas de un cliente en la cola
                    {
                        borra = aux->ini;
                        aux->ini = aux->ini->next;
                        free(borra);
                        aux->montoTotal = aux->montoTotal - saldo;
                        aux->tamClientes = aux->tamClientes - 1;
                    }
                }
            }
        }
        aux = aux->der;
    }

    return;
}

/**
 * @details Esta función se encarga de insertar una nueva factura. ||LISTA||
 * @param **inicio: puntero de puntero de tipo nodoLSL.
 * @param tamAlimento: variable de tipo int.
 * @param tempNombre: variable de tipo char.
 * @param alimento: variable de tipo char.
 * @param tempMonedero: variable de tipo float.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void insertarFactura(nodoLSL **inicio, int tamAlimento, char tempNombre[], char alimento[], float tempMonedero)
{
    nodoLSL *nuevo;

    nuevo = (nodoLSL *)malloc(sizeof(nodoLSL));
    if(nuevo == NULL)
    {
        printf("\nNo hay memoria disponible.\n");
        exit(1);
    }

    nuevo->numFactura = rand() % 1000;
    strcpy(nuevo->NombreTicket, tempNombre);
    strcpy(nuevo->compra, alimento);
    //printf("\n%s\n", alimento);
    if(strcmp(alimento, "tacos") == 0) // Valida si el alimento es tacos
    {
        nuevo->montoAcumulado = tamAlimento * 25.00;
    }
    else // Valida si el alimento es pizza
    {
        nuevo->montoAcumulado = tamAlimento * 89.00;
    }

    if(*inicio == NULL) // Si la lista de facturas esta vacia
    {
        *inicio = nuevo;
        nuevo->sig = NULL;
    }
    else // si hay un dato o mas en la lista de facturas
    {
        nuevo->sig = *inicio;
        *inicio = nuevo;
    }

    return;
}

/**
 * @details Esta función se encarga de facturar. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @param tamAlimento: variable de tipo int.
 * @param tempNombre: variable de tipo char.
 * @param alimento: variable de tipo char.
 * @param tempMonedero: variable de tipo float.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void facturar(nodoLDL *aux, int tamAlimento, char tempNombre[], char alimento[], float tempMonedero)
{
    while(aux != NULL) // Recorre la lista LIFO (principal)
    {
        if(strcmp(aux->cola, "facturacion") == 0) // Si la lista es facturacion
        {
            aux->tamClientes = aux->tamClientes + 1;
            aux->montoTotal = aux->montoTotal + tempMonedero;
            insertarFactura(&aux->start, tamAlimento, tempNombre, alimento, tempMonedero);
        }
        aux = aux->der; // mueve al siguiente nodo
    }
    

    return;
}

/**
 * @details Esta función se encarga de insertar al clinte en la lista de tacos o pizzas según sea el caso. ||LISTA||
 * @param **ini: puntero de puntero de tipo nodoLSCF.
 * @param **fin: puntero de puntero de tipo nodoLSCF.
 * @param alimento: variable de tipo char.
 * @param tempMonedero: variable de tipo float.
 * @param tempNombre: variable de tipo char.
 * @param tempNumCuenta: variable de tipo int.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void insertarColaNueva(nodoLSCF **ini, nodoLSCF **fin, char alimento[], float tempMonedero, char tempNombre[], int tempNumCuenta)
{
    nodoLSCF *nuevo;

    nuevo = (nodoLSCF *)malloc(sizeof(nodoLSCF)); //Asigna memoria para el nuevo nodo tipoLSCF
     if(nuevo == NULL) // Verifica si hay memoria disponible
     {
        printf("\nNo hay memoria disponible.\n");
        exit(1);
     }
     // Asigna valores a los campos del nuevo nodo
     strcpy(nuevo->cliente, tempNombre);
     nuevo->monedero = tempMonedero;
     nuevo->numCuenta = tempNumCuenta;


        if(strcmp(alimento, "tacos") == 0) //Se inserta lista Fifo
        {
            if((*ini == NULL) && (*fin == NULL)) //La lista esta vacia
            {
                *ini = nuevo;
                *fin = nuevo;
                nuevo->next = NULL;
            }
            else // Hay un nodo o mas en la lista
            {
                (*fin)->next = nuevo;
                (*fin) = nuevo;
                nuevo->next = NULL;
            }

        }
        else // Se inserta lista circular simple (pizzas)
        {
            if((*ini == NULL) && (*fin == NULL)) //La lista esta vacia
            {
                *ini = nuevo;
                *fin = nuevo;
                nuevo->next = *ini;
            }
            else //Hay un nodo o mas
            {
                nuevo->next = *ini;
                (*fin)->next = nuevo;
                *fin = nuevo;
            }
            
        }


    return;
}

/**
 * @details Esta función se encarga de atender al primer cliente de la cola y lo elimina de la lista. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @param monedero: variable de tipo float.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void atenderColaCaja(nodoLDL *aux, float monedero)
{
    nodoLSCF *borra;

    if((aux->ini == NULL) && (aux->fin == NULL)) // Si la cola esta vacia
    {
        printf("\nNo se puede atender una cola vacia\n");
    }
    else if(aux->ini == aux->fin) //Hay una persona en la fila
    {
        aux->montoTotal = aux->montoTotal - monedero;
        aux->tamClientes = aux->tamClientes- 1;
        borra = aux->ini;
        aux->ini = NULL;
        aux->fin = NULL;
        free(borra);
    }
    else //Hay mas de una persona en la fila
    {
        aux->montoTotal = aux->montoTotal - monedero;
        aux->tamClientes = aux->tamClientes - 1;
        borra = aux->ini;
        aux->ini = aux->ini->next;
        free(borra);
    }

    return;
}

/**
 * @details Esta función se encarga de atender la caja y le permite al cliente elegir entre tacos y pizzas. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void atenderCaja(nodoLDL *aux, tipoArbol *raiz)
{
    nodoLDL *principio;
    principio = aux; // principio toma la direccion valor de aux

    char tempNombre[20];
    int tempNumCuenta;
    float tempMonedero;

    int tamAlimento = 0;
    char alimento[40];
    float cuentaTotal = 0;
    int opcion = 1;

    while(aux != NULL) // Recorre la lista LIFO (principal)
    {
        if(strcmp(aux->cola, "caja") == 0) // Si la cola es caja
        {
            strcpy(tempNombre, aux->ini->cliente);
            tempMonedero =  aux->ini->monedero;
            tempNumCuenta = aux->ini->numCuenta;
            atenderColaCaja(aux, tempMonedero);
        }
        aux = aux->der;
    }
    aux = principio; // aux toma la direccion de principio

    printf("\nUsted cuenta con un saldo de %f\n", tempMonedero); // Muestra el saldo del cliente

    do // Ciclo para pedir alimentos
    {
        printf("\nEscriba si desea ordenar tacos o pizza [tacos/pizzas]\n");
        scanf("%s", alimento);
        printf("\nCuantos desea ordenar?\n");
        scanf("%d", &tamAlimento);
        
        if(strcmp(alimento, "tacos") == 0) // Si el pedido es tacos
        {
            cuentaTotal = tamAlimento * 25.00;
            if(tempMonedero >= cuentaTotal) // Si el saldo es suficiente
            {
                while(aux != NULL) // Recorre la lista LIFO (Principal)
                {  
                    //printf("\n%s\n", aux->cola);
                    if(strcmp(aux->cola, "tacos") == 0) // Si el alimento solicitado es tacos
                    {
                        //printf("\n%s\n", aux->cola);
                        tempMonedero = tempMonedero - (25 * tamAlimento );
                        aux->montoTotal = aux->montoTotal + tempMonedero;
                        aux->tamClientes = aux->tamClientes + 1;
                        insertarColaNueva(&aux->ini, &aux->fin, alimento, tempMonedero, tempNombre, tempNumCuenta);
                        raiz = adicionarArbol(raiz, tempNumCuenta, tempNombre, alimento, tamAlimento, tempMonedero);
                    }
                    aux = aux->der;
                }
                printf("\nEl cliente ha pasado a la cola de %s\n", alimento);
                break;
            }
            else // Si el saldo no es suficiente
            {
                printf("saldo insuficiente, favor de revisar su orden\n");
                tamAlimento = 0;
                cuentaTotal = 0;
            }
        }
        else if(strcmp(alimento, "pizzas") == 0) // Si el pedido es pizzas
        {
            cuentaTotal = tamAlimento * 89.00;
            if(tempMonedero >= cuentaTotal) // Si el saldo es suficiente para pizzas
            {
                while(aux != NULL) // Recorre la lista LIFO (principal)
                {  
                    //printf("\n%s\n", aux->cola);
                    if(strcmp(aux->cola, "pizzas") == 0) // Si el alimento solicitado es pizzas
                    {
                        //printf("\n%s\n", aux->cola);
                        tempMonedero = tempMonedero - (89 * tamAlimento );
                        aux->montoTotal = aux->montoTotal + tempMonedero;
                        aux->tamClientes = aux->tamClientes + 1;
                        insertarColaNueva(&aux->ini, &aux->fin, alimento, tempMonedero, tempNombre, tempNumCuenta);
                        raiz = adicionarArbol(raiz, tempNumCuenta, tempNombre, alimento, tamAlimento, tempMonedero);
                    }
                    aux = aux->der;
                }
                printf("\nEl cliente ha pasado a la cola de %s\n", alimento);
                break;
            }
            else // Si el saldo no es suficiente
            {
                printf("saldo insuficiente, favor de revisar su orden\n");
                tamAlimento = 0;
                cuentaTotal = 0;
            }
        }
        else // Si la orden o cantidad de alimentos es incorrecta
        {
            printf("orden o cantidad de alimentos incorrecta, favor de revisar su orden\n");
            tamAlimento = 0;
            cuentaTotal = 0;
        }
                

    }while(opcion == 1);

    aux = principio;
    facturar(aux, tamAlimento, tempNombre, alimento, tempMonedero);

    return;
}

/**
 * @details Esta función se encarga de imprimir las facturas. ||LISTA||
 * @param inicio: puntero de tipo nodoLSL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void imprimirFacturas(nodoLSL *inicio)
{
    if (inicio == NULL) // Si la lista de facturas esta vacia
    {
        printf("No hay facturas.\n");
    }
    else // Si hay un dato o mas en la lista de facturas
    {
        while (inicio != NULL)
        {
            printf("-Factura #%d, Cliente: %s, Compra: %s, Monto: %.2f\n", inicio->numFactura, inicio->NombreTicket, inicio->compra, inicio->montoAcumulado);
            inicio = inicio->sig;
        }
    }
    return;
}

/**
 * @details Esta función se encarga de imprimir los clientes de la cola de pizzas. ||LISTA||
 * @param ini: puntero de tipo nodoLSCF.
 * @param fin: puntero de tipo nodoLSCF.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void imprimirLSC(nodoLSCF *ini, nodoLSCF *fin)
{

    if((ini == NULL) && (fin == NULL)) // Si la lista esta vacia
    {
        printf("\nCola Vacia\n");
    }
    else // Si hay un nodo o mas en la lista
    {
        do
        {
            printf("-Cliente %s, cuenta %d, saldo %.2f\n", ini->cliente, ini->numCuenta, ini->monedero);
            ini = ini->next; 
        }while(ini != fin->next);
    }

    return;
}

/**
 * @details Esta función se encarga de imprimir los clientes de la lista FIFO. ||LISTA||
 * @param aux: puntero de tipo nodoLSCF.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void imprimirLSF(nodoLSCF *aux)
{
    if(aux == NULL) // Si la lista esta vacia
    {
        printf("\nCola vacia\n");
    }
    else // Si hay un nodo o mas en la lista
    {
        while(aux != NULL) // Recorre la lista FIFO
        {
            printf("\n-Cliente %s, cuenta %d, saldo %.2f\n", aux->cliente, aux->numCuenta, aux->monedero);
            aux = aux->next;
        }
    }
    return;
}

/**
 * @details Esta función se encarga de imprimir el estado del sistema. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void imprimirListaCompleta(nodoLDL *aux)
{
    while(aux != NULL) // Recorre la lista LIFO (principal)
    {
        printf("\n[Fila] %s, tamaño %d, monto acumulado %.2f\n", aux->cola, aux->tamClientes, aux->montoTotal);
        if(strcmp(aux->cola, "pizzas") == 0) // Si es la cola de pizzas
        {
            imprimirLSC(aux->ini, aux->fin);
        }
        else if(strcmp(aux->cola, "facturacion") == 0) // Si es la cola de facturacion
        {
            imprimirFacturas(aux->start);
        }
        else // Si es la cola de tacos
        {
            imprimirLSF(aux->ini);
        }

        aux = aux->der;
    }
    

    return;
}

/**
 * @details Esta función se encarga de insertar el cliente en la lista FIFO. ||LISTA||
 * @param **ini: puntero de puntero de tipo nodoLSCF.
 * @param **fin: puntero de puntero de tipo nodoLSCF.
 * @param idCuenta: variable de tipo int.
 * @param nickname: variable de tipo char.
 * @param saldo: variable de tipo float.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
void cajaLSF(nodoLSCF **ini, nodoLSCF **fin, int idCuenta, char nickname[], float saldo)
{
    nodoLSCF *nuevo;
    
    nuevo = (nodoLSCF *)malloc(sizeof(nodoLSCF)); //Asigna memoria para el nuevo nodo tipoLSCF
    if(nuevo == NULL) // Verifica si hay memoria disponible
    {
        printf("\nNo hay memoria disponible.\n");
        exit(1);
    }
    // Asigna valores a los campos del nuevo nodo
    strcpy(nuevo->cliente, nickname);
    nuevo->numCuenta = idCuenta;
    nuevo->monedero = saldo;
    nuevo->next = NULL;
    
    if((*ini == NULL) && (*fin == NULL)) //Lista Vacia
    {
        *ini = nuevo;
        *fin = nuevo;
        nuevo->next = NULL;
    }
    else //Hay un elemento en la lista
    {
        (*fin)->next = nuevo;
        *fin = nuevo;
    }

    return;
}

/**
 * @details Esta función se encarga de imprimir el estado de la lista doble lineal. ||LISTA||
 * @param aux: puntero de tipo nodoLDL.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void imprimirLDL(nodoLDL *aux)
{
    while(aux != NULL) // Recorre la lista LIFO (principal)
    {
        printf("Fila %s, tamaño %d, monto acumulado %.2f\t", aux->cola, aux->tamClientes, aux->montoTotal);
        printf("[%p| %p |%p]\n", aux->izq, aux, aux->der);
        printf("[%p| %p |%p]\n", aux->ini, aux->start, aux->fin);
        aux = aux->der;
    }

    return;
}


/**
 * @details Esta función se encarga de crear un nuevo nodo en la LDL. ||LISTA||
 * @param **inicio: puntero de puntero de tipo nodoLDL.
 * @param colaAlimento: variable de tipo char.
 * @param tamCliente: variable de tipo int.
 * @param monto: variable de tipo float.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void crearLDL(nodoLDL **inicio, char colaAlimento[], int tamCliente, float monto)
{
    nodoLDL *nuevo;

    nuevo = (nodoLDL *) malloc(sizeof(nodoLDL)); //Asigna memoria para el nuevo nodo tipoLDL
    if(nuevo == NULL) // Verifica si hay memoria disponible
    {
        printf("/nNo hay memoria disponible.\n");
        exit(1);
    } // Asigna valores a los campos del nuevo nodo
    nuevo->ini = NULL;
    nuevo->fin = NULL;
    nuevo->start = NULL;
    strcpy(nuevo->cola, colaAlimento);
    nuevo->tamClientes = tamCliente;
    nuevo->montoTotal = monto;

    if(*inicio == NULL) //Lista Vacia
    {
        *inicio = nuevo;
        nuevo->izq = NULL;
        nuevo->der = NULL;
    }
    else //Hay un elemento o mas
    {
        nuevo->izq = NULL;
        nuevo->der = *inicio;
        (*inicio)->izq = nuevo;
        *inicio = nuevo;        
    }

    return;
}

/**
 * @details Esta función se encarga de insertar clientes en la caja. ||LISTA||
 * @param *inicio: puntero de tipo nodoLDL.
 * @param archClientes: variable de tipo char.
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void insertarClientes(nodoLDL *inicio, char archClientes[])
{
    nodoLDL *aux;
    FILE *fp;
    char nickname[40];
    float saldo;
    int idCuenta;

    aux = inicio; // aux toma la direccion de inicio
    
    fp = fopen(archClientes, "r"); //Abrir archivo clientes.txt
    if(fp == NULL) //Verificamos integridad del archivo
    {
        printf("\nArchivo negocio.txt no disponible.\n");
        exit(1);
    }

    while(aux != NULL) // Recorre la lista LIFO (principal)
    {
        if(strcmp(aux->cola, "caja") == 0) // Si es la cola de caja
        {
            while(fscanf(fp, "%d\t%[^\t]%f", &idCuenta, nickname, &saldo) == 3) // Lee los datos del archivo clientes.txt
            {
                if(saldo >= 25.00) // Si el saldo es suficiente
    	        {   
                    cajaLSF(&aux->ini, &aux->fin, idCuenta, nickname, saldo);
                    aux->montoTotal += saldo;
                    aux->tamClientes++;
                }
                else // Si el saldo no es suficiente
                {
                    printf("El cliente %s no cuenta con el saldo minimo ($25.00) para realizar un pedido [saldo actual: %.2f]\nSacando de la cola...\n", nickname, saldo);
                }
                {
                    printf("El cliente %s no cuenta con el saldo minimo ($25.00) para realizar un pedido [saldo actual: %.2f]\nSacando de la cola...\n", nickname, saldo);
                }
                
            }
        }
        aux = aux->der; //Nos movemos al siguiente nodo
    }
    fclose(fp); //Cerrar archivo clientes.txt

    return;
}

/**
 * @details Esta función se encarga de darle una interfaz al menu. ||LISTA||
 * @param void
 * @return void
 * @author Emilio Rodriguez Díaz
 */
extern void menu(void)
 {
    printf("\n");
    printf("╔═════════════════════════════════╗\n");
    printf("║        Menú Principal           ║\n");
    printf("╠═════════════════════════════════╣\n");
    printf("║  1. Atender cliente en Caja     ║\n");
    printf("║  2. Atender cliente en Tacos    ║\n");
    printf("║  3. Atender cliente en Pizzas   ║\n");
    printf("║  4. Buscar cliente en historico ║\n");
    printf("║  5. Imprimir estado del arbol   ║\n");
    printf("║  6. Imprimir estado del sistema ║\n");
    printf("║  7. Salir                       ║\n");
    printf("╚═════════════════════════════════╝\n");
    printf("Seleccione una opción: ");

    return;
 }