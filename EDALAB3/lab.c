#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TDAgrafo.h"

/* Estructura para los archivos de entrada*/
typedef struct Insumos{
	int **matriz;
	int subsidio;
	int capacidadTotal;
}insumos;
/* Estructura para la cola*/
typedef struct listaCola{
	int id;
	int idAnterior;
	int costo;
	int peso;
	int *caminoRecorrido;
	int largoCaminoRecorrido;
	int posicion;
}TDAcola;
int aux;
/* Dominio: un string
Recorrido: una variable de tipo insumos
Descripcion: Funcion que permite leer los archivos de entrada y almacenarlos*/
insumos leerInsumos(char *nombreArchivo);
/* Dominio: int costo, int peso, int *caminoRecorrido,int largoCaminoRecorrido,int idAnterior
Recorrido: TDAcola
Descripcion: Funcion que permite crear un nodo a partir de los datos entregados*/
TDAcola crearNodo(int costo,int peso,int posicion,int largoCaminoRecorrido,int idAnterior);
/* Dominio: un arreglo de enteros, un entero para el largo del arreglo y el dato a agregar
Recorrido: un arreglo
Descripcion: funcion que permite agregar al final un dato a un arreglo de enteros*/
int *append(int *lista,int size,int dato);
/* Dominio: una Cola, el tamano de la cola, el dato a encolar
Recorrido: una cola
Descripcion: Funcion que permite encolar un dato a una cola*/
TDAcola *encolar(TDAcola* cola,int *size,TDAcola dato);
/* Dominio: una cola y el tamano de la cola
Recorrido: una cola
Descripcion: funcion que permite desencolar una cola*/
TDAcola *desencolar(TDAcola* cola, int *size);
/* Dominio: un nodo
Recorrido: nada
Descripcion: esta funcion permite conocer los datos individuales de un nodo, solo para uso del programador*/
void printearNodo(TDAcola entrada);
/* Dominio: un arreglo de enteros, una variable de tipo entero y el largo del arreglo
Recorrido: t / f
Descripcion: Funcio que permite saber si se repiten o no vertices*/
int sin_visitar(int *caminoRecorrido,int posicionActual,int largoCaminoRecorrido);
/* Dominio: un arreglo de enteros a, un arreglo de enteros b y el largo del arreglo a
Recorrido: t / f
Descripcion: permite comparar si dos rutas son exactamente iguales*/
int compararRutas(int* lista1, int* lista2,int largo);
/* Dominio: una cola, el largo de la cola, la variable a revisar
Recorrido: t o f
Descripcion: permite reconocer si un estado ya se encuentra en una cola*/
int estaEstado(TDAcola *cola,int largo,TDAcola revisar);
/* Dominio: una cola y su tamano
Recorrido: nada
Descripcion: entrega todos los datos de todos los elementos de una cola*/
void recorrerCola(TDAcola *cola,int size);
/* Dominio: una cola, el largo de la cola, una variable de tipo insumos,un entero
Recorrido: nodo
Descripcion: permite generar el archivo salida.out*/
void encontrarMinimo(TDAcola *cola,int size,insumos listaInsumos,int vertices);

