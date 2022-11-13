#include <thread>
#include <string.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;
pthread_mutex_t mutex;

string cant_paquetes;
string nombre_archivo;
int num;

vector<string> ips;
vector<string> info;

void *estructura(int i){
    string posicion = to_string(i);
    int posicion2 = i;
    
    string linea = "ping -q -c"+cant_paquetes+" "+ips[posicion2]+" > archivo"+posicion+".txt";
    const char * comando = linea.c_str();
    system(comando);

    //--------------------------------------Recolección de info
    
    char temp[100];
    int cont = 0;
    
    FILE *file;
    string archivo = "archivo"+posicion+".txt";
    const char* a = archivo.c_str();
    file = fopen(a, "r");

    if(file == NULL){
        cerr << "Error al abrir archivo" << endl;
        terminate();
    }
    while(!feof(file)){
        cout << "asd";
        // if(cont == 3){
        //     fgets(temp, 100, file);
        //     if(!(temp[0] == '\n')){
                
        //         strtok(temp, "\n");
        //         info.insert(info.begin() + posicion2, temp);
        //     }
        // }
        // cont++;
    }
    cout << "instrucción "+posicion+" realizada \n";
    return 0;
}

int cant_ips(){
    char temp[50];
    int cont = 0;

    FILE *archivo;
    const char* a = nombre_archivo.c_str();
    archivo = fopen(a, "r");

    if(archivo == NULL){
        cerr << "Error al abrir archivo" << endl;
        terminate();
    }

    while(!feof(archivo)){
        fgets(temp, 50, archivo);
        cont++;
    }

    rewind(archivo);
    int indice = cont-1;

    while(indice != 0){
        fgets(temp, 50, archivo);
        if(!(temp[0] == '\n')){
            strtok(temp, "\n");
            ips.push_back(temp);
        }
        indice--;
    }
    return cont-1;
}

//argc: cantidad de variables en el comando de ejecución
//argv: arreglo con las variables utilizadas en el comando de ejecución
//comando de ejecución: ./intento3 archivoips.txt cantidad_paquetes
//argv[0]: ./intento3
//argv[1]: archivo.txt
//argv[2]: cantidad_paquetes
int main(int argc, char *argv[]){
    pthread_mutex_init(&mutex, NULL);
    //guarda el nombre del archivo y la cantidad de paquetes en 
    //strings globales para que se pueda acceder a estos datos 
    //desde cualquier parte del código
    
    nombre_archivo = argv[1];
    cant_paquetes = argv[2];
    
    //creación de un arreglo con la misma cantidad
    //de hebras que la cantidad de ips en el archivo
    num = cant_ips();
    
    thread threads[num];
    //void *argumento;

    for(int i = 0; i < num; i++){
        //creación de las hebras y envío de 
        //argumento con su posición en el arreglo de hebras
        //argumento = reinterpret_cast<void*>(i);
        threads[i] = thread(estructura, i);
    }

    //Ciclo for para esperar por el término de todos los hilos
    for(int i = 0; i < num; i++){
        threads[i].join();
    }
    
    return 0;
}