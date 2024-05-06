#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <map>
#include <queue>

bool casillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
{
	return (mapa[x.f][x.c] != 'P' && mapa[x.f][x.c] != 'M');
}
ubicacion NextCasilla(const ubicacion &pos)
{
	ubicacion next = pos;
	switch (pos.brujula)
	{
	case norte:
		next.f = pos.f - 1;
		break;
	case noreste:
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	case este:
		next.c = pos.c + 1;
		break;
	case sureste:
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	case sur:
		next.f = pos.f + 1;
		break;
	case suroeste:
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	case oeste:
		next.c = pos.c - 1;
		break;
	case noroeste:
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	}
	return next;
}
// NIVEL 0
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> mapa)
{
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch (a)
	{
	case actWALK: // si prox casilla es transitable y no está ocupada por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (casillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c))
		{
			st_result.jugador = sig_ubicacion;
		}
		break;
	case actRUN: // si prox 2 casillas son transitables y no están ocupadas por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (casillaTransitable(sig_ubicacion, mapa) && !(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c))
		{
			sig_ubicacion2 = NextCasilla(sig_ubicacion);
			if (casillaTransitable(sig_ubicacion2, mapa) && !(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c))
			{
				st_result.jugador = sig_ubicacion2;
			}
		}
		break;
	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
		break;
	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1) % 8);
		break;
	}
	return st_result;
}

void AnulaMatriz(vector<vector<unsigned char>> &matriz)
{
	for (int i = 0; i < matriz.size(); i++)
	{
		for (int j = 0; j < matriz[0].size(); j++)
		{
			matriz[i][j] = 0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();

	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) && (*it != act_CLB_TURN_SR) && (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}
		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}
void ComportamientoJugador::VisualizaPlanN1(const stateN1 &st, const list<Action> &plan)
{
	AnulaMatriz(mapaConPlan);
	stateN1 cst = st;

	auto it = plan.begin();

	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) && (*it != act_CLB_TURN_SR) && (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}
		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

void PintaPlan(const list<Action> &plan)
{
	auto it = plan.begin();
	while (it != plan.end())
	{
		if (*it == actWALK)
		{
			cout << "W ";
		}
		else if (*it == actRUN)
		{
			cout << "R ";
		}
		else if (*it == actTURN_L)
		{
			cout << "L ";
		}
		else if (*it == actTURN_SR)
		{
			cout << "r ";
		}
		else if (*it == act_CLB_WALK)
		{
			cout << "cW ";
		}
		else if (*it == act_CLB_TURN_SR)
		{
			cout << "cr ";
		}
		else if (*it == act_CLB_STOP)
		{
			cout << "cS ";
		}
		else if (*it == actIDLE)
		{
			cout << "I ";
		}
		else
		{
			cout << "-_ ";
		}
		it++;
	}
	cout << " (" << plan.size() << " acciones)" << endl;
}
list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.jugador.f == final.f && current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() && !SolutionFound)
	{
		frontier.pop_front();
		explored.insert(current_node);
		// Generar Hijo actWALK
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK, current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		if (child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c)
		{
			current_node = child_walk;
			SolutionFound = true;
		}
		else if (explored.find(child_walk) == explored.end())
		{
			frontier.push_back(child_walk);
		}

		if (!SolutionFound)
		{
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c)
			{
				current_node = child_run;
				SolutionFound = true;
			}
			else if (explored.find(child_run) == explored.end())
			{
				frontier.push_back(child_run);
			}
		}
		if (!SolutionFound)
		{
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if (explored.find(child_turnl) == explored.end())
			{
				frontier.push_back(child_turnl);
			}
			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if (explored.find(child_turnsr) == explored.end())
			{
				frontier.push_back(child_turnsr);
			}
		}
		if (!SolutionFound && !frontier.empty())
		{
			current_node = frontier.front();
			while (!frontier.empty() && explored.find(current_node) != explored.end())
			{
				frontier.pop_front();
				if (!frontier.empty())
					current_node = frontier.front();
			}
		}
	}

	if (SolutionFound)
	{
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
}
// NIVEL 1

