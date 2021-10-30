#pragma once

#include "Ponto.h"
#define BEZIER_POINTS 3

namespace BezierStruct
{
	struct Curva
	{
		Ponto pontos[BEZIER_POINTS];
		bool starToEnd;

		Curva();
		Curva(Ponto p1, Ponto p2, Ponto p3);
		~Curva();

		Ponto GetPoint(int index);
		Ponto* GetPoints();

		bool operator==(const Curva& c1)
		{
			return {
				this->pontos[0] == c1.pontos[0] &&
				this->pontos[1] == c1.pontos[1] &&
				this->pontos[2] == c1.pontos[2]
			};
		}
	};
}
