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
    int longitud;
    int retorno = 0;
    FILE *archivoEsc;
    FILE *archivoLec;
    char caracter = ' ';
    char argumento[] = "evaluador";
    //if(argc == 3 && strcmp(argv[1],argumento) == 0){
      //  arhivoEsc = fopen(argv[2],"r");
        //archivoLec= fopen(argv[2],"r");
        archivoEsc = fopen("prueba.txt","r");
        archivoLec= fopen("prueba.txt","r");
        if(archivoEsc != NULL){
            crear_mapeo(&map,10,&hash,&comparador);
            while(caracter != EOF){
                int *valor;
                longitud = 0;
                while((caracter = fgetc(archivoLec)) != EOF && caracter != ' ')
                    longitud++;
                char *palabra = malloc(sizeof(char)*longitud);
                for(int i=0; i<longitud; i++)
                    palabra[i] = fgetc(archivoEsc);
                fgetc(archivoEsc);
                printf("%s'\n'",palabra);
                valor = m_recuperar(map, &palabra);
                if(valor == NULL){
                    valor = malloc(sizeof(int));
                    *valor = 1;
                }
                else{
                    *valor = (*valor) + 1;
                }
                m_insertar(map,&palabra,valor);
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
        fclose(archivoEsc);
        fclose(archivoLec);
    //}
    //else
       // retorno = -2;
    return retorno;
}
