/////////////////////////// Student Work /////////////////////////////////

#include <iostream>
#include <string.h>
#include <cstdlib>
#include <limits>
#include "TreeNode.h"


using namespace std;


Node::Node(){ //Default constructor

    pts.clear();

    indCenter2pts.clear();

    center << 0,0,0;

    leaf = false;

    root = false;

    for (int i=0;i<8;i++) subNode[i] = NULL;

}

Node::Node(Vector3d xyzMin,Vector3d xyzMax, int depth){ //Constructor

    Vector3d pts0(xyzMin[0],xyzMax[1],xyzMax[2]);
    pts.push_back(pts0);
    pts0 << xyzMin[0],xyzMax[1],xyzMin[2];
    pts.push_back(pts0);
    pts0 <<xyzMin[0],xyzMin[1],xyzMin[2];
    pts.push_back(pts0);
    pts0 <<xyzMin[0],xyzMin[1],xyzMax[2];
    pts.push_back(pts0);
    pts0 <<xyzMax[0],xyzMax[1],xyzMax[2];
    pts.push_back(pts0);
    pts0 <<xyzMax[0],xyzMax[1],xyzMin[2];
    pts.push_back(pts0);
    pts0 <<xyzMax[0],xyzMin[1],xyzMin[2];
    pts.push_back(pts0);
    pts0 <<xyzMax[0],xyzMin[1],xyzMax[2];
    pts.push_back(pts0);

    this->center = (xyzMax + xyzMin)/2;

    root = false;

    if (depth>0){

        leaf = false;

        xyzMax << ((pts[0][0]+pts[4][0])/2),pts[0][1],pts[0][2];
        xyzMin << pts[0][0],((pts[0][1]+pts[3][1])/2),((pts[0][2]+pts[1][2])/2);

        subNode[0] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[0]->pts[5];
        xyzMin << pts[0][0],((pts[0][1]+pts[3][1])/2),pts[2][2];

        subNode[1] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[1]->pts[7];
        xyzMin << pts[2];

        subNode[2] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[0]->pts[7];
        xyzMin << pts[0][0],pts[3][1],((pts[0][2]+pts[1][2])/2);

        subNode[3] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << pts[4];
        xyzMin << subNode[0]->pts[6];

        subNode[4] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[4]->pts[5];
        xyzMin << subNode[1]->pts[6];

        subNode[5] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[5]->pts[7];
        xyzMin << subNode[2]->pts[6];

        subNode[6] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[4]->pts[7];
        xyzMin << subNode[2]->pts[7];

        subNode[7] = new Node(xyzMin,xyzMax,depth-1);

    }else{

        leaf = true;

        for (int i=0;i<8;i++) subNode[i] = NULL;

    }

}

vector<Vector3d> Node::TreeNodeDim(const Mesh &myMesh){ //Get treenode dimension to surround mesh

    vector<Vector3d> Dim;

    Vector3d myMean(0,0,0), vecTemp;

    double dist,distemp;

    dist = 0;

    for(int j=0;j<myMesh.VertexNumber();j++){

        myMean += myMesh.Vertex(j);

    }

    myMean /= myMesh.VertexNumber();

    for(int j=0;j<myMesh.VertexNumber();j++){

        distemp = sqrt(pow(myMesh.vertices[j][0]-myMean[0],2)+pow(myMesh.vertices[j][1]-myMean[1],2)+pow(myMesh.vertices[j][2]-myMean[2],2));

        if (distemp>dist) dist = distemp;

    }

    vecTemp << dist,dist,dist;

    myMean += vecTemp;

    Dim.push_back(myMean);

    myMean -= (vecTemp*2);

    Dim.push_back(myMean);

    return Dim;

}


vector<Vector3d> Node::initNode(const Mesh &myMesh , int depth){ //Treenode initialisation

    vector<Vector3d> Dim;

    vector<Vector3d> centers;

    Dim = TreeNodeDim(myMesh);

    Vector3d xyzMax = Dim[0] + 0.001*Dim[0];
    Vector3d xyzMin = Dim[1] + 0.001*Dim[1];

    Vector3d pts0(xyzMin[0],xyzMax[1],xyzMax[2]);
    pts.push_back(pts0);
    pts0 << xyzMin[0],xyzMax[1],xyzMin[2];
    pts.push_back(pts0);
    pts0 <<xyzMin[0],xyzMin[1],xyzMin[2];
    pts.push_back(pts0);
    pts0 <<xyzMin[0],xyzMin[1],xyzMax[2];
    pts.push_back(pts0);
    pts0 <<xyzMax[0],xyzMax[1],xyzMax[2];
    pts.push_back(pts0);
    pts0 <<xyzMax[0],xyzMax[1],xyzMin[2];
    pts.push_back(pts0);
    pts0 <<xyzMax[0],xyzMin[1],xyzMin[2];
    pts.push_back(pts0);
    pts0 <<xyzMax[0],xyzMin[1],xyzMax[2];
    pts.push_back(pts0);

    center = (xyzMax + xyzMin)/2;

    root = true;

    if (depth>0){

        leaf = false;

        xyzMax << ((pts[0][0]+pts[4][0])/2),pts[0][1],pts[0][2];
        xyzMin << pts[0][0],((pts[0][1]+pts[3][1])/2),((pts[0][2]+pts[1][2])/2);

        subNode[0] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[0]->pts[5];
        xyzMin << pts[0][0],((pts[0][1]+pts[3][1])/2),pts[2][2];

        subNode[1] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[1]->pts[7];
        xyzMin << pts[2];

        subNode[2] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[0]->pts[7];
        xyzMin << pts[0][0],pts[3][1],((pts[0][2]+pts[1][2])/2);

        subNode[3] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << pts[4];
        xyzMin << subNode[0]->pts[6];

        subNode[4] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[4]->pts[5];
        xyzMin << subNode[1]->pts[6];

        subNode[5] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[5]->pts[7];
        xyzMin << subNode[2]->pts[6];

        subNode[6] = new Node(xyzMin,xyzMax,depth-1);

        xyzMax << subNode[4]->pts[7];
        xyzMin << subNode[2]->pts[7];

        subNode[7] = new Node(xyzMin,xyzMax,depth-1);

    }else{

        leaf = true;

        for (int i=0;i<8;i++) subNode[i] = NULL;

    }

    GetCenters(centers);

    SetWeight(myMesh, centers);

    SetCdf();

    return centers;

}

