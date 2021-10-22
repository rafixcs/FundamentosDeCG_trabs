#include "Labirinto.h"
#include <fstream>

using namespace Bezier;

Labirinto::Labirinto()
{

}

Labirinto::Labirinto(std::string fileName)
{
	this->Initialize();
}

Bezier::Labirinto::~Labirinto()
{
}

void Labirinto::Initialize()
{
	std::string fileCurvas = "config/Curvas.txt";
	std::string filePontos = "config/Pontos.txt";
	std::fstream loadFile;	
	std::vector<Ponto> pontos;
	Ponto pBuffer;
	Ponto p1;
	Ponto p2;
	Ponto p3;

	pontos.clear();
	this->curvasLabirinto.clear();

	loadFile.open(filePontos.c_str(), std::ios::in);
	if (!loadFile)
	{
		std::cerr << "Falha ao carregar arquivo de configuração do labirinto de pontos" << std::endl;
		return;
	}

	do {
		loadFile >> pBuffer.x >> pBuffer.y;
		pontos.push_back(pBuffer);
	} while (!loadFile.eof());

	loadFile.close();

	loadFile.open(fileCurvas.c_str(), std::ios::in);

	if (!loadFile)
	{
		std::cerr << "Falha ao carregar arquivo de configuração do labirinto de curvas" << std::endl;
		return;
	}

	do {
		int a, b, c;
		loadFile >> a >> b >> c;		
		this->curvasLabirinto.push_back(Curvas(pontos[a], pontos[b], pontos[c]));
	} while (!loadFile.eof());

	loadFile.close();
}