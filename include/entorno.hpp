#ifndef __Entorno__
#define __Entorno__
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "casilla.hpp"

typedef std::vector<std::vector<Casilla>> Matriz_casillas;

// Clase que actua como espacio entorno donde
class Entorno {
 public:
  Entorno(int, int);
  Entorno(std::string);
  virtual ~Entorno();
  // Getters
  std::vector<std::vector<Casilla>> get_entorno();
  int get_filas() { return filas_; }
  int get_columnas() { return columnas_; }
  std::pair<int, int> get_inicio() { return inicio_; }
  std::pair<int, int> get_final() { return final_; }
  bool get_calcular();
  bool get_reset();

  // setter
  void set_matriz(Matriz_casillas entorno);
  void set_calcular(bool);
  // Entorno sfml
  void reset_entorno();
  void clean();

  void iniciar_entorno() {
    ventana = new sf::RenderWindow(dimension, "Entorno", sf::Style::Titlebar | sf::Style::Close);
    ventana->setFramerateLimit(60);
  }
  // Inicilizacion

  // Renderizacion
  void renderizarEntorno();

  void renderizar();

  // Actualizacion
  void actualizarPosMouse();
  void actualizarEvento();
  void actualizarCasillas();

  void actualizar();

  const bool ejecutando() const;

  // Inicializacion inicio, final y obstaculos
  void inicializacion_manual();
  void inicializacion_auto(bool);  // true = manual, false = consola
  std::ostream& write(std::ostream&) const;

 private:
  // Estructura
  std::vector<std::vector<Casilla>> matriz_entorno;
  int filas_;
  int columnas_;
  std::pair<int, int> inicio_;
  std::pair<int, int> final_;
  int porcentaje_;
  bool calcular_;
  bool reset_;
  bool clean_;

  // Entorno grafico sfml
  sf::RenderWindow* ventana;
  std::vector<std::vector<sf::RectangleShape>> matriz_sf;
  bool mouse_presionado;
  sf::Event evento;
  sf::Vector2i posicion_mouse;
  sf::Vector2f VistaPosMouse;
  sf::VideoMode dimension;
};

#endif