Node::~Node(){ //Destructeur

    for (int i=0;i<8;i++) delete subNode[i];

    ClearAll();

}


void Node::Draw(bool draw){ //Draw leafs

	if (leaf && draw){

        glDisable(GL_LIGHTING);

        glPointSize(2.0);
        glBegin(GL_POINTS);

        glColor3d(0,0,10);

        for(int i=0 ; i<8; i++ ){

            glVertex3d(pts[i][0],pts[i][1],pts[i][2]);

        }

        glEnd();
        glEnable(GL_LIGHTING);

	}

    if (subNode[0] != NULL){

        for (int i=0;i<8;i++) subNode[i]->Draw(draw);

    }

}

void Node::DrawBound(bool draw, vector<Vector3d> &centers ,multimap<int, int> &indCenter2pts){//Draw boundaries (not finished)

    if (leaf){

        vector<Vector3d>::iterator myItCent;

        myItCent = find(centers.begin(),centers.end(), center);

        int count,dist;

//        cout<<"center : "<<center<<endl<<endl;

        if (myItCent!=centers.end()){

//            cout<<"dist : "<<distance(centers.begin(),myItCent)<<endl<<endl;

//    cout<<"first : "<< distance(centers.begin(),myItCent);

//    multimap<int, int>::iterator myItInd;

//    if (myItCent != centers.end()){

//        myItInd = indCenter2pts.find(distance(centers.begin(),myItCent));

            dist = distance(centers.begin(),myItCent);

            count = indCenter2pts.count(dist);

//            cout<<"indCen2pts size : "<<indCenter2pts.size()<<endl<<endl;
//
//            cout<<"count : "<<count<<endl<<endl;stop(1);


            if ( count != 0 && draw){

        //        stop(count);

                glDisable(GL_LIGHTING);

                glPointSize(2.0);
                glBegin(GL_POINTS);

                glColor3d(0,0,10);

                for(int i=0 ; i<8; i++ ){

                    glVertex3d(pts[i][0],pts[i][1],pts[i][2]);

                }

                glEnd();
                glEnable(GL_LIGHTING);

        }







        }

//        count = indCenter2pts.count(distance(centers.begin(),myItCent));
//
//        cout<<"count : "<<count<<endl<<endl;

//    }else{

//        myItInd = indCenter2pts.end();

//        stop(indCenter2pts.count(distance(centers.begin(),myItCent)));

//    }

//    stop(count);



	}else{

        for (int i=0;i<8;i++) subNode[i]->DrawBound(draw,centers,indCenter2pts);

    }

}

void Node::Disp(){//Display leafs point values

    if (leaf){

        for(int i=0 ; i<8; i++ ){

            cout<<"Node center : "<<i<<" : "<<pts[i]<<endl;

        }
    }else{

        for (int i=0;i<8;i++) subNode[i]->Disp();

    }

    cout<<endl<<endl;
}

int Node::size(){//Return number of leafs

    int temp = 1;

    if (subNode[0] == NULL){
        return temp;
    }else{
        temp = 8;
        return  temp *= subNode[0]->size();
    }
}


void Node::GetCenters(vector<Vector3d> &myCenters){//Get leafs centers coordinates

    if (leaf == true){
        myCenters.push_back(center);
    }else{
        for (int i=0;i<8;i++) subNode[i]->GetCenters(myCenters);
    }
}


bool Node::SetWeight(const Mesh &myMesh,vector<Vector3d> &centers){//Count number of vertices per leafs

    if (root){

        double distemp, distemp2;

        int tempWeight;

        int indCen;

        multimap<int, int> ptsCent;
        multimap<int, int>::iterator myIt;

        for (int pts=0;pts<myMesh.VertexNumber();pts++){

            distemp2 = numeric_limits<double>::max();

            for (int cent=0;cent<centers.size();cent++){

                distemp = sqrt( pow(myMesh.vertices[pts][0] - centers[cent][0],2) + pow(myMesh.vertices[pts][1] - centers[cent][1],2) + \
                             pow(myMesh.vertices[pts][2] - centers[cent][2],2));

                if (distemp<distemp2){
                    indCen = cent;
                    distemp2 = distemp;
                }

            }

            ptsCent.insert(make_pair((int)indCen,(int)pts));

        }

        indCenter2pts = ptsCent;

        return true;
    }else{
        return false;
    }

}

bool Node::SetCdf(){//Compute Cumulative density function

    if(root){

        int tempWeight = 0;
        int indMove = 0;

        map<int, int>::iterator myIt;
        myIt = indCenter2pts.begin();

        while (myIt!=indCenter2pts.end()){

            tempWeight += indCenter2pts.count(myIt->first);
            indMove = indCenter2pts.count(myIt->first);

            nodeCdf.insert(make_pair((int)tempWeight,(int)myIt->first));

            advance(myIt,indMove);

        }

        return true;
    }else{
        return false;
    }
}

void Node::ClearAll(){//Clear treenode

    indCenter2pts.clear();

    pts.clear();

    nodeCdf.clear();

}
