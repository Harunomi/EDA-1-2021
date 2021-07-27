#include <stdio.h>
#include <stdlib.h>

/*------------- estructura de datos -------------*/

typedef struct nodoVacunaGenerico{
  int id;
  char *codigo;
  char *nombre;
  int periodo;
  int dosis;
  struct nodoVacunaGenerico* siguiente;
}nodoVacuna;

typedef struct listaGenericaV{
  nodoVacuna* inicio;
}TDAlistaVacuna;

/*------------- operaciones -------------*/

TDAlistaVacuna* crearListaVaciaVacunas(){
  TDAlistaVacuna* lista=(TDAlistaVacuna*)malloc(sizeof(TDAlistaVacuna));
  lista->inicio=NULL;
  return lista;
}
int esListaVaciaVacunas(TDAlistaVacuna* lista){
  if (lista->inicio == NULL)
    return 1;
  else
    return 0;
}

void recorrerListaVacunas(TDAlistaVacuna* lista){
  if (!esListaVaciaVacunas(lista)){
    nodoVacuna* auxiliar=lista->inicio;
    while (auxiliar!=NULL){
      printf("%d %s %s %d\n",auxiliar->id,auxiliar->codigo,auxiliar->nombre,auxiliar->periodo);
      auxiliar=auxiliar->siguiente;
    }
    printf("\n");
  }else{
    printf("Kono risuto wa soradesu~ (;A;)-\n");
  }
}

void insertarInicioVacunas(TDAlistaVacuna* lista,int id, char codigo[20],char nombre[20],int periodo){
  nodoVacuna* nuevo=(nodoVacuna*)malloc(sizeof(nodoVacuna));
  nuevo->id=id;
  nuevo->codigo = codigo;
  nuevo->nombre = nombre;
  nuevo->periodo = periodo;
  nuevo->siguiente = lista->inicio;
  lista->inicio=nuevo;
}

void liberarListaVacunas(TDAlistaVacuna* lista){
  if(!esListaVaciaVacunas(lista)){
    nodoVacuna* auxiliar;
    auxiliar = lista->inicio;
    while(auxiliar!=NULL){
      lista->inicio = lista->inicio->siguiente;
      free(auxiliar);
      auxiliar = lista->inicio;
    }
  }
}

void insertarnodoVacunaFinal(TDAlistaVacuna* lista,int id, char codigo[20],char nombre[20],int periodo){
  if(!esListaVaciaVacunas(lista)){
    nodoVacuna* nuevo=(nodoVacuna*)malloc(sizeof(nodoVacuna));
    nuevo->id=id;
    nuevo->codigo = codigo;
    nuevo->nombre = nombre;
    nuevo->periodo = periodo;
    nuevo->dosis = 0;
    nuevo->siguiente=NULL;
    nodoVacuna* actual=lista->inicio;
    while(actual->siguiente != NULL ){
        actual=actual->siguiente;
    }
    actual->siguiente=nuevo;
  }else{
    insertarInicioVacunas(lista,id,codigo,nombre,periodo);
  }
}
int largoListaVacuna(TDAlistaVacuna* lista){     
  int contador=0;
  nodoVacuna* actual=lista->inicio;
  if(esListaVaciaVacunas(lista)){
    return contador;
  }else if(actual->siguiente == NULL){
    return 1;
  }else{
    while(actual->siguiente != NULL ){
        actual=actual->siguiente;
        contador++;
    }
    return contador;
  }
}

nodoVacuna *retornarNodoVacuna(TDAlistaVacuna *lista, int id){
  nodoVacuna* nodo = lista->inicio;
  while (nodo!=NULL){
    if (nodo->id == id){
      return nodo;
    }
    nodo = nodo->siguiente;
  }
}

