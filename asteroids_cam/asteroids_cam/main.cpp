#include <stdlib.h>
#include <stdio.h>
#ifdef LINUX
#include <GL/glfw.h>
#else
#include <gl/glfw.h>
#endif
#include <math.h>
#include "Vectors.h"

class object// are there class prototypes?
{
	public:
		Vector3 position;
		Vector3 velocity;
		Rot_Vector facing;
};

class Poly
{
	public: Vector3 vertices[4]; Vector3 center; int num_v; Color color;
			Vector3 get_center (void) {	return center; }
			Color get_color (void) {	return color; }
			void rotate_this (Rot_Vector spin, Vector3 rot_center)
			{
				for (int i = 0; i < num_v; i++)
				{
					vertices[i].rotate_this( spin, rot_center );
				}
			}

			void operator= (Poly init)
			{
				this->num_v = init.num_v;
				for (int i = 0; i < init.num_v; i++)
					this->vertices[i] = init.vertices[i];
				this->color = init.color;
				this->center = init.center;
			}
};

const float alpha[3] = {0.0f,0.5f,1.0f};
const Color white = {1.0,1.0,1.0}, 
	  red =     {1.0,0.0,0.0}, 
	  green =  {0.0,1.0,0.0}, 
	  blue = {0.0,0.0,1.0},
	  black = {0.0,0.0,0.0}, 
	  magenta = {1.0,0.0,1.0}, 
	  yellow = {1.0,1.0,0.0}, 
	  teal = {0.0,1.0,1.0};

void GLFWCALL handleResize(int w,int h)
{	
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );					//Switch to setting the camera perspective
	glLoadIdentity();								//reset the camera
	gluPerspective( 45, ( (float)w / (float)h ), 1, 100);
	return;
}

void display (Poly to_draw[], int num_p, object camera)
{
	glClearColor( white.r,white.g,white.b, 0.0f ); //clears background screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW ); //switch to setting the draw frame
	for (int p = 0; p < num_p; p++)
	{
		glColor3f( to_draw[p].color.r, to_draw[p].color.g, to_draw[p].color.b );
		glBegin(GL_POLYGON);
		for (int i = 0; i < to_draw[p].num_v; i++)
		{
			glVertex3f( to_draw[p].vertices[i].oriented_to(camera.position, camera.facing).getx(), 
					to_draw[p].vertices[i].oriented_to(camera.position, camera.facing).gety(),
					to_draw[p].vertices[i].oriented_to(camera.position, camera.facing).getz() );
		}
		glEnd();
	}
	glfwSwapBuffers();
}

void control (object *camera)//currently movement rotation is correct, but not view rotation
{	
	Rot_Vector add_facing; add_facing.ab_x = 0; add_facing.ab_y = 0; add_facing.ab_z = 0; 
	if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS && glfwGetKey( GLFW_KEY_DOWN ) != GLFW_PRESS )
	{
		add_facing.ab_x = 0.001f;
	}
	else if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS && glfwGetKey(GLFW_KEY_UP) != GLFW_PRESS)
	{
		add_facing.ab_x = -0.001f;
	}
	if (glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS && glfwGetKey( GLFW_KEY_RIGHT ) != GLFW_PRESS)
	{
		add_facing.ab_y = -0.001f;
	}
	else if (glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS && glfwGetKey( GLFW_KEY_LEFT ) != GLFW_PRESS)
	{
		add_facing.ab_y += 0.001f;
	}
	camera->facing += add_facing;
	Vector3 w,s,a,d;
	w.init(    0, 0,-0.02 );//signs might not be right, check later
	s.init(    0, 0, 0.02 );
	a.init(-0.02, 0, 0	  );
	d.init( 0.02, 0, 0	  );
	Vector3 null; null.init(0,0,0);
	Vector3 vi = camera->velocity;
	camera->velocity = null;
	if (glfwGetKey('W') == GLFW_PRESS && vi == null) {
		camera->velocity += w; 
	}//if rotated around camera position, would have to translate wasd vector first
	else if (glfwGetKey('S') == GLFW_PRESS && vi == null) {
		camera->velocity += s;
	}//clearly currently acts as if this says '=' instead of '+='. Is '+=' not working????
	if (glfwGetKey('A') == GLFW_PRESS && vi == null) {
		camera->velocity += a;
	}//yeah, really not working!!!
	else if (glfwGetKey('D') == GLFW_PRESS && vi == null) 	{	
		camera->velocity += d;
	}
	camera->position += camera->velocity.oriented_to(camera->position, camera->facing);//currently in rot, 
}

int main (int argc, char *argv[])
{
	int running = GL_TRUE, height = 0, width = 0;
	Poly to_draw[2];
	Vector3 center;
	object camera;

	camera.position.init(0,0,0);
	camera.facing.ab_x = 0;
	camera.facing.ab_y = 0;
	camera.facing.ab_z = 0;

	float sidelen = 1.0; int i = 0, v = 0;
	center.init(0,0,-5);
	to_draw[i].color = green;
	to_draw[i].num_v = 4;
	to_draw[i].center.init( 0, 0,-5 );
	to_draw[i].vertices[v++].init( center.getx() + sidelen, center.gety() + sidelen, center.getz() );
	to_draw[i].vertices[v++].init( center.getx() + sidelen, center.gety() - sidelen, center.getz() );
	to_draw[i].vertices[v++].init( center.getx() - sidelen, center.gety() - sidelen, center.getz() );
	to_draw[i].vertices[v  ].init( center.getx() - sidelen, center.gety() + sidelen, center.getz() );

	sidelen = 0.5; i = 1; v = 0; center.init(0,0,-4);
	to_draw[i].color = red;
	to_draw[i].num_v = 4;
	to_draw[i].center.init( 0, 0,-4 );
	to_draw[i].vertices[v++].init( center.getx() + sidelen,  center.gety() + sidelen, center.getz() );
	to_draw[i].vertices[v++].init( center.getx() + sidelen,  center.gety() - sidelen, center.getz() );
	to_draw[i].vertices[v++].init( center.getx() - sidelen,  center.gety() - sidelen, center.getz() );
	to_draw[i].vertices[v  ].init( center.getx() - sidelen,  center.gety() + sidelen, center.getz() );

	glfwInit();
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
	if(!glfwOpenWindow(1920/1.5, 1080/1.5, 0,0,0,0,0,0, GLFW_WINDOW))	{
		glfwTerminate();
		return 0;
	}
	glfwSetWindowSizeCallback(handleResize);
	while (running)
	{
		control( &camera );
		display( to_draw, 2, camera );
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}
	glfwCloseWindow();
	glfwTerminate();
	return 0;
}