bool ColaboradorVisible(const ubicacion &j, const ubicacion &c)
{
	int fila_inicio, fila_fin, col_inicio, col_fin;

	switch (j.brujula)
	{
	case norte:
		fila_inicio = -3;
		fila_fin = -1;
		col_inicio = -3;
		col_fin = 3;
		break;
	case este:
		fila_inicio = -3;
		fila_fin = 3;
		col_inicio = 1;
		col_fin = 3;
		break;
	case sur:
		fila_inicio = 1;
		fila_fin = 3;
		col_inicio = -3;
		col_fin = 3;
		break;
	case oeste:
		fila_inicio = -3;
		fila_fin = 3;
		col_inicio = -3;
		col_fin = -1;
		break;
	case noroeste:
		fila_inicio = -3;
		fila_fin = 0;
		col_inicio = -3;
		col_fin = 0;
		break;
	case noreste:
		fila_inicio = -3;
		fila_fin = 0;
		col_inicio = 0;
		col_fin = 3;
		break;
	case sureste:
		fila_inicio = 0;
		fila_fin = 3;
		col_inicio = 0;
		col_fin = 3;
		break;
	case suroeste:
		fila_inicio = 0;
		fila_fin = 3;
		col_inicio = -3;
		col_fin = 0;
		break;
	}

	for (int fila = fila_inicio; fila <= fila_fin; fila++)
	{
		for (int col = col_inicio; col <= col_fin; col++)
		{
			if ((j.f + fila) == c.f && (j.c + col) == c.c)
				return true;
		}
	}

	return false;
}
bool samePosition(const ubicacion &a, const ubicacion &b)
{
	return a.f == b.f && a.c == b.c;
}
stateN1 applyN1(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> mapa)
{
	stateN1 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;

	switch (a)
	{
	case actWALK:
		sig_ubicacion = NextCasilla(st.jugador);
		if (casillaTransitable(sig_ubicacion, mapa) && !samePosition(sig_ubicacion, st.colaborador))
		{
			st_result.jugador = sig_ubicacion;
		}
		break;
	case actRUN:
		sig_ubicacion = NextCasilla(st.jugador);
		if (casillaTransitable(sig_ubicacion, mapa) && !samePosition(sig_ubicacion, st.colaborador))
		{
			sig_ubicacion2 = NextCasilla(sig_ubicacion);
			if (casillaTransitable(sig_ubicacion2, mapa) && !samePosition(sig_ubicacion2, st.colaborador))
			{
				st_result.jugador = sig_ubicacion2;
			}
		}
		break;
	case actIDLE:
		break;
	case actTURN_L:
		st_result.jugador.brujula = static_cast<Orientacion>((st.jugador.brujula + 6) % 8);
		break;
	case actTURN_SR:
		st_result.jugador.brujula = static_cast<Orientacion>((st.jugador.brujula + 1) % 8);
		break;
	case act_CLB_WALK:
		sig_ubicacion = NextCasilla(st.colaborador);
		if (casillaTransitable(sig_ubicacion, mapa) && !samePosition(sig_ubicacion, st.jugador))
		{
			st_result.colaborador = sig_ubicacion;
			st_result.ultimaOrdenColaborador = a;
		}
		break;
	case act_CLB_TURN_SR:
		st_result.colaborador.brujula = static_cast<Orientacion>((st.colaborador.brujula + 1) % 8);
		st_result.ultimaOrdenColaborador = a;
		break;
	case act_CLB_STOP:
		st_result.ultimaOrdenColaborador = a;
		break;
	}

	// Manejo de acciones continuas para el colaborador
	if (a != act_CLB_WALK && a != act_CLB_TURN_SR && a != act_CLB_STOP)
	{
		switch (st.ultimaOrdenColaborador)
		{
		case act_CLB_WALK:
			sig_ubicacion = NextCasilla(st.colaborador);
			if (casillaTransitable(sig_ubicacion, mapa) && !samePosition(sig_ubicacion, st.jugador))
			{
				st_result.colaborador = sig_ubicacion;
			}
			break;
		case act_CLB_TURN_SR:
			st_result.colaborador.brujula = static_cast<Orientacion>((st.colaborador.brujula + 1) % 8);
			break;
		}
	}

	return st_result;
}

