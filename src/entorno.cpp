#include "../include/entorno.hpp"

//Constructor
Entorno::Entorno(int fila, int columna) {
  filas_ = fila;
  columnas_ = columna;
  dimension.height = fila * 20;
  dimension.width = columna * 20;
  ventana = nullptr;
  mouse_presionado = false;
  porcentaje_ = -1;
  calcular_ = false;
  reset_ = false;
  clean_ = false;

  matriz_sf.resize(filas_);

  for (int i = 0; i < filas_; i++) {
    matriz_sf[i].resize(columnas_);
  }
  matriz_entorno.resize(filas_);

  for (int j = 0; j < filas_; j++) {
    matriz_entorno[j].resize(columnas_);
  }
  for (int i = 0; i < filas_; i++) {
    for (int j = 0; j < columnas_; j++) {
      std::pair<int, int> aux(i, j);
      matriz_entorno[i][j].set_coords(aux);
    }
  }
}

// Constructor por fichero
Entorno::Entorno(std::string nombre_fichero) {
  std::ifstream fich_in;
  fich_in.open(nombre_fichero, std::ios::in);
  if (fich_in.is_open()) {
    std::cout << "El fichero se ha abierto\n";
  } else {
    std::cout << "El fichero no se ha abierto\n";
  }
  int x_ini, y_ini, x_fin, y_fin;
  fich_in >> filas_ >> columnas_ >> x_ini >> y_ini >> x_fin >> y_fin;
  x_ini--;
  y_ini--;
  x_fin--;
  y_fin--;
  inicio_.first = x_ini;
  inicio_.second = y_ini;
  final_.first = x_fin;
  final_.second = y_fin;

  int obstaculos = 0;
  fich_in >> obstaculos;
  dimension.height = filas_ * 20;
  dimension.width = columnas_ * 20;
  ventana = nullptr;
  mouse_presionado = false;

  matriz_sf.resize(filas_);
  for (int i = 0; i < filas_; i++) {
    matriz_sf[i].resize(columnas_);
  }

  matriz_entorno.resize(filas_);
  for (int j = 0; j < filas_; j++) {
    matriz_entorno[j].resize(columnas_);
  }
  for (int i = 0; i < filas_; i++) {
    for (int j = 0; j < columnas_; j++) {
      std::pair<int, int> aux(i, j);
      matriz_entorno[i][j].set_coords(aux);
    }
  }
  for (int i = 0; i < obstaculos; i++) {
    int x = 0, y = 0;
    fich_in >> x >> y;
    matriz_entorno[x - 1][y - 1].set_tipo(3);
  }

  matriz_entorno[x_ini][y_ini].set_tipo(1);
  matriz_entorno[x_fin][y_fin].set_tipo(2);

  fich_in.close();
  ventana = new sf::RenderWindow(dimension, "Entorno", sf::Style::Titlebar | sf::Style::Close);
  ventana->setFramerateLimit(60);
}

// Destructor
Entorno::~Entorno() {
  delete ventana;
}

void Entorno::set_matriz(Matriz_casillas entorno) {
  matriz_entorno = entorno;
}

// Devuelve si calcular o no el algortimo
void Entorno::set_calcular(bool cal) { calcular_ = cal; }

std::vector<std::vector<Casilla>> Entorno::get_entorno() {
  return matriz_entorno;
}

// getter
bool Entorno::get_calcular() { return calcular_; }

bool Entorno::get_reset() { return reset_; }

// Se encarga de limpiar el entorno y dejarla si ningun elemento
void Entorno::clean() {
  for (int i = 0; i < filas_; i++) {
    for (int j = 0; j < columnas_; j++) {
      if (((i != inicio_.first || j != inicio_.second) && (i != final_.first || j != final_.second)) &&
          matriz_entorno[i][j].get_tipo() != 3) {
        matriz_entorno[i][j].set_tipo(0);
        matriz_entorno[i][j].set_G(0);
        matriz_entorno[i][j].set_H(0);
        matriz_entorno[i][j].set_pred(nullptr);
      }
    }
  }
}

