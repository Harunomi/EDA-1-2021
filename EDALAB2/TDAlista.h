#include <stdio.h>
#include <stdlib.h>

/*------------- estructura de datos -------------*/

typedef struct nodoGenerico{
  char *rut;
  char *nombre;
  char *apellido;
  char *segundaDosis; // dd/mm/yyyy
  int age;
  int day;
  int month;
  int year;
  int id;
  int day2; 
  int month2;
  int year2;
  struct nodoGenerico* siguiente;
}nodo;

typedef struct listaGenerica{
  nodo* inicio;
}TDAlista;

/*------------- operaciones -------------*/

TDAlista* crearListaVacia()
{
  TDAlista* lista=(TDAlista*)malloc(sizeof(TDAlista));
  lista->inicio=NULL;
  return lista;
}

int esListaVacia(TDAlista* lista)
{
  if (lista->inicio == NULL)
    return 1;
  else
    return 0;
}

void recorrerLista(TDAlista* lista){
  if (!esListaVacia(lista)){
    nodo* auxiliar=lista->inicio;
    while (auxiliar!=NULL){
      printf("%s %s %s %d %d/%d/%d %d\n",auxiliar->rut,auxiliar->nombre,auxiliar->apellido,auxiliar->age,auxiliar->day,auxiliar->month,auxiliar->year,auxiliar->id);
      auxiliar=auxiliar->siguiente;
    }
    printf("\n");
  }else{
    printf("Kono risuto wa soradesu~ (;A;)-\n");
  }
}
void recorrerLista2D(TDAlista* lista){
  if (!esListaVacia(lista)){
    nodo* auxiliar=lista->inicio;
    while (auxiliar!=NULL){
      printf("%s %s %s %d %d/%d/%d %d %s\n",auxiliar->rut,auxiliar->nombre,auxiliar->apellido,auxiliar->age,auxiliar->day,auxiliar->month,auxiliar->year,auxiliar->id,auxiliar->segundaDosis);
      auxiliar=auxiliar->siguiente;
    }
    printf("\n");
  }else{
    printf("Kono risuto wa soradesu~ (;A;)-\n");
  }
}


void insertarInicio(TDAlista* lista, char rut[10],char nombre[25],char apellido[25],int age, int day,int month,int year,int id ){
  nodo* nuevo=(nodo*)malloc(sizeof(nodo));
  nuevo->rut = rut;
  nuevo->nombre = nombre;   
  nuevo->apellido =apellido;
  nuevo->age = age;
  nuevo->day = day;
  nuevo->month = month;
  nuevo->year = year ;
  nuevo->id = id;
  nuevo->siguiente = lista->inicio;
  lista->inicio=nuevo;
}

void eliminarInicio(TDAlista* lista){
  nodo* auxiliar;
  if(!esListaVacia(lista)){
    auxiliar=lista->inicio;
    lista->inicio=lista->inicio->siguiente;
    free(auxiliar);
  }
}

int buscarDato(TDAlista* lista, int dato);


void liberarLista(TDAlista* lista){
  if(!esListaVacia(lista)){
    nodo* auxiliar;
    auxiliar = lista->inicio;
    while(auxiliar!=NULL){
      lista->inicio = lista->inicio->siguiente;
      free(auxiliar);
      auxiliar = lista->inicio;
    }
  }
}
int obtenerNumeroNodos(TDAlista* lista){
  int cantidad = 0;
  if(!esListaVacia(lista)){
    nodo* auxiliar;
    auxiliar = lista->inicio;
    while (auxiliar != NULL){
      cantidad++;
      auxiliar = auxiliar->siguiente;
    }
  }
}

void insertarNodoFinal(TDAlista* lista, char rut[10], char nombre[25], char apellido[25], int age, int day, int month, int year, int id){
  if(!esListaVacia(lista)){
    nodo* nuevo=(nodo*)malloc(sizeof(nodo));
    nuevo->rut = rut;
    nuevo->nombre = nombre;   
    nuevo->apellido =apellido;
    nuevo->age = age;
    nuevo->day = day;
    nuevo->month = month;
    nuevo->year = year ;
    nuevo->id = id;
    nuevo->siguiente=NULL;
    nodo* actual=lista->inicio;
    while(actual->siguiente != NULL ){
        actual=actual->siguiente;
    }
    actual->siguiente=nuevo;
  }else{
    insertarInicio(lista,rut,nombre,apellido,age,day,month,year,id);
  }
}

