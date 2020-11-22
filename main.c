#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"

#define longitudMaxima 50
#define EVL_ERROR_MEMORIA -3
#define ARGS_INCORRECTOS -2
#define ERROR_APERTURA_ARCHIVO -1

int hash(void* s){
    char* string = s;
    int hash = 0;
    for (int i = 0; i < strlen(string); i++)
        hash = 31 * hash + string[i];
    return hash;
}

int comparador(void* s1, void* s2){
    int toReturn = strcmp(s1,s2);
    if(toReturn < 0 || toReturn > 0)
        toReturn = 1;
    return toReturn;
}

void fEliminarV(tValor valor){
    free(valor);
}

void fEliminarC(tClave clave){
    free(clave);
}

int evaluador(char* ruta){
    tMapeo map;
    int *cantidad;
    int *cantApariciones;
    int opcion, retorno;
    FILE *archivo;
    char *aInsertar;
    char aBuscar [longitudMaxima], palabra[longitudMaxima];
    retorno = 0;
    archivo= fopen(ruta,"r");
    if(archivo != NULL){
        crear_mapeo(&map,10,&hash,&comparador);
        while (fscanf(archivo,"%s",palabra) != EOF){
            cantApariciones = m_recuperar(map, &palabra);
            if(cantApariciones != NULL){
                (*cantApariciones) = (*cantApariciones) + 1;
            }
            else{
                aInsertar = malloc(sizeof(char)*(strlen(palabra)+1));
                cantApariciones = malloc(sizeof(int));
                strcpy(aInsertar,palabra);
                if(palabra != NULL && cantApariciones != NULL){
                    (*cantApariciones) = 1;
                    m_insertar(map,aInsertar,cantApariciones);
                }
                else{
                    exit(EVL_ERROR_MEMORIA);
                }
            }
        }
        fclose(archivo);
        printf("Seleccione una opcion: \n");
        printf("1. Cantidad de apariciones. \n");
        printf("2. Salir. \n");
        scanf("%d",&opcion);
        while(opcion == 1){
            printf("Ingrese la palabra de la cual desea saber su cantidad de aparaciones: ");
            fflush(stdin);
            scanf("%s",aBuscar);
            cantidad = m_recuperar(map,&aBuscar);
            if(cantidad != NULL)
                printf("\n La cantidad de aparaciones de %s es de %d. \n",aBuscar,(*cantidad));
            else
                printf("La palabra no presenta aparaciones dentro del texto. \n");
            printf(" \n Seleccione una opcion: \n");
            printf("1. Cantidad de apariciones. \n");
            printf("2. Salir. \n");
            fflush(stdin);
            scanf("%d",&opcion);
        }
        m_destruir(&map,&fEliminarC,&fEliminarV);
        printf("Gracias por utilizar evaluador.");
    }
    else
        exit(ERROR_APERTURA_ARCHIVO);
    return retorno;
}

int main(int argc, char *argv[]){
    if(argc != 2)
        exit(ARGS_INCORRECTOS);
    return evaluador(argv[1]);
}
