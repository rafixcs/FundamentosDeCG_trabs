// **********************************************************************
// PUCRS/Escola PolitŽcnica
// COMPUTA‚ÌO GRçFICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

// Para uso no Xcode:
// Abra o menu Product -> Scheme -> Edit Scheme -> Use custom working directory
// Selecione a pasta onde voce descompactou o ZIP que continha este arquivo.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;

#ifdef _WIN32
    #include <windows.h>
    #include <GL/freeglut.h>
#else
    #include <sys/time.h>
#endif

#ifdef __APPLE__
    #include <GLUT/glut.h>
#endif

#ifdef __linux__
    #include <glut.h>
#endif

#include "Ponto.h"
#include "Poligono.h"
#include "Temporizador.h"
#include "Labirinto.h"
#include "Bezier.h"
#include "Instancia.h"
#include <random>

Temporizador T;
double AccumDeltaT=0;

// Limites l—gicos da ‡rea de desenho
Ponto Min, Max;
#define QNT_PERSONAGENS 11
Instancia personagens[QNT_PERSONAGENS];

bool desenha = false;
bool animando = false;
bool startGame = false;

float angulo=0.0;

float tempo; // tempo em segundos para atravessar a tela
float TempoDaAnimacao;

BezierLab::Curva curvaAtual;
BezierLab::Labirinto labirinto;

double nFrames = 0;
double TempoTotal = 0;

// **********************************************************************
void DesenhaLabirinto()
{
    double t=0.0;
    double DeltaT = 1.0/10;
    for (int i = 0; i < labirinto.curvasLabirinto.size(); i++)
    {
        t = 0.f;
        if (personagens[0].bProxCurvaSel)
            if (personagens[0].proxCurva == i)
                glColor3f(0, 1, 0);
        else
            glColor3f(1, 0, 0);

        glBegin(GL_LINE_STRIP);
        while(t<1.0)
        {
            Ponto P = BezierCalculo::Bezier::CalculaBezier3(labirinto.curvasLabirinto[i], 1, t);
            glVertex2f(P.x, P.y);
            t += DeltaT;
        }
        glEnd();
    }
}

void Init()
{
    Ponto MinPoly, MaxPoly, Folga;
    srand((unsigned)time(NULL));
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    Min = Ponto (-40, -40);
    Max = Ponto (40, 40);

    labirinto.Initialize();

    for (int i = 0; i < QNT_PERSONAGENS; i++)
    {
        if (i == 0)
            personagens[i].Inicializa(0, 0, Max, Min, "config/Triangulo.txt");
        else
        {
            personagens[i].Inicializa(
                (rand() % (labirinto.curvasLabirinto.size()) - 1) + 1,
                0.5f,
                Max, Min,
                "config/Triangulo.txt"
            );
        }
    }
}

void AvancaComBezier(int index, double deltaTime)
{
    // TODO: arruma velocidade para cada curva
    personagens[index].t += deltaTime * 0.35;
    if (personagens[index].t > 1.0)
    {
        personagens[index].t = 0.f;
        personagens[index].AtualizaCurva();
        personagens[index].bProxCurvaSel = false;
    }
    else if (personagens[index].t > 0.5 && !personagens[index].bProxCurvaSel)
    {
        labirinto.ProxsCurva(personagens[index]);
        personagens[index].bProxCurvaSel = true;
    }

    personagens[index].posicaoAnt = personagens[index].posicao;
    personagens[index].posicao = BezierCalculo::Bezier::CalculaBezier3(
        labirinto.curvasLabirinto[personagens[index].curvaAtual],
        personagens[index].direcao,
        personagens[index].t
    );
}

void VerificaColisao()
{
    for (int i = 1; i < QNT_PERSONAGENS; i++)
    {
        if (personagens[0].curvaAtual == personagens[i].curvaAtual && startGame)
        {
            int t1 = (personagens[0].direcao > 0) ? personagens[0].t * 100 : (1 - personagens[0].t) * 100;
            int t2 = (personagens[i].direcao > 0) ? personagens[i].t * 100 : (1 - personagens[i].t) * 100;
            int total = abs(t1 - t2);
            if (total < 2)
                exit(0);            
        }
    }
}