void insertarNodoDespues(TDAlista* lista, char rut[10], char nombre[25], char apellido[25], int age, int day, int month, int year, int id, char apellidoAnterior[25]){
  nodo* anterior = lista->inicio;
  while (strcmp(anterior->apellido,apellidoAnterior)!=0){
    anterior = anterior->siguiente;
  }
  nodo* nuevo=(nodo*)malloc(sizeof(nodo));
  nuevo->rut = rut;
  nuevo->nombre = nombre;   
  nuevo->apellido = apellido;
  nuevo->age = age;
  nuevo->day = day;
  nuevo->month = month;
  nuevo->year = year ;
  nuevo->id = id;
  nuevo->siguiente = anterior->siguiente;
  anterior->siguiente = nuevo;
}  

void eliminarFinal(TDAlista* lista){
  if(esListaVacia(lista)==1){//si esta vacia no hago nada

  }else{
    nodo* actual=lista->inicio;
    nodo* anterior;
    while(actual->siguiente != NULL ){
        anterior=actual;
        actual=actual->siguiente;
    }
    anterior->siguiente=NULL;
    free(actual);
  }
}
int largoLista(TDAlista* lista){     
  int contador=0;
  nodo* actual=lista->inicio;
  if(esListaVacia(lista)){
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
void eliminarDato(TDAlista* lista, char apellido[25]){
  nodo* indicado = lista->inicio;
  nodo* anteriorIndicado = NULL;
  int contador = 0;
  while (strcmp(indicado->apellido,apellido) != 0){
    anteriorIndicado = indicado;
    indicado = indicado->siguiente;
    contador++;
  }
  if (contador == 0){
    eliminarInicio(lista);
  }else{
    anteriorIndicado->siguiente = indicado->siguiente;
    free(indicado);
  }
}
void insertAlfabeticamente (TDAlista *lista, char rut[10],char nombre[25],char apellido[25],int age, int day,int month,int year,int id){
  nodo* nuevo = (nodo*)malloc(sizeof(nodo));
  nodo* anterior = (nodo*)malloc(sizeof(nodo));
  nodo* actual = (nodo*)malloc(sizeof(nodo));
  int cmp;
  anterior = NULL;
  actual = lista->inicio;
  // recorro la lista hasta que actual->apellido sea alfabeticamente mayor que el apellido ingresado
  while ( actual != NULL ) {
    cmp = strcmp(apellido, actual->apellido); // busca el caso en que actual->apellido sea alfabeticamente mayor sea mayor al apellido ingresado
    if (cmp < 0) {
        break; // salgo del ciclo while, puesto que encontre la condicion que quiero 
    } else if (cmp==0) { // caso en que el apellido insertado sea igual al actual->apellido, lo inserto despues de actual->apellido
        insertarNodoDespues(lista,rut,nombre,apellido,age,day,month,year,id,actual->apellido);
        return;
    }
    anterior = actual; // guardo el actual
    actual = actual->siguiente; // muevo el cabezal
  }
  // creo el nuevo nodo a agregar
  nuevo->rut = rut;
  nuevo->nombre = nombre;   
  nuevo->apellido =apellido;
  nuevo->age = age;
  nuevo->day = day;
  nuevo->month = month;
  nuevo->year = year ;
  nuevo->id = id;
  if ( anterior == NULL ) { // caso en que sea una lista vacia, lo agrega al inicio
      nuevo->siguiente = lista->inicio;
      lista->inicio = nuevo;
  } else { 
      anterior->siguiente = nuevo; // agrego el nodo nuevo antes que el nodo actual
      nuevo->siguiente = actual; // agrego el resto de la lista al nodo nuevo agregado
  }
}
void insertarNodoDespues2(TDAlista* lista, char rut[10], char nombre[25], char apellido[25], int age, int day, int month, int year, int id,int day2,int month2,int year2, char segundaDosisAnt[20]){
  nodo* anterior = lista->inicio;
  while (strcmp(anterior->apellido,segundaDosisAnt)!=0){
    anterior = anterior->siguiente;
  }
  nodo* nuevo=(nodo*)malloc(sizeof(nodo));
  nuevo->rut = rut;
  nuevo->nombre = nombre;   
  nuevo->apellido = apellido;
  nuevo->age = age;
  nuevo->day = day;
  nuevo->month = month;
  nuevo->year = year ;
  nuevo->id = id;
  nuevo->day2 = day2;
  nuevo->month2 = month2;
  nuevo->year2 = year2;
  nuevo->segundaDosis = segundaDosisAnt;
  nuevo->siguiente = anterior->siguiente;
  anterior->siguiente = nuevo;
}

void insertarCronologicamenteDia(TDAlista *lista, char rut[10],char nombre[25],char apellido[25],int age, int day,int month,int year,int id,int day2,int month2,int year2,char segundaDosis[20]){
  nodo* nuevo = (nodo*)malloc(sizeof(nodo));
  nodo* anterior = (nodo*)malloc(sizeof(nodo));
  nodo* actual = (nodo*)malloc(sizeof(nodo));
  anterior = NULL;
  actual = lista->inicio;
  while(actual!=NULL){
    if (day2 < actual->day2){
      break;
    }
    anterior = actual; // guardo el actual
    actual = actual->siguiente; // muevo el cabezal  
  }
  nuevo->rut = rut;
  nuevo->nombre = nombre;   
  nuevo->apellido =apellido;
  nuevo->age = age;
  nuevo->day = day;
  nuevo->month = month;
  nuevo->year = year ;
  nuevo->id = id;
  nuevo->day2 = day2;
  nuevo->month2 = month2;
  nuevo->year2 = year2;
  nuevo->segundaDosis = segundaDosis;
  if (anterior == NULL ) { // caso en que sea una lista vacia, lo agrega al inicio
    nuevo->siguiente = lista->inicio;
    lista->inicio = nuevo;
  }else{ 
    anterior->siguiente = nuevo; // agrego el nodo nuevo antes que el nodo actual
    nuevo->siguiente = actual; // agrego el resto de la lista al nodo nuevo agregado
  }
} 


void insertarCronologicamenteMes(TDAlista *lista, char rut[10],char nombre[25],char apellido[25],int age, int day,int month,int year,int id,int day2,int month2,int year2,char segundaDosis[20]){
  nodo* nuevo = (nodo*)malloc(sizeof(nodo));
  nodo* anterior = (nodo*)malloc(sizeof(nodo));
  nodo* actual = (nodo*)malloc(sizeof(nodo));
  anterior = NULL;
  actual = lista->inicio;
  while(actual!=NULL){
    if (month2 < actual->month2){
      break;
    }
    anterior = actual; // guardo el actual
    actual = actual->siguiente; // muevo el cabezal  
    }
  nuevo->rut = rut;
  nuevo->nombre = nombre;   
  nuevo->apellido =apellido;
  nuevo->age = age;
  nuevo->day = day;
  nuevo->month = month;
  nuevo->year = year ;
  nuevo->id = id;
  nuevo->day2 = day2;
  nuevo->month2 = month2;
  nuevo->year2 = year2;
  nuevo->segundaDosis = segundaDosis;
  if ( anterior == NULL ) { // caso en que sea una lista vacia, lo agrega al inicio
    nuevo->siguiente = lista->inicio;
    lista->inicio = nuevo;
  }else { 
    anterior->siguiente = nuevo; // agrego el nodo nuevo antes que el nodo actual
    nuevo->siguiente = actual; // agrego el resto de la lista al nodo nuevo agregado
  }
} 

nodo* obtenerNodo(TDAlista* lista, char apellido[25]){
  nodo* retorno;
  retorno = lista->inicio;
  while(retorno!=NULL){
    if (strcmp(retorno->apellido,apellido) == 0){
      break;
    }
    retorno = retorno->siguiente;
  }
  if (retorno == NULL){
    return NULL;
  }else{
    return retorno;
  }
}
