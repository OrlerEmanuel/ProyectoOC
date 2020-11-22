#include "mapeo.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

/**
 Inicializa un mapeo vacio, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves sera computado mediante la funcion fHash.
 A todo efecto, la comparacion de claves se realizara mediante la funcion fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){
    tLista bucket;
    *m = (tMapeo) malloc(sizeof(struct mapeo));
    if((*m) == NULL)
        exit(MAP_ERROR_MEMORIA);
    (*m)->cantidad_elementos = 0;
    (*m)->longitud_tabla = ci>10?ci:10;
    (*m)->comparador = fComparacion;
    (*m)->hash_code = fHash;
    (*m)->tabla_hash = (tLista *) malloc(sizeof(tLista) * (*m)->longitud_tabla);

    if((*m)-> tabla_hash == NULL)
        exit(MAP_ERROR_MEMORIA);

    for(int i=0; i<((*m)->longitud_tabla);i++){
        crear_lista(& bucket);
        (*m)->tabla_hash[i]=bucket;
    }
}

/**
Procedimiento vacio, no tiene ningun efecto.
**/
void falsoEliminarEntrada(void * entry){};

/**
En caso que se excesa del factor de carga la funcion reHash incrementa
la cantidad de buckets
**/
void reHash(tMapeo m){
    tLista *tabla_Hash = m->tabla_hash;
    tLista  bucket;
    tLista *nuevaTablaHash;
    tPosicion fin;
    tPosicion puntero;
    tEntrada entry;
    int hash;
    int tamanio = m->longitud_tabla;
    m->longitud_tabla = tamanio*2;
    nuevaTablaHash = malloc (sizeof(tLista)* m->longitud_tabla);

    if(nuevaTablaHash == NULL)
        exit(MAP_ERROR_MEMORIA);

    for(int i=0; i<(tamanio*2) ;i++){
        crear_lista(&bucket);
        nuevaTablaHash[i] = bucket;
    }

    for(int i=0; i<tamanio ; i++){
        puntero = l_primera(tabla_Hash[i]);
        fin = l_fin(tabla_Hash[i]);
        while(puntero != fin){
            entry = l_recuperar(tabla_Hash[i],puntero);
            hash = m->hash_code(entry->clave) % (m->longitud_tabla);
            bucket = nuevaTablaHash[hash];
            l_insertar(bucket, l_primera(bucket) , entry);
            puntero = l_siguiente(tabla_Hash[i],puntero);
        }
        l_destruir(&tabla_Hash[i],&falsoEliminarEntrada);
    }
    free(m->tabla_hash);
    m->tabla_hash = nuevaTablaHash;
}

/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no exist�a en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
tValor m_insertar(tMapeo m, tClave c, tValor v){
    tValor retorno = NULL;
    tEntrada entry;
    int encontrado = 1;
    int hash = m->hash_code(c) % (m->longitud_tabla);
    tLista bucket = m->tabla_hash[hash];
    tPosicion puntero = l_primera(bucket);
    tPosicion finLista = l_fin(bucket);

    while(puntero != finLista && encontrado == 1){
        entry = l_recuperar(bucket,puntero);
        encontrado = m->comparador(entry->clave,c);
        if(encontrado == 1)
            puntero = l_siguiente(bucket,puntero);
        else{
            retorno = entry ->valor;
            entry->valor = v;
        }
    }

    if(encontrado == 1){
        entry = (tEntrada) malloc(sizeof(struct entrada));
        entry->clave = c;
        entry-> valor = v;
        l_insertar(bucket, l_primera(bucket) , entry );
        m->cantidad_elementos++;
    }
    if(((float)m->cantidad_elementos)/m->longitud_tabla > 0.75)
        reHash(m);
    return retorno;
}

/**
Libera la memoria reservada para una entrada apuntada por entry.
**/
void eliminarEntrada(void *  entry){
    tEntrada entrada = (tEntrada) entry;
    free(entrada);
}

/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    int hash = m->hash_code(c) % (m->longitud_tabla);
    tLista bucket = m->tabla_hash[hash];
    tPosicion puntero = l_primera(bucket);
    tPosicion finLista = l_fin(bucket);
    tEntrada entry;
    int encontrado = 1;

    while(puntero != finLista && encontrado == 1){
        encontrado = m->comparador(l_recuperar(bucket,puntero),c);
        if(encontrado == 1)
            puntero = l_siguiente(bucket,puntero);
    }

    if(encontrado==1){
        entry = l_recuperar(bucket,puntero);
        fEliminarC(entry->clave);
        fEliminarV(entry->valor);
        l_eliminar(bucket,puntero, &eliminarEntrada);
        m->cantidad_elementos--;
    }
}

/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    tLista * tabla_Hash = (*m)->tabla_hash;
    int tamanio = (*m)->longitud_tabla;
    tEntrada entry;
    tPosicion puntero;

    for(int i=0; i<tamanio ; i++){
        puntero = l_primera(tabla_Hash[i]);
        while(l_longitud(tabla_Hash[i])!= 0){
            entry = l_recuperar(tabla_Hash[i],puntero);
            fEliminarC(entry->valor);
            fEliminarV(entry->clave);
            l_eliminar(tabla_Hash[i],puntero,&eliminarEntrada);
        }
        l_destruir(&tabla_Hash[i],&eliminarEntrada);
    }

    (*m)->cantidad_elementos = 0;
    (*m)->longitud_tabla = 0;
    (*m)->comparador = NULL;
    (*m)->hash_code = NULL;

    free((*m)->tabla_hash);
    (*m)->tabla_hash = NULL;
    free(*m);
    (*m) = NULL;
}

/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/
tValor m_recuperar(tMapeo m, tClave c){
    tValor retorno = NULL;
    tEntrada entry;
    int encontrado = 1;
    int hash = m->hash_code(c) % (m->longitud_tabla);
    tLista bucket = m->tabla_hash[hash];
    tPosicion puntero = l_primera(bucket);
    tPosicion finLista = l_fin(bucket);

    while(puntero != finLista && encontrado == 1){
        entry = l_recuperar(bucket,puntero);
        encontrado = m->comparador(entry->clave,c);
        if(encontrado == 1)
            puntero = l_siguiente(bucket,puntero);
        else{
            entry = l_recuperar(bucket,puntero);
            retorno = entry->valor;
        }
    }
    return retorno;
}
