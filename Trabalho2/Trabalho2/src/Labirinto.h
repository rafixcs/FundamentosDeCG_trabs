#pragma once

#include <vector>
#include <string>

#include "Curva.h"
#include "Instancia.h"

namespace BezierLab
{
	class Labirinto
	{
	public:
		std::vector<Curva> curvasLabirinto;
		
		Labirinto();
		Labirinto(std::string fileName);
		~Labirinto();

		void Initialize();
		void getAsCurva(Curva& curva, const int& index);
		void ProxsCurva(struct Instancia& personagem);
	};
}