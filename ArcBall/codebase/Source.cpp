#include <iostream>
#include <strstream>
#include <algorithm>

//link OpenGL libraries
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "ObjModel.h"

// Initial size of graphics window.
const int WIDTH = 800;
const int HEIGHT = 600;

// Current size of window.
int width = WIDTH;
int height = HEIGHT;

ObjModel* mesh = 0;
using namespace std;

// Toggle display mode (fill or wireframe)
short displayMode = 0;

// Mouse Coordinates
int prev_mx = 0, prev_my = 0;
int curr_mx = 0, curr_my = 0;

// Mouse motion
float x_motion = 0;
float y_motion = 0;
float z_motion = -2;

// Rotation values
float rotAngle = 0;
TVector rotAxis;
TMatrix4x4 track_matrix;

// Picking
#define PICK_TOL 15
#define PICK_BUFFER_SIZE 256
int selectedPoint;
unsigned int pickBuffer[PICK_BUFFER_SIZE];

// States
int translate = false;
int zoom = false;
int arcball = false;
int picking = false;

// Added function to get arcball vector
TVector getArcballVector(float x, float y) {

	TVector av((x / ((float)width)) * 2 - 1.0,
			   (y / ((float)height)) * 2 - 1.0,
				0.0);
	av[1] = -av[1];
	float op2 = av[0] * av[0] + av[1] * av[1];
	if (op2 <= 1.0) {
		av[2] = sqrt(1.0 - op2);
	}
	else {
		av.normalize();
	}
	return av;
}

void setupMaterial(){
		float diff[4] = {0.3,0.3,0.5, 0}, spec[4]={1, 1, 1, 0};
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
		glMaterialf(GL_FRONT, GL_SHININESS, 125);

		glMaterialfv(GL_BACK, GL_SPECULAR, spec);
		glMaterialfv(GL_BACK, GL_DIFFUSE, diff);
		glMaterialf(GL_BACK, GL_SHININESS, 125);
		// the shininess factor is a number between 0 and 128, 
		// where 0 is the shiniest the object can be

		// Enable offset for rendering both wireframe and solid
		glEnable(GL_POLYGON_OFFSET_FILL);
		
}

void setupLights(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	GLfloat light_position1[] = { 0, 0, 0, 1 };
	GLfloat light1[] = { 1, 1, 1, 1 };
	GLfloat light2[] = { 0.5, 0.5, 0.5, 1.0 };

	//ambient light is reflected environmental light 
	GLfloat ambient[] = { 0.2, 0.2, 0.2, 0 };

	// setup
	glEnable(GL_COLOR_MATERIAL);	// Allows glColor3f() to function
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);	
	glShadeModel(GL_SMOOTH);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light2);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	
	glPopMatrix();
}

// This funciton renders the object once all 'setup' is done in display
void drawObject(int renderMode) {
	if (renderMode == GL_RENDER) {
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < mesh->nfaces.size(); i += 1)
		for (int j = 0; j < 3; j += 1){
			glNormal3f(mesh->normal[mesh->nfaces[i][j]][0],
				mesh->normal[mesh->nfaces[i][j]][1],
				mesh->normal[mesh->nfaces[i][j]][2]);

			glVertex3f(mesh->vertex[mesh->faces[i][j]][0],
				mesh->vertex[mesh->faces[i][j]][1],
				mesh->vertex[mesh->faces[i][j]][2]);
		}
		glEnd();
	} else if (renderMode == GL_SELECT) {
		// Viewport initialization
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport); // From ballAndTaurusPicking.cpp sample code from textbook files

		// Select picking buffer
		glSelectBuffer(PICK_BUFFER_SIZE, pickBuffer);

		// Matrix Mode
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Apply Transformations
		glTranslatef(x_motion, y_motion, z_motion);
		glMultMatrixf(track_matrix.getBuffer());

		// Set Render Mode
		glRenderMode(GL_SELECT);

		// Set Matrix Mode
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		// Set up pick matrix and perspective
		gluPickMatrix(curr_mx, viewport[3] - curr_my, PICK_TOL, PICK_TOL, viewport);
		gluPerspective(40, (float)width / (float)height, 0.1, 1000);

		// Name all the points
		glInitNames();
		glPushName(-1);

		for (int i = 0; i < mesh->vertex.size(); i++){
			glLoadName(i);
			glBegin(GL_POINTS);
				glVertex3f(mesh->vertex[i][0], mesh->vertex[i][1], mesh->vertex[i][2]);
			glEnd();
		}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

}

// This function is called to display the scene.
void display()
{
	//Background color
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Matrix setup
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width, height);
	glLoadIdentity();
	gluPerspective(40, (float)width / (float)height, 0.1, 1000);

	// Matrix setup
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Apply Transformations
	glTranslatef(x_motion, y_motion, z_motion);
	glMultMatrixf(track_matrix.getBuffer());

	// Draw object based on current rendering mode selection
	switch (displayMode)
	{

		// Smooth
	case 0:
		// Set mode to fill, specify color
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0, 0.3, 1);
		drawObject(GL_RENDER);
		break;

		// Smooth + Wireframe
	case 1:
		// Set mode to fill, specify color
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0, 0.3, 1);

		// Set offset to avoid overlap of both modes
		glPolygonOffset(1, 1);

		// Draw smooth object
		drawObject(GL_RENDER);

		// Toggle wireframe mode, specify color
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0, 1, 0);

		// Draw wireframe over smooth
		drawObject(GL_RENDER);
		break;

		// Wireframe
	case 2:
		// Toggle wireframe mode, specify color
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0, 1, 0);

		// Draw wireframe
		drawObject(GL_RENDER);
		break;
	}
	glutSwapBuffers();
}

