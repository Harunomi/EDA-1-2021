#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* Variables Globales */
char box1Char[2] = "B1";
char box2Char[2] = "B2";
char box3Char[2] = "B3";
char box4Char[2] = "B4";
char sanitizarChar[13] = "SANITIZACION";

typedef struct Medico{
    char nombre[5];
    char hora[500];
}Medico;

typedef struct Box{
    int estado; // 0 si esta vacunando, 1 si esta en sanitizacion, 2 si esta vacia
    int medicosActual; // variable de medicos por cada box
    int minutosParaVacunar; // copia inmutable de los minutos
    int minutosParaVacunarResta; // variable de los minutos para copiar
    int minutosSalirSanitizacion; // variable que indica cuantos minutos falta para la sanitizacion
    int vacunadosActual; // variable local de vacunados para saber cuando sanitizar
    char hora[500]; // variable donde anoto las modificaciones uqe ocurren
    Medico *listaMedicos;
}Box;

int maximaEficienciaBox1(int n);

int maximaEficienciaBox3(int n);

int maximaEficienciaBox4(int n);

void cambiarMinutos(Box *entrada);

char* pasarMinutosAHora(int minutos);


int main(int argc,char *argv[]){
    clock_t start = clock();
    FILE *entrada = fopen(argv[1],"r"); 
    int numeroEntrada = atoi(argv[1]);
    if (numeroEntrada <= 0 || numeroEntrada > 20){
        printf("****Entrada incorrecta****\n");
        exit(1);
    }
    fclose(entrada);
    int resultado;
    int vacunadosTotal = 0;
    int vacunadosMayor = 0;
    int tiempoInicial = 540;
    int tiempoTotal = 780;
    int aux = 1;
    char *string1;
    char *string2;
    Box * box = (Box*)malloc(sizeof(Box)*4);
    FILE *fp = fopen("resultados.csv","w");
    FILE *salida1 = fopen("profesionales.out","w");
    FILE *salida2 = fopen("box.out","w");
    FILE *salida3 = fopen("resumen_atenciones.out","w");
    fprintf(fp,"Box1,Box2,Box3,Box4,Vacunados Totales\n");
    for (int q = 0; q < 4; q++){
        for (int w = 0; w < 4; w++){
            for (int e = 0; e < 7; e++){
                for (int r = 0; r < 9 ; r++){
                    if ((q + w + e + r ) == numeroEntrada){
                        resultado = maximaEficienciaBox1(q)+maximaEficienciaBox1(w)+maximaEficienciaBox3(e)+maximaEficienciaBox4(r);
                        if(resultado>vacunadosMayor){ 
                            vacunadosMayor = resultado; 
                            box[0].medicosActual = q;
                            box[1].medicosActual = w;
                            box[2].medicosActual = e;
                            box[3].medicosActual = r;   
                        }
                        fprintf(fp,"%d,%d,%d,%d,%d\n",q,w,e,r,resultado);
                    }
                }  
            } 
        }
    }
    fclose(fp);
    fprintf(salida3,"%d %d",numeroEntrada,vacunadosMayor);
  
  /* Distribuyendo los nombres de los medicos dentro de cada BOX */
    for (int i = 0; i < 4; i++){
        fprintf(salida2,"BOX %d ",i);
        for (int j = 0; j < box[i].medicosActual; j++){
            if (box[i].medicosActual == 0 ){
                break;
            }else{
                char charNumero[5];
                char persona[5] = "P";
                box[i].listaMedicos = (Medico*)malloc(sizeof(int)*box[i].medicosActual);
                strcpy(box[i].listaMedicos[j].nombre,persona);
                sprintf(charNumero,"%d",aux);
                strcat(box[i].listaMedicos[j].nombre,charNumero);
                fprintf(salida2,"%s ",box[i].listaMedicos[j].nombre);
                aux++;
            }
        }
        string1 = pasarMinutosAHora(540);
        string2 = pasarMinutosAHora(780);
        fprintf(salida2,"%s %s",string1,string2);
        fprintf(salida2,"\n");  
    }
    while (tiempoInicial < tiempoTotal){
        //printf("                    %d\n",tiempoInicial);
        /* BOX 1 NO SANITIZANDO */
        if (box[0].estado == 0){ // no esta sanitizando
            if (box[0].minutosParaVacunarResta == 0){ // momento de vacunar
                vacunadosTotal = vacunadosTotal + 1; // aumento el vacunados global
                box[0].vacunadosActual = box[0].vacunadosActual + 1;  // aumento el vacunados del box
                box[0].minutosParaVacunarResta = box[0].minutosParaVacunar; // reinicio el contador para vacunar
                if ((box[0].vacunadosActual % 12) == 0){ // si han vacunado 12 personas, pasamos a sanitizar
                    box[0].estado = 1; // ENTRA A ESTADO DE SANITIZACION
                    box[0].minutosSalirSanitizacion = 4;
                }
            }else{
                box[0].minutosParaVacunarResta = box[0].minutosParaVacunarResta  - 1;
            }
        }

        /* BOX 2 NO SANITIZANDO */
        if (box[1].estado == 0){ // no esta sanitizando
            if (box[1].minutosParaVacunarResta == 0){ // momento de vacunar
                vacunadosTotal = vacunadosTotal + 1; // aumento el vacunados global
                box[1].vacunadosActual = box[1].vacunadosActual + 1;  // aumento el vacunados del box
                box[1].minutosParaVacunarResta = box[1].minutosParaVacunar; // reinicio el contador para vacunar
                if ((box[1].vacunadosActual % 12) == 0){ // si han vacunado 12 personas, pasamos a sanitizar
                    box[1].estado = 1; // ENTRA A ESTADO DE SANITIZACION
                    box[1].minutosSalirSanitizacion = 4;
                }
            }else{
                box[1].minutosParaVacunarResta = box[1].minutosParaVacunarResta  - 1;
            }
        }
        
        /* BOX 3 NO SANITIZANDO */
        if (box[2].estado == 0){ // no esta sanitizando
            if (box[2].minutosParaVacunarResta == 0){ // momento de vacunar
                vacunadosTotal = vacunadosTotal + 1; // aumento el vacunados global
                box[2].vacunadosActual = box[2].vacunadosActual + 1;  // aumento el vacunados del box
                box[2].minutosParaVacunarResta = box[2].minutosParaVacunar; // reinicio el contador para vacunar
                if ((box[2].vacunadosActual % 21) == 0){ // si han vacunado 12 personas, pasamos a sanitizar
                    box[2].estado = 1; // ENTRA A ESTADO DE SANITIZACION
                    box[2].minutosSalirSanitizacion = 6;
                }
            }else{
                box[2].minutosParaVacunarResta = box[2].minutosParaVacunarResta  - 1;
            }
        }

        /* BOX 4 NO SANITIZADO */
          if (box[3].estado == 0){ // no esta sanitizando
            if (box[3].minutosParaVacunarResta == 0){ // momento de vacunar

                vacunadosTotal = vacunadosTotal + 1; // aumento el vacunados global
                box[3].vacunadosActual = box[3].vacunadosActual + 1;  // aumento el vacunados del box
                box[3].minutosParaVacunarResta = box[3].minutosParaVacunar; // reinicio el contador para vacunar
                if ((box[3].vacunadosActual % 27) == 0){ // si han vacunado 12 personas, pasamos a sanitizar
                    box[3].estado = 1; // ENTRA A ESTADO DE SANITIZACION
                    box[3].minutosSalirSanitizacion = 8;
                    /* Escribir en el archivo */
                }
            }else{
                box[3].minutosParaVacunarResta = box[3].minutosParaVacunarResta  - 1;
            }
        }
                                  /* PARA LOS BOX EN FASE DE SANITIZACION */
        /* BOX 1 */ 
        if ((box[0].estado == 1) && (box[0].medicosActual > 0)){
            /* Buscar donde meter a los medicos */
                /* Reviso si el box 4 puede recibir medicos */ 
            if ((box[3].estado == 0) && (box[3].medicosActual < 3)){ // candidato a mover medicos
                while ((box[0].medicosActual + box[3].medicosActual) < 3 && (box[0].medicosActual > 0)){
                    box[3].medicosActual = box[3].medicosActual + 1;
                    box[0].medicosActual = box[0].medicosActual - 1;
                } 
            } /* reviso si el box 3 puede recibir medicos */ 
            if ((box[2].estado == 0) && (box[2].medicosActual < 3)){ // candidato a mover medicos
                while ((box[0].medicosActual + box[2].medicosActual) < 3 && (box[0].medicosActual > 0)){
                    box[2].medicosActual = box[2].medicosActual + 1;
                    box[0].medicosActual = box[0].medicosActual - 1;
                }
            } /*  Reviso si el box 2 puede recibir medicos */
            if ((box[1].estado == 0) && (box[1].medicosActual < 3)){ // candidato a mover medicos
                while ((box[0].medicosActual + box[1].medicosActual) < 3 && (box[0].medicosActual > 0)){
                    box[1].medicosActual = box[1].medicosActual + 1;
                    box[0].medicosActual = box[0].medicosActual - 1;
                }
            }
            cambiarMinutos(box);
            box[0].minutosSalirSanitizacion = box[0].minutosSalirSanitizacion - 1;
        }
        /* BOX 2 */
        if ((box[1].estado == 1) && (box[1].medicosActual > 0)){
            /* Buscar donde meter a los medicos */
                /* Reviso si el box 4 puede recibir medicos */ 
            if ((box[3].estado == 0) && (box[3].medicosActual < 3)){ // candidato a mover medicos
                while ((box[1].medicosActual + box[3].medicosActual) < 3 && (box[1].medicosActual > 0)){
                    box[3].medicosActual = box[3].medicosActual + 1;
                    box[1].medicosActual = box[1].medicosActual - 1;
                } 
            } /* reviso si el box 3 puede recibir medicos */ 
            if ((box[2].estado == 0) && (box[2].medicosActual < 3)){ // candidato a mover medicos
                while ((box[1].medicosActual + box[2].medicosActual) < 3 && (box[1].medicosActual > 0)){
                    box[2].medicosActual = box[2].medicosActual + 1;
                    box[1].medicosActual = box[1].medicosActual - 1;

                }
            } /*  Reviso si el box 1 puede recibir medicos */
            if ((box[0].estado == 0) && (box[0].medicosActual < 3)){ // candidato a mover medicos
                while ((box[1].medicosActual + box[0].medicosActual) < 3 && (box[1].medicosActual > 0)){
                    box[0].medicosActual = box[0].medicosActual + 1;
                    box[1].medicosActual = box[1].medicosActual - 1;
                }
            }
            cambiarMinutos(box);
            box[1].minutosSalirSanitizacion = box[1].minutosSalirSanitizacion - 1;
        }
        /* BOX 3 */
        if ((box[2].estado == 1) && (box[2].medicosActual > 0)){
            /* Buscar donde meter a los medicos */
                /* Reviso si el box 4 puede recibir medicos */ 
            if ((box[3].estado == 0) && (box[3].medicosActual < 3)){ // candidato a mover medicos
                while ((box[2].medicosActual + box[3].medicosActual) < 3 && (box[2].medicosActual > 0)){
                    box[3].medicosActual = box[3].medicosActual + 1;
                    box[2].medicosActual = box[2].medicosActual - 1;
                } 
            } /* reviso si el box 1 puede recibir medicos */ 
            if ((box[0].estado == 0) && (box[0].medicosActual < 3)){ // candidato a mover medicos
                while ((box[2].medicosActual + box[0].medicosActual) < 3 && (box[2].medicosActual > 0)){
                    box[0].medicosActual = box[0].medicosActual + 1;
                    box[2].medicosActual = box[2].medicosActual - 1;
                }
            } /*  Reviso si el box 2 puede recibir medicos */
            if ((box[1].estado == 0) && (box[2].medicosActual < 3)){ // candidato a mover medicos
                while ((box[0].medicosActual + box[2].medicosActual) < 3 && (box[2].medicosActual > 0)){
                    box[1].medicosActual = box[1].medicosActual + 1;
                    box[2].medicosActual = box[2].medicosActual - 1;
                }
            }
            cambiarMinutos(box);
            box[2].minutosSalirSanitizacion = box[2].minutosSalirSanitizacion - 1;
        }
        /* BOX 4 */
        if ((box[3].estado == 1) && (box[3].medicosActual > 0)){
            /* Buscar donde meter a los medicos */
                /* Reviso si el box 1 puede recibir medicos */ 
            if ((box[0].estado == 0) && (box[0].medicosActual < 3)){ // candidato a mover medicos
                while ((box[3].medicosActual + box[0].medicosActual) < 3 && (box[3].medicosActual > 0)){
                    box[0].medicosActual = box[0].medicosActual + 1;
                    box[3].medicosActual = box[3].medicosActual - 1;
                } 
            } /* reviso si el box 3 puede recibir medicos */ 
            if ((box[2].estado == 0) && (box[2].medicosActual < 3)){ // candidato a mover medicos
                while ((box[3].medicosActual + box[2].medicosActual) < 3 && (box[3].medicosActual > 0)){
                    box[2].medicosActual = box[2].medicosActual + 1;
                    box[3].medicosActual = box[3].medicosActual - 1;
                }
            } /*  Reviso si el box 2 puede recibir medicos */
            if ((box[1].estado == 0) && (box[1].medicosActual < 3)){ // candidato a mover medicos
                while ((box[3].medicosActual + box[1].medicosActual) < 3 && (box[3].medicosActual > 0)){
                    box[1].medicosActual = box[1].medicosActual + 1;
                    box[3].medicosActual = box[3].medicosActual - 1;
                }
            }
            cambiarMinutos(box);
            box[3].minutosSalirSanitizacion = box[3].minutosSalirSanitizacion - 1;
        }
    tiempoInicial = tiempoInicial + 1;
    }
    
    free(box);
    clock_t end = clock();
    double tTranscurrido = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Tiempo total de ejecucion: %f seconds.\n",tTranscurrido);
    return 0;

}   

