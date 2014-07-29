/***************************************************************************
                                  useful.h
                             -------------------
    update               : 2009-10-09
    copyright            : (C) 2008-2009 by Yohan Fougerolle
    email                : yohan.fougerolle@u-bourgogne.fr
 ***************************************************************************/

// this program is just a bunch of functions I often need, and students too


#ifndef _USEFUL_
#define _USEFUL_

#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

#include "Mesh.h"
#include <ctime>
#include "constante.h"

#include <Eigen/Core>
using namespace Eigen;

    // convertion RGB <--> value between [0,1]
	Vector3d DoubleToColor( double d );
	double ColorToDouble(Vector3d RGB);
	Vector3d DoubleToColorDiscrete( const double d, double n =20);

    //OpenGL text display
	void PrintMessage(int,int, const string);

    //mesh perturbation if you want to play a little bit
    void AddGaussianNoise(vector<Vector3d> & vertices, vector<Vector3d> normals, double sigma,double mu=0);

    //read and write 3D files
    bool ReadIv( Mesh& mesh, const string& file_name );
    bool WriteIv( const Mesh& mesh, const string& file_name, const bool& vrml1 );

    //miscellanous
    void ScreenShot(string File); // from opengl buffer to infamous .tga export


/////////////////////////// Student Work /////////////////////////////////

    bool LoadMesh(Mesh&, string&); //Load completely Mesh from 3D file

    bool ReadOff( Mesh& , const string& , bool); //Read OFF file

    void DispEig( const Mesh& ); //Display within console Mesh EigenVectors, EigenValues and Mean

    void stop(int); //system("pause")

    void CompMesh(Mesh&, Mesh&); //Compare two Meshes based on their vertices, if a point of second mesh
                                 //exist in first, first mesh 'mainBody' is updated

    bool AskFile(string &); //Ask user to select a 3D file through a winAPI 'GetOpenFileNameA' window.
                            //Doesn't work, influ somehow onto results.

/////////////////////////// Student Work /////////////////////////////////

#endif

