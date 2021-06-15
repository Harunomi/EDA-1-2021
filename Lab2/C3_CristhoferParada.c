#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "TDAlista.h"
#include "TDAVacuna.h"

//Entrada: una variable de tipo TDAlistaVacuna
//Salida: ninguna
//Descripcion: Funcion que modifica la lista dada con los datos que se encuentran en el archivo vacunas.in
void leerArchivoVacunas(TDAlistaVacuna *lista);

//Entrada: una variable tipo TDAlista y una variable tipo FILE
//Salida: ninguna
//Descripcion: esta funcion permite leer los archivos vacunados1D.in y vacunados2D.in y modificar la lista de entrada con dichos datos
//ademas lee los datos de tal forma que se guarden ordenados alfabeticamente por el apellido
void leerVacunados(TDAlista *lista, FILE *archivo);

//Entrada: dos variables tipo TDAlista y una variable tipo TDAlistaVacuna
//Salida: ninguna
//Descripcion: funcion que recibe la lista de vacunados2D y de vacunados1D y crea el archivo VacunacionCompleta.out
void listadoVacunacionCompleta(TDAlista *vacunados2D, TDAlista *vacunados1D,TDAlistaVacuna *listaVacunas);

//Entrada: una variable tipo nodo, una variable tipo TDAlistaVacuna y una variable tipo FILE
//Salida: ninguna
//Descripcion: Funcion que permite escribir en un archivo el nodo dado
void printNodoVCompleta(nodo* nodo, TDAlistaVacuna *listaVacunas,FILE *archivo);

//Entrada: una variable tipo nodo, una variable tipo TDAlistaVacuna y una variable tipo FILE
//Salida:ninguna
//Descripcion: funcion que permite escribir un nodo, con su segunda dosis, en un archivo 
void printNodoLOut(nodo* nodo, TDAlistaVacuna *listaVacunas,FILE *archivo);

//Entrada: dos archivos tipo TDAlista y un archivo tipo TDAlistaVacuna
//Salida: ninguna
//Descripcion: funcion que recive la lista vacunados1D y le calcula su segunda dosis, ademas la ordena cronologicamente primero por dia y despues por mes
void actualizarFechaSegundaDosis(TDAlista * listaEntrada, TDAlista *listaSalida, TDAlistaVacuna *vacunas);

//Entrada: recibe una variable tipo TDAlista y una variable tipo TDAlistaVacuna
//Salida: ninguna
//Descripcion: Funcion que calcula las dosis necesarias por mes de cada vacuna y las escribe en un archivo
void listadoProvision(TDAlista *lista ,TDAlistaVacuna *vacunas);

//Entrada: una variable tipo TDAlistaVacuna, 4 variables para el dia, mes, año y la ID de la vacuna
//Salida: una variable tipo char*
//Descripcion: dado el dia, mes y año, calcula la fecha correspondiente a la segunda dosis y la guarda en un char*
// la fecha teniendo formato DD/MM/YYYY
char *fechaSegundaDosis(TDAlistaVacuna *lista, int dia, int month, int anio, int id);


int main(){
    clock_t start = clock();
    TDAlistaVacuna *listaVacunas = crearListaVaciaVacunas();
    FILE *archivo1D = fopen("vacunados1D.in","r");
    FILE *archivo2D = fopen("vacunados2D.in","r");
    TDAlista *vacunados1D = crearListaVacia();
    TDAlista *vacunados2D = crearListaVacia();
    TDAlista *vacunados1DFecha2Dosis = crearListaVacia();
    leerArchivoVacunas(listaVacunas);
    leerVacunados(vacunados1D,archivo1D);
    leerVacunados(vacunados2D,archivo2D);
    fclose(archivo1D);
    fclose(archivo2D);
    /*---------------------------------------------------------*/ 
    actualizarFechaSegundaDosis(vacunados1D,vacunados1DFecha2Dosis,listaVacunas); // listado.out
    listadoProvision(vacunados1DFecha2Dosis,listaVacunas); // provision.out
    listadoVacunacionCompleta(vacunados2D,vacunados1D,listaVacunas); // vacunacionCompleta.out
    liberarLista(vacunados1D);
    liberarLista(vacunados2D);
    liberarLista(vacunados1DFecha2Dosis);
    liberarListaVacunas(listaVacunas);
    /*-------------------------------------------------------*/
    clock_t end = clock();
    double tTranscurrido = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Tiempo total de ejecucion: %f seconds.\n",tTranscurrido);
    return 0;
}

