//
//  Instancia.h
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//


#ifndef Instancia_hpp
#define Instancia_hpp

#include <iostream>
#include <vector>
using namespace std;

#include "Poligono.h"
#include "Labirinto.h"

typedef struct Instancia{
    Ponto posicao;
    Ponto posicaoAnt;
    Ponto escala;
    Ponto velocidade;
    Poligono carro;
    
    int direcao;
    int modelo;
    float rotacao;
    double t;

    bool bProxCurvaSel;
    int curvaAtual;
    int proxCurva;
    int proxDirecao;
    vector<int> proxsCurvas;

    Instancia();
    void Inicializa(Ponto& Max, Ponto& Min, std::string fileName);
    void Desenha();
    void AtualizaCurva();
    void TrocaProxCurva();
    void TrocaDirecao();
    void CalculaRotacao();
}Instancia;

#endif /* Instancia_hpp */
