#include <unistd.h>

#include <iostream>

#include "../include/coche.hpp"
#include "../include/entorno.hpp"

void text_fich(void);
void text(bool);  //false = aleatorio, true = manual

int main() {
    int opcion = 0;

    do {
        std::cout << "----------------------Menu----------------------\n";
        std::cout << "1. Generar entorno a partir de fichero\n";
        std::cout << "2. Generar entorno con obstaculos aleatorios\n";
        std::cout << "3. Generar entorno de forma manual\n";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                text_fich();
                break;
            case 2:
                text(false);
                break;
            case 3:
                text(true);
                break;
            default:
                std::cout << "Por favor introduzca un numero de los indicados en el menu \n";
                break;
        }
    } while (opcion > 3 || opcion < 1);

    return 0;
}

void text_fich(void) {
    std::string nombre;
    std::cout << "Introduce el nombre del fichero: ";
    std::cin >> nombre;
    int funcion_heuristica = 0;
    std::cout << "Elija la heuristica: " << std::endl;
    std::cout << "1. Distancia de Manhattan" << std::endl;
    std::cout << "2. Distancia de Euclides" << std::endl;
    std::cin >> funcion_heuristica;
    Entorno mapa(nombre);
    Coche cochecito(mapa, funcion_heuristica);
    cochecito.path_finding();
    mapa.set_matriz(cochecito.get_entorno());
    while (mapa.ejecutando()) {
        //actualizar
        mapa.actualizarEvento();
        //renderizar
        mapa.renderizar();
    }
}

void text(bool modalidad) {
    int altura = 0, anchura = 0;
    std::cout << "Introduzca las dimensiones del entorno: \n";
    std::cout << "Altura: ";
    std::cin >> altura;
    std::cout << "Anchura: ";
    std::cin >> anchura;
    Entorno mapa(altura, anchura);

    int funcion_heuristica = 0;
    do {
        std::cout << "Elija la heuristica: " << std::endl;
        std::cout << "1. Distancia de Manhattan" << std::endl;
        std::cout << "2. Distancia de Euclides" << std::endl;
        std::cin >> funcion_heuristica;
    } while (funcion_heuristica > 2 || funcion_heuristica < 1);

    if (modalidad)
        mapa.inicializacion_manual();
    else {
        int opcion_ini_dest = 0;
        do {
            std::cout << "¿Desea introducir las coordenadas de inicio y destino por consola?" << std::endl;
            std::cout << "1. Si" << std::endl;
            std::cout << "2. No" << std::endl;
            std::cin >> opcion_ini_dest;
        } while (opcion_ini_dest > 2 || opcion_ini_dest < 1);
        if (opcion_ini_dest == 1)
            mapa.inicializacion_auto(true);
        else
            mapa.inicializacion_auto(false);
    }

    sleep(1);
    while (mapa.ejecutando()) {
        //actualizar
        mapa.actualizar();
        //renderizar
        mapa.renderizar();
        if (mapa.get_calcular()) {
            mapa.set_calcular(false);
            sleep(1);
            Coche cochecito(mapa, funcion_heuristica);
            cochecito.path_finding();
            mapa.set_matriz(cochecito.get_entorno());
        }
        if (mapa.get_reset()) {
            mapa.reset_entorno();
        }
    }
}