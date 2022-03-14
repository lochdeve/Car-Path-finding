#ifndef __Casilla__
#define __Casilla__

#include <iostream>
#include <vector>

//Clase que actua como espacio Casilla donde
class Casilla {
 public:
  Casilla(int, int);
  Casilla();
  //Casilla(Casilla&);  // contructor de copia
  virtual ~Casilla();
  std::pair<int, int> get_coords() { return coords_; }
  int get_G() { return G_; }
  int get_H() { return H_; }
  int get_F() { return (G_ + H_); }
  int get_tipo() const { return tipo_; }
  Casilla* get_pred() { return Pred_; }

  void set_tipo(int tip) { tipo_ = tip; }
  void set_coords(std::pair<int, int> x) { coords_ = x; }
  void set_G(int g) { G_ = g; }
  void set_H(int h) { H_ = h; }
  void set_pred(Casilla* pred) { Pred_ = pred; }

 private:
  int tipo_;                    // tipo (0=vacia,1=start,2=finish,3=obstaculo,4=camino)
  std::pair<int, int> coords_;  // identificador
  int G_, H_;                   // función F = G + H
  Casilla* Pred_;               // el mejor predecesor

  // F es la suma de G y H
  // G es el coste minimo para llegar a esta casilla desde la inicial
  // H es el coste minimo para llegar a la casilla final desde esta
};

#endif