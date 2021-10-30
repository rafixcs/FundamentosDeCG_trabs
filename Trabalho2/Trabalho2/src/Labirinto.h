#pragma once

#include <vector>
#include <string>

#include "Curva.h"
#include "Instancia.h"

namespace BezierStruct
{
	struct Labirinto
	{
		std::vector<Curva> curvasLabirinto;
		
		Labirinto();
		Labirinto(std::string fileName);
		~Labirinto();

		void Initialize();
		void getAsCurva(Curva& curva, const int& index);
		void ProxCurva(Curva& curvaAtual, Instancia& personagem);
	};
}