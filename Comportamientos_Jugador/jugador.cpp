#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

list<Action> AvanzaASaltosDeCaballo()
{
	list<Action> secuencia;
	secuencia.push_back(actWALK);
	secuencia.push_back(actWALK);
	secuencia.push_back(actTURN_L);
	secuencia.push_back(actWALK);
	return secuencia;
}


// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	if (!hayPlan){
		cout << "Calculamos un plan" << endl;
		plan = AvanzaASaltosDeCaballo();
		hayPlan=true;
	}
	if (hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();;
	}
	if(plan.size()== 0){
		cout << "Se completó el plan"<< endl;
		hayPlan = false;
	}
	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
