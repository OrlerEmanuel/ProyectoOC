#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
/**
 Inicializa una lista vac�a.
 Una referencia a la lista creada es referenciada en *L.
 Finaliza indicando LST_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
extern void crear_lista(tLista * l){
    (*l) = (tLista) malloc(sizeof(struct celda));
    if((*l) == NULL)
        exit(LST_ERROR_MEMORIA);
    (*l)->elemento=NULL;
    (*l)->siguiente=NULL;
}
/**
 Inserta el elemento E, en la posici�n P, en L.
 Con L = A,B,C,D y la posici�n P direccionando C, luego:
 L' = A,B,E,C,D.
 Finaliza indicando LST_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion current,newPos;
    current = p->siguiente;
    newPos = (tPosicion) malloc(sizeof(struct celda));
    if(newPos == NULL)
        exit(LST_ERROR_MEMORIA);
    newPos->elemento = e;
    newPos->siguiente = current;
    p->siguiente = newPos;
}
/**
 Elimina la celda P de L.
 El elemento almacenado en la posici�n P es eliminado mediante la funci�n fEliminar.
 Finaliza indicando LST_POSICION_INVALIDA si P es fin(L).
**/
void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)){
    tPosicion next,toRemove;
    if(p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);
    next = p->siguiente->siguiente;
    toRemove = p->siguiente;
    fEliminar(toRemove->elemento);
    free(toRemove);
    p->siguiente = next;
}
/**
 Destruye la lista L, elimininando cada una de sus celdas.
 Los elementos almacenados en las celdas son eliminados mediante la funci�n fEliminar.
**/
void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    tPosicion first = (*l);
    tPosicion toRemove = first->siguiente;
    while(toRemove!=NULL){
        first->siguiente = toRemove->siguiente;
        fEliminar(toRemove->elemento);
        toRemove->elemento = NULL;
        toRemove->siguiente = NULL;
        free(toRemove);
        toRemove = first->siguiente;
    }
    free(l);
    (*l) = NULL;
}

 /**
 Recupera y retorna el elemento en la posici�n P.
 Finaliza indicando LST_POSICION_INVALIDA si P es fin(L).
**/
tElemento l_recuperar(tLista l, tPosicion p){
    if(p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);
    return (p->siguiente)->elemento;
}

/**
 Recupera y retorna la primera posici�n de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_primera(tLista l){
    return l;
}

/**
 Recupera y retorna la posici�n siguiente a P en L.
 Finaliza indicando LST_NO_EXISTE_SIGUIENTE si P es fin(L).
**/
tPosicion l_siguiente(tLista l, tPosicion p){
    if(p->siguiente == NULL)
        exit(LST_POSICION_INVALIDA);
    return p->siguiente;
}

/**
 Recupera y retorna la posici�n anterior a P en L.
 Finaliza indicando LST_NO_EXISTE_ANTERIOR si P es primera(L).
**/
tPosicion l_anterior(tLista l, tPosicion p){
    tPosicion current;
    if(l == p)
        exit(LST_NO_EXISTE_ANTERIOR);
    current = l;
    while(current->siguiente != p)
        current = current->siguiente;
    return current;
}

/**
 Recupera y retorna la �ltima posici�n de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_ultima(tLista l){
    tPosicion current = l;
    int found = current->siguiente == NULL;
    while(!found){
        current = current->siguiente;
        found = current->siguiente->siguiente == NULL;
    }
    return current;
}

/**
 Recupera y retorna la posici�n fin de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_fin(tLista l){
    tPosicion current = l;
    int found = current->siguiente == NULL;
    while(!found){
        current = current->siguiente;
        found = current->siguiente == NULL;
    }
    return current;
}

/**
 Retorna la longitud actual de la lista.
**/
int l_longitud(tLista l){
    tPosicion current = l;
    int lon;
    lon = 0;
    while(current->siguiente!=NULL){
        lon = lon +1;
        current =  current->siguiente;
    }
    return lon;
}
