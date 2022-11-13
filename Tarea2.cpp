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
#include <sstream>

using namespace std;
pthread_mutex_t mutex;


//----Variables
string tipoarchivo;
string paquetes;
int cantidad_ips;
//----Listas
vector<string> IPs;
vector<string> info;
vector<string> valores;

int Identificador()
{
    int auxiliar = 0;
    char registrador[50];
    FILE *archivo;
    const char* leer = tipoarchivo.c_str();
    archivo = fopen(leer, "r");
    //Condicion para que el archivo no esté vacío.
    if(archivo == NULL){
        cerr << "Error al abrir archivo" << endl;
        terminate();
    }
    //Extrae mediante un caracter el contenido del archivo.
    while(!feof(archivo)){
        fgets(registrador, 50, archivo);
        auxiliar++;
    }
    rewind(archivo);

    int contador = auxiliar-1;

    while(contador != 0){
        fgets(registrador, 50, archivo);
        if(!(registrador[0] == '\n')){
            strtok(registrador, "\n");
            IPs.push_back(registrador);
        }
        contador--;
    }
    return auxiliar-1;
}

void *salida(int x)
{
    string posicion = to_string(x);
    int auxiliar = x;

    string formato = "ping -q -c"+paquetes+" "+IPs[auxiliar]+" > archivo"+posicion+".txt";
    const char * comando = formato.c_str();
    system(comando);

    char registrador[100];
    int contador = 0;

    FILE *file;
    string archivo = "archivo"+posicion+".txt";
    const char* a = archivo.c_str();
    file = fopen(a, "r");

    if(file == NULL){
        cerr << "No se ha podido abrir el archivo porque esta vacio" << endl;
        terminate();
    }
    while((!feof(file)) && (contador != 4)){
        fgets(registrador, 100, file);
        if(contador == 3 && (!(registrador[0] == '\n'))){
            strtok(registrador, "\n");
            info.insert(info.begin() + auxiliar, registrador);
        }
        contador++;
    }
    contador = 0;
    string temp;
    stringstream input_stringstream(info[auxiliar]);
    while(getline(input_stringstream, temp, ' ')){
        if(contador == 0){
            valores[0] = temp;
        }
        if(contador == 3){
            valores[1] = temp;
        }
        if(contador == 5){
            valores[2] = temp;
        }
        contador++;
    }
    if(valores[1] != "0"){
        cout << IPs[auxiliar] << "     " << valores[0] << "        " << valores[1] << "     " << valores[2]+"      UP\n"; 
    }else{
        cout << IPs[auxiliar] << "     " << valores[0] << "        " << valores[1] << "     " << valores[2]+"      DOWN\n"; 
    }

    pthread_mutex_unlock(&mutex);
    return 0;
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);

    tipoarchivo = argv[1];
    paquetes = argv[2];

    cantidad_ips = Identificador();
    info.resize(cantidad_ips);
    valores.resize(3);
    thread threads[cantidad_ips];
    cout << "IP     Trans.      Rec.    Perd.       Estado\n";
    cout << "-----------------------------------------------\n";
    for(int i = 0; i < cantidad_ips; i++)
    {
        threads[i] = thread(salida, i);
    }

    for(int i = 0; i < cantidad_ips; i++)
    {
        threads[i].join();
    }

    return 0;
}