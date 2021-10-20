#pragma once

#include "Ponto.h"

#define BEZIER_POINTS 3

namespace Bezier
{
	class Curvas
	{
	private:

	public:
		Ponto pontos[BEZIER_POINTS];

		Curvas(Ponto p1, Ponto p2, Ponto p3);
		~Curvas();

		Ponto GetPoint(int index);
		Ponto* GetPoints();
	};
}
