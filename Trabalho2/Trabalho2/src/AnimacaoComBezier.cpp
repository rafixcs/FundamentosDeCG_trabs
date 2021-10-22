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

Temporizador T;
double AccumDeltaT=0;

// Limites l—gicos da ‡rea de desenho
Ponto Min, Max;
Poligono Quadrado;

#include "Instancia.h"

Instancia Personagens[10];

bool desenha = false;
bool animando = false;

float angulo=0.0;

float tempo;// tempo em segundos para atravessar a tela
float TempoDaAnimacao;

//Ponto Curva1[3];
Bezier::Labirinto labirinto;

// **********************************************************************
Ponto CalculaBezier3(Ponto PC[3], double t)
{
    Ponto P;
    double UmMenosT = 1-t;
    
    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}
// **********************************************************************
void TracaBezier3Pontos()
{
    double t=0.0;
    double DeltaT = 1.0/10;
    //cout << "DeltaT: " << DeltaT << endl;
    for (int i = 0; i < labirinto.curvasLabirinto.size(); i++)
    {
        t = 0.f;
        glBegin(GL_LINE_STRIP);
        while(t<1.0)
        {
            Ponto P = CalculaBezier3(labirinto.curvasLabirinto[i].pontos, t);
            glVertex2f(P.x, P.y);
            t += DeltaT;
        }
        glEnd();        
    }
}
// **********************************************************************
//
// **********************************************************************
void init()
{
    Ponto MinPoly, MaxPoly, Folga;
    
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    
    Quadrado.LePoligono("config/Retangulo.txt");
    
    Min = Ponto (-20, -20);
    Max = Ponto (20, 20);

    // No trabalho, este ’ndice [0] NAO pode ser hard-coded.
    
    Personagens[0].Posicao = Ponto(0,0);
    Personagens[0].Direcao = Ponto(1,0);
    
    tempo = 5;
    // No trabalho, este ’ndice [0] NAO pode ser hard-coded.
    Personagens[0].Velocidade.x = (Max.x - Min.x)/tempo;
    Personagens[0].Velocidade.y = (Max.y - Min.y)/tempo;
    
    labirinto.Initialize();
}

double nFrames=0;
double TempoTotal=0;

// **********************************************************************
//
// **********************************************************************
void AvancaMRU(double dt)
{
    if (Personagens[0].Posicao.x >= Max.x)
    {
        animando = false;
        cout << "Tempo da Animacao: " << TempoDaAnimacao << " segundos." << endl;
        Personagens[0].Posicao = Ponto(0,0); // retorna o objeto a sua posicao inicial
    }

    // No trabalho, este ’ndice [0] NAO pode ser hard-coded.
    Ponto Deslocamento;
    Deslocamento.x = dt * Personagens[0].Velocidade.x * Personagens[0].Direcao.x;
    Deslocamento.y = dt * Personagens[0].Velocidade.y * Personagens[0].Direcao.y;
    Personagens[0].Posicao = Personagens[0].Posicao + Deslocamento;
}
// **********************************************************************
//
// **********************************************************************
void AvancaComBezier()
{
    
    double t;
    t = TempoDaAnimacao/tempo;
    if (t>1.0)
    {
        animando = false;
        cout << "Tempo da Animacao: " << TempoDaAnimacao << " segundos." << endl;
        Personagens[0].Posicao = Ponto(0,0); // retorna o objeto a sua posicao inicial
    }
    //Personagens[0].Posicao = CalculaBezier3(Curva1,t);

}
// **********************************************************************
//
// **********************************************************************
void AvancaPersonagens(double dt)
{
    //AvancaMRU(dt);
    AvancaComBezier();
    
}
// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza‹o da tela em 30
    {
        AccumDeltaT = 0;
        //angulo+=0.05;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
    if (animando)
    {
        AvancaPersonagens(dt);
        TempoDaAnimacao += dt;
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
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
// **********************************************************************
//
// **********************************************************************
void DesenhaEixos()
{
    Ponto Meio;
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    glBegin(GL_LINES); // vou desenhar LINHAS
    //  eixo horizontal
        glVertex2f(Min.x,Meio.y);
        glVertex2f(Max.x,Meio.y);
    //  eixo vertical
        glVertex2f(Meio.x,Min.y);
        glVertex2f(Meio.x,Max.y);
    glEnd();
}
// **********************************************************************
//
// **********************************************************************
void RotacionaAoRedorDeUmPonto(float alfa, Ponto P)
{
    glTranslatef(P.x, P.y, P.z);
    glRotatef(alfa, 0,0,1);
    glTranslatef(-P.x, -P.y, -P.z);
}
// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{
	// Limpa a tela com a cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	// Coloque aqui as chamadas das rotinas que desenham os objetos
	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	glLineWidth(3);
	glColor3f(1,1,1); // R, G, B  [0..1]
   // glRotatef(angulo, 0,0,1); // rotiona no eixo Z
    DesenhaEixos();
    
    glPushMatrix();
        glColor3f(1,0,0); // R, G, B  [0..1]
        Ponto Metade;
        Ponto Min, Max;
        Quadrado.obtemLimites(Min,Max);
        Metade.x = (Max.x-Min.x)/2;
        Metade.y = (Max.y-Min.y)/2;
        Metade.z = (Max.z-Min.z)/2;

        // Personagens[0].desenha(); Esta Ž forma organizada de fazer o desenho

    // Esta Ž a forma DESORGANIZADA
                glTranslatef(-Metade.x, -Metade.y, -Metade.z); // Muda o sistema de REF para o meio do objeto

                // No trabalho, este ’ndice [0] NAO pode ser hard-coded.
                glTranslatef(Personagens[0].Posicao.x,
                             Personagens[0].Posicao.y,
                             Personagens[0].Posicao.z);  // posiciona o objeto
                //Posicoes[0].imprime(); cout << endl;
                Quadrado.desenhaPoligono();
    glPopMatrix();
    
    glColor3f(1,0,0);
    glPointSize(3);
    TracaBezier3Pontos();
    
    
	glutSwapBuffers();
}
// **********************************************************************
// ContaTempo(double tempo)
//      conta um certo nœmero de segundos e informa quanto frames
// se passaram neste per’odo.
// **********************************************************************
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
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27:        // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 't':
            ContaTempo(3);
            break;
        case '1':
            animando = true;
            TempoDaAnimacao = 0;
            Personagens[0].Posicao = Ponto (Min.x,0);
            Personagens[0].Direcao = Ponto (1,0);
            cout << "Posicao Inicial: "; Personagens[0].Posicao.imprime(); cout << endl;
            break;
        case '2':
            animando = true;
            TempoDaAnimacao = 0;
            Personagens[0].Posicao = Ponto (Min.x,Min.y);
            Personagens[0].Direcao  = Ponto (1,1);
            cout << "Posicao Inicial: "; Personagens[0].Posicao.imprime(); cout << endl;
            break;
		default:
			break;
	}
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
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
	    case GLUT_KEY_DOWN:     // Se pressionar UP
								// Reposiciona a janela
            glutPositionWindow (50,50);
			glutReshapeWindow ( 700, 500 );
			break;
		default:
			break;
	}
//    glutPostRedisplay();
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 500, 500);  // Define o SRD

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow    ( "Primeiro Programa em OpenGL" );

    // executa algumas inicializações
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // será chamada automaticamente quando
    // for necessário redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalida‹o da tela. A funcao "display"
    // será chamada automaticamente sempre que a
    // m‡quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // será chamada automaticamente quando
    // o usuário alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // será chamada automaticamente sempre
    // o usuário pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" será chamada
    // automaticamente sempre o usuário
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