int main(){
	clock_t start = clock();
	TDAgrafo *grafo = leerGrafo("conexiones.in");
	insumos listaInsumos;
	listaInsumos = leerInsumos("insumos.in");
	aux = 0;
	int cantAbiertos = 0;
	int cantCerrados = 0;
	int *temporal = (int*)malloc(sizeof (int)*1);
	temporal[0] = 0;
	TDAcola *abiertos = (TDAcola*)malloc(sizeof(TDAcola)*cantAbiertos);
	TDAcola *cerrados = (TDAcola*)malloc(sizeof(TDAcola)*cantCerrados);
	TDAcola inicial,actual,siguiente;
	inicial = crearNodo(0,listaInsumos.capacidadTotal,0,1,0);
	inicial.caminoRecorrido = temporal;
	abiertos = encolar(abiertos,&cantAbiertos,inicial);
	while (cantAbiertos>0){
		actual = abiertos[0];
		abiertos = desencolar(abiertos,&cantAbiertos);
		cerrados = encolar(cerrados,&cantCerrados,actual);
		int i = 0;
		while (i != grafo->cvertices){
			if(grafo->adyacencias[actual.posicion][i]!= 0){
				if (sin_visitar(actual.caminoRecorrido,i,actual.largoCaminoRecorrido) == 1){
					int *nuevaRuta =  append(actual.caminoRecorrido,actual.largoCaminoRecorrido,i);
					int peso;
					peso = listaInsumos.matriz[i-1][1];
					int distancia,recorrido,id;
					distancia = grafo->adyacencias[actual.posicion][i];
					recorrido = (distancia*actual.peso)/listaInsumos.subsidio;
					siguiente = crearNodo((actual.costo+recorrido),(actual.peso-peso),i,(actual.largoCaminoRecorrido+1),actual.id);
					siguiente.caminoRecorrido = nuevaRuta;
					if((estaEstado(abiertos,cantAbiertos,siguiente) == 0 ) || (estaEstado(cerrados,cantCerrados,siguiente) == 0)){
						abiertos = encolar(abiertos,&cantAbiertos,siguiente);
					}else{
						aux = aux - 1;
					}
				}	
			}
			i++;
		}
	}
	encontrarMinimo(cerrados,cantCerrados,listaInsumos,grafo->cvertices);
	clock_t end = clock();
    double tTranscurrido = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Tiempo total de ejecucion: %f seconds.\n",tTranscurrido);

} 
insumos leerInsumos(char *nombreArchivo){
	FILE *fp = fopen(nombreArchivo,"r");
	int cantFilas;
	int subsidio;
	insumos retorno;
	retorno.capacidadTotal = 0;
	fscanf(fp,"%d %d",&cantFilas,&subsidio);
	int **matrizAux = (int**)malloc(sizeof(int*)*cantFilas);  
	for (int i = 0; i < cantFilas; i++){
		matrizAux[i] = (int*)malloc(sizeof(int)*2);
		fscanf(fp,"%d %d",&matrizAux[i][0],&matrizAux[i][1]);
		retorno.capacidadTotal = retorno.capacidadTotal + matrizAux[i][1];
	}
	retorno.matriz = matrizAux;
	retorno.subsidio = subsidio;
	fclose(fp);
	return retorno;
}
TDAcola crearNodo(int costo,int peso,int posicion,int largoCaminoRecorrido,int idAnterior){
	TDAcola nuevo;
	nuevo.caminoRecorrido = (int*)malloc(sizeof(int)*largoCaminoRecorrido);
	nuevo.largoCaminoRecorrido = largoCaminoRecorrido;
	nuevo.idAnterior = idAnterior;
	nuevo.id = aux;
	nuevo.peso = peso;
	nuevo.costo = costo;
	nuevo.posicion = posicion;
	aux = aux + 1;
	return nuevo;

}
TDAcola *encolar(TDAcola* cola,int *size,TDAcola dato){
	TDAcola * nuevaCola = (TDAcola*)malloc(sizeof(TDAcola)*(*size+1));
	for (int i = 0; i < *size; ++i){
		nuevaCola[i] = cola[i];
	}
	nuevaCola[*size] = dato;
	*size = *size+1;
	free(cola);
	return nuevaCola;
	
}
TDAcola *desencolar(TDAcola* cola, int *size){
	TDAcola * nuevaCola = (TDAcola*)malloc(sizeof(TDAcola)*(*size-1));
	for (int i = 1; i < *size; ++i){
		nuevaCola[i-1] = cola[i];
	}
	*size = *size - 1;
	free(cola);
	return nuevaCola;
}
int *append(int *lista,int size,int dato){
	int *listaNueva = (int*)malloc(sizeof(int)*(size+1));
	for (int i = 0; i < size; i++){
		listaNueva[i] = lista[i];
	}
	listaNueva[size] = dato;
	return listaNueva;
}

int sin_visitar(int *caminoRecorrido,int posicionActual,int largoCaminoRecorrido){
	for (int i = 0; i < largoCaminoRecorrido; i++){
		if(caminoRecorrido[i] == posicionActual){
			return 0;
		}
	}
	return 1;
}
int compararRutas(int* lista1, int* lista2,int largo){
	for (int i = 0; i < largo; i++){
		if (lista1[i] != lista2[i]){
			return 0;
		}
	}
	return 1;
}
void printearNodo(TDAcola entrada){
	printf("ID: %d\n",entrada.id);
	printf("ID ANTERIOR: %d\n",entrada.idAnterior);
	printf("Posicion Actual: %d\n",entrada.posicion);
	printf("Costo actual: %d\n",entrada.costo);
	printf("Peso actual: %d\n",entrada.peso);
	printf("LARGO CAMINO RECORRIDO: %d\n",entrada.largoCaminoRecorrido);
	printf("Ruta Recorrida: \n");
	for (int i = 0; i < entrada.largoCaminoRecorrido; i++){
		printf("%d ",entrada.caminoRecorrido[i]);
	}
	printf("\n/-----------------------/\n\n");
}
int estaEstado(TDAcola *cola, int size, TDAcola revisar){
	for (int i = 1; i < size; i++){
		if(compararRutas(cola[i].caminoRecorrido,revisar.caminoRecorrido,size)==1){
			return 1;
		}
	}
	return 0;
}
void recorrerCola(TDAcola *cola,int size){
	for (int i = 0; i < size; i++){
		printearNodo(cola[i]);
	}
}
void encontrarMinimo(TDAcola *cola,int size,insumos listaInsumos,int vertices){
	FILE *fp = fopen("salida.out","w");
	TDAcola estadoMinimo;
	int cantSoluciones = 0;
	TDAcola *soluciones = (TDAcola*)malloc(sizeof(TDAcola)*cantSoluciones);
	for (int i = 0; i < size; i++){
		if(cola[i].largoCaminoRecorrido == vertices){
			soluciones = encolar(soluciones,&cantSoluciones,cola[i]);
		}
	}
	int minimo;
	minimo = soluciones[0].costo;
	for (int i = 0; i < cantSoluciones; i++){
		if (soluciones[i].costo<minimo){
			minimo = soluciones[i].costo;
			estadoMinimo = soluciones[i];
		}
	}
	fprintf(fp,"Capacidad: %dton\n",listaInsumos.capacidadTotal);
	fprintf(fp,"Subsidio: %dum\n",listaInsumos.subsidio);
	fprintf(fp,"Coste Minimo: %dum\n",estadoMinimo.costo);
	fprintf(fp,"Ruta:\nCentro");
	for (int i = 1; i < estadoMinimo.largoCaminoRecorrido; i++){
		fprintf(fp,"->%d",estadoMinimo.caminoRecorrido[i]);
	}
	fclose(fp);

}

