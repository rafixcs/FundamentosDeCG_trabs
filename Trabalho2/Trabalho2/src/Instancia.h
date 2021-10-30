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
using namespace std;

#include "Poligono.h"

struct Instancia{
    Ponto posicao;
    Ponto escala;
    Ponto velocidade;
    
    int direcao;
    float rotacao;
    int modelo;

    void desenha();
};


#endif /* Instancia_hpp */
