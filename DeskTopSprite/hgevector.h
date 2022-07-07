/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeVector helper class
*/


#ifndef HGEVECTOR_H
#define HGEVECTOR_H


#include <math.h>


/*
** Fast 1.0/sqrtf(float) routine
*/
inline
float InvSqrt(float x)
{
	union
	{
		int intPart;
		float floatPart;
	} convertor;

	convertor.floatPart = x;
	convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);
	return convertor.floatPart*(1.5f - 0.4999f*x*convertor.floatPart*convertor.floatPart);
}

class hgeVector
{
public:
	float	x,y;

	hgeVector(float _x, float _y)	{ x=_x; y=_y; }
	hgeVector()						{ x=0; y=0; }

	hgeVector	operator-  ()					const { return hgeVector(-x, -y);		}
	hgeVector	operator-  (const hgeVector &v) const { return hgeVector(x-v.x, y-v.y); }
	hgeVector	operator+  (const hgeVector &v) const { return hgeVector(x+v.x, y+v.y); }
	hgeVector&	operator-= (const hgeVector &v)		  { x-=v.x; y-=v.y; return *this;	}
	hgeVector&	operator+= (const hgeVector &v)		  { x+=v.x; y+=v.y; return *this;	}
	bool		operator== (const hgeVector &v)	const { return (x==v.x && y==v.y);		}
	bool		operator!= (const hgeVector &v)	const { return (x!=v.x || y!=v.y);		}

	hgeVector	operator/  (const float scalar)	const { return hgeVector(x/scalar, y/scalar); }
	hgeVector	operator*  (const float scalar) const { return hgeVector(x*scalar, y*scalar); }
	hgeVector&	operator*= (const float scalar)		  { x*=scalar; y*=scalar; return *this;   }

	float		Dot(const hgeVector *v) const { return x*v->x + y*v->y; }
	float		Length() const { return sqrtf(Dot(this)); }
	float		Angle(const hgeVector *v = 0) const
	{
		if(v)
		{
			hgeVector s=*this, t=*v;

			s.Normalize(); t.Normalize();
			return acosf(s.Dot(&t));
		}
		else return atan2f(y, x);
	}
	
	void		Clamp(const float max) { if(Length() > max)	{ Normalize(); x *= max; y *= max; } }
	hgeVector*	Normalize() { float rc=InvSqrt(Dot(this)); x*=rc; y*=rc; return this; }
	hgeVector*	Rotate(float a)
	{
		hgeVector v;

		v.x=x*cosf(a) - y*sinf(a);
		v.y=x*sinf(a) + y*cosf(a);

		x=v.x; y=v.y;

		return this;
	}
};

inline hgeVector operator* (const float s, const hgeVector &v)		{ return v*s; }
inline float	 operator^ (const hgeVector &v, const hgeVector &u) { return v.Angle(&u); }
inline float	 operator% (const hgeVector &v, const hgeVector &u) { return v.Dot(&u); }


#endif
