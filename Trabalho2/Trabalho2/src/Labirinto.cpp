#include "Labirinto.h"
#include <random>
#include <fstream>

using namespace BezierStruct;

Labirinto::Labirinto()
{

}

Labirinto::Labirinto(std::string fileName)
{
	this->Initialize();
}

Labirinto::~Labirinto()
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
	float scale = 5.f;

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
		this->curvasLabirinto.push_back(Curva(pontos[a] * scale, pontos[b] * scale, pontos[c] * scale));
	} while (!loadFile.eof());

	loadFile.close();
}

void Labirinto::getAsCurva(Curva& curva, const int& index)
{
	curva.pontos[0] = this->curvasLabirinto[index].pontos[0];
	curva.pontos[1] = this->curvasLabirinto[index].pontos[1];
	curva.pontos[2] = this->curvasLabirinto[index].pontos[2];
}

void Labirinto::ProxCurva(Curva& curvaAtual, Instancia& personagem)
{
	std::vector<Curva> tempCurvas;
	int randIndex = 0;
	std::random_device rd;
	std::mt19937 gen(rd());

	for (auto& cv : this->curvasLabirinto)
	{
		for (int i = 0; i < BEZIER_POINTS; i+=2)
		{
			for (int j = 0; j < BEZIER_POINTS; j+=2)
			{
				if (curvaAtual == cv)
				{
					break;
				}
				else
				{
					if (curvaAtual.pontos[i] == cv.pontos[j])
					{
						if (personagem.direcao > 0 && curvaAtual.pontos[2] == cv.pontos[j])
						{
							cv.starToEnd = true;
							tempCurvas.push_back(cv);
						}
						else if (personagem.direcao < 0 && curvaAtual.pontos[0] == cv.pontos[j])
						{
							cv.starToEnd = false;
							tempCurvas.push_back(cv);
						}
						break;
					}
				}
			}
		}
	}
	
	std::uniform_int_distribution<> distr(0, tempCurvas.size()-1);
	randIndex = distr(gen);

	if (tempCurvas[randIndex].starToEnd)
	{
		if (tempCurvas[randIndex].pontos[2] == curvaAtual.pontos[0])
		{
			personagem.direcao = 1;
		}
		else
		{
			personagem.direcao = -1;
		}
	}
	else
	{
		if (tempCurvas[randIndex].pontos[0] == curvaAtual.pontos[0])
		{
			personagem.direcao = 1;
		}
		else
		{
			personagem.direcao = -1;
		}
	}
	curvaAtual = tempCurvas[randIndex];
}
