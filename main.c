#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"TADPilaEst.h"

//Declaracion de funciones

boolean RevisionParentesis(char[], int); //Revisa que los parentesis esten bien
void ConvertirPostfijo(char[],int,char *);//Convierte la expresion a postfija
int nivelPrioridad(char);//Da un nivel de prioridad a los operadores
void evaluarFuncion(char []); //Evaluar expresion en postfijo
void obtenerVariables (char [],char *);//Encuentra cada variable de la expresion
boolean buscarLetra(char [], char);//Busca cada letra en la expresion sin que se repitan
void obtenerValores(char[],float *);//Pide al usuario los valores de cada variable

int main(){    
    char expresion[100],posfijo[100];
    printf("Ingresa la expresion: ");
    scanf("%s",expresion);
    int longitud=strlen(expresion);
    //Para poder convertir a postfija primero verifico que los parentesis esten bien
    if(RevisionParentesis(expresion,longitud)){
        ConvertirPostfijo(expresion,longitud,posfijo);
        printf("La expresion en posfijo es: %s\n",posfijo);
        evaluarFuncion(posfijo);
    }
    exit(0);
}

/*
Descripcion: Revisa que los parentesis abran y cirren correctamente
Recibe: char expresion[] (La expresion a analizar), int longitud (Longitud de la expresion)
Devuelve: boolean (retorna TRUE si esta bien la expresion, si no retorna FALSE)
Observaciones: La expresion no debe estar vacia; en caso de no tener parentesis la acepta como correcto
*/
boolean RevisionParentesis(char expresion[],int longitud){
    pila PilaParentesis;
    Initialize(&PilaParentesis);
    elemento temp;
    boolean verificacion;
    int i;
    //Reviso la cadena en busca de los parentesis
    for (i = 0; i < longitud; i++){
        //Si encuentra un parentesis de apertura lo apila y si es de cierre desapila un elemento
        if (expresion[i]=='('){
            temp.c='(';
            Push(&PilaParentesis,temp);
        }else if(expresion[i]==')'){
            Pop(&PilaParentesis);
        }
    }
    //Si la pila esta vacia entonces esta bien la expreion y si no entonces es incorrecta
    if (Empty(&PilaParentesis)) {
        printf("Los parentesis son correctos\n");
        verificacion= TRUE;
    }else {
        printf("Los parentesis son incorrectos\n");
        verificacion= FALSE;
    }
    Destroy(&PilaParentesis);
    return verificacion;
}

/*
Descripcion: Convierte un expresion infija a postfija
Recibe: char expresion[] (La expresion a convertir), int longitud (Longitud de la expresion), char *posfijo (Cadena que la almacenara la expresion en postfijo)
Devuelve: void (No devuelve nada)
Observaciones: Solo sirve con parentesis, no usar corchetes o llaves
*/
void ConvertirPostfijo(char expresion[],int longitud,char *posfijo){
    pila pilaOperaciones;
    Initialize(&pilaOperaciones);
    elemento temp;
    int i;
    //Recorro toda la cadena de la expresion
    for (i = 0; i < longitud; i++){
        //Si es un parentesis de apertura lo apilamos
        if(expresion[i]=='('){
            temp.c='(';
            Push(&pilaOperaciones,temp);
        }
        //En caso de ser un operador entra aqui
        else if(nivelPrioridad(expresion[i])>0 && nivelPrioridad(expresion[i])<=3){
            //Verificamos que la pila no este vacia porque daria error si lo estuviera
            if(!Empty(&pilaOperaciones)){
                //Va a desapilar y poner en la expresion hasta que encuentre un operador de menor nivel o hasta que la pila este vacia (Primero verifica y despues desapila)
                while(nivelPrioridad(Top(&pilaOperaciones).c)>=nivelPrioridad(expresion[i])){
                    temp=Pop(&pilaOperaciones);
                    *posfijo=temp.c;
                    posfijo++;
                    if (Empty(&pilaOperaciones))
                        break;
                    
                }
            }
            //Apilamos el operador actual
            temp.c=expresion[i];
            Push(&pilaOperaciones,temp);
        }
        //En caso de ser parentesis de cierre va a desapilar hasta que encuentre un parentesis de apertura
        else if(expresion[i]==')'){
            while(nivelPrioridad(Top(&pilaOperaciones).c)>0){
                temp=Pop(&pilaOperaciones);
                *posfijo=temp.c;
                posfijo++;
            }
            temp=Pop(&pilaOperaciones);
        }
        //En caso de ser una variable la pone directamente en la expresion
        else{
            *posfijo=expresion[i];
            posfijo++;
        }
    }
    //Si aun no esta vacia la pila pone el ultimo operador que falta
    if(!Empty(&pilaOperaciones)){
        *posfijo=Pop(&pilaOperaciones).c;
        posfijo++;
        *posfijo='\0';
    }
    Destroy(&pilaOperaciones);
}

/*
Descripcion: Da un nivel de prioridad en base a un operador
Recibe: char operador (Es el operador que vamos a evaluar y solo pueden ser [^,/,*,+,-,(])
Devuelve: int (Es el nivel de prioridad del 0 al 3)
Observaciones: Estos niveles son con el fin de facilitar el algoritmo para convertir a postfijo
*/
int nivelPrioridad(char operador){
    if(operador=='^'){
        return 3;
    }else if(operador=='*' || operador=='/'){
        return 2;
    }else if(operador=='+' || operador=='-'){
        return 1;
    }else if(operador=='('){
        return 0;
    }
}


void evaluarFuncion(char postfijo[]){
    int longitud=strlen(postfijo);
    pila pilaEvaluacion;
    Initialize(&pilaEvaluacion);
    char variables[100];
    float valores[100];
    obtenerVariables(postfijo,variables);
    obtenerValores(variables,valores);

}

/*
Descripcion: Obtiene las varibles usadas en la expresion
Recibe: char expresion[] (Expresion que analizara),char *variables (Arreglo que almacenara las variables)
Devuelve: void (No retorna algo)
Observaciones: Solo busca caracteres en mayusculas
*/
void obtenerVariables(char expresion[],char *variables){
    char *primerVariable=variables;
    int i;
    for(i=0;i<strlen(expresion);i++){
        if(expresion[i]>=65 && expresion[i]<=90){
            if(!buscarLetra(primerVariable,expresion[i])){
                *variables=expresion[i];
                variables++;
            }
        }
    }
}

/*
Descripcion: Revisa que la letra no este almacenada en las variables
Recibe: char *variables (Arreglo de variables), char letra (Variable que revisara)
Devuelve: boolean (Retorna TRUE si encuentra la letra, en caso contrario retorna FALSE)
*/
boolean buscarLetra(char *variables, char letra){
    int i;
    for(i=0;i<strlen(variables);i++){
        if(letra==variables[i])
            return TRUE;
    }
    return FALSE;
}

/*
Descripcion: Pide al usuario los valores de cada variable
Recibe: char variables[] (Arreglo de variables), float *valores (Arreglo que almacenara el valor de cada variable)
Devuelve: void (No devuelve algo)
*/
void obtenerValores(char variables[], float *valores){
    int  i;
    for(i = 0; i < strlen(variables); i++){
        printf("Ingresa el valor de %c:",variables[i]);
        scanf("%f",valores);
        valores++;
    }
}

/*
Expresiones de prueba
(A+B*C-D+B)-(F/B*C)+D*(H^Y)
(A-B)*(C+(D-B))
*/