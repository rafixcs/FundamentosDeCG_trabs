// **********************************************************************
// PUCRS/FACIN
// COMPUTAÇÃO GRÁFICA
//
// Teste de colisão em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@inf.pucrs.br
// **********************************************************************
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>

using namespace std;

#ifdef WIN32
    #include <windows.h>
    #ifdef CODEBLOCKS
        #include <glut.h>
    #else
        #include <GL/freeglut.h>
    #endif // CODEBLOCKS
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
#include "Linha.h"
#include "AABB.h"
#include "CollisionChecker.h"
#include "Temporizador.h"
#include "BlockSpasce.h"


const int N_LINHAS = 50;
const int TAM_MAX = 10;
const int MAX_X = 100;  // Coordenada X maxima da janela

long int ContadorInt = 0;
long int ContChamadas = 0;
long int ContBBxHit = 0;

bool MOSTRA_BBX = false;
bool BBX_TEST = true;
bool SECTIONS_TEST = true;
bool RESTART = false;
bool DEBUGLINESBLOCK = false;

int NBlocksSpaces = 34;
int blockIndex = 0;
int teste = 10;

Linha Linhas[N_LINHAS];
BlockSpace blocksSpaces;

// **********************************************************************
//  void init(void)
//  Inicializa os parâmetros globais de OpenGL
// **********************************************************************
void init(void)
{
    blocksSpaces.Init(100.f, 100.f, NBlocksSpaces);

    // Define a cor do fundo da tela (BRANCO)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    srand(unsigned(time(NULL)));

    for (int i = 0; i < N_LINHAS; i++)
    {
        Linhas[i].geraLinha(MAX_X, TAM_MAX);
        blocksSpaces.InsertLine(Linhas[i], i);
    }

    // Use as a breakpoint to verify the object data
    // before running the whole program
    //blocksSpaces;
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
    // Define os limites lógicos da área OpenGL dentro da Janela
    glOrtho(0,100,0,100,0,1);

    // Define a área a ser ocupada pela área OpenGL dentro da Janela
    glViewport(0, 0, w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

/* ********************************************************************** */
/*                                                                        */
/*  Calcula a interseccao entre 2 retas (no plano "XY" Z = 0)             */
/*                                                                        */
/* k : ponto inicial da reta 1                                            */
/* l : ponto final da reta 1                                              */
/* m : ponto inicial da reta 2                                            */
/* n : ponto final da reta 2                                              */
/*                                                                        */
/* s: valor do parâmetro no ponto de interseção (sobre a reta KL)         */
/* t: valor do parâmetro no ponto de interseção (sobre a reta MN)         */
/*                                                                        */
/* ********************************************************************** */
int intersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t)
{
    double det;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0)
        return 0 ; // não há intersecção

    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;

    return 1; // há intersecção
}

// **********************************************************************
//
// **********************************************************************
bool HaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n)
{
    int ret;
    double s,t;
    ret = intersec2d( k,  l,  m,  n, s, t);
    if (!ret) return false;
    if (s>=0.0 && s <=1.0 && t>=0.0 && t<=1.0)
        return true;
    else return false;

}
// **********************************************************************
//
// **********************************************************************
void DesenhaLinhas()
{
    glColor3f(0,1,0);
    for(int i=0; i< N_LINHAS; i++)
        Linhas[i].desenhaLinha();
}

