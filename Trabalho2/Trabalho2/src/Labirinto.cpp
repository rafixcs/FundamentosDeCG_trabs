#include "Labirinto.h"
#include <random>
#include <fstream>

using namespace BezierLab;

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
	float scale = 4.f;

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

void Labirinto::ProxsCurva(Instancia& personagem)
{
	// TODO: garantir troca certa das curvas
	std::random_device rd;
	std::mt19937 gen(rd());
	int randIndex = 0;
	int labIndexCont = 0;
	int tempCurva = 0;

	personagem.proxsCurvas.clear();

	for (auto& cv : this->curvasLabirinto)
	{
		for (int i = 0; i < BEZIER_POINTS; i+=2)
		{
			for (int j = 0; j < BEZIER_POINTS; j+=2)
			{
				if (this->curvasLabirinto[personagem.curvaAtual] == cv)
				{
					break;
				}
				else
				{
					if (this->curvasLabirinto[personagem.curvaAtual].pontos[i] == cv.pontos[j])
					{
						if (personagem.direcao > 0 && this->curvasLabirinto[personagem.curvaAtual].pontos[2] == cv.pontos[j])
						{
							cv.starToEnd = true;
							personagem.proxsCurvas.push_back(labIndexCont);
						}
						else if (personagem.direcao < 0 && this->curvasLabirinto[personagem.curvaAtual].pontos[0] == cv.pontos[j])
						{
							cv.starToEnd = false;
							personagem.proxsCurvas.push_back(labIndexCont);
						}
						break;
					}
				}
			}
		}
		labIndexCont++;
	}
	
	std::uniform_int_distribution<> distr(0, personagem.proxsCurvas.size() - 1);
	randIndex = distr(gen);
	tempCurva = personagem.proxsCurvas[randIndex];

	if (this->curvasLabirinto[tempCurva].starToEnd)
	{
		if ((this->curvasLabirinto[personagem.curvaAtual].pontos[2] == this->curvasLabirinto[tempCurva].pontos[0])
			|| (this->curvasLabirinto[personagem.curvaAtual].pontos[0] == this->curvasLabirinto[tempCurva].pontos[2]))
		{
			personagem.proxDirecao = 1;
		}
		else
		{
			personagem.proxDirecao = -1;
		}
	}
	else
	{
		if ((this->curvasLabirinto[tempCurva].pontos[0] == this->curvasLabirinto[personagem.curvaAtual].pontos[0])
			|| (this->curvasLabirinto[tempCurva].pontos[2] == this->curvasLabirinto[personagem.curvaAtual].pontos[2]))
		{
			personagem.proxDirecao = 1;
		}
		else
		{
			personagem.proxDirecao = -1;
		}
	}

	personagem.proxCurva = tempCurva;
}
