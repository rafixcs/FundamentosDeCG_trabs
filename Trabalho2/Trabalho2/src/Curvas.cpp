#include "Curvas.h"

using namespace Bezier;

Curvas::Curvas(Ponto p1, Ponto p2, Ponto p3)
{
	this->pontos[0] = p1;
	this->pontos[1] = p2;
	this->pontos[2] = p3;
}

Bezier::Curvas::~Curvas()
{
}

Ponto Bezier::Curvas::GetPoint(int index)
{
	return this->pontos[index];
}

Ponto* Bezier::Curvas::GetPoints()
{
	return this->pontos;
}
