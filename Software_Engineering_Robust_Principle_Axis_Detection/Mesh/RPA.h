/////////////////////////// Student Work /////////////////////////////////

#ifndef RPA_H_INCLUDED
#define RPA_H_INCLUDED

#include "TreeNode.h"

struct GenEigen { //Structure containing EigenVectors and Mean

    Vector3d eigenMean;

    Vector3d eigenVec1;

    void CopyEig(const Mesh&); //Copy EigenVectors and Mean from a mesh

};

void RemainingPoints(Mesh&,Mesh&,vector<int>); //Extract points, referenced by their indexes, from a mesh and add
                                               //them to another

multimap<double, int> Distance2Eigen1(const Mesh&, const GenEigen&); //Compute residual distances from mesh vertices to main EigenVector

multimap<double, int> Distance2Eigen1(const Node&,const vector<Vector3d>&, const GenEigen&); //Compute residual distances from projected Node centers to
                                                                                 //main EigenVector

vector<int> GetClosestPoint(multimap<double, int>,int,double); //Get indexes of closest points regarding a threshold (rMax)

double SetRmax( Mesh&); //Compute residual distances and keep the greater

vector<int> RandomKPoints(Node&,Mesh&, int); //Select points with a weighted random

vector<Vector3d> ProjectNodeCenters(const vector<Vector3d>&, const Mesh&); //Project Node centers onto mesh EigenSpace

void First_Part(const Mesh&, Mesh&, Mesh&, Node&, const int, const int, const int); //First part of the whole algorithm (algo2)

void Second_Part(Mesh&, Mesh&, const int, const int); //Second part of the whole algorithm (algo1)


#endif // RPA_H_INCLUDED