list<Action> AnchuraNivel1(const stateN1 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN1 current_node;
	list<nodeN1> frontier;
	set<nodeN1> explored;
	list<Action> plan;
	int iteraciones = 0;
	int abiertos = 1; // Comienza con el nodo inicial ya en la frontera
	int cerrados = 0;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.colaborador.f == final.f && current_node.st.colaborador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() && !SolutionFound)
	{
		current_node = frontier.front();
		frontier.pop_front();
		cerrados++;
		explored.insert(current_node);

		// Determinar las acciones apropiadas según la visibilidad del colaborador
		vector<Action> acciones = ColaboradorVisible(current_node.st.jugador, current_node.st.colaborador) ? vector<Action>{act_CLB_WALK, act_CLB_TURN_SR, act_CLB_STOP} : vector<Action>{actWALK, actRUN, actTURN_L, actTURN_SR, actIDLE};

		for (auto accion : acciones)
		{
			nodeN1 hijo = current_node;
			hijo.st = applyN1(accion, current_node.st, mapa);
			hijo.secuencia.push_back(accion);

			// Comprobar si se ha alcanzado la solución
			if (hijo.st.colaborador.f == final.f && hijo.st.colaborador.c == final.c)
			{
				current_node = hijo;
				SolutionFound = true;
				break; // Salir del bucle, ya que se alcanzó el objetivo
			}

			// Si no se alcanzó el objetivo, considerar añadir el estado a la frontera
			if (explored.find(hijo) == explored.end() && !(hijo.st == current_node.st))
			{
				frontier.push_back(hijo);
				abiertos++;
			}
		}

		if (!SolutionFound && !frontier.empty())
		{
			current_node = frontier.front();
			while (!frontier.empty() && explored.find(current_node) != explored.end())
			{
				frontier.pop_front();
				if (!frontier.empty())
					current_node = frontier.front();
			}
		}
		iteraciones++;
	}

	if (SolutionFound || (current_node.st.colaborador.f == final.f && current_node.st.colaborador.c == final.c))
	{
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
		cout << "Abiertos: " << abiertos << endl;
		cout << "Cerrados: " << cerrados << endl;
		cout << "Iteraciones: " << iteraciones << endl;
		return plan;
	}
}

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{

	Action accion = actIDLE;
	if (sensores.nivel != 4)
	{
		if (!hayPlan)
		{
			jugador.f = sensores.posF;
			jugador.c = sensores.posC;
			jugador.brujula = sensores.sentido;
			colaborador.f = sensores.CLBposF;
			colaborador.c = sensores.CLBposC;
			colaborador.brujula = sensores.CLBsentido;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			switch (sensores.nivel)
			{
			case 0:
				c_state.jugador = jugador;
				c_state.colaborador = colaborador;
				plan = AnchuraSoloJugador(c_state, goal, mapaResultado);
				break;
			case 1:
				c_state1.jugador = jugador;
				c_state1.colaborador = colaborador;
				plan = AnchuraNivel1(c_state1, goal, mapaResultado);
				break;
			case 2:
				cout << "Nivel 2" << endl;
				break;
			case 3:
				cout << "Nivel 3" << endl;
				break;
			}
			if (plan.size() > 0)
			{
				if (sensores.nivel == 0)
				{
					VisualizaPlan(c_state, plan);
				}
				else
				{
					VisualizaPlanN1(c_state1, plan);
				}
				hayPlan = true;
			}
		}
		if (hayPlan && plan.size() > 0)
		{
			accion = plan.front();
			plan.pop_front();
		}
		if (plan.size() == 0)
		{
			cout << "Se completó el plan" << endl;
			hayPlan = false;
		}
	}
	else
	{
		// nivel 4
	}
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