// This function is called when there is nothing else to do.
void idle()
{
	//glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

	case('s'):
	case('S'):
		displayMode = (displayMode + 1) % 3;	// Toggle between 3 modes
		cout << "Rendering mode: ";
		switch (displayMode)
		{
		case 0:
			cout << "smooth";
			break;
		case 1:
			cout << "smooth + wireframe";
			break;
		case 2:
			cout << "wireframe";
			break;
		default:
			cout << "how did you do this?";
			break;
		}
		cout << endl;
		break;
	case(27) :
		exit(0);
		break;
	}
	glutPostRedisplay();		// Redisplay the object
}

void specialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		//do something
		break;
	case GLUT_KEY_LEFT:
		//do something
		cout << "left key" << endl;
		break;
	}
	glutPostRedisplay();		// Redraw the scene
}

void mouse_button(int button, int state, int x, int y)
{
	// Assign mouse coordinates
	prev_mx = curr_mx = x;
	prev_my = curr_my = y;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){ // Translation
		translate = true;
	} else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){ // Zooming
		zoom = true;
	} else if ((glutGetModifiers() & GLUT_ACTIVE_CTRL) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){ // Picking
		picking = true;
		drawObject(GL_SELECT); // "Fake" render the object for picking

		// Declare variables for hit-size and z-coords
		int numHits(0), numItems(0), point;
		unsigned int z_max, z_min;

		// Check that you hit a point (or series of points)
		numHits = glRenderMode(GL_RENDER);
		

		int index = 0;
		numItems = pickBuffer[index++];
		z_min = pickBuffer[index++];
		z_max = pickBuffer[index++];
		
		if (numHits == 0) {
			selectedPoint = -1; // "We don't want previous pick's point to be used
		} else {
			selectedPoint = pickBuffer[index++];
			for (int i = 1; i < numItems; i++) {
				point = pickBuffer[index++];
				// Select closest point based on z-values
				if (mesh->vertex[point][2] == z_min)
					selectedPoint = point;
			}
		}
		cout << "Picking: Selected Point = " << selectedPoint << endl;

	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){ // Arcball rotation
		arcball = true;
	} else {
		translate = false;
		zoom = false;
		picking = false;
		arcball = false;
	}
	glutPostRedisplay();
}

void mouse_motion(int x, int y)
{
	curr_mx = x;
	curr_my = y;
	if (translate){
		// Register motion on x-y plane and normalize
		x_motion += (curr_mx - prev_mx) / (0.5*(float)width);
		y_motion += (prev_my - curr_my) / (0.5*(float)height);

		cout << "Translate( x: " << x_motion << " y: " << y_motion << " )" << endl;
	} 
	
	if (zoom){
		// Apply motion detected on y-axis to z-axis to perform zoom
		z_motion += 3*(prev_my - curr_my) / ((float)height);

		cout << "Zoom( z: " << z_motion << " )" << endl;
	}

	if (picking && (selectedPoint > 0)){
		TVector pickMotion(	2*(curr_mx - prev_mx)/((float)width),
							2*(prev_my - curr_my)/((float)height),
							0.0);

		cout << "Moving selected point..." << endl;

		// The new x and y coords of selected vertex are the current x and y of mouse
		mesh->vertex[selectedPoint] = mesh->vertex[selectedPoint] + pickMotion;

	}

	if (arcball){
		if (curr_mx != prev_mx || curr_my != prev_my){
			// Get vectors before and after mouse move
			TVector a = getArcballVector(prev_mx, prev_my);
			TVector b = getArcballVector(curr_mx, curr_my);
			
			// Get angle of rotation
			rotAngle = acos(min(1.0, (double)a.dot(b))) * 180.0 / (3.141592654); // Want degrees, not radians
			
			// Get rotation axis
			rotAxis = a*b; // Cross product

			cout << "Rotate( " << rotAngle << " deg )" << endl;

			// ARCBALL ROTATION
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glRotatef(4*rotAngle, rotAxis[0], rotAxis[1], rotAxis[2]);

			GLdouble M[16];
			glGetDoublev(GL_MODELVIEW_MATRIX, M);
			TMatrix4x4 tm;
			for (int i = 0; i < 16; ++i)
				tm[i] = M[i];

			track_matrix = tm * track_matrix;
			glPopMatrix();
		}

	}
	
	glutPostRedisplay();
	prev_mx = x;
	prev_my = y;
}

// Respond to window resizing, preserving proportions.
// Parameters give new window size in pixels.
void reshapeMainWindow(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	glutPostRedisplay();
}


void initialize(){
	//loads object
	mesh = new ObjModel();
	Load(*mesh, "cheb.obj");
	mesh->CenterObject();

	setupMaterial();
	setupLights();
}


void main(int argc, char **argv)
{
	

	// GLUT initialization.
	glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("CodeBase");
	
	// Register call backs.
	initialize();

	glutDisplayFunc(display);
	glutReshapeFunc(reshapeMainWindow);
	glutMotionFunc(mouse_motion);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse_button);
	
	// Enter GLUT loop.
	glutMainLoop();

	delete mesh;
}
