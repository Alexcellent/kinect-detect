// COMP 477 - Project
// Team Teem
//
// We certify this submission is our original work and meets the Faculty's
// Expectations of Originality
//
// ACKNOWLEDGEMENTS:
// OpenGL aspects of this project use the codebase from both our 371 and 477
// assignments as a foundation, on which we built our needed functionality.
///////////////////////////////////////////////////////////////////////////////

#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>
#include "defMesh.h"
using namespace std;


//Create Mesh Vector
vector<DefMesh*> meshes;
int selectedMesh = -1; // ID fo selected mesh

//Switches
int meshModel = 0;

//Window parameters
int width = 1024;
int height = 768;
///* Ortho (if used) */
double _left = 0.0;		/* ortho view volume params */
double _right = 0.0;
double _bottom = 0.0;
double _top = 0.0;
double _zNear = 0.1;
double _zFar = 50.0;
double fovy = 45.0;
double prev_z = 0;

//Model matrices
double  _matrix[16];
double _matrixI[16];

// Mouse interface
int  _mouseX = 0;
int  _mouseY = 0;
bool _mouseLeft = false;
bool _mouseMiddle = false;
bool _mouseRight = false;

float _dragPosX = 0.0;
float _dragPosY = 0.0;
float _dragPosZ = 0.0;

// Transformation states
bool _translate = false;
bool _rotate    = false;

float prev_x(0), prev_y(0);
float curr_x(0), curr_y(0);

