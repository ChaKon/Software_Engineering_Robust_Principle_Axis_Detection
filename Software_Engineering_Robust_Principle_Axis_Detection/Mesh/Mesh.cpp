/***************************************************************************
                                  Mesh.cpp
                             -------------------
    update               : 2002-12-05
    copyright            : (C) 2002 by Michaël ROY
    email                : michaelroy@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <fstream>
#include <iostream>
#include <GL/glut.h>

#include "Mesh.h"
#include "constante.h"
#include "useful.h"
#include "cv.h"

using namespace std;

using namespace cv;

Mesh::Mesh(){}
Mesh::~Mesh(){}
//--
//
// ClearAll
//
//--

void Mesh::ClearAll()
{
	// Initialize mesh
	// Clear all data
	ClearVertices();
	ClearFaces();
	ClearColors();
	ClearTextures();
	ClearFaceNormals();
	ClearVertexNormals();
	ClearTextureName();
	ClearEigens();
	mainBody.clear();

	isProjected = false;
}

//--
//
// ComputeFaceNormals
//
//--
// Compute unit normal of every faces
void Mesh::ComputeFaceNormals()
{
	// Resize face normal array
	face_normals.resize( FaceNumber() );

	// For every face
	int MaxFaceNumber = FaceNumber();
	for( int i=0; i<MaxFaceNumber; i++ )
	{
		// Compute unit face normal
		//FaceNormal(i) = ComputeFaceNormal(i);
		face_normals[i] = ComputeFaceNormal(i);
	}
}


//--
//
// ComputeVertexNormals
//
//--
// Compute unit normal of every vertex
void Mesh::ComputeVertexNormals()
{

	int i;

	// Assume that face normals are computed
	assert( FaceNormalNumber() == FaceNumber() );

	// Resize and initialize vertex normal array
	vertex_normals.assign( VertexNumber(), Vector3d(0,0,0) );

	// For every face
	for( i=0 ; i<FaceNumber() ; i++ )
	{
		// Add face normal to vertex normal
		VertexNormal(i,0) += FaceNormal(i);
		VertexNormal(i,1) += FaceNormal(i);
		VertexNormal(i,2) += FaceNormal(i);
	}

	// For every vertex
	for( i=0 ; i<VertexNumber() ; i++)
	{
		// Normalize vertex normal
		VertexNormal(i).normalized();
	//	logfile<<vertex_normals[i]<<endl;
	}


}




//--
//
// ReadFile
//
//--
// Read mesh from a file
bool Mesh::ReadFile( const string& file_name )
{
	FileFormat file_format(UNKNOWN_FILE);

	cout<<"READ FILE"<<endl;
	// Find file extension
	int pos = file_name.find_last_of('.');
	if( pos == -1 )
	{
		// File has no extension
		return false;
	}

	// Format extension string
	string extension = UpperCase( file_name.substr( ++pos ) );

	cout<<"Extension = "<<extension.data()<<endl;

	//Point Cloud extension
	if( extension == "XYZ" )
	{
		file_format = CLOUD_FILE;
	}
	else
	// RANGE TXT extension
	if( extension == "TXT" )
	{
		file_format = RANGE_FILE;
	}
	else
	// WRL extension
	if( extension == "WRL" )
	{
		file_format = VRML_1_FILE;
	}
	// IV extension
	else if( extension == "IV" )
	{
		file_format = IV_FILE;
	}
	// SMF extension
	else if( extension == "SMF" )
	{
		file_format = SMF_FILE;
	}
	// STL extension
	else if( extension == "STL" )
	{
		ifstream file(file_name.c_str(), ios::binary);
		if( file.is_open() == false )
		{
			return false;
		}
		char header[5];
		file.read(header, 5);
		if( strcmp(header, "solid") == 0 )
		{
			file_format = STL_ASCII_FILE;
		}
		else
		{
			file_format = STL_BINARY_FILE;
		}
		file.close();
	}
	// STLA extension
	else if( extension == "STLA" )
	{
		file_format = STL_ASCII_FILE;
	}
	// STLB extension
	else if( extension == "STLB" )
	{
		file_format = STL_BINARY_FILE;
	}
	// OFF extension
	else if( extension == "OFF" )
	{
		file_format = OFF_FILE_1;
	}

	// Other extension
	else
	{
		// Unknown file format
		cout<<"Unknown file extension"<<endl;
		return false;
	}

	// Read file
	switch( file_format )
	{

		// OpenInventor file
		case IV_FILE :

		// VRML 1.0 file
		case VRML_1_FILE :
			return ReadIv( *this, file_name );


        // OFF 1.0 file
		case OFF_FILE_1 :
			return ReadOff( *this, file_name, true );

		// Other file
		default :{cout<<"Unknown file format"<<endl;
			// Unknown file format
			return false;
				 }
	}
}

//--
//
// WriteFile
//
//--
// Write mesh to a file
bool Mesh::WriteFile( const string& file_name, const FileFormat& file_format ) const
{
	// Write file
	switch( file_format )
	{
		// OpenInventor file
		case IV_FILE :
			return WriteIv( *this, file_name, false );
			break;

		// SMF file
		case SMF_FILE :
		//	return WriteSmf( *this, file_name );
			break;

		// STL ascii file
		case STL_ASCII_FILE :
		//	return WriteStlA( *this, file_name );
			break;

		// STL binary file
		case STL_BINARY_FILE :
		//	return WriteStlB( *this, file_name );
			break;

		// VRML 1.0 file
		case VRML_1_FILE :
			return WriteIv( *this, file_name, true );
			break;

		// Other file
		default :
			// Unknown file format
			return false;
	}
}

//--
//
// UpperCase
//
//--
// Upper case string of a given one
string UpperCase( const string& s )
{
	// Upper case string
	string us;

	// For every character in the string
	string::const_iterator it = s.begin();
	while( it != s.end() )
	{
		// Convert character to upper case
		us += toupper( *it );

		// Next character
		++it;
	}

	// Return upper case string
	return us;
}



void Mesh::Draw_Default(int i){


	for(int j=0; j<3; j++)
		glVertex3d(vertices[i][0],vertices[i][1],vertices[i][2]);


}

void Mesh::Draw_Face_Normal(int i){



	for(int j=0; j<3; j++)	{
		glNormal3d(face_normals[i][0],face_normals[i][1],face_normals[i][2]);
		Vector3d V=Vertex(i,j);
		glVertex3d(V[0],V[1],V[2]);
	}


}

void Mesh::Draw_Vertex_Normal(int i){

	for(int j=0; j<3; j++)	{

		int vertex_index = faces[i][j];

        if (mainBody[i]==true){
            glColor3d(0,1,0);
        }else{
            glColor3d(1,0,0);
        }

		Vector3d N(vertex_normals[vertex_index]);
		Vector3d V(vertices[vertex_index]);
		glNormal3d(N[0],N[1],N[2]);
		glVertex3d(V[0],V[1],V[2]);

	}


}


void Mesh::Draw(int DRAW_MODE)
{
int nb_faces=faces.size(),i;


switch(DRAW_MODE)
	{
	case ONLY_VERTEX	:
		{
			glDisable(GL_LIGHTING);
			cout<<"Vertices displayed = "<<vertices.size()<<endl<<endl;

			glPointSize(3.0);
			glBegin(GL_POINTS);

			for( i=0 ; i<vertices.size(); i++ )
			{
				if (mainBody[i]==true){
                    glColor3d(0.1,0.7,0);
                }else{
                    glColor3d(0.6,0,0);
                }
				glVertex3d(vertices[i][0],vertices[i][1],vertices[i][2]);
			}

			glEnd();

			glEnable(GL_LIGHTING);

		}break;

	case FACE_NORMAL:
		{	glBegin(GL_TRIANGLES);
			for( i=0; i<nb_faces; i++) 				Draw_Face_Normal(i);
		}break;

	case FACE_NORMAL_RGB:
		{
			glBegin(GL_TRIANGLES);
			for( i=0; i<nb_faces; i++) Draw_Face_Normal_Rgb(i);
			glEnd();
		}break;

	case VERTEX_NORMAL		:
		{
			glBegin(GL_TRIANGLES);
                for( i=0; i<nb_faces; i++) Draw_Vertex_Normal(i);
			glEnd();

		}	break;

	case VERTEX_NORMAL_RGB	:
		{
			glBegin(GL_TRIANGLES);
			for( i=0; i<faces.size(); i++) Draw_Vertex_Normal_Rgb(i);
			glEnd();
		}	break;
	}
}


void Mesh::Print_Stats()
{
	char s[250];
	sprintf(s,"Faces   : %d", faces.size());
	PrintMessage( 10, 10, s );
	sprintf(s,"Vertices: %d", vertices.size());
	PrintMessage( 10, 25, s );

	//add any information you want to display as text in the OGL window here

}


void Mesh::Draw_Face_Normal_Rgb(int i)
{


	for(int j=0; j<3; j++)	{
		glNormal3d(face_normals[i][0],face_normals[i][1],face_normals[i][2]);
		Vector3d V=vertices[faces[i][j]];
		Vector3d C=colors[faces[i][j]];
		glColor3d(C[0],C[1],C[2]);
		glVertex3d(V[0],V[1],V[2]);
	}


}

void Mesh::Draw_Vertex_Normal_Rgb(int i){

	for(int j=0; j<3; j++)	{

		int vertex_index = faces[i][j];

		Vector3d N(vertex_normals[vertex_index]);
		Vector3d V(vertices[vertex_index]);
		Vector3d Col(colors[vertex_index]);

		glNormal3d( N[0] , N[1] , N[2]);
		glColor3d( Col[0] , Col[1] , Col[2]);
		glVertex3d( V[0] , V[1] , V[2]);
	}


}


/////////////////////////// Student Work /////////////////////////////////

void Mesh::Meshpca(){ //Compute PCA

    int NBrows=VertexNumber(); //Matrix opencv, dimension row

    Vector3d myEigenVec; //Temp Vector3d

    Vector3d myEigenVal;

    Vector3d myEigenMean;

    Vector3d myVertexProjected;

    Mat_<double> myMat(NBrows,3); //Mat opencv

    Mat_<double> myMatProjected(1,3);

    Mat_<double> myMean; //Mat opencv meanvalue for PCA

    for (int i=0;i<NBrows;i++) //Copy cloud point in opencv Mat, one row per vector (vertex)
    {
        myMat.row(i)(0)=vertices[i][0];
        myMat.row(i)(1)=vertices[i][1];
        myMat.row(i)(2)=vertices[i][2];
    }

    ClearEigens(); //Clear all Eigens for a new computation

    PCA myPca(myMat,myMean,CV_PCA_DATA_AS_ROW,3); //Compute PCA over opencv Mat

    myPca.project(myMat,myMatProjected); //Project Mat onto eigen subspace

    for(int i=0;i<3;i++) //Get eigenvectors, eigenvalues and mean from pca & store them in Mesh members
    {
        myEigenVec[0]=(myPca.eigenvectors.at<double>(i,0));
        myEigenVec[1]=(myPca.eigenvectors.at<double>(i,1));
        myEigenVec[2]=(myPca.eigenvectors.at<double>(i,2));

        myEigenVal[i]=(myPca.eigenvalues.at<double>(i));

        myEigenMean[i]=(myPca.mean.at<double>(i));

        AddEigenVec(myEigenVec);
    }

    eigenVal << myEigenVal;
    AddEigenMean(myEigenMean);

    for (int i=0;i<NBrows;i++) //Copy local projected matrix in Mesh member
    {
        myVertexProjected[0] = myMatProjected.row(i)(0);
        myVertexProjected[1] = myMatProjected.row(i)(1);
        myVertexProjected[2] = myMatProjected.row(i)(2);

        AddVerticesProjected(myVertexProjected);
    }

}

void Mesh::ProjectMesh() //Swap 'vertices' and 'verticesProjected' and set 'isProjected' to true
{
    vector<Vector3d> myTemp;

    if (!isProjected)
    {
        myTemp = vertices;
        vertices = verticesProjected;
        verticesProjected = myTemp;

        isProjected=true;
    }
}

void Mesh::RetroProjectMesh(){ //Swap 'verticesProjected' and 'vertices' and set 'isProjected' to false

    vector<Vector3d> myTemp;

    if (isProjected)
    {
        myTemp = vertices;
        vertices = verticesProjected;
        verticesProjected = myTemp;

        isProjected=false;
    }

}

void Mesh::Draw_EigenBasis(const int &i, const bool &EigenValScale) //Display Eigenbasis
{

    if (!isProjected){

        Vector3d basisX;
        Vector3d basisY;
        Vector3d basisZ;

        if (EigenValScale){

            basisX = (eigenVec[0]*eigenVal[0]*i)+ eigenMean;
            basisY = (eigenVec[1]*eigenVal[1]*i)+ eigenMean;
            basisZ = (eigenVec[2]*eigenVal[2]*i)+ eigenMean;

        }else{

            basisX = (eigenVec[0]*i)+ eigenMean;
            basisY = (eigenVec[1]*i)+ eigenMean;
            basisZ = (eigenVec[2]*i)+ eigenMean;

        }

        glLineWidth(4.0);

        glBegin(GL_LINES);

            glColor3d(10,0,0);
            glVertex3d(eigenMean[0],eigenMean[1],eigenMean[2]);
            glVertex3d(basisX[0],basisX[1],basisX[2]);

            glColor3d(0,10,0);
            glVertex3d(eigenMean[0],eigenMean[1],eigenMean[2]);
            glVertex3d(basisY[0],basisY[1],basisY[2]);

            glColor3d(0,0,10);
            glVertex3d(eigenMean[0],eigenMean[1],eigenMean[2]);
            glVertex3d(basisZ[0],basisZ[1],basisZ[2]);

        glEnd();

        glEnable(GL_LIGHTING);

    }else{

        glLineWidth(4.0);
        glBegin(GL_LINES);

        if (EigenValScale){

            glColor3d(10,0,0);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(i*eigenVec[0][0]*eigenVal[0],i*eigenVec[0][1]*eigenVal[0],i*eigenVec[0][2]*eigenVal[0]);

            glColor3d(0,10,0);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(i*eigenVec[1][0]*eigenVal[1],i*eigenVec[1][1]*eigenVal[1],i*eigenVec[1][2]*eigenVal[1]);

            glColor3d(0,0,10);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(i*eigenVec[2][0]*eigenVal[2],i*eigenVec[2][1]*eigenVal[2],i*eigenVec[2][2]*eigenVal[2]);

        }else{

            glColor3d(10,0,0);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(i*eigenVec[0][0],i*eigenVec[0][1],i*eigenVec[0][2]);

            glColor3d(0,10,0);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(i*eigenVec[1][0],i*eigenVec[1][1],i*eigenVec[1][2]);

            glColor3d(0,0,10);
            glVertex3d(0.0,0.0,0.0);
            glVertex3d(i*eigenVec[2][0],i*eigenVec[2][1],i*eigenVec[2][2]);

        }

        glEnd();

        glEnable(GL_LIGHTING);

    }

}


/////////////////////////// Student Work /////////////////////////////////
