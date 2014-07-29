/***************************************************************************
                                 main.cpp
                             -------------------
    update               : 2009-10-09
    copyright            : (C) 2008-2009 by Yohan Fougerolle
    email                : yohan.fougerolle@u-bourgogne.fr
 ***************************************************************************/

// this program is just a bunch of functions I often need, and students too

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <map>

#include "scene.h"
#include "Constante.h"
#include "useful.h"
#include "Stopwatch.h"
#include "RPA.cpp"
#include "TreeNode.cpp"


using namespace std;

// horrible global variables next
// opengl rendering of the scene implemented "quick and (very) dirty" way

Stopwatch timer;   // in case you want to have a class to measure time

scene My_Scene;    // class to handle lights, position, etc

Mesh globalmesh;   // yes another infamous global variable

Mesh Q;
Mesh Crem;

int id_globalmesh; // identifier for display list. See function display() in scene.cpp

//global variables... could be handled as attributes in the scene class
Vector3d Camera_Target;
Vector3d Camera_Position;
double znear, zfar;

Node mynode;

bool dispEigs = true;

int main(int argc,char ** argv){

    //openGL initialization
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(200,200);
	glutCreateWindow("YF_Viewer 1.1.2");
	glutReshapeFunc(reshape); // function call when reshape the window
	glutMotionFunc(Motion);   // function called when mouse move
	glutMouseFunc(Mouse);     // function called when mouse click
	glutKeyboardFunc(Keyboard); // if key pressed
	glutSpecialFunc(Special);   // if special key pressed
	glutDisplayFunc(display);   // display function

	string file_name;

//    if(!AskFile(file_name)) exit(0);  //Ask user to select an OFF file through a winAPI 'GetOpenFileNameA' window.
                                        //Doesn't work, influ somehow onto results.


    //example on How To Use the timer
    timer.Reset();
    timer.Start();

    //my own inits here
	Init();
	My_Scene.Axis_Id_List=My_Scene.Draw_Axis(30);


    //load a mesh from a file
    file_name = ("OFF/SHREC2012/D00025.off");  //"VRML/David/David.wrl" //"OFF/SHREC2012/D00011.off" airplane  //025.off human


    LoadMesh(globalmesh,file_name); //Initialise a Mesh using an OFF file


    timer.Stop();
    cout<<file_name<<" has been loaded "<<endl<<endl;
    cout<<"Loading Off file time :"<< timer.GetTotal()/1000.0<<" s"<<endl<<endl;

    timer.Reset();
    timer.Start();



    First_Part(globalmesh,Crem,Q,mynode,5000,4,5); //Algo2 (OCTREE + LMS)



    timer.Stop();
    cout<<"Loading First_Part time :"<< timer.GetTotal()/1000.0<<" s"<<endl<<endl;

    timer.Reset();
    timer.Start();



    Second_Part(Crem, Q, 5000, 50); //Algo1 (Iterative PCA)



    timer.Stop();
    cout<<"Loading Second_Part time :"<< timer.GetTotal()/1000.0<<" s"<<endl<<endl;

    timer.Reset();
    timer.Start();

    CompMesh(globalmesh,Q); //Compare original Mesh points and Major Region points



    //roughly adjust view frustrum to object and camera position
    Vector3d Pmin(globalmesh.vertices[0]), Pmax(globalmesh.vertices[0]);
    Vector3d Center(0,0,0);

    for( int i=0; i<globalmesh.vertices.size(); i++)
    {
        Vector3d P(globalmesh.vertices[i]);
        for( int j=0; j<2; j++)
        {
            Pmin[j] = min(Pmin[j],P[j]);
            Pmax[j] = max(Pmax[j],P[j]);
        }
        Center += P;
    }

    Center/=globalmesh.vertices.size();
    Camera_Target = Center;

    //length of the diagonal of the bouding box
    double distance = (Pmax-Pmin).norm();

    //set arbitraty position to camera and adjusts max and min view planes
    Camera_Position = Camera_Target + Vector3d(0,0,distance*3);
    znear = distance*0.1;
    zfar = distance*5;

    //adjust displacements consequently

    My_Scene.Object_Move[0]=My_Scene.Object_Move[1]=My_Scene.Object_Move[2] = distance/20;

    //creates lights accordingly to the position and size of the object

    My_Scene.Create_Lighting(Pmax,Pmin, Camera_Position, Camera_Target);


    //ok now here render
    id_globalmesh=glGenLists(1);
    glNewList(id_globalmesh,GL_COMPILE_AND_EXECUTE);

    glEnable(GL_LIGHTING);
    globalmesh.Draw(ONLY_VERTEX);

    Q.Draw_EigenBasis(1,true); //Display EigenVectors of Major Region

//    mynode.Draw(true);


    cout<<"Q size : "<<Q.VertexNumber()<<endl<<endl; //Display How many points belong to Major Region;


    timer.Stop();
    cout<<"Displaying loading time :"<< timer.GetTotal()/1000.0<<" s"<<endl<<endl;

    DispEig(Q);


    cout<<"\nWant to see ? .....";
    system("Pause");


    glEndList();

    //now render
    glutMainLoop();

	return 0;

}
