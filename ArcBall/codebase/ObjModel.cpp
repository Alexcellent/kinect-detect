#include "ObjModel.h"
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>



#ifdef __APPLE__ 
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

ObjModel::ObjModel(){
}


ObjModel::ObjModel(const ObjModel& m){
  copy(m);
}

ObjModel& ObjModel::operator=(const ObjModel& m){

  // clear up the old ones

  copy(m);

  return *this;
}

void ObjModel::copy(const ObjModel& m){
  vertex = m.vertex;
  normal = m.normal;  
  texture = m.texture;

  faces = m.faces;
  nfaces = m.nfaces;
  fnormal = m.fnormal;
  txfaces = m.txfaces;  

  
  sfaces = m.sfaces;
  
  matlib = m.matlib;

  vfacets = m.vfacets;
}


bool ObjModel::operator==(const ObjModel& m){

  if(vertex.size()!=m.vertex.size()){
    std::cout<<"Different number of vertices"<<std::endl;
    return false;
  }

  if(faces.size() !=m.faces.size()){
    std::cout<<"Different number of faces"<<std::endl;
    return false;
  }

  bool ret = true;
  for(int i = 0;i<faces.size();i++){
    if(faces[i].size()!=m.faces[i].size()){
      std::cout<<"Face "<<i<<" does not match"<<std::endl;
      ret = false;
      continue;
    }
       
    bool success2 = false;    
    for(int k=0;k<faces[i].size();k++){
      bool success = true;
      for(int j=0;j<faces[i].size();j++)      
	if(faces[i][j]!=m.faces[i][(j+k)%m.faces[i].size()]){
	  success = false;
	}
      if(success){
	success2 = true;
	break;
      }
    }
   
    if(!success2){
      ret = false;
      std::cout<<"Face "<<i<<" does not match2"<<std::endl;
    }
  }

  return ret;
}
    
ObjModel:: ~ObjModel(){

}

int ObjModel::Normalize(){
  return 0;
}

int ObjModel::SubstractConsistent(double displacement){
  TVector V(displacement);
  for(int i=0;i<vertex.size();i+=1)
	vertex[i]=vertex[i] + V;
    return 0;
}
	
int ObjModel::CenterObject(TPoint P){
  TVector V = P;
  for(int i=0;i<vertex.size();i+=1)
    vertex[i]=vertex[i]-V;
  return 0;
}


int ObjModel::CenterObject(){
    ComputeBoundingBox();
    CenterObject(C);
    return 0;
}
	    
int ObjModel::AssertValid(){
  int ret =0;
  return ret;
}

int ObjModel::AddVertex(TPoint P){
  vertex.push_back(P);
  std::vector<int> dummy;
  vfacets.push_back(dummy);
  
  return vertex.size()-1;
}

int ObjModel::AddTexture(TVector v){
        texture.push_back(v);
    return 0;
}

int ObjModel::AddTangent(TVector v){
    return 0;
}


int ObjModel::AddNormal(TVector v){
  normal.push_back(v);
  return 0;
}

int ObjModel::AddGeneric(int N, int* f, std::vector< std::vector<int> >& vec){
  
  if(!f)
    return -1;
  
  std::vector<int> v;
  
  for(int i=0;i<N;i+=1)
    v.push_back(f[i]);

  vec.push_back(v);
  return 0;
}



int ObjModel::AddFace(int N, int* face, std::string mat, int* nf, int*txf, int* tgf, int *btgf){

  sfaces.push_back(mat);
  AddGeneric(N, face, faces);  
  
  for(int i=0;i<N;++i)
    vfacets[face[i]].push_back(faces.size()-1);
  
  AddGeneric(N, nf, nfaces);
  AddGeneric(N, txf, txfaces);

  return 0;
}



// query functions
int ObjModel::QTexture()const{
  if(texture.size()>0)
    return 1;
  else
    return 0;
}
// query functions
int ObjModel::QTangents()const{
  return 0;
}

int ObjModel::QNormals()const{
  if(normal.size()>0)
    return 1;
  else
    return 0;
}

/// rendering functions

void ObjModel::RenderEdges(){
  
  glBegin(GL_LINES);
	  int degree = 3;
	  int tf = faces.size();
	  for(int i=0;i<tf;i+=1)
		for(int j=0;j<degree;j+=1){
		  /// not sure if I should put the color here ... probably not
		  glColor3f(0, 1, 0);
      
		  glVertex3f(vertex[faces[i][j]][0], 
			 vertex[faces[i][j]][1], 
			 vertex[faces[i][j]][2]); 

		  glVertex3f(vertex[faces[i][(j+1)%degree]][0], 
			 vertex[faces[i][(j+1)%degree]][1], 
			 vertex[faces[i][(j+1)%degree]][2]); 
		}
  glEnd();

  glFlush();
}

