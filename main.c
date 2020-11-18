#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"


#define longitudMaxima  50
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
    valor = NULL;
    free(valor);
}

void fEliminarC(tClave clave){
    clave = NULL;
    free(clave);
}

int main(int argc, char *argv[]){
    tMapeo map;
    int opcion;
    int retorno = 0;
    int *cantApariciones;
    FILE *archivo;
    char argumento[] = "evaluador";
    char *palabra;
    //if(argc == 3 && strcmp(argv[1],argumento) == 0){
        //archivoLec= fopen(argv[2],"r");
        archivo= fopen("prueba.txt","r");
        if(archivo != NULL){
            crear_mapeo(&map,10,&hash,&comparador);
            while (fscanf(archivo,"%49s",palabra) != EOF){
                cantApariciones = m_recuperar(map, &palabra);
                if(cantApariciones != NULL){
                    *cantApariciones = (*cantApariciones) + 1;
                }
                else{
                   palabra = malloc(sizeof(char)*longitudMaxima);
                   cantApariciones = malloc(sizeof(int));
                   if(palabra != NULL && cantApariciones != NULL){
                        *cantApariciones = 1;
                        m_insertar(map,&palabra,cantApariciones);
                   }
                   else{

                   }
                }
            }
            printf("Seleccione una opcion: '\n'");
            printf("1. Cantidad de apariciones '\n'");
            printf("2. Salir '\n'");
            scanf("%d",&opcion);
            while(opcion == 1){
                int *cant;
                char aBuscar[longitudMaxima];
                printf("Ingrese la palabra de la cual desea saber su cantidad de aparaciones '\n'");
                scanf("%\n",&aBuscar);
                cant = m_recuperar(map,&aBuscar);
                if(cant != NULL)
                    printf("La cantidad de aparaciones de %s es de %d",aBuscar,(*cant));
                else
                    printf("La palabra no presenta aparaciones dentro del texto");
                printf("Seleccione una opcion: '\n'");
                printf("1. Cantidad de apariciones '\n'");
                printf("2. Salir '\n'");
                scanf("%d",&opcion);
            }
            m_destruir(map,&fEliminarC,&fEliminarV);
        }
        else
            retorno = -1;
        printf("%d",retorno);
        fclose(archivo);
    //}
    //else
       // retorno = -2;
    return retorno;
}