int maximaEficienciaBox1(int n){
    int tiempoTotal = 0;
    int vacunadosTotales = 1; // se inicia en 1 puesto que al comenzar a vacunar, no tienen que esperar a que terminen de vacunar al enterior (porque no hay), entonces es como decir que entra al box y se vacuna inmediatamente
    int tiempoRestante;
    int tiempoRestanteCopia;
    /* Verifico cuanto es lo que se demora el box por vacunar */
    if (n == 1){
        tiempoRestante = 4;
        tiempoRestanteCopia = tiempoRestante;
    }else if (n == 2){
        tiempoRestante = 2;
        tiempoRestanteCopia = tiempoRestante;
    }else if (n >= 3){
        tiempoRestante = 1;
        tiempoRestanteCopia = tiempoRestante;
    }else{
        return 0;
    }
    while (tiempoTotal<240){
        if (tiempoRestante == 0){ // momento de vacunar
            vacunadosTotales = vacunadosTotales + 1;
            tiempoRestante = tiempoRestanteCopia;
            tiempoTotal = tiempoTotal + 1;
            if ((vacunadosTotales%12) == 0){
                tiempoTotal = tiempoTotal + 3;
            }
        }else{ // le resto al contador de tiempo restante y le agrego al tiempo global
            tiempoRestante = tiempoRestante - 1;
            tiempoTotal = tiempoTotal + 1;
        }
    }
    return vacunadosTotales;
}

