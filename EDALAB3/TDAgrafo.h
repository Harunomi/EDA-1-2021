#include <stdio.h>
#include <stdlib.h>


/*------------- estructura de datos -------------*/

typedef struct matrizGrafo{
	int cvertices;
	int** adyacencias;
}TDAgrafo;


/*----------------- operaciones -----------------*/

TDAgrafo* crearGrafoVacio(int vertices){
	TDAgrafo* grafo = (TDAgrafo*)malloc(sizeof(TDAgrafo));
	grafo->cvertices = vertices;
	grafo->adyacencias = (int**)malloc(vertices * sizeof(int*));
	int i,j;
	for (i = 0; i < vertices; i++){
		grafo->adyacencias[i] = (int*)malloc(vertices * sizeof(int));
		//Inicializa en cero
		for(j=0;j<vertices;j++){
			grafo->adyacencias[i][j] = 0;
		}	
	}
	return grafo;
}

void imprimirMatrizAdyacencia(TDAgrafo* grafo) {
	int i, j;
	for (i = 0; i < grafo->cvertices; i++){
		for (j = 0; j < grafo->cvertices; j++){
			printf("%d ", grafo->adyacencias[i][j]);
		}
		printf("\n");
	}
}

int adyacenciaNodos(TDAgrafo * grafo, int vertA, int vertB) {
	if (grafo->adyacencias[vertA - 1][vertB - 1] == 1){
		return 1;
	}
	return 0;
}

TDAgrafo* leerGrafo(char *nombreArchivo){
    FILE*pf;           //para abrir archivo
    pf = fopen(nombreArchivo,"r");
    int n, aristas;
    int i,j,k,valor;
    fscanf(pf, "%d %d", &n,&aristas); //Cantidad de nodos y aristas
    TDAgrafo *g=crearGrafoVacio(n);
    //dependiendo de las lineas de archivo , pone 1 para grafo no dirigido
    for(k=0;k<aristas;k++){
        fscanf(pf,"%d %d %d",&i, &j, &valor);
        g->adyacencias[i][j] = valor;
        g->adyacencias[j][i] = valor;
    }
    fclose(pf);
    return  g;    
}