void AvancaPersonagens(double dt)
{
    for (int i = 0; i < QNT_PERSONAGENS; i++)
    {
        AvancaComBezier(i, dt);
    }
    VerificaColisao();
}

void Animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualizacao da tela em 30
    {
        AccumDeltaT = 0;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: " << TempoTotal << " segundos. ";
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames / TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
    if (animando)
    {
        AvancaPersonagens(dt);
    }
}

void Reshape(int w, int h)
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);  // Janela de Exibi‹o
    // Define os limites logicos da area OpenGL dentro da Janela

    glOrtho(Min.x,Max.x, Min.y,Max.y, 0,1); //Janela de Sele‹o
    //glOrtho(0,10, 0,10, 0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES);
    {
        //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
        //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    }
    glEnd();
}

void RotacionaAoRedorDeUmPonto(float alfa, Ponto P)
{
    glTranslatef(P.x, P.y, P.z);
    glRotatef(alfa, 0,0,1);
    glTranslatef(-P.x, -P.y, -P.z);
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glLineWidth(3);
    glColor3f(1.f, 0.f, 0.f);

    DesenhaLabirinto();

    for (int i = 0; i < QNT_PERSONAGENS; i++)
    {
        if (i == 0)
            glColor3f(0.75f, 0.3f, 0.5f);
        else
            glColor3f(0.f, 0.f, 0.f);
        personagens[i].Desenha();
        //personagens[i].bbox.DrawBox();
    }

    glColor3f(1.f, 0.f, 0.f);
    glPointSize(3);
	glutSwapBuffers();
}

void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}

void Keyboard(unsigned char key, int x, int y)
{

	switch ( key )
	{
		case 27:
			exit ( 0 );
			break;
        case 't':
            ContaTempo(3);
            break;
        case 32:
            animando = !animando;
            startGame = true;
            break;
        case 'x':
            personagens[0].TrocaProxCurva();
            break;
        case 'z':
            personagens[0].TrocaDirecao();
            if (personagens[0].t > 0.5f)
                labirinto.ProxsCurva(personagens[0]);
		default: break;
	}
}

void ArrowKeysHandler(int a_keys, int x, int y)
{
    switch (a_keys)
	{
		case GLUT_KEY_UP:       // Se pressionar UP
			glutFullScreen ( ); // Vai para Full Screen
			break;
        case GLUT_KEY_RIGHT:       // Se pressionar SETA DIR
            angulo--;
            break;
        case GLUT_KEY_LEFT:       // Se pressionar SETA ESQ
            angulo++;
            break;
	    case GLUT_KEY_DOWN:     // Se pressionar UP reposiciona a janela
            glutPositionWindow(50, 50);
            glutReshapeWindow(700, 500);
			break;
		default:
			break;
	}
//    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    cout << "Programa OpenGL" << endl;

    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition(0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize( 500, 500);  // Define o SRD

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow("T2 Fundamentos de CG");

    // executa algumas inicializações
    Init();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // será chamada automaticamente quando
    // for necessário redesenhar a janela
    glutDisplayFunc(Display);

    // Define que o tratador de evento para
    // o invalida‹o da tela. A funcao "display"
    // será chamada automaticamente sempre que a
    // m‡quina estiver ociosa (idle)
    glutIdleFunc(Animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // será chamada automaticamente quando
    // o usuário alterar o tamanho da janela
    glutReshapeFunc(Reshape);

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // será chamada automaticamente sempre
    // o usuário pressionar uma tecla comum
    glutKeyboardFunc(Keyboard);

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" será chamada
    // automaticamente sempre o usuário
    // pressionar uma tecla especial
    glutSpecialFunc(ArrowKeysHandler);

    // inicia o tratamento dos eventos
    glutMainLoop();

    return 0;
}