// Resetea el entorno  vuelve a introducir obstaculos aleatorios
void Entorno::reset_entorno() {
  reset_ = false;
  for (int i = 0; i < filas_; i++) {
    for (int j = 0; j < columnas_; j++) {
      if (((i != inicio_.first || j != inicio_.second) && (i != final_.first || j != final_.second))) {
        matriz_entorno[i][j].set_tipo(0);
        matriz_entorno[i][j].set_G(0);
        matriz_entorno[i][j].set_H(0);
        matriz_entorno[i][j].set_pred(nullptr);
      }
    }
  }
  if (porcentaje_ != -1) {
    int numero_random = 0;
    for (int k = 0; k < filas_; k++) {
      for (int l = 0; l < columnas_; l++) {
        numero_random = (rand() % 100);
        if (numero_random < porcentaje_ && ((k != inicio_.first || l != inicio_.second) &&
                                            (k != final_.first || l != final_.second)))
          matriz_entorno[k][l].set_tipo(3);
      }
    }
  }
}

// Se encarga de los eventos y esta pendiente de cerrar la ventana cuando se le indique
void Entorno::actualizarEvento() {
  while (ventana->pollEvent(evento)) {
    switch (evento.type) {
      case sf::Event::Closed:
        ventana->close();
        break;
      case sf::Event::KeyPressed:
        if (this->evento.key.code == sf::Keyboard::Escape)
          this->ventana->close();
        break;
      default:
        break;
    }
  }
}

// Se encarga de darle color a los diferentes elementos del entorno dependiendo del tipo que sea
void Entorno::renderizarEntorno() {
  sf::RectangleShape cuadrado(sf::Vector2f(20, 20));
  cuadrado.setOutlineThickness(1);
  cuadrado.setOutlineColor(sf::Color::Black);
  cuadrado.setFillColor(sf::Color::Blue);
  for (int i = 0; i < filas_; i++) {
    for (int j = 0; j < columnas_; j++) {
      cuadrado.setPosition(j * 20, i * 20);
      switch (matriz_entorno[i][j].get_tipo()) {
        case 0:  //casilla normal
          cuadrado.setFillColor(sf::Color(112, 128, 144));
          break;
        case 1:  //start
          cuadrado.setFillColor(sf::Color::Green);
          break;
        case 2:  //finish
          cuadrado.setFillColor(sf::Color::Red);
          break;
        case 3:  //obstaculo
          cuadrado.setFillColor(sf::Color::Yellow);
          break;
        case 4:  //camino
          cuadrado.setFillColor(sf::Color::White);
          break;
        case 5:  //visitado
          cuadrado.setFillColor(sf::Color(13, 71, 161));
          break;
        case 6:
          cuadrado.setFillColor(sf::Color(13, 71, 161));
          break;
        default:
          break;
      }
      matriz_sf[i][j] = cuadrado;
      ventana->draw(cuadrado);
    }
  }
}

// Llama a los diferentes metodos de renderizacion para que se muestre por pantalla
void Entorno::renderizar() {
  ventana->clear();

  // entorno
  renderizarEntorno();
  //mostrar pantalla
  ventana->display();
}

// Actualizar posicion del raton
void Entorno::actualizarPosMouse() {
  posicion_mouse = sf::Mouse::getPosition(*ventana);
  VistaPosMouse = ventana->mapPixelToCoords(posicion_mouse);
}

// Llama  a  los metodos necesarios para pode actualizar lo valores del entorno
void Entorno::actualizar() {
  actualizarEvento();
  actualizarPosMouse();
  actualizarCasillas();
}

// Devuelve true si la ventana esta abierta/ejecutandose
const bool Entorno::ejecutando() const {
  return ventana->isOpen();
}

// Detecta los clicks en los cuadrados y cambio sus valores
// dependiendo de las combinaciones de teclas y clicks
void Entorno::actualizarCasillas() {
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))) {
    if (mouse_presionado == false) {
      mouse_presionado = true;
      if (clean_)
        clean();
      for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
          if (matriz_sf[i][j].getGlobalBounds().contains(VistaPosMouse)) {
            matriz_entorno[inicio_.first][inicio_.second].set_tipo(0);
            matriz_entorno[i][j].set_tipo(1);
            inicio_.first = i;
            inicio_.second = j;
          }
        }
      }
    } else
      mouse_presionado = false;
  } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))) {
    if (this->mouse_presionado == false) {
      this->mouse_presionado = true;
      if (clean_)
        clean();
      for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
          if (matriz_sf[i][j].getGlobalBounds().contains(VistaPosMouse)) {
            matriz_entorno[final_.first][final_.second].set_tipo(0);
            matriz_entorno[i][j].set_tipo(2);
            final_.first = i;
            final_.second = j;
          }
        }
      }
    } else
      mouse_presionado = false;
  } else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if (this->mouse_presionado == false) {
      this->mouse_presionado = true;
      if (clean_)
        clean();
      for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
          if (matriz_sf[i][j].getGlobalBounds().contains(VistaPosMouse)) {
            matriz_entorno[i][j].set_tipo(3);
          }
        }
      }
    } else
      mouse_presionado = false;
  } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
    if (this->mouse_presionado == false) {
      this->mouse_presionado = true;
      if (clean_)
        clean();
      for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
          if (matriz_sf[i][j].getGlobalBounds().contains(VistaPosMouse)) {
            matriz_entorno[i][j].set_tipo(0);
          }
        }
      }
    } else
      mouse_presionado = false;
  } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))) {
    calcular_ = true;
    clean_ = true;
  } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::R))) {
    reset_ = true;
  } else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::C)))
    clean();
}

