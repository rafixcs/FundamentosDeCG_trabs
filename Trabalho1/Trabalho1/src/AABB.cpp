#include "AABB.h"
#include "CollisionChecker.h"

AABB::AABB()
{
	this->width = 0.f;
	this->height = 0.f;
	this->_bounding_box = std::make_unique<Poligono>();
}

AABB::AABB(float x1, float y1, float x2, float y2)
{
	float sign_w = 0;
	float sign_h = 0;

	this->p1_bbx.x = x1;
	this->p1_bbx.y = y1;
	this->p1_bbx.z = 0.f;

	this->p2_bbx.x = x2;
	this->p2_bbx.y = y2;
	this->p2_bbx.z = 0.f;

	sign_w = (x1 < x2) ? 1.f : -1.f;
	sign_h = (y1 < y2) ? 1.f : -1.f;
	
	this->width = abs(x1 - x2) * sign_w;
	this->height = abs(y1 - y2) * sign_h;

	this->center.x = x1 + (this->width / 2.f);
	this->center.y = y1 + (this->height / 2.f);
	this->midLen.x = abs(this->width / 2.f);
	this->midLen.y = abs(this->height / 2.f);
	
	this->ResetPoligon();
}

AABB::~AABB()
{
	// Do nothing
}

void AABB::DrawBox()
{
	this->_bounding_box->desenhaPoligono();
}

void AABB::SetAABB(Ponto A, Ponto B)
{

	if (A == this->p1_bbx && B == this->p2_bbx || A == this->p2_bbx && B == this->p1_bbx)
	{
		return;
	}
	else
	{
		float sign_w = 0;
		float sign_h = 0;

		this->p1_bbx = A;
		this->p2_bbx = B;

		sign_w = (A.x < B.x) ? 1.f : -1.f;
		sign_h = (A.y < B.y) ? 1.f : -1.f;

		this->width = (A.x - B.x) * sign_w;
		this->height = (A.y - B.y) * sign_h;

		this->ResetPoligon();		
	}
}

void AABB::ResetPoligon()
{
	Ponto fx;
	Ponto iy;

	fx.x = this->p1_bbx.x;
	fx.y = this->p1_bbx.y + this->height;
	iy.x = this->p1_bbx.x + this->width;
	iy.y = this->p1_bbx.y;

	this->_bounding_box.reset(new Poligono());
	this->_bounding_box->insereVertice(this->p1_bbx);
	this->_bounding_box->insereVertice(fx);
	this->_bounding_box->insereVertice(this->p2_bbx);
	this->_bounding_box->insereVertice(iy);

}