void leerArchivoVacunas(TDAlistaVacuna *lista){
    FILE *fp = fopen("vacunas.in","r");
    int cantidadVacunas;
    int id;
    int periodo;
    fscanf(fp,"%d",&cantidadVacunas);
    for (int i = 0; i < cantidadVacunas; i++){
        char *str1 = (char*)malloc(sizeof(char)*25);
        char *str2 = (char*)malloc(sizeof(char)*25);
        fscanf(fp,"%d %s %s %d",&id,str1,str2,&periodo);
        insertarnodoVacunaFinal(lista,id,str1,str2,periodo);
    }
    fclose(fp);
}

void leerVacunados(TDAlista *lista, FILE *archivo){
    int age,day,month,year,id,filas;
    char separacion[2] = "/";
    fscanf(archivo,"%d",&filas);
    for (int i = 0; i < filas; i++){
        char *rut = (char*)malloc(sizeof(char)*25);
        char *nombre = (char*)malloc(sizeof(char)*25);
        char *apellido = (char*)malloc(sizeof(char)*25);
        char *fecha = (char*)malloc(sizeof(char)*25);
        char *pedacito;
        fscanf(archivo,"%s %s %s %d %s %d",rut,nombre,apellido,&age,fecha,&id);
        pedacito = strtok(fecha,separacion);
        day = atoi(pedacito);
        pedacito = strtok(NULL, separacion);
        month = atoi(pedacito);
        pedacito = strtok(NULL, separacion);
        year = atoi(pedacito);
        insertAlfabeticamente(lista,rut,nombre,apellido,age,day,month,year,id);
    }
}

void listadoVacunacionCompleta(TDAlista *vacunados2D, TDAlista *vacunados1D,TDAlistaVacuna *listaVacunas){
    FILE *archivo = fopen("VacunacionCompleta.out","w");
    TDAlista *listaSalida = crearListaVacia();
    nodo* actual = (nodo*)malloc(sizeof(nodo));
    nodo* salida = (nodo*)malloc(sizeof(nodo));
    int totalPersonas = 0;
    actual = vacunados2D->inicio;
    while (actual != NULL){
        nodo* duplicado = (nodo*)malloc(sizeof(nodo));
        if (obtenerNodo(vacunados1D,actual->apellido)!=NULL){ // el apellido existe en la primera
            duplicado = obtenerNodo(vacunados1D,actual->apellido); // saco el nodo de los vacunados1D
            insertarNodoFinal(listaSalida,duplicado->rut,duplicado->nombre,duplicado->apellido,duplicado->age,duplicado->day,duplicado->month,duplicado->year,duplicado->id);
            insertarNodoFinal(listaSalida,actual->rut,actual->nombre,actual->apellido,actual->age,actual->day,actual->month,actual->year,actual->id);
            totalPersonas++;
        }else{
            insertarNodoFinal(listaSalida,actual->rut,actual->nombre,actual->apellido,actual->age,actual->day,actual->month,actual->year,actual->id);
            totalPersonas++;
        }
        actual = actual->siguiente;
    }
    salida = listaSalida->inicio;
    fprintf(archivo,"%d\n",totalPersonas);
    while (salida != NULL ){
        printNodoVCompleta(salida,listaVacunas,archivo);
        salida = salida->siguiente;
    }
    fclose(archivo);
}

