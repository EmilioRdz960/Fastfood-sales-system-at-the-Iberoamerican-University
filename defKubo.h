/**
 *@file: defCafe.h
 *@brief: Este el Header File para Elkubo.c
 * @author: Emilio Rodriguez Díaz
 * @date: 01/04/2025
 */

#include <gtk-2.0/gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cajaAlimentos
{
  int numCuenta;
  char cliente[40];
  float monedero;

  struct cajaAlimentos *next;
};
typedef struct cajaAlimentos nodoLSCF;

struct facturacion
{
  int numFactura;
  char NombreTicket[40];
  char compra[40];
  float montoAcumulado;

  struct facturacion *sig;
};
typedef struct facturacion nodoLSL;

struct principal
{
  char cola[40];
  float montoTotal;
  int tamClientes;

  struct principal *izq, *der;
  nodoLSCF *ini, *fin; //lista de Tacos y Pizzas (FIFO Y CIRCULAR)
  nodoLSL *start; //lista de facturas
};
typedef struct principal nodoLDL;

struct arbol
{
  int digitosCuenta;
  char cliente[40];
  int tamTacos;
  int tamPizzas;
  float gasto;
  struct arbol *izq, *der;
};
typedef struct arbol tipoArbol;