int maximaEficienciaBox3(int n){
    int tiempoTotal = 0;
    int vacunadosTotales = 1; // se inicia en 1 puesto que al comenzar a vacunar, no tienen que esperar a que terminen de vacunar al enterior (porque no hay), entonces es como decir que entra al box y se vacuna inmediatamente
    int tiempoRestante;
    int tiempoRestanteCopia;
    if (n == 1){
        tiempoRestante = 4;
        tiempoRestanteCopia = tiempoRestante;
    }else if (n == 2){
        tiempoRestante = 2;
        tiempoRestanteCopia = tiempoRestante;
    }else if (n >= 3){
        tiempoRestante = 1;
        tiempoRestanteCopia = tiempoRestante;
    }else{
        return 0;
    }
    while (tiempoTotal<240){
        if (tiempoRestante == 0){ // momento de vacunar
            vacunadosTotales = vacunadosTotales + 1;
            tiempoRestante = tiempoRestanteCopia;
            tiempoTotal = tiempoTotal + 1;
            if ((vacunadosTotales%21) == 0){
                tiempoTotal = tiempoTotal + 5;
            }
        }else{
            tiempoRestante = tiempoRestante - 1;
            tiempoTotal = tiempoTotal + 1;
        }
    }
    return vacunadosTotales;
}