// **********************************************************************
// void DesenhaCenario()
// **********************************************************************
void DesenhaCenario()
{
    Ponto PA, PB, PC, PD;
    ContChamadas = ContadorInt = ContBBxHit = 0;

    // Desenha as linhas do cenário
    glLineWidth(1);
    glColor3f(1,0,0);

    if (BBX_TEST)
    {
        for(int i=0; i< N_LINHAS; i++)
        {
            for(int j=i+1; j< N_LINHAS; j++)
            {
                ContChamadas++;
                if (CollisionChecker::CheckCollisionBoxes(Linhas[i].aabb, Linhas[j].aabb))
                {
                    ContBBxHit++;

                    PA.set(Linhas[i].x1, Linhas[i].y1);
                    PB.set(Linhas[i].x2, Linhas[i].y2);
                    PC.set(Linhas[j].x1, Linhas[j].y1);
                    PD.set(Linhas[j].x2, Linhas[j].y2);

                    if (HaInterseccao(PA, PB, PC, PD))
                    {
                        ContadorInt++;
                        Linhas[i].desenhaLinha();
                        Linhas[j].desenhaLinha();
                    }
                }

            }

            if (MOSTRA_BBX)
            {
                glColor3f(0.f, 0.f, 0.f);
                Linhas[i].aabb->DrawBox();
                glColor3f(1, 0, 0);
            }
        }
    }
    else if (SECTIONS_TEST)
    {
        int teste_a = 0;
        int teste_b = 0;
        for (auto lines_indexes : blocksSpaces.GetAllBlocks())
        {
            for (int i = 0; i < lines_indexes.size(); i++)
            {
                teste_a = lines_indexes[i];
                for (int j = i+1; j < lines_indexes.size(); j++)
                {
                    ContChamadas++;
                    teste_b = lines_indexes[j];
                    if (CollisionChecker::CheckCollisionBoxes(Linhas[teste_a].aabb, Linhas[teste_b].aabb))
                    {
                        ContBBxHit++;

                        PA.set(Linhas[teste_a].x1, Linhas[teste_a].y1);
                        PB.set(Linhas[teste_a].x2, Linhas[teste_a].y2);
                        PC.set(Linhas[teste_b].x1, Linhas[teste_b].y1);
                        PD.set(Linhas[teste_b].x2, Linhas[teste_b].y2);


                        //Linhas[teste_a].aabb->DrawBox();
                        //Linhas[teste_b].aabb->DrawBox();

                        if (HaInterseccao(PA, PB, PC, PD))
                        {
                            ContadorInt++;
                            Linhas[teste_a].desenhaLinha();
                            Linhas[teste_b].desenhaLinha();
                        }
                    }
                }
            }
            if (MOSTRA_BBX)
            {
                glColor4f(0.f, 0.f, 0.f, 0.5f);
                blocksSpaces.DrawSections();
                glColor3f(1, 0, 0);

                if (DEBUGLINESBLOCK)
                {
                    std::vector<int> lines_indexes = blocksSpaces.GetLinesIndexBySection(blockIndex);
                    for (auto a : lines_indexes)
                    {
                        Linhas[a].desenhaLinha();
                        Linhas[a].aabb->DrawBox();
                    }
                }
            }
        }
    }
}
// **********************************************************************
//  void display( void )
// **********************************************************************
void display( void )
{
    // Limpa a tela com  a cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    DesenhaLinhas();
    DesenhaCenario();

    glutSwapBuffers();

    if (RESTART)
    {
        init();
        RESTART = false;
    }

}

double nFrames=0;
double TempoTotal=0;
Temporizador T;
double AccumDeltaT=0;
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

    if (AccumDeltaT > 1.0/30) // fixa a atualização da tela em 30
    {
        AccumDeltaT = 0;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
        cout << "Contador de Intersecoes Existentes:" << ContadorInt/2.0 << endl;
        cout << "Contador de Chamadas:" << ContChamadas << endl;
        cout << "Contador de BoundingBox hit:" << ContBBxHit << endl;
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
    case' ':
            init();
        break;
    case 'b':
        MOSTRA_BBX = !MOSTRA_BBX;
        if (MOSTRA_BBX)
            std::cout << "Ativando modo debug de bounding box" << std::endl;
        else std::cout << "Desativando modo debug de bounding box" << std::endl;
        break;
    case 'c':
        BBX_TEST = !BBX_TEST;
        if (BBX_TEST)
            std::cout << "Ativando modo de teste de bounding box" << std::endl;
        else std::cout << "Desativando modo de teste de bounding box" << std::endl;
        break;
    case 'f':
        std::cout << "\nBoundingBox test: " << BBX_TEST << std::endl;
        std::cout << "Sections test: " << !BBX_TEST << std::endl;
        std::cout << std::endl;
        break;
    case 'g':
        std::cout << "Insert the value of the number of section:" << std::endl;
        std::cin >> NBlocksSpaces;
        while (NBlocksSpaces < 2 || NBlocksSpaces > 50)
        {
            std::cout << "Invalid value. Try again..." << std::endl;
            std::cin >> NBlocksSpaces;
        }
        RESTART = true;
        break;
    case 'd':
        if (!DEBUGLINESBLOCK)
        {
            DEBUGLINESBLOCK = true;

            std::cout << "Insert the block index: ";

            std::cin >> blockIndex;
            while (blockIndex < 0 || blockIndex > NBlocksSpaces)
            {
                std::cout << "Invalid index value. Try again..." << std::endl;
                std::cin >> blockIndex;
            }
        }
        else
        {
            std::cout << ">> DEBUGLINESBLOCK disable <<" << std::endl;
            DEBUGLINESBLOCK = false;
        }
    default:
        break;
    }
    glutPostRedisplay();
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

        break;
    case GLUT_KEY_DOWN:     // Se pressionar DOWN

        break;
    case GLUT_KEY_LEFT:       // Se pressionar LEFT

        break;
    case GLUT_KEY_RIGHT:     // Se pressionar RIGHT

        break;
    default:
        break;
    }
    glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
    glutPostRedisplay();
}
void mouseMove(int x, int y)
{
    glutPostRedisplay();
}
// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    cout << "ARGC: " << argc << endl;
    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 650, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de título da janela.
    glutCreateWindow    ( "Algorimos de Calculo de Colisao" );

    // executa algumas inicializações
    init ();


    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // será chamada automaticamente quando
    // for necessário redesenhar a janela
    glutDisplayFunc ( display );
    glutIdleFunc ( animate );

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
    //glutMouseFunc(mouse);
    //glutMotionFunc(mouseMove);

    // inicia o tratamento dos eventos
    glutMainLoop ( );

    return 0;
}
