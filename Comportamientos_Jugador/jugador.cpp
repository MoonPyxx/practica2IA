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
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
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
		if (casillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c))
		{
			st_result.jugador = sig_ubicacion;
		}
		break;
	case actRUN: // si prox 2 casillas son transitables y no están ocupadas por el colaborador
		sig_ubicacion = NextCasilla(st.jugador);
		if (casillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c))
		{
			sig_ubicacion2 = NextCasilla(sig_ubicacion);
			if (casillaTransitable(sig_ubicacion2, mapa) and !(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c))
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
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
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
bool Find(const stateN0 &item, const list<stateN0> &lista)
{
	auto it = lista.begin();
	while (it != lista.end() and !((*it) == item))
		it++;

	return (!(it == lista.end()));
}
// sobrecargar
bool Find(const stateN0 &item, const list<nodeN0> &lista)
{
	auto it = lista.begin();
	while (it != lista.end() && !(it->st == item))
	{
		it++;
	}
	return (!(it == lista.end()));
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
		if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c)
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
			if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c)
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
			while (!frontier.empty() and explored.find(current_node) != explored.end())
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
bool ColaboradorVisible(ubicacion &j, ubicacion &c)
{
	switch (j.brujula)
	{
	case norte:
		if ((j.f - 1) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c + 3) == c.c)
			return true;
		break;
	case este:
		if ((j.f - 1) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c + 3) == c.c)
			return true;
		break;
	case sur:
		if ((j.f + 1) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c - 3) == c.c)
			return true;
		break;
	case oeste:
		if ((j.f + 1) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c - 3) == c.c)
			return true;
		break;
	case noroeste:
		if ((j.f) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c) == c.c)
			return true;
		if ((j.f) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c) == c.c)
			return true;
		if ((j.f) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c) == c.c)
			return true;
		break;
	case noreste:
		if ((j.f - 1) == c.f && (j.c) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f - 3) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f - 2) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f - 1) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f) == c.f && (j.c + 3) == c.c)
			return true;
		break;
	case sureste:
		if ((j.f) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c) == c.c)
			return true;
		if ((j.f) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c) == c.c)
			return true;
		if ((j.f) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c + 3) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c + 2) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c + 1) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c) == c.c)
			return true;
		break;
	case suroeste:
		if ((j.f + 1) == c.f && (j.c) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c - 1) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c - 2) == c.c)
			return true;
		if ((j.f + 3) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f + 2) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f + 1) == c.f && (j.c - 3) == c.c)
			return true;
		if ((j.f) == c.f && (j.c - 3) == c.c)
			return true;
		break;
	}
}
stateN0 aplicarAccionColaborador(stateN0 estadoActual, Action accion, const vector<vector<unsigned char>> &mapa)
{

	return estadoActual; // Retorna el nuevo estado
}
list<Action> AnchuraNivel1(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, Sensores sensores)
{
	nodeN0 current_node;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.colaborador.f == final.f && current_node.st.colaborador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() && !SolutionFound)
	{
		frontier.pop_front();
		explored.insert(current_node);

		// Comprobar si el colaborador está en el campo de visión del jugador
		if (!esVisible(sensores))
		{
			// Programar acciones del jugador
			// Generar Hijo actWALK
			nodeN0 child_walk = current_node;
			child_walk.st = apply(actWALK, current_node.st, mapa);
			child_walk.secuencia.push_back(actWALK);
			if (explored.find(child_walk) == explored.end())
			{
				frontier.push_back(child_walk);
			}

			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			child_run.secuencia.push_back(actRUN);
			if (explored.find(child_run) == explored.end())
			{
				frontier.push_back(child_run);
			}

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
		else
		{
			// Programar acciones del colaborador
			// Generar Hijo actCLB_WALK
			nodeN0 child_clbwalk = current_node;
			child_clbwalk.st = apply(act_CLB_WALK, current_node.st, mapa);
			child_clbwalk.secuencia.push_back(act_CLB_WALK);
			if (child_clbwalk.st.colaborador.f == final.f and child_clbwalk.st.colaborador.c == final.c)
			{
				current_node = child_clbwalk;
				SolutionFound = true;
			}
			else if (explored.find(child_clbwalk) == explored.end())
			{
				frontier.push_back(child_clbwalk);
			}
			// Generar hijo actCLB_TURN_SR
			nodeN0 child_clbturnsr = current_node;
			child_clbturnsr.st = apply(act_CLB_TURN_SR, current_node.st, mapa);
			child_clbturnsr.secuencia.push_back(act_CLB_TURN_SR);
			if (explored.find(child_clbturnsr) == explored.end())
			{
				frontier.push_back(child_clbturnsr);
			}
		}

		if (!SolutionFound && !frontier.empty())
		{
			current_node = frontier.front();
			while (!frontier.empty() and explored.find(current_node) != explored.end())
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
void crearCurrentState(const Sensores &sensores, stateN0 &c_state)
{
	c_state.jugador.f = sensores.posF;
	c_state.jugador.c = sensores.posC;
	c_state.jugador.brujula = sensores.sentido;
	c_state.colaborador.f = sensores.CLBposF;
	c_state.colaborador.c = sensores.CLBposC;
	c_state.colaborador.brujula = sensores.CLBsentido;
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
			cout << "Calculamos un plan" << endl;
			crearCurrentState(sensores, c_state);
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			switch (sensores.nivel)
			{
			case 0:
				plan = AnchuraSoloJugador(c_state, goal, mapaResultado);
				break;
			case 1:
				cout << "Nivel 1" << endl;
				if (esVisible(sensores))
				{
					cout << "El colaborador es visible" << endl;
				}
				else
				{
					cout << "El colaborador no es visible" << endl;
				}
				plan = AnchuraNivel1(c_state, goal, mapaResultado, sensores);
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
				VisualizaPlan(c_state, plan);
				hayPlan = true;
			}
		}
		if (hayPlan and plan.size() > 0)
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