void printNodoVCompleta(nodo* nodo, TDAlistaVacuna *listaVacunas,FILE *archivo){
    nodoVacuna* aux = listaVacunas->inicio;
    fprintf(archivo,"%s %s %s %d %d/%d/%d ",nodo->rut,nodo->nombre,nodo->apellido,nodo->age,nodo->day,nodo->month,nodo->year);
    for (int i = 0; i <=largoListaVacuna(listaVacunas) ; i++)    {
        if (nodo->id == aux->id){
            fprintf(archivo,"%s\n",aux->codigo);
            break;
        }
        aux = aux->siguiente;
    }
    
}

void printNodoLOut(nodo* nodo, TDAlistaVacuna *listaVacunas,FILE *archivo){
    nodoVacuna* aux = listaVacunas->inicio;
    fprintf(archivo,"%s %s %s %d %d/%d/%d %s ",nodo->rut,nodo->nombre,nodo->apellido,nodo->age,nodo->day,nodo->month,nodo->year,nodo->segundaDosis);
    for (int i = 0; i <=largoListaVacuna(listaVacunas) ; i++)    {
        if (nodo->id == aux->id){
            fprintf(archivo,"%s\n",aux->codigo);
            break;
        }
        aux = aux->siguiente;
    }
    
}

void actualizarFechaSegundaDosis(TDAlista * listaEntrada, TDAlista *listaSalida, TDAlistaVacuna *vacunas){
    FILE *archivo = fopen("listado.out","w");
    TDAlista *listaDia = crearListaVacia();
    nodo* actual = listaEntrada->inicio;
    char *pedacito;
    char separacion[2] = "/";
    char meses[12][12]={"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre", "Octubre","Noviembre","Diciembre"};
    int day;
    int month;
    int year;
    while (actual!=NULL){
        char *fecha = (char*)malloc(sizeof(char)*25);
        char *fechaCopia = (char*)malloc(sizeof(char)*25);
        char *fechaCopia2 = (char*)malloc(sizeof(char)*25);
        fecha = fechaSegundaDosis(vacunas,actual->day,actual->month,actual->year,actual->id); // tomo la fecha de la segunda dosis
        // transformo el string a entero y lo guardo en sus respectivas variables
        strcpy(fechaCopia,fecha);
        strcpy(fechaCopia2,fechaCopia);
        pedacito = strtok(fechaCopia,separacion);
        day = atoi(pedacito);
        pedacito = strtok(NULL, separacion);
        month = atoi(pedacito);
        pedacito = strtok(NULL, separacion);
        year = atoi(pedacito);
        // guardo la lista entrada en una lista vacia, ordenando a las personas por dia
        insertarCronologicamenteDia(listaDia,actual->rut,actual->nombre,actual->apellido,actual->age,actual->day,actual->month,actual->year,actual->id,day,month,year,fechaCopia2);
        actual = actual->siguiente;
    }
    /* creo una variable a que me servira para recorrer la listaDia y ordenar esa lista por mes */
    nodo* a = listaDia->inicio;
    while (a!=NULL){
        insertarCronologicamenteMes(listaSalida,a->rut,a->nombre,a->apellido,a->age,a->day,a->month,a->year,a->id,a->day2,a->month2,a->year2,a->segundaDosis);
        a = a->siguiente;
    }
    liberarLista(listaDia);
    nodo* out;
    int count;
    // printeo por mes
    for (int i = 1; i <= 12; i++){
        count = 0;
        out = listaSalida->inicio;
        // cuento cuantas personas hay en un mes i
        for (int j = 0; j <= largoLista(listaSalida); j++){
            if (out->month2 == i){
                count++;
            }
            out = out->siguiente;
        }
        out = listaSalida->inicio;
        // si hay al menos una persona en el mes i
        if (count > 0){
            // escribo el mes i y su cantidad de personas vacunadas en dicho dia
            fprintf(archivo,"%s %d\n",meses[i-1],count);
            for (int j = 0; j <= largoLista(listaSalida); j++){
                if (out->month2 == i){
                    // si el mes de la segunda dosis de la persona coincide con el mes actual, lo escribo en el archivo salida
                    printNodoLOut(out,vacunas,archivo);
                }
                out = out->siguiente;   
            }
        }         
    }
    fclose(archivo);
}

