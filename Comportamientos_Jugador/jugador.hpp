#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct stateN0
{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN0 &x) const
  {
    if (jugador == x.jugador and colaborador == x.colaborador)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
};
struct nodeN0
{
  stateN0 st;
  list<Action> secuencia;
  bool operator==(const nodeN0 &n) const
  {
    return (st == n.st);
  }
  bool operator<(const nodeN0 &b) const
  {
    if (st.jugador.f < b.st.jugador.f)
      return true;
    else if (st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c)
      return true;
    else if (st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula)
      return true;
    else
      return false;
  }
};
struct stateN1 
{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN1 &x) const
  {
    if (jugador == x.jugador && colaborador == x.colaborador && ultimaOrdenColaborador == x.ultimaOrdenColaborador)
    {
      return true;  
    }
    else
    {
      return false;
    }
  }
};  
struct nodeN1{
  stateN1 st;
  list<Action> secuencia;
  bool operator==(const nodeN1 &n) const
  {
    return (st == n.st);
  }
  bool operator<(const nodeN1 &b) const
  {
    if (st.jugador.f < b.st.jugador.f)
      return true;
    else if (st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c)
      return true;
    else if (st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula)
      return true;
    else if (st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f < b.st.colaborador.f)
      return true;
    else if (st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f == b.st.colaborador.f && st.colaborador.c < b.st.colaborador.c)
      return true;    
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f == b.st.colaborador.f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula < b.st.colaborador.brujula)
      return true;
     else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula && st.colaborador.f == b.st.colaborador.f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula == b.st.colaborador.brujula && st.ultimaOrdenColaborador < b.st.ultimaOrdenColaborador)
      return true;
    else
    return false;
  }
};
struct stateN2{
  ubicacion jugador;
  ubicacion colaborador;
  bool con_bikini;
  bool con_zapatillas;
  int coste;

  bool operator== (const stateN2 &x) const{
    return (jugador.f == x.jugador.f && jugador.c == x.jugador.c && jugador.brujula == x.jugador.brujula &&
            con_bikini == x.con_bikini && con_zapatillas == x.con_zapatillas && coste == x.coste);
  }

  bool operator<(const stateN2 &st) const{ // Operador para distinguir estados en set de cerrados
    return (jugador.f < st.jugador.f ||
    jugador.f == st.jugador.f and jugador.c < st.jugador.c ||
    jugador.f == st.jugador.f and jugador.c == st.jugador.c and jugador.brujula < st.jugador.brujula ||
    jugador.f == st.jugador.f and jugador.c == st.jugador.c and jugador.brujula == st.jugador.brujula and con_bikini < st.con_bikini ||
    jugador.f == st.jugador.f and jugador.c == st.jugador.c and jugador.brujula == st.jugador.brujula and con_bikini == st.con_bikini and con_zapatillas < st.con_zapatillas);
  }
};

struct nodeN2{
  stateN2 st;
  list<Action> secuencia;

  bool operator==(const nodeN2 &n) const{
    return (st == n.st);
  }

  bool operator<(const nodeN2 &n) const{ // Operador para ordenar los nodos en la priority queue
    return (st.coste > n.st.coste);
  }
};


class ComportamientoJugador : public Comportamiento
{
public:
  ComportamientoJugador(unsigned int size) : Comportamiento(size)
  {
    // Inicializar Variables de Estado
     abiertos = 1;
    cerrados = 0;
    iteraciones = 0;
  }
  ComportamientoJugador(std::vector<std::vector<unsigned char>> mapaR) : Comportamiento(mapaR)
  {
    // Inicializar Variables de Estado
    hayPlan = false;
    abiertos = 1;
    cerrados = 0;
    iteraciones = 0;
  }
  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);
  void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
  void VisualizaPlanN1(const stateN1 &st, const list<Action> &plan);

private:
  // Declarar Variables de Estado
  list<Action> plan;
  bool hayPlan;
  ubicacion goal, jugador, colaborador;
  stateN0 c_state;
  stateN1 c_state1;
  stateN2 c_state2;
  // contadores
  	int iteraciones;
    int abiertos;
    int cerrados;
};

#endif
