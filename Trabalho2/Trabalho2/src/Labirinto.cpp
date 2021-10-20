#include "Labirinto.h"
#include <fstream>

using namespace Bezier;

Labirinto::Labirinto()
{

}

Labirinto::Labirinto(std::string fileName)
{
	this->Initialize(fileName);
}

Bezier::Labirinto::~Labirinto()
{
}

void Labirinto::Initialize(std::string fileName)
{
	Ponto p1;
	Ponto p2;
	Ponto p3;

	std::fstream loadFile;

	loadFile.open(fileName.c_str(), std::ios::in);

	if (!loadFile)
	{
		std::cerr << "Falha ao carregar arquivo de configuração do labirinto" << std::endl;
		return;
	}

	while (loadFile)
	{
		int a, b, c;
		loadFile >> a >> b >> c;
		p1.x = p1.y = a;
		p2.x = p2.y = b;
		p3.x = p3.y = c;
		this->curvasLabirinto.push_back(Curvas(p1, p2, p3));
	}

	loadFile.close();
}