// Explicacion de como editar el entorno
void guia_edicion() {
  std::cout << std::endl;
  std::cout << "--------------Guia-de-edicion-de-entorno--------------\n";
  std::cout << "\U0001F6EB Ctrl+Click-Derecho: Punto de partida" << std::endl;
  std::cout << "\U0001F6EC Ctrl+Click-Izquierdo: Punto de destino" << std::endl;
  std::cout << "\U000026F0  Click-Derecho: Colocar obstaculos" << std::endl;
  std::cout << "\U0001F4A5 Click-Izquierdo: Quitar obstaculos" << std::endl;
  std::cout << "\U0001F4C8 Enter: Calcular camino optimo" << std::endl;
  std::cout << "\U0001F501 R: Restaurar entorno para volver a inicializar" << std::endl;
  std::cout << "\U0001F3D7  C: Restaurar entorno sin quitar obstaculos" << std::endl;
  std::cout << "\U0001F51A Esc: Terminar ejecución del programa" << std::endl;
  std::cout << std::endl;
}

// Sirve para que el usuario vea las opciones que tiene e inicializa la ventana
void Entorno::inicializacion_manual() {
  guia_edicion();
  ventana = new sf::RenderWindow(dimension, "Entorno", sf::Style::Titlebar | sf::Style::Close);
  ventana->setFramerateLimit(60);
}

// Genera obstaculos aleatoriamente yte da la opcion de introducir o no el punto
// inicial y final
void Entorno::inicializacion_auto(bool opcion) {
  srand(time(NULL));
  int i = 1, j = 1, x = 0, y = 0, numero_random = 0;
  if (opcion == true) {
    do {
      std::cout << "Introduzca el punto de partida: \n";
      std::cout << "i: ";
      std::cin >> i;
      std::cout << "j: ";
      std::cin >> j;
      std::cout << "Introduzca el punto de llegada: \n";
      std::cout << "i: ";
      std::cin >> x;
      std::cout << "j: ";
      std::cin >> y;
    } while ((i > filas_) || (j > columnas_) || (x > filas_) || (y > columnas_));

    inicio_.first = i - 1;
    inicio_.second = j - 1;
    final_.first = x - 1;
    final_.second = y - 1;
    matriz_entorno[i - 1][j - 1].set_tipo(1);
    matriz_entorno[x - 1][y - 1].set_tipo(2);
  }

  do {
    std::cout << "Introduzca el porcentaje de obstaculos que quieres: ";
    std::cin >> porcentaje_;
  } while (porcentaje_ < 0 && porcentaje_ > 100);

  for (int k = 0; k < filas_; k++) {
    for (int l = 0; l < columnas_; l++) {
      numero_random = (rand() % 100);
      if (numero_random < porcentaje_ && ((k != i - 1 || l != j - 1) && (k != x - 1 || l != y - 1)))
        matriz_entorno[k][l].set_tipo(3);
    }
  }

  guia_edicion();
  ventana = new sf::RenderWindow(dimension, "Entorno", sf::Style::Titlebar | sf::Style::Close);
  ventana->setFramerateLimit(60);
}

//muestra la matriz por la terminal usado para testear al principio
std::ostream &Entorno::write(std::ostream &os) const {
  for (int i = 0; i < filas_; i++) {
    for (int j = 0; j < columnas_; j++) {
      switch (matriz_entorno[i][j].get_tipo()) {
        case 0:  //casilla normal
          os << "\u2B1B";
          break;
        case 1:  //start
          os << "\U0001F535";
          break;
        case 2:  //finish
          os << "\U0001F534";
          break;
        case 3:  //obstaculo
          os << "\U0000274C";
          break;
        case 4:  //camino
          os << "\U0001F698";
          break;
        default:
          break;
      }
    }
    os << std::endl;
  }
  return os;
}