void invertMatrix(const GLdouble * m, GLdouble * out)
{

/* NB. OpenGL Matrices are COLUMN major. */
#define MAT(m,r,c) (m)[(c)*4+(r)]

/* Here's some shorthand converting standard (row,column) to index. */
#define m11 MAT(m,0,0)
#define m12 MAT(m,0,1)
#define m13 MAT(m,0,2)
#define m14 MAT(m,0,3)
#define m21 MAT(m,1,0)
#define m22 MAT(m,1,1)
#define m23 MAT(m,1,2)
#define m24 MAT(m,1,3)
#define m31 MAT(m,2,0)
#define m32 MAT(m,2,1)
#define m33 MAT(m,2,2)
#define m34 MAT(m,2,3)
#define m41 MAT(m,3,0)
#define m42 MAT(m,3,1)
#define m43 MAT(m,3,2)
#define m44 MAT(m,3,3)

    GLdouble det;
    GLdouble d12, d13, d23, d24, d34, d41;
    GLdouble tmp[16];		/* Allow out == in. */

    /* Inverse = adjoint / det. (See linear algebra texts.) */

    /* pre-compute 2x2 dets for last two rows when computing */
    /* cofactors of first two rows. */
    d12 = (m31 * m42 - m41 * m32);
    d13 = (m31 * m43 - m41 * m33);
    d23 = (m32 * m43 - m42 * m33);
    d24 = (m32 * m44 - m42 * m34);
    d34 = (m33 * m44 - m43 * m34);
    d41 = (m34 * m41 - m44 * m31);

    tmp[0] = (m22 * d34 - m23 * d24 + m24 * d23);
    tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
    tmp[2] = (m21 * d24 + m22 * d41 + m24 * d12);
    tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

    /* Compute determinant as early as possible using these cofactors. */
    det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

    /* Run singularity test. */
    if (det == 0.0) {
	/* printf("invert_matrix: Warning: Singular matrix.\n"); */
    /* memcpy(out,_identity,16*sizeof(double)); */
    } else {
	GLdouble invDet = 1.0 / det;
	/* Compute rest of inverse. */
	tmp[0] *= invDet;
	tmp[1] *= invDet;
	tmp[2] *= invDet;
	tmp[3] *= invDet;

	tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
	tmp[5] = (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
	tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
	tmp[7] = (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

	/* Pre-compute 2x2 dets for first two rows when computing */
	/* cofactors of last two rows. */
	d12 = m11 * m22 - m21 * m12;
	d13 = m11 * m23 - m21 * m13;
	d23 = m12 * m23 - m22 * m13;
	d24 = m12 * m24 - m22 * m14;
	d34 = m13 * m24 - m23 * m14;
	d41 = m14 * m21 - m24 * m11;

	tmp[8] = (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
	tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
	tmp[10] = (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
	tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
	tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
	tmp[13] = (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
	tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
	tmp[15] = (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

	memcpy(out, tmp, 16 * sizeof(GLdouble));
    }

#undef m11
#undef m12
#undef m13
#undef m14
#undef m21
#undef m22
#undef m23
#undef m24
#undef m31
#undef m32
#undef m33
#undef m34
#undef m41
#undef m42
#undef m43
#undef m44
#undef MAT
}

void pos(float *px, float *py, float *pz, const int x, const int y,
	 const int *viewport)
{
    /*
       Use the ortho projection and viewport information
       to map from mouse co-ordinates back into world 
       co-ordinates
     */

    *px = (float)(x - viewport[0]) / (float)(viewport[2]);
    *py = (float)(y - viewport[1]) / (float)(viewport[3]);

    *px = _left + (*px) * (_right - _left);
    *py = _top + (*py) * (_bottom - _top);
    *pz = _zNear;
}

// screenToWorld and worldToScreen were based off of similar functions
// available in our comp371 assignment codebases.

void screenToWorld(GLdouble* x, GLdouble* y, GLdouble* z){

    // Get viewport, projection and modelview
    GLint    viewport[4] = { 0, 0, 1, 1 };
    GLdouble modelview[16];
    GLdouble projection[16];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    gluUnProject(*x, *y, *z, modelview, projection, viewport, x, y, z);
}

void worldToScreen(GLdouble* x, GLdouble* y, GLdouble* z){
    
    // Get viewport, projection and modelview
    GLint    viewport[4] = { 0, 0, 1, 1 };
    GLdouble modelview[16];
    GLdouble projection[16];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    gluProject(*x, *y, *z, modelview, projection, viewport, x, y, z);
}

void getMatrix()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, _matrix);
    invertMatrix(_matrix, _matrixI);
}

void init()
{
    //OpenGL initialize functions goes here

    /*glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);

    std::cout<<"Vendor: "<<glGetString(GL_VENDOR)<<std::endl;
    std::cout<<"Version: "<<glGetString(GL_VERSION)<<std::endl;
    std::cout<<"GLSL: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;*/

      //Light values and coordinates
     GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
     GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
     GLfloat lightPos[] = {20.0f, 20.0f, 50.0f};
     glEnable(GL_DEPTH_TEST);
     glFrontFace(GL_CCW);
     //glEnable(GL_CULL_FACE);
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     // Hidden surface removal // Counterclockwise polygons face out // Do not calculate inside of jet // Enable lighting
     glEnable(GL_LIGHTING);
     // Set up and enable light 0
     glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
     glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
     glEnable(GL_LIGHT0);
     // Enable color tracking
     glEnable(GL_COLOR_MATERIAL);
     // Set material properties to follow glColor values
     glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

     glClearColor(0.2f, 0.2f, 0.2f, 3.0f );
    
     //Rescale normals to unit length
     glEnable(GL_NORMALIZE);
     glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

     glShadeModel(GL_FLAT);
     getMatrix(); //Init matrix

     //Translate camera
     glPushMatrix();
     glLoadIdentity();
     glTranslatef(0,0,-5.0);
     glMultMatrixd(_matrix);
     getMatrix();
     glPopMatrix();
}

void changeSize(int w, int h)
{
    //GLfloat aspectRatio;
    //if(h==0)
    //    h = 1;
    //glViewport(0, 0, w, h);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //aspectRatio = (GLfloat)w / (GLfloat)h;
    //gluPerspective(45.0f, aspectRatio, 1.0f, 900.0f);    //using perspective
    //
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    glViewport(0, 0, w, h);


    _top = 1.0;
    _bottom = -1.0;
    _left = -(double) w / (double) h;
    _right = -_left;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /* glOrtho(_left,_right,_bottom,_top,_zNear,_zFar);  Ortho */
    gluPerspective(fovy, (double) w / (double) h, _zNear, _zFar);	/* Perspective for stereo */

    glMatrixMode(GL_MODELVIEW);
}

void timerFunction(int value)       
{
    glutTimerFunc(10,timerFunction,1);
    glutPostRedisplay();
}

float* getArcballVector(float x, float y) 
{
    // From COMP371 Assignment 1
    float* av = new float[3];
    av[0] =  (x / ((float)width )) * 2 - 1.0;
    av[1] =  (y / ((float)height)) * 2 - 1.0;
    av[2] =  0.0;

    av[1] = -av[1];

    float op2 = av[0] * av[0] + av[1] * av[1];
    if (op2 <= 1.0) {
        av[2] = sqrt(1.0 - op2);
    }
    else {
        av = normalize(av);
    }
    return av;
}

// NOT USED //////////////////////////
void checkHoveringStatus(int x, int y)
{
    //int hoveredID = -1;
    //float minDistance = FLT_MAX;
    //for (unsigned i = 0; i < meshes.size(); i++)
    //{
    //    // Reset status
    //    meshes[i]->status = IDLE;
    //    
    //    // Get screen 'radius' for bbox (max of w/h/d for box)
    //    GLdouble center_x = meshes[i]->bbox.xCen, 
    //             center_y = meshes[i]->bbox.yCen,
    //             center_z = meshes[i]->bbox.zCen,
    //             max_x    = meshes[i]->bbox.xMax,
    //             max_y    = meshes[i]->bbox.yMax,
    //             max_z    = meshes[i]->bbox.zMax;
    //    worldToScreen(&center_x, &center_y, &center_z);
    //    worldToScreen(&max_x, &max_y, &max_z);

    //    float screenRadius = sqrt((max_x - center_x)*(max_x - center_x) + 
    //                              (max_y - center_y)*(max_y - center_y) +
    //                              (max_z - center_z)*(max_z - center_z));

    //    // Get distance of mouse from mesh center
    //    float distance = sqrt((x - center_x)*(x - center_x) + 
    //                          (y - center_y)*(y - center_y));
    //    if (distance > screenRadius) continue;
    //    if (center_z < minDistance)
    //    {
    //        minDistance = center_z;
    //        hoveredID = i;
    //    }

    //}
    //if (hoveredID != -1) meshes[hoveredID]->status = HOVERED;
}

void selectMesh(int i)
{
    if (selectedMesh == -1)
        return;

    meshes[i]->status = SELECTED;
}

void releaseMesh(int i)
{
    if (selectedMesh == -1) 
        return;

    meshes[selectedMesh]->status = IDLE;
}

void handleKeyPress(unsigned char key, int x, int y)
{ 
    switch(key)
    {
    case 'q': // PREV
        releaseMesh(selectedMesh);
        
        if (selectedMesh <= 0)
            selectedMesh = meshes.size() - 1;
        else
            selectedMesh--;
        
        selectMesh(selectedMesh);
        break;

    case 'w': // RELEASE ALL
        if (selectedMesh != -1)
            releaseMesh(selectedMesh);
        selectedMesh = -1;
        break;

    case 'e': // NEXT
        releaseMesh(selectedMesh);
        if(selectedMesh == -1)
            selectedMesh = 0;
        else
            selectedMesh = (selectedMesh + 1) % meshes.size();
        
        selectMesh(selectedMesh);
        break;

    case 'p':
        meshModel = (meshModel % 2) + 1;
        break;

    case 'm':
        meshModel = 0;
        break;

    case  27:
        exit(0);
    }
}

void mouseEvent(int button, int state, int x, int y)
{
    int viewport[4];

    _mouseX = x;
    _mouseY = y;
   
    prev_x = curr_x = x;
    prev_y = curr_y = y;

    // BUTTON RELEASE
    if (state == GLUT_UP)
    {
        switch (button) {
        case GLUT_LEFT_BUTTON:
            _rotate = false;
            _mouseLeft = false;
            break;
        case GLUT_MIDDLE_BUTTON:
            _mouseMiddle = false;
            break;
        case GLUT_RIGHT_BUTTON:
            _translate = false;
            _mouseRight = false;
            break;
        }
    }
    // BUTTON PRESS
    else
    {
        switch (button) {
        case GLUT_LEFT_BUTTON:
            _mouseLeft = true;
            if (selectedMesh > -1)
                _rotate = true;
               
            break;
        case GLUT_MIDDLE_BUTTON:
            _mouseMiddle = true;
            break;
        case GLUT_RIGHT_BUTTON:
            _mouseRight = true;
            if (selectedMesh > -1)
                _translate = true;

            break;
        case 4:         //Zoomout
            glLoadIdentity();
            glTranslatef(0, 0, -0.1);
            glMultMatrixd(_matrix);
            getMatrix();
            glutPostRedisplay();
            break;
        case 3:         //Zoomin
            glLoadIdentity();
            glTranslatef(0, 0, 0.1);
            glMultMatrixd(_matrix);
            getMatrix();
            glutPostRedisplay();
            break;
        default:
            break;
        }
    }

    glGetIntegerv(GL_VIEWPORT, viewport);
    pos(&_dragPosX, &_dragPosY, &_dragPosZ, x, y, viewport);
}

void mouseMoveEvent(int x, int y)
{
    const int dx = x - _mouseX;
    const int dy = y - _mouseY;

    curr_x = x;
    curr_y = y;

    bool changed = false;

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (dx == 0 && dy == 0)
        return;

    if (_mouseMiddle || (_mouseLeft && _mouseRight)) {
        /* double s = exp((double)dy*0.01); */
        /* glScalef(s,s,s); */
        /* if(abs(prev_z) <= 1.0) */

        glLoadIdentity();
        glTranslatef(0, 0, dy * 0.01);
        glMultMatrixd(_matrix);

        changed = true;
    } else if (_rotate) { 
        // Get vectors before and after mouse move
        float* a = getArcballVector(prev_x, prev_y);
        float* b = getArcballVector(curr_x, curr_y);

        float  angle = acos(min((float)1.0, dot3(a, b))) *180.0 / (M_PI);
        float* axis  = cross3(a, b);

        // ARCBALL ROTATION
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glTranslatef(meshes[selectedMesh]->bbox.xCen,
                     meshes[selectedMesh]->bbox.yCen,
                     meshes[selectedMesh]->bbox.zCen);
        glRotatef(2 * angle, axis[0], axis[1], axis[2]);
        glTranslatef(-meshes[selectedMesh]->bbox.xCen, 
                     -meshes[selectedMesh]->bbox.yCen, 
                     -meshes[selectedMesh]->bbox.zCen);
        glMultMatrixf(meshes[selectedMesh]->t_matrix);

        
        GLdouble M[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, M);
        float r[16];
        for (int i = 0; i < 16; ++i)
            meshes[selectedMesh]->t_matrix[i] = M[i];

        //mult(meshes[selectedMesh]->t_matrix, r, meshes[selectedMesh]->t_matrix);
        glPopMatrix();

    } else if (_translate) {
        GLdouble p[3] = { prev_x, prev_y, _zNear };
        GLdouble q[3] = { curr_x, curr_y, _zNear };
        screenToWorld(&p[0], &p[1], &p[2]);
        screenToWorld(&q[0], &q[1], &q[2]);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glTranslatef((q[0] - p[0]) * 15, -(q[1] - p[1]) * 15, (q[2] - p[2]) * 15);
        glMultMatrixf(meshes[selectedMesh]->t_matrix);

        GLdouble M[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, M);
        for (int i = 0; i < 16; ++i)
            meshes[selectedMesh]->t_matrix[i] = M[i];

        glPopMatrix();


    } else if (_mouseLeft && !_rotate) {
        double ax, ay, az;
        double bx, by, bz;
        double angle;

        ax = dy;
        ay = dx;
        az = 0.0;
        angle = sqrt(ax*ax + ay*ay + az*az) / (double) (viewport[2] + 1) * 180.0;

        /* Use inverse matrix to determine local axis of rotation */

        bx = _matrixI[0] * ax + _matrixI[4] * ay + _matrixI[8]  * az;
        by = _matrixI[1] * ax + _matrixI[5] * ay + _matrixI[9]  * az;
        bz = _matrixI[2] * ax + _matrixI[6] * ay + _matrixI[10] * az;

        glRotatef(angle, bx, by, bz);

        changed = true;
    } else if (_mouseRight && !_translate) {
        float px, py, pz;

        pos(&px, &py, &pz, x, y, viewport);

        glLoadIdentity();
        glTranslatef(px - _dragPosX, py - _dragPosY, pz - _dragPosZ);
        glMultMatrixd(_matrix);

        _dragPosX = px;
        _dragPosY = py;
        _dragPosZ = pz;

        changed = true;
    }

    prev_x = _mouseX = x;
    prev_y = _mouseY = y;

    if (changed) {
        getMatrix();
        glutPostRedisplay();
    }
   
}

//void mousePassiveFunc(int x, int y)
//{
//    checkHoveringStatus(x, y);
//}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glLoadIdentity();
    glMultMatrixd(_matrix);

    glPushMatrix();

    for (int i = 0; i < meshes.size(); i++)
         meshes[i]->glDraw(meshModel);
     
    glPopMatrix();
    
    glutSwapBuffers();
}

int main(int argc, char **argv)
{

    cout << " <<<<<<<<<<< INSTRUCTIONS >>>>>>>>>>>"   << endl << endl;
    cout << " q           -> previous mesh"           << endl        ;
    cout << " w           -> release all meshes"      << endl        ;
    cout << " e           -> next mesh"               << endl        ;
    cout << " p           -> render next point-style" << endl        ;
    cout << " m           -> render mesh"             << endl        ;
    cout << " left click  -> rotate mesh/world"       << endl        ;
    cout << " right click -> translate mesh/world"    << endl        ;
    cout << " scroll      -> zoom in/our"             << endl << endl;

    // LOAD MESHES
    meshes.push_back(new DefMesh("./model/pot.ply"));
    meshes.push_back(new DefMesh("./model/pot2.ply"));

    glutInit(&argc, argv);
    //Print contex info
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	//double buffer
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Mesh Merger");
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutTimerFunc(10, timerFunction, 1);

    glutMouseFunc(mouseEvent);
    glutMotionFunc(mouseMoveEvent);
    glutKeyboardFunc(handleKeyPress);
    //glutPassiveMotionFunc(mousePassiveFunc);
    
    init();
    glutMainLoop();
    return 0;

}

