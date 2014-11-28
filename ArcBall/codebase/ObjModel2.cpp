
#include "ObjModel.h"
#include "ObjParser.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>

#include "debug.h"

using namespace std;


int ObjModel::NormalizeFacetIndex(){
  int min = faces[0][0];

  for(int i=0;i<faces.size();++i)
    for(int j=0;j<faces[i].size();++j)
      if(min>faces[i][j])
	min = faces[i][j];

  
  for(int i=0;i<faces.size();++i)
    for(int j=0;j<faces[i].size();++j)
      faces[i][j]-=min;

  return 0;
}


int ObjModel::AbsFacetIndex(){
  for(int i=0;i<faces.size();++i)
    for(int j=0;j<faces[i].size();++j)
      faces[i][j] = abs(faces[i][j] + 1)-1;

  return 0;
}


int ObjModel::GenerateNormals(int bAlways){
  
  if(!bAlways && QNormals())
    return 1;

  vector<int> nf;
  
  for(int i=0;i<vertex.size();i++){
    normal.push_back(TVector(0, 0, 0));
    nf.push_back(0);
  }

  for(int i=0;i<faces.size();i++)
    for(int j=0;j<faces[i].size();j++){
      normal[faces[i][j]] =  normal[faces[i][j]] + fnormal[i];
      nf[faces[i][j]]+=1;
    }

  for(int i=0;i<normal.size();i++){
    double f = 1.0 / (double) nf[i];
    normal[i] = f *  normal[i];
    normal[i].normalize();
  }
  
  nfaces = faces;
  return 0;
}


TVector ObjModel::ComputeFaceNormals(int face){
  TPoint ret[3];
  for(int i=0;i<3;i+=1)
    ret[i] = vertex[faces[face][i]];

  TVector t1 = (ret[1] - ret[0]);
  TVector t2 = (ret[2] - ret[0]);
  t1.normalize();
  t2.normalize();

  return t1*t2;
}


void ObjModel::ComputeAllFaceNormals(){
  for(int i=0;i<faces.size();i++)
    fnormal.push_back(ComputeFaceNormals(i));
}

void ObjModel::ComputeBoundingBox(){
  int N = vertex.size();
  C = TPoint(0, 0, 0);
  for(int i=0;i<N;++i){
    C = C + vertex[i];
  }

  double dn = 1.0 / ((double) N);

  C = C * dn;

  cout<<C[0]<<", "<<C[1]<<", "<<C[2]<<endl;
}


int ObjModel::EliminateEmptyVertices(){
	return 0;
}


void ObjModel::dumpampl(const char* fname){
  std::ofstream out(fname);

  if(faces.size()!=2*vertex.size()-4){
    std::cout<<"Not a manifold!!!"<<std::endl;
    return;
  }

  int m = 3*vertex.size() - 6;

  

}

int ObjModel::GenerateObjModel(std::ostream& f){
    
  // slap the material lib first
  if(!matlib.empty())
    f<<"mtllib "<<matlib<<endl;
   

// generate vertices & normals first
    for(int i=0;i<vertex.size();i+=1)
	f<<"v "<<vertex[i][0]<<" "<<vertex[i][1]<<" "<<vertex[i][2]<<std::endl;

    f<<std::endl;

    // textures
    for(int i=0;i<texture.size();i+=1)
	f<<"vt "<<texture[i][0]<<" "<<texture[i][1]<<std::endl;

    f<<std::endl;

    
    for(int i=0;i<normal.size();i+=1)
	f<<"vn "<<normal[i][0]<<" "<<normal[i][1]<<" "<<normal[i][2]<<std::endl;

    f<<std::endl;

    
    std::string mat;

    ASSERT(sfaces.size()==faces.size());

    // now faces
    for(int i=0;i<faces.size();i+=1){

      if(!sfaces[i].empty() && sfaces[i]!=mat){
	mat = sfaces[i];
	f<<"usemtl "<<mat<<endl;
      }

	f<<"f";
	for(int j=0;j<3;j+=1){

	  f<<" "<<faces[i][j]+1;//<<"/";

	    //   if(QTexture())
	  //   f<<txfaces[i][j]+1;

	    // the normal
	  // f<<"/"<<nfaces[i][j]+1;

	}
	f<<std::endl;
    }

}


// flip the x and y coordinates
void ObjModel::RotateLeft(){
  for(int i=0;i<vertex.size();i+=1){
    TPoint& P = vertex[i];
    double a = P[0];
    P[0] = P[1];
    P[1] = a;
  }

}

// translate object on x and y corrdinates
void ObjModel::Translate(const TVector& trans){
	for (int i = 0; i<vertex.size(); i += 1){
		TPoint& P = vertex[i];
		P = P + trans;
	}
}

int ObjModel::Triangulate(){
  int size = faces.size();
  for(int i=0;i<size;i++){
    std::vector<int> first, nfirst, txfirst;
    std::vector<int> tmp, ntmp, txtmp;
    first.clear();
    nfirst.clear();
    txfirst.clear();
    for(int j=0;j<faces[i].size()-2;j++){
      tmp.clear();
      tmp.push_back(faces[i][0]);

      ntmp.clear();
      ntmp.push_back(nfaces[i][0]);

      txtmp.clear();
      txtmp.push_back(txfaces[i][0]);

      for(int k=1;k<3;k++){
	tmp.push_back(faces[i][j+k]);
	ntmp.push_back(nfaces[i][j+k]);
	txtmp.push_back(txfaces[i][j+k]);
      }

      if(j==0){
	first = tmp;
	nfirst = ntmp;
	txfirst = txtmp;
      } else {
	faces.push_back(tmp);
	nfaces.push_back(ntmp);
	txfaces.push_back(txtmp);
      }

    }// for j
    
    faces[i] = first;
    nfaces[i] = nfirst;
    txfaces[i] = txfirst;
  }// for i

  ComputeAllFaceNormals();


  return 0;
}// 



/* external functions */
void SaveAs(ObjModel& obj, char* file){

    std::ofstream f(file);
    if(f)
      f<<obj;
}


void Load(ObjModel& obj, char* file){
  std::ifstream f(file);
  if(f)
    f>>obj;

  obj.ComputeAllFaceNormals();
  obj.GenerateNormals();
}

std::ostream& operator<<(std::ostream& o, ObjModel& m){
  m.GenerateObjModel(o);

  return o;
}


std::istream& operator>>(std::istream& in, ObjModel&m){
  ObjParser(&in, &m);

  return in;
} 

