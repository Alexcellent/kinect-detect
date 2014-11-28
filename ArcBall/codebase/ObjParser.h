#ifndef _OBJ_PARSER_H_
#define _OBJ_PARSER_H_

#include "ObjModel.h"
#include <string>
#include <istream>

typedef enum {
    VERTEX = 0, 
    NORMAL, 
    FACE, 
    COMMENT, 
    EMPTY, 
    TEXTURE, 
    TANGENT, 
    MATLIB, 
    MAT,
    UNKNOWN
} LineType;


int Tokenize(std::string &dst, std::string &source, std::string k = " ");
int ParseVertexLine(ObjModel* model, std::string line);
int ParseNormalLine(ObjModel* model, std::string line);
int ParseTangentLine(ObjModel* model, std::string line);
int ParseOneFaceString(std::string s, int &value);
int ParseFaceLine(ObjModel* model, std::string line, std::string& mat);
int ParseFaceLineSmf(ObjModel* model, std::string line);
LineType GetType(std::string s, int t_obj);

int ObjParser(std::istream* file,  ObjModel* model);

#endif // _OBJ_PARSER_H_
