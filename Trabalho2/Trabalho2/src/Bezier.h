#pragma once

#include "Curva.h"

namespace BezierCalculo
{
	struct Bezier
	{
		static Ponto CalculaBezier3(const BezierStruct::Curva& curva, const int& direction, double t)
		{
			Ponto P;
			double UmMenosT = 1 - t;

            if (direction > 0)
            {
                P = curva.pontos[0] * UmMenosT * UmMenosT +
                    curva.pontos[1] * 2 * UmMenosT * t +
                    curva.pontos[2] * t * t;
            }
            else
            {
                P = curva.pontos[2] * UmMenosT * UmMenosT +
                    curva.pontos[1] * 2 * UmMenosT * t +
                    curva.pontos[0] * t * t;
            }

			return P;
		}

        /*static double CalculaComprimentoDaCurva(const BezierStruct::Curva& curva)
        {
            double deltaT = 1.0 / 50;
            double t = deltaT;            
            double comprimentoTotalDaCurva = 0.f;
            Ponto P1 = curva.pontos[0];
            Ponto P2;
            while (t < 1.0)
            {
                P2 = Calcula(t);
                comprimentoTotalDaCurva += calculaDistancia(P1, P2);
                P1 = P2;
                t += deltaT;
            }
            P2 = Calcula(1.0); // faz o fechamento da curva
            comprimentoTotalDaCurva += calculaDistancia(P1, P2);            
            
            return comprimentoTotalDaCurva;
        }*/
	};
}