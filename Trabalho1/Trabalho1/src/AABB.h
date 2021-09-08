#ifndef AABB_H
#define AABB_H

#include <memory>

#include "Ponto.h"
#include "Poligono.h"

class AABB 
{
private:	
	//unique_ptr<Poligono> _bounding_box;
	Poligono _bounding_box;

public:
	Ponto p1_bbx;
	Ponto p2_bbx;
	Ponto center;
	Ponto midLen;
	float width;
	float height;

public:
	AABB();
	AABB(float x1, float y1, float x2, float y2);
	~AABB();
	
	void DrawBox();

	void SetAABB(Ponto A, Ponto B);

private:
	void ResetPoligon();
};

#endif