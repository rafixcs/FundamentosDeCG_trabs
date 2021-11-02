//
//  Instancia.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "Instancia.h"
#include "Bezier.h"

// ***********************************************************
//  void InstanciaPonto(Ponto3D *p, Ponto3D *out)
//  Esta funcao calcula as coordenadas de um ponto no
//  sistema de referencia do universo (SRU), ou seja,
//  aplica as rotacoes, escalas e translacoes a um
//  ponto no sistema de referencia do objeto (SRO).
// ***********************************************************
void InstanciaPonto(Ponto &p, Ponto &out)
{
    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;
    
    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);
    
    for(i=0;i<4;i++)
    {
        ponto_novo[i]= matriz_gl[0][i] * p.x+
        matriz_gl[1][i] * p.y+
        matriz_gl[2][i] * p.z+
        matriz_gl[3][i];
    }
    out.x=ponto_novo[0];
    out.y=ponto_novo[1];
    out.z=ponto_novo[2];
}

Ponto InstanciaPonto(Ponto P)
{
    Ponto temp;
    InstanciaPonto(P, temp);
    return temp;
}

Instancia::Instancia()
{
    this->direcao = 0;
    this->modelo = 0;
    this->rotacao = 0;
    this->t = 0.f;
    this->bProxCurvaSel = false;
    this->curvaAtual = 0;
    this->proxCurva = 0;
    this->proxDirecao = 0;
    this->proxsCurvas.clear();
}

void Instancia::Inicializa(Ponto& Max, Ponto& Min, std::string fileName)
{
    this->carro.LePoligono(fileName.c_str());
    this->posicao = Ponto(0, 0);
    this->direcao = 1;
    this->curvaAtual = 0;
    this->bProxCurvaSel = false;
    this->t = 0.f;
    this->velocidade.x = (Max.x - Min.x) / 5;
    this->velocidade.y = (Max.y - Min.y) / 5;
    this->rotacao = 0.f;
}

void Instancia::Desenha()
{
    Ponto Metade;
    Ponto Min, Max;

    glColor3f(0.f, 0.f, 0.f);
    glPushMatrix();
    {
        glTranslatef(this->posicao.x,
                    this->posicao.y,
                    this->posicao.z);  // posiciona o objeto
        this->CalculaRotacao();
        glRotated(this->rotacao, 0, 0, 1);

        this->carro.desenhaPoligono();

        this->carro.desenhaPoligono();
    }    
    glPopMatrix();
}

void Instancia::AtualizaCurva()
{
    this->curvaAtual = this->proxCurva;
    this->direcao = this->proxDirecao;
    this->proxsCurvas.clear();
}

void Instancia::TrocaProxCurva()
{
    if (this->proxsCurvas.size() > 0)
    {
        std::vector<int>::iterator itr = std::find(
            this->proxsCurvas.begin(), 
            this->proxsCurvas.end(), 
            this->proxCurva
        );

        int tempProxIndex = static_cast<int>(std::distance(this->proxsCurvas.begin(), itr));

        if (tempProxIndex++ != this->proxsCurvas.size()-1)
        {
            this->proxCurva = this->proxsCurvas[tempProxIndex++];
        }
        else
        {
            this->proxCurva = this->proxsCurvas[0];
        }
    }
}

void Instancia::TrocaDirecao()
{
    this->direcao *= -1;
    this->t = 1 - this->t;
    if (this->t < 0.5)
    {
        this->bProxCurvaSel = false;
        this->proxsCurvas.clear();
    }
}

void Instancia::CalculaRotacao()
{
    Ponto vDir;
    vDir = this->posicao - this->posicaoAnt;

    this->rotacao = 180 / 3.1459265 * acos(vDir.escalar(Ponto(1,0,0)));
    
    if (vDir.y < 0.f)
        rotacao *= -1;
}
