#ifndef __Coche__
#define __Coche__

#include <sys/time.h>

#include <cmath>
#include <ctime>
#include <list>

#include "entorno.hpp"
//#include "dll_t.hpp"

typedef std::pair<std::pair<int, int>, int> pair_coords_cost;

class Coche {
   public:
    Coche(Entorno&, int);
    ~Coche();

    void path_finding();
    void analizar_casilla(Casilla, int, int);
    bool get_salida() const;
    void analizar_vecinos(Casilla&);

    Matriz_casillas get_entorno() { return entorno_; }

   private:
    std::vector<pair_coords_cost> abierta;

    Matriz_casillas entorno_;
    Casilla inicio_, final_;
    bool final_found_;
    int heuristica_;
    int longitud_camino_;
    int numero_nodos_expandidos_;
};

#endif