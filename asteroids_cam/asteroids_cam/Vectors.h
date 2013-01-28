#ifndef OG_VECTORS_H
#define OG_VECTORS_H

//class three_float?
class Color
{	public:
	float r, g, b;
	void operator= (Color initial)
	{	
		this->r = initial.r;
		this->g = initial.g;
		this->b = initial.b;
		return; 
	}
};

class Rot_Vector
{
	public:
		float ab_x, ab_y, ab_z;
		void operator= (Rot_Vector initial)
		{
			this->ab_x = initial.ab_x;
			this->ab_y = initial.ab_y;
			this->ab_z = initial.ab_z;
			return; 
		}
		void operator+= (Rot_Vector beta)
		{
			this->ab_x += beta.ab_x;
			this->ab_y += beta.ab_y;
			this->ab_z += beta.ab_z;
			return;
		}
};

class Euler_Angle { public: float theta, phi; };//faster than axis-based rot_vectors?

/*
 * could make this basically a linklist so as to easily step through the base vectors to find "real vector"
 * for instance, have (the relationship of a vertex to it's obj) as one vector with (the obj's relationship to the origin) as it's base vector,
 * when doing hard coordinate calculations, they could be added up through a function that returns the "pure" vector
 * would this idea make rotation calulations easier? (partial answer: would eliminate requirement for translation to origin before hard body rotation, internal origin used instead)
 * This could make newtonian-system (think solar or planetary system) branching easier as well
 * would certainly require a formal rotation matrix function (still need to find one that isnt for Euler angles)
 * a vector class variable might be a good idea as well (think coordinate vs velocity vs acceleration)
 */
class Vector3
{	
	protected:
		float x, y, z;
	public:
		float getx (void) {	return x; }
		float gety (void) {	return y; }
		float getz (void) {	return z; }
		void init (float x, float y, float z)
		{	this->x = x; this->y = y; this->z = z; }	
		void operator= (Vector3 initial)
		{	this->init( initial.getx(), initial.gety(), initial.getz() ); return; }
		int operator== (Vector3 b)
		{	return b.getx() == this->getx() && b.gety() == this->gety() && b.getz() == this->getz(); }
		void operator+= (Vector3 delta)
		{
			this->init( this->getx() + delta.getx(),
				this->gety() + delta.gety(),
				this->getz() + delta.getz() );
		return;
		}
		Vector3 operator+ (Vector3 beta)
		{
			Vector3 gamma;
			gamma.init ( beta.x + this->x, beta.y + this->y, beta.z + this->z );
			return gamma;
		}
		Vector3 operator- (Vector3 beta)
		{
			Vector3 gamma;
			gamma.init ( this->x - beta.x, this->y - beta.y, this->z - beta.z );
			return gamma;
		}
		Vector3 operator* (float scalar)
		{
			Vector3 gamma;
			gamma.init ( scalar * this->x, scalar * this->y, scalar * this->z );
			return gamma;
		}
		Vector3 cross_product (Vector3 b)
		{
			Vector3 c;
			c.init ( this->gety() * b.getz() - this->getz() * b.gety(),
					this->getz() * b.getx() - this->getx() * b.getz(),
					this->getx() * b.gety() - this->gety() * b.getx() );
			return c;
		}
		float dot_product (Vector3 b)
		{
			float c;
			c = x * b.getx() + y * b.gety() + z * b.getz();
			return c;
		}
		void rotate_this (Rot_Vector spin, Vector3 rot_center)//~_this changes the contents, ~d returns transformed without changing contents
		{
			*this += (rot_center * -1); //'this' transformed to be the coordinates of 'this', relative to rot_center
			this->init(this->getx(), this->y*cosf( spin.ab_x ) - this->z*sinf( spin.ab_x ), this->z*cosf( spin.ab_x ) + this->y*sinf( spin.ab_x ));// 2-D transformation of y and z (about the x axis)
			this->init(this->x*cosf( spin.ab_y ) - this->z*sinf( spin.ab_y ), this->gety(), this->z*cosf( spin.ab_y ) + this->x*sinf( spin.ab_y ));// changes x and z, about y
			this->init(this->x*cosf( spin.ab_z ) - this->y*sinf( spin.ab_z ), this->y*cosf( spin.ab_z ) + this->x*sinf( spin.ab_z ), this->getz());// changes x and y, about z
			*this += rot_center;
			return;
		}
		void orient_to (Vector3 center, Rot_Vector offset)
		{
			*this = (*this + center * -1).rotated(offset, center); 
		}
		Vector3 oriented_to (Vector3 center, Rot_Vector offset)
		{
			return (*this + center * -1).rotated(offset, center);
		}
		Vector3 rotated (Rot_Vector spin, Vector3 rot_center)//are these functions mathematically correct?
		{
			Vector3 f = *this; //f needs to be the coordinates of 'this', relative to rot_center
			f.init( f.getx(),
					f.y*cosf( spin.ab_x ) + f.z*sinf( spin.ab_x ),//unsure if the signs are correct
					f.z*cosf( spin.ab_x ) - f.y*sinf( spin.ab_x ));
			// 2-D transformation of y and z (about the x-axis)
			f.init( f.x*cosf( spin.ab_y ) + f.z*sinf( spin.ab_y ),
					f.gety(),
					f.z*cosf( spin.ab_y ) - f.x*sinf( spin.ab_y ));
			// changes x and z, about y-axis
			f.init( f.x*cosf( spin.ab_z ) + f.y*sinf( spin.ab_z ),
					f.y*cosf( spin.ab_z ) - f.x*sinf( spin.ab_z ),
					f.getz());// changes x and y, about z-axis
			f += rot_center;// by subtracting before and re-adding after, the rotation should be about the "new" origin, rot_center
			return f;
		}
};



#endif
