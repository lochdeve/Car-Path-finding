#include "../include/coche.hpp"

//Constructor
Coche::Coche(Entorno& entorno_Coche, int heu) {
  entorno_ = entorno_Coche.get_entorno();
  inicio_.set_coords(entorno_Coche.get_inicio());
  inicio_.set_G(0);
  heuristica_ = heu;
  inicio_.set_pred(nullptr);
  final_.set_coords(entorno_Coche.get_final());
  final_.set_G(0);
  final_.set_H(0);
  final_.set_pred(nullptr);
  final_found_ = false;
  numero_nodos_expandidos_ = 0;
  longitud_camino_ = 0;
  if (heu == 1) {
    inicio_.set_H((abs(final_.get_coords().first - inicio_.get_coords().first) + abs(final_.get_coords().second - inicio_.get_coords().second)) * 10);  //manhattan
  } else
    inicio_.set_H((sqrt(pow(final_.get_coords().first - inicio_.get_coords().first, 2) + pow(final_.get_coords().second - inicio_.get_coords().second, 2))) * 10);
}

Coche::~Coche() {
}

// Se encarga de analizar todos los vecinos adyacentes
void Coche::analizar_vecinos(Casilla& casilla) {
  analizar_casilla(casilla, -1, 0);  // arriba
  analizar_casilla(casilla, 0, 1);   // derecha
  analizar_casilla(casilla, 1, 0);   // abajo
  analizar_casilla(casilla, 0, -1);  // izda
}

// Se trata del algoritmo para encontrar el camino optimo
void Coche::path_finding() {
  unsigned t0, t1;
  t0 = clock();
  pair_coords_cost pcc;

  pcc.first = inicio_.get_coords();
  pcc.second = inicio_.get_F();
  abierta.push_back(pcc);
  while (!final_found_ && !abierta.empty()) {
    Casilla box;
    bool entra = false;
    pcc.second = abierta.back().second;

    for (int i = 0; i < int(abierta.size()); i++) {
      if (abierta[i].second < pcc.second) {  // recorre la lista abierta y guarda en pcc la id
                                             // y el coste de la casilla con menos coste
        pcc.second = abierta[i].second;
        pcc.first = abierta[i].first;
        entra = true;
      }
    }

    if (!entra) {
      pcc.first = abierta.back().first;
      pcc.second = abierta.back().second;
    }

    box = entorno_[pcc.first.first][pcc.first.second];  // metemos en box el adyacente con mejor f
                                                        //para ahora analizar su adjacencia y meterlo en la lista cerrada
    pair_coords_cost casilla_pcc;
    casilla_pcc.first = box.get_coords();
    casilla_pcc.second = box.get_F();
    // Se elimina la casilla actual de la lista abierta y se mete en la cerrada
    if (entorno_[pcc.first.first][pcc.first.second].get_coords() != inicio_.get_coords()) {
      entorno_[pcc.first.first][pcc.first.second].set_tipo(6);
    }
    for (int i = 0; i < int(abierta.size()); i++) {
      if (abierta[i].first == casilla_pcc.first) {
        abierta.erase(abierta.begin() + i);
      }
    }

    analizar_vecinos(box);
  }

  std::cout << "-----------------------------------------------------------" << std::endl;
  if (!final_found_) {
    std::cout << "-- No existe ningun camino al punto final\n";
    final_found_ = false;
  } else {
    Casilla* pred = final_.get_pred();
    while (pred != &entorno_[inicio_.get_coords().first][inicio_.get_coords().second]) {
      entorno_[pred->get_coords().first][pred->get_coords().second].set_tipo(4);
      pred = pred->get_pred();
      longitud_camino_++;
    }
  }
  t1 = clock();
  float time1 = float(t1 - t0);
  std::cout << "-- Longitud del camino optimo: " << longitud_camino_ << " casillas." << std::endl;
  std::cout << "-- Numero de nodos creados: " << numero_nodos_expandidos_ << " casillas." << std::endl;

  if (time1 > 100000) {
    time1 = time1 / 1000000;
    std::cout << "-- Tiempo total: " << time1
              << " segundos"
              << std::endl;
  } else {
    std::cout << "-- Tiempo total: " << time1
              << " milisegundos"
              << std::endl;
  }
  std::cout << "-----------------------------------------------------------" << std::endl;
}