int maximaEficienciaBox4(int n){
    int tiempoTotal = 0;
    int vacunadosTotales = 1; // se inicia en 1 puesto que al comenzar a vacunar, no tienen que esperar a que terminen de vacunar al enterior (porque no hay), entonces es como decir que entra al box y se vacuna inmediatamente
    int tiempoRestante;
    int tiempoRestanteCopia;
    if (n == 1){
        tiempoRestante = 4;
        tiempoRestanteCopia = tiempoRestante;
    }else if (n == 2){
        tiempoRestante = 2;
        tiempoRestanteCopia = tiempoRestante;
    }else if (n >= 3){
        tiempoRestante = 1;
        tiempoRestanteCopia = tiempoRestante;
    }else{
        return 0;
    }
    while (tiempoTotal<240){
        if (tiempoRestante == 0){
            vacunadosTotales = vacunadosTotales + 1;
            tiempoRestante = tiempoRestanteCopia;
            tiempoTotal = tiempoTotal + 1;
            if ((vacunadosTotales % 27) == 0){
                tiempoTotal = tiempoTotal + 7;
            }
        }else{
            tiempoRestante = tiempoRestante - 1;
            tiempoTotal = tiempoTotal + 1;
        }
    }
    return vacunadosTotales;
}

/* cambia los minutos que faltan para vacunar al proximo paciente*/
void cambiarMinutos(Box *entrada){
    for (int i = 0; i < 4; i++){
        if (entrada[i].medicosActual == 1){
            entrada[i].minutosParaVacunar = 4;
            entrada[i].minutosParaVacunarResta = 4;
        }else if (entrada[i].medicosActual == 2){
            entrada[i].minutosParaVacunar = 2;
            entrada[i].minutosParaVacunarResta = 2;
        }else if (entrada[i].medicosActual >= 3){
            entrada[i].minutosParaVacunar = 1;
            entrada[i].minutosParaVacunarResta = 1;
        }
    }
}

char* pasarMinutosAHora(int minutos){//con esto calculo los minutos desde que comenzo a atenderse gente (9am hasta la hora de cierre 1pm) y agrego las horas faltantes 
    int horas;
    char puntos[5]=":";
    char* horasC =(char*)malloc(sizeof(char)*10);
    char minutosC[10];
    horas = minutos / 60;
    minutos = minutos % 3600; 
    minutos = minutos % 60; 
    sprintf(horasC, "%d", horas);
    sprintf(minutosC, "%d", minutos);
    strcat(horasC,puntos);
    if(minutos<10){
        char cero[5]="0";
        strcat(cero,minutosC);
        strcat(horasC,cero);
        return horasC;
    }
    strcat(horasC,minutosC);
    return horasC;

}