void listadoProvision(TDAlista *lista ,TDAlistaVacuna *vacunas){
    FILE *archivo = fopen("provision.out","w");
    nodoVacuna* vacuna;
    nodo* paciente;
    int count;
    char meses[12][12]={"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre", "Octubre","Noviembre","Diciembre"};
    for (int i = 1; i <= 12; i++){
        paciente = lista->inicio;
        vacuna = vacunas->inicio;
        count = 0;
        /* Cuento cuantas personas hay en un mes i */ 
        while (paciente!=NULL){
            if (paciente->month2 == i){
                vacuna = retornarNodoVacuna(vacunas,paciente->id);
                vacuna->dosis=vacuna->dosis+1;
                count++;
            }
            paciente = paciente->siguiente;
        }
        // si hay al menos una persona vacunada en dicho mes
        if (count > 0){
            fprintf(archivo,"%s %d\n",meses[i-1],count);
            vacuna = vacunas->inicio;
            // escribo el codigo de la vacuna y su stock necesario
            while (vacuna!=NULL){
                fprintf(archivo,"%s %d\n",vacuna->codigo,vacuna->dosis);
                vacuna->dosis = 0;
                vacuna = vacuna->siguiente;
            }
            fprintf(archivo,"\n");
        }
    }
    fclose(archivo);
}

char *fechaSegundaDosis(TDAlistaVacuna *lista, int dia, int mes, int anio, int id){
    nodoVacuna * vacuna = lista->inicio;
    int day = dia;
    int month = mes;
    int year = anio;
    char cero[2] = "0";
    char cero2[2] = "0";
    char fecha[20];
    char *retorno;
    for (int i = 0; i <= largoListaVacuna(lista); i++){
        if (id == vacuna->id){ // me detengo cuando encuentro la vacuna ingresada
            break;
        }
        vacuna = vacuna->siguiente;
    }
    int diasASumar = (vacuna->periodo)*7;
    for (int i = 0; i < diasASumar; i++){
        day = day + 1;
        if (month == 2){ // sumo un mes en el caso que el mes sea febrero
            if(day>28){ // caso que me salga del limite de febrero
                day = 1; // reinicio el dia
                month = month + 1; // sumo un mes
            }
        }else if (month == 4 || month == 6 || month == 9 || month == 11){ // sumo un mes en caso que el month actual sea de 30 dias
            if (day>30){ // caso que me salga de los limites del mes actual
                day = 1; // reinicio el dia
                month = month + 1; // sumo un mes
            }
        }else if ( month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){ // sumo un mes en caso que el month actual sea de 31 dias
            if (day>31) { // caso que me salga de los limites del mes adctual
                day = 1; // reinicio el dia
                month = month + 1; // sumo un mes
            }     
        }else if (month>12){ // sumo el a;o
            month = 1; // reinicio el mes
            day = 1; // reinicio el dia
            year = year + 1;
        }
    }   
    if (day<10 && month<10){
        sprintf(fecha,"%s%d/%s%d/%d",cero,day,cero2,month,year);
    }else if (day<10){
        sprintf(fecha,"%s%d/%d/%d",cero,day,month,year);
    }else if (month<10){
        sprintf(fecha,"%d/%s%d/%d",day,cero,month,year);
    }else{
        sprintf(fecha,"%d/%d/%d",day,month,year);
    }
    retorno = fecha;
    return retorno;    
}