void ObjModel::Render(int pick){
  //assume triangles
  int fn = QNormals();
  int ft = QTangents();
  int ftxt = QTexture();
  
  int degree = 3;
  int tf = faces.size();

  if(!pick){
    glBegin(GL_TRIANGLES);
    for(int i=0;i<tf;i+=1)
      for(int j=0;j<degree;j+=1){
	/// not sure if I should put the color here ... probably not
	glColor3f(1, 1, 1);
	
	glTexCoord3f(texture[txfaces[i][j]][0], 
		     texture[txfaces[i][j]][1], 
		     texture[txfaces[i][j]][2]); 
	
//      if(fn)
	//glNormal3f(normal[nfaces[i][j]][0], 
	//	   normal[nfaces[i][j]][1], 
	//	   normal[nfaces[i][j]][2]); 
	
	glNormal3f(fnormal[i][0], fnormal[i][1],fnormal[i][2]);
	
	glVertex3f(vertex[faces[i][j]][0], 
		   vertex[faces[i][j]][1], 
		   vertex[faces[i][j]][2]); 
      }
    glEnd();
  } else {
    std::cout<<"Here!"<<std::endl;
    for(int i=0;i<tf;i+=1){
      glPushName(i+1);
      glBegin(GL_TRIANGLES);
      for(int j=0;j<degree;j+=1){
	/// not sure if I should put the color here ... probably not
	glColor3f(1, 1, 1);
	
	
//      if(fn)
	//glNormal3f(normal[nfaces[i][j]][0], 
	//	   normal[nfaces[i][j]][1], 
	//	   normal[nfaces[i][j]][2]); 
	
	glNormal3f(fnormal[i][0], fnormal[i][1],fnormal[i][2]);
	
	glVertex3f(vertex[faces[i][j]][0], 
		   vertex[faces[i][j]][1], 
		   vertex[faces[i][j]][2]); 
      }// j
      glEnd();
      glPopName();     
    }// i

  }
  glFlush();
}	

void ObjModel::RenderSmooth(int pick){


  //assume triangles
  int fn = QNormals();
  int ft = QTangents();
  int ftxt = QTexture();
  

  int degree = 3;
  int tf = faces.size();

  if(!pick){
     glBegin(GL_TRIANGLES);
     for(int i=0;i<tf;i+=1)
      for(int j=0;j<degree;j+=1){
	/// not sure if I should put the color here ... probably not
	glColor3f(1, 1, 1);
	glNormal3f(normal[nfaces[i][j]][0], 
		   normal[nfaces[i][j]][1], 
		   normal[nfaces[i][j]][2]); 
	
	glVertex3f(vertex[faces[i][j]][0], 
		   vertex[faces[i][j]][1], 
		   vertex[faces[i][j]][2]); 
      }    
    glEnd();
  } else {
 
    for(int i=0;i<tf;i+=1){
      glPushName(i+1);
      glBegin(GL_TRIANGLES);
      for(int j=0;j<degree;j+=1){
	/// not sure if I should put the color here ... probably not
	glColor3f(1, 1, 1);
	glNormal3f(normal[nfaces[i][j]][0], 
		   normal[nfaces[i][j]][1], 
		   normal[nfaces[i][j]][2]); 
	
	glVertex3f(vertex[faces[i][j]][0], 
		   vertex[faces[i][j]][1], 
		   vertex[faces[i][j]][2]); 
      }   
      glEnd();
    }
    glPopName();
  }

  glFlush();
}	

void ObjModel::RenderPoints(int pick){

  if(!pick){
    glBegin(GL_POINTS);

    glColor3f(1, 0, 0);
    
    glVertex3f(219.886 - 454.81, -1585.13 - 487.625, 667.508 -  94.2316); 

    for(int i=0;i<vertex.size();i+=1){
      if(i==0)
      glColor3f(0, 1, 0);
    else
      glColor3f(0, 0, 1);
      glVertex3f(vertex[i][0], vertex[i][1],vertex[i][2]);
    }
    glEnd();
  } else {
    for(int i=0;i<vertex.size();i+=1){
      glPushName(i+1);
      glBegin(GL_POINTS);
      glColor3f(0, 0, 1);
      glVertex3f(vertex[i][0], vertex[i][1],vertex[i][2]);
      glEnd();
      glPopName();
    }
  }
  
}	

// trivial function
void ObjModel::Subdivide(){

  int N = faces.size();
  for(int i=0;i<N;++i){
    TPoint middles[3];
    for(int j=0;j<3;++j)
      middles[j] = 0.5 * (vertex[faces[i][j]] + vertex[faces[i][(j+1)%3]]);
    
    int index[3];
    for(int j=0;j<3;++j){
      index[j] = faces[i][j];
      faces[i][j] = AddVertex(middles[j]);
    }// j
     
    for(int j=0;j<3;++j){
      int seq[3];
      seq[0] = index[j];
      seq[1] = faces[i][j];
      seq[2] = faces[i][(j+2)%3];
      AddFace(3, seq, std::string("blah"));
    }// j
  }// i
}// subdivide
