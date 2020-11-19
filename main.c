#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"


#define longitudMaxima  20
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

int main(int argc, char *argv[]){
    tMapeo map;
    int opcion;
    int retorno = 0;
    int *cantidad;
    int *cantApariciones;
    int finDeArchivo = 0;
    FILE *archivo;
    char aBuscar [longitudMaxima];
    char argumento[] = "evaluador";
    char palabra[longitudMaxima];
    //if(argc == 3 && strcmp(argv[1],argumento) == 0){
        //archivo= fopen(argv[2],"r");
        archivo= fopen("teamRocketLema.txt","r");
        if(archivo != NULL){
            crear_mapeo(&map,1000,&hash,&comparador);
            while (!finDeArchivo){
                palabra[0] = malloc(sizeof(char)*(longitudMaxima));
                if(fscanf(archivo,"%s",palabra)!=EOF){
                    printf(" la palabra leida es %s \n",palabra);
                    cantApariciones = m_recuperar(map, &palabra);
                    if(cantApariciones != NULL){
                        printf(" modifica valor de '%s', no inserta '\n'",palabra);
                        (*cantApariciones) = (*cantApariciones) + 1;
                        m_insertar(map,&palabra,cantApariciones);
                        free(palabra);
                        printf("cantidad de apariciones %d '\n'", *cantApariciones);
                    }
                    else{
                        cantApariciones = malloc(sizeof(int)); //preguntar
                        printf(" hace los malloc %s '\n'",palabra);
                        if(palabra != NULL && cantApariciones != NULL){
                            (*cantApariciones) = 1;
                            printf("cantidad de apariciones %d '\n'", *cantApariciones);
                            m_insertar(map,&aInsertar,cantApariciones);
                            printf(" hizo la insercion %s '\n'",aInsertar);
                        }
                        else{
                            printf(" no reserva espacio '\n'");
                        }
                    }
                }
                else
                    finDeArchivo = 1;
            }
            fclose(archivo);
            printf("Seleccione una opcion: '\n'");
            printf("1. Cantidad de apariciones '\n'");
            printf("2. Salir '\n'");
            scanf("%d",&opcion);
            while(opcion == 1){
                printf("Ingrese la palabra de la cual desea saber su cantidad de aparaciones '\n'");
                scanf("%'\n'",&aBuscar);
                cantidad = m_recuperar(map,&aBuscar);
                if(cantidad != NULL)
                    printf("La cantidad de aparaciones de %s es de %d",aBuscar,(*cantidad));
                else
                    printf("La palabra no presenta aparaciones dentro del texto");
                printf("Seleccione una opcion: '\n'");
                printf("1. Cantidad de apariciones '\n'");
                printf("2. Salir '\n'");
                scanf("%d",&opcion);
            }
            m_destruir(&map,&fEliminarC,&fEliminarV);
        }
        else
            retorno = -1;
        printf("%d",retorno);

    //}
    //else
       // retorno = -2;
    return retorno;
}
