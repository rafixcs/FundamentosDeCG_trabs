

#include <cstdlib>

#include "Linha.h"
#include "AABB.h"

Linha::Linha()
{
    //this->aabb.reset();
    this->aabb = nullptr;
    this->x1 = 0;
    this->y1 = 0;
    this->x2 = 0;
    this->y2 = 0;
}

Linha::~Linha()
{
    delete this->aabb;
}

void Linha::geraLinha(int limite, int TamMax)
{
    float deltaX,deltaY;
    int sign_w = 0;
    int sign_h = 0;

    x1 = (rand() % limite*10)/10.0;
    y1 = (rand() % limite*10)/10.0;

    deltaX = (rand() % limite)/(float)limite;
    deltaY = (rand() % limite)/(float)limite;

    if (rand() % 2)
    {
        x2 = x1 + deltaX * TamMax;
        x2 = (x2 > 650.f) ? 650.f : x2;
    }
    else
    {
        x2 = x1 - deltaX * TamMax;
        x2 = (x2 < 0.f) ? 0.f : x2;
    }

    if (rand() % 2)
    {
        y2 = y1 + deltaY * TamMax;
        y2 = (y2 > 500.f) ? 500.f : y2;
    }
    else
    {
        y2 = y1 - deltaY * TamMax;
        y2 = (y2 < 0.f) ? 0.f : y2;
    }

    //this->aabb.reset(new AABB(x1, y1, x2, y2));
    if (this->aabb != nullptr)
        delete this->aabb;
    this->aabb = new AABB(x1, y1, x2, y2);
}


void Linha::desenhaLinha()
{
	glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
	glEnd();
}
