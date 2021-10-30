#include "Curva.h"

using namespace BezierStruct;

BezierStruct::Curva::Curva()
{
	this->pontos[0] = Ponto(0, 0);
	this->pontos[1] = Ponto(0, 0);
	this->pontos[2] = Ponto(0, 0);
}

Curva::Curva(Ponto p1, Ponto p2, Ponto p3)
{
	this->pontos[0] = p1;
	this->pontos[1] = p2;
	this->pontos[2] = p3;
}

BezierStruct::Curva::~Curva()
{
}

Ponto BezierStruct::Curva::GetPoint(int index)
{
	return this->pontos[index];
}

Ponto* BezierStruct::Curva::GetPoints()
{
	return this->pontos;
}
