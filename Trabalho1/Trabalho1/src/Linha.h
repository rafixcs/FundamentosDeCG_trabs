#ifndef __LINHA_H__
#define __LINHA_H__

#ifdef WIN32
    #include <windows.h>

    #ifdef CODEBLOCKS
        #include <glut.h>
    #else
        #include <GL/freeglut.h>
    #endif // CODEBLOCKS
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

//#include "Ponto.h"
#include "AABB.h"
#include <memory>

class Linha
{
public:
	//std::unique_ptr<AABB> aabb;
	AABB * aabb;

	float x1;
	float y1;
	float x2;
	float y2;

	Linha();
	~Linha();

    void geraLinha(int limite, int TamMax);
	void desenhaLinha();
};

#endif // !__LINHA_H__
