
#include "defMesh.h"

DefMesh::DefMesh(std::string filename) : status(IDLE)
{
    pmodel = new PLYmodel;
    pmodel->Load((char*)filename.c_str());
    bbox.makeBBox(pmodel);
    loadIdentity(t_matrix);
    
    //pmodel = NULL;
    //if (!pmodel) {	/* load up the model */

    //char* meshFile = (char*)filename.c_str();
    //pmodel = glmReadOBJ(meshFile);
    //if (!pmodel) {
    //    return;
    //}
    //    glmFacetNormals(pmodel);
    //    glmVertexNormals(pmodel, 0);
    //    glmFacetNormals(pmodel);

    //    // save initial values for vertices
    //    for (int i = 0; i <= pmodel->numvertices; i++){

    //        int x = i * 3;
    //        int y = i * 3 + 1;
    //        int z = i * 3 + 2;

    //        pmodel->initVertices[x] = pmodel->vertices[x];
    //        pmodel->initVertices[y] = pmodel->vertices[y];
    //        pmodel->initVertices[z] = pmodel->vertices[z];
    //    }

    //    // save initial values for normals
    //    for (int i = 0; i <= pmodel->numnormals; i++){
    //        
    //        int x = i * 3;
    //        int y = i * 3 + 1;
    //        int z = i * 3 + 2;

    //        pmodel->initNormals[x] = pmodel->normals[x];
    //        pmodel->initNormals[y] = pmodel->normals[y];
    //        pmodel->initNormals[z] = pmodel->normals[z];
    //    }

    //}
   
}

void DefMesh::updateMesh(){

    for (int i = 0; i < pmodel->TotalConnectedPoints; i++){

        // Simplify getting coordinate of vertex
        // Note: vertices start at i = 1
        int x = (i + 1) * 3;
        int y = (i + 1) * 3 + 1;
        int z = (i + 1) * 3 + 2;

        // Zero out current vertex
        pmodel->Vertex_Buffer[x] = 0.0;
        pmodel->Vertex_Buffer[y] = 0.0;
        pmodel->Vertex_Buffer[z] = 0.0;
        float initPos[3] = { pmodel->Init_Vertex_Buffer[x], pmodel->Init_Vertex_Buffer[y], pmodel->Init_Vertex_Buffer[z] };

        // Update vertex positions
        float newPos[3];
        multv(t_matrix, initPos, newPos);
        pmodel->Vertex_Buffer[x] += newPos[0];
        pmodel->Vertex_Buffer[y] += newPos[1];
        pmodel->Vertex_Buffer[z] += newPos[2];

    }
}

void DefMesh::glDraw()
{
    
    switch (status){
    case IDLE:
        glColor3f(0.5, 0.5, 0.5);
        break;
    case HOVERED:
        glColor3f(0.5, 0.6, 0.5);
        break;
    case SELECTED:
        glColor3f(0.0, 0.5, 0.0);
        break;
    }

    // DRAW MESH
    glPushMatrix();
    glMultMatrixf(t_matrix);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, pmodel->Faces_Triangles);
    glNormalPointer(GL_FLOAT, 0, pmodel->Normals);
    glDrawArrays(GL_TRIANGLES, 0, pmodel->TotalConnectedTriangles);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glColor3f(0.5, 0.0, 0.0);

    glPopMatrix();

    
    // DRAW POINTS
    //
    //glPointSize(0.01f);//set point size to 10 pixels
    //glBegin(GL_POINTS); //starts drawing of points
    //for (int i = 0; i < pmodel->TotalConnectedPoints; i++)
    //{
    //    glVertex3f(pmodel->Vertex_Buffer[0 + (i * 3)], 
    //               pmodel->Vertex_Buffer[1 + (i * 3)], 
    //               pmodel->Vertex_Buffer[2 + (i * 3)]);

    //    glNormal3f(pmodel->Normals[0 + (i * 3)],
    //               pmodel->Normals[1 + (i * 3)],
    //               pmodel->Normals[2 + (i * 3)]);
    //}
    //glEnd();//end drawing of points

}