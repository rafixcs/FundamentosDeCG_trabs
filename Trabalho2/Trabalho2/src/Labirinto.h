#pragma once

#include <vector>
#include <string>
#include "Curvas.h"

namespace Bezier
{
	class Labirinto
	{
	public:
		std::vector<Curvas> curvasLabirinto;
		
		Labirinto();
		Labirinto(std::string fileName);
		~Labirinto();

		void Initialize(std::string fileName);
	};
}