// Se encarga de analizar una casilla vecina  que depende de los parametros
void Coche::analizar_casilla(Casilla box, int i, int j) {
  std::pair<int, int> posicion_vecino;
  posicion_vecino.first = box.get_coords().first + i;
  posicion_vecino.second = box.get_coords().second + j;
  bool pertenece_lista_cerrada = false;
  bool pertenece_lista_abierta = false;

  Casilla* pred = &entorno_[box.get_coords().first][box.get_coords().second];

  if (posicion_vecino.first < int(entorno_.size()) && posicion_vecino.second < int(entorno_[1].size()) &&
      posicion_vecino.first >= 0 && posicion_vecino.second >= 0) {                                                         // entra en las dimensiones
    if ((posicion_vecino.first != final_.get_coords().first) || (posicion_vecino.second != final_.get_coords().second)) {  // si no es la casilla final entra
      if (entorno_[posicion_vecino.first][posicion_vecino.second].get_tipo() != 3) {                                       //  si no es un obstaculo entra

        if (entorno_[posicion_vecino.first][posicion_vecino.second].get_tipo() == 6) {
          pertenece_lista_cerrada = true;
        }
        for (int i = 0; i < int(abierta.size()) && !pertenece_lista_cerrada; i++) {
          if (entorno_[posicion_vecino.first][posicion_vecino.second].get_coords() == abierta[i].first) {
            pertenece_lista_abierta = true;
          }
        }

        if (!pertenece_lista_abierta && !pertenece_lista_cerrada) {  // si no estaba en la lista abierta, la introduce e inicializa los valores F, G, H y pred
          entorno_[posicion_vecino.first][posicion_vecino.second].set_G(box.get_G() + 10);
          if (heuristica_ == 1) {
            entorno_[posicion_vecino.first][posicion_vecino.second].set_H((abs(final_.get_coords().first - posicion_vecino.first) + abs(final_.get_coords().second - posicion_vecino.second)) * 10);  //manhattan
          } else {
            entorno_[posicion_vecino.first][posicion_vecino.second].set_H((sqrt(pow(final_.get_coords().first - posicion_vecino.first, 2) + pow(final_.get_coords().second - posicion_vecino.second, 2))) * 10);  //euclidiana
          }
          entorno_[posicion_vecino.first][posicion_vecino.second].set_pred(pred);
          pair_coords_cost par_vecino(posicion_vecino, entorno_[posicion_vecino.first][posicion_vecino.second].get_F());

          if (entorno_[posicion_vecino.first][posicion_vecino.second].get_coords() != inicio_.get_coords()) {
            entorno_[posicion_vecino.first][posicion_vecino.second].set_tipo(5);
            numero_nodos_expandidos_++;
          }
          abierta.push_back(par_vecino);
        } else if (!pertenece_lista_cerrada) {  // no pertenece a la lista cerrada
          if (entorno_[posicion_vecino.first][posicion_vecino.second].get_G() > (box.get_G() + 10)) {
            entorno_[posicion_vecino.first][posicion_vecino.second].set_G(box.get_G() + 10);
            entorno_[posicion_vecino.first][posicion_vecino.second].set_pred(pred);
          }
        }
      }
    } else {
      final_.set_pred(pred);
      final_found_ = true;
    }
  }
}

// Getter
bool Coche::get_salida() const { return final_found_; }

/* 
x-1,y
x,y+1
x+1,y
x,y-1

Paso 1: Meter casilla en lista cerrada
Paso 2: Meter sus adyacentes en lista abierta
Paso 2.1: Si una de las casillas adyacentes es la final termina la ejecucion
Paso 2.2: Si una de las casillas adyacentes es un obstaculo no se añade a la lista abierta
Paso 3: Calcular las F, G y H de cada casilla adyacentes (A menos que este en la lista cerrada)
Paso 4: Buscar la casilla adyacente con menor F
Paso 5: Analizar esa casilla
*/
