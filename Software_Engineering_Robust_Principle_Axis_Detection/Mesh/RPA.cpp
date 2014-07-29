/////////////////////////// Student Work /////////////////////////////////

#include "math.h"
#include <algorithm>
#include <map>
#include "RPA.h"


void GenEigen::CopyEig(const Mesh &myMesh){ //Copy EigenVectors and Mean from a mesh

    eigenMean = myMesh.eigenMean;

    eigenVec1 = myMesh.eigenVec[0];
}

void RemainingPoints(Mesh &Cn, Mesh &Q, vector<int> ind){ //Extract points, referenced by their indexes, from a mesh and add
                                                          //them to another

    int i = ind.size(); //Number of point to extract

    for (int k=0;k<i;k++){

        Q.AddVertex( Cn.Vertex( ind[k] ) ); //Copy points into 'Q'

        Cn.vertices.erase( Cn.vertices.begin()+ind[k] ); //Erase points into 'Cn'

    }
}


multimap<double, int> Distance2Eigen1(const Mesh &Crem, const GenEigen &genEig){ //Compute residual distances from mesh vertices to main EigenVector

    double d, k, xh, yh, zh, pointX, pointY, pointZ, ua, ub, uc, ox, oy, oz, resDist;

    multimap<double, int> dist; //Contains distances (key) and points indexes (value)

    ua = genEig.eigenVec1[0]; //Main EigenVector
    ub = genEig.eigenVec1[1];
    uc = genEig.eigenVec1[2];

    ox = genEig.eigenMean[0]; //Eigen origin
    oy = genEig.eigenMean[1];
    oz = genEig.eigenMean[2];


    for (int i=0; i<Crem.VertexNumber(); i++){ //For all vertex

        pointX = Crem.vertices[i][0]; //Get points coordinates
        pointY = Crem.vertices[i][1];
        pointZ = Crem.vertices[i][2];

        d = - ( (ua*pointX) + (ub*pointY) + (uc*pointZ) ); //Compute distance from a vertex to its orthogonal projection onto
                                                           //a line defined by main eigenVector through mean value

        k = - ( (ua*ox) + (ub*oy) + (uc*oz) + d ) / ( (ua*ua) + (ub*ub) + (uc*uc) );

        xh = (ox + ua*k);
        yh = (oy + ub*k);
        zh = (oz + uc*k);

        resDist = sqrt(pow((pointX-xh),2) + pow((pointY-yh),2) + pow((pointZ-zh),2));

        dist.insert(make_pair((double)resDist, (int)i)); //Insert pair : distance (key) and point index (value)

    }

    return dist; //Return distances

}

multimap<double, int> Distance2Eigen1(const Node& myNode,const vector<Vector3d> &centers, const GenEigen& genEig){
//Compute residual distances from projected Node centers to main EigenVector

    double d, k, xh, yh, zh, pointX, pointY, pointZ, ua, ub, uc, ox, oy, oz, resDist;

    int nodeInd, nodeWeight;

    multimap<double, int> dist;

    multimap<int, int>::const_iterator myIt;
    myIt = myNode.indCenter2pts.begin();

    ua = genEig.eigenVec1[0]; //Main EigenVector
    ub = genEig.eigenVec1[1];
    uc = genEig.eigenVec1[2];

    ox = genEig.eigenMean[0]; //Eigen origin
    oy = genEig.eigenMean[1];
    oz = genEig.eigenMean[2];

    while (myIt!=myNode.indCenter2pts.end()){ //For all projected Node centers

        nodeInd = myIt->first;

        pointX = centers[nodeInd][0]; //Get centers coordinates
        pointY = centers[nodeInd][1];
        pointZ = centers[nodeInd][2];

        d = - ( (ua*pointX) + (ub*pointY) + (uc*pointZ) ); //Compute distance from a vertex to its orthogonal projection onto
                                                           //a line defined by main eigenVector through mean value

        k = - (( (ua*ox) + (ub*oy) + (uc*oz) + d ) / ( (ua*ua) + (ub*ub) + (uc*uc) ));

        xh = (ox + ua*k);
        yh = (oy + ub*k);
        zh = (oz + uc*k);

        resDist = sqrt(pow((pointX-xh),2) + pow((pointY-yh),2) + pow((pointZ-zh),2));

        dist.insert(make_pair((double)resDist, (int)nodeInd)); //Insert pair : distance (key) and node index (value)

        nodeWeight = myNode.indCenter2pts.count(nodeInd); //Count how many values have the same key

        advance(myIt,nodeWeight); //Go to next key which is different from the current one

    }

    return dist;

}

vector<int> GetClosestPoint(multimap<double, int> myMap, int m, double rmax){ //Get indexes of closest points regarding a threshold (rMax)

    vector<int> dum;

    multimap<double, int>::iterator myIt = myMap.begin();

    for (int i=0;i<m;i++){ //For m closest points

        if (myIt->first < rmax){            //If point distance is less than rMax
            dum.push_back(myIt->second);    //then get its index
        }else{
            sort(dum.rbegin(),dum.rend());  //Else, sort descending points indexes and stop loop
            return dum;
        }
        myIt++; //Go to next point
    }
    sort(dum.rbegin(),dum.rend()); //sort descending points indexes
    return dum;
}


vector<int> RandomKPoints(Node &myNode,Mesh &myMesh, int k){ //Select points with a weighted random

    int randomInd, nodeInd, nbOfPts, indPts, randRange;

    vector<int> myVec; //K points containers

    multimap<int, int>::iterator myItCdf; //Iterators

    multimap<int, int>::iterator myItNode;


    for (int i=0;i<k;i++){ //for k points

        randomInd = rand() %myMesh.VertexNumber() ; //Rand a number in the range 1 - number of vertices

        myItCdf = myNode.nodeCdf.upper_bound(randomInd); //Go to the first cell whose its key is greater or equal
                                                         //to the rand

        nodeInd = myItCdf->second; //Get node index (key) of that cell

        nbOfPts = myNode.indCenter2pts.count(nodeInd); //Count number of points within that node (values associated
                                                       //to that key)

        myItNode = myNode.indCenter2pts.find(nodeInd); //Go to that node (key)

        advance(myItNode,rand() %nbOfPts); //Select randomly a point (index) of that node (a value associated to that key)

        myVec.push_back(myItNode->second); //Store that point

    }

    sort(myVec.rbegin(), myVec.rend()); //Sort descending

    return myVec;
}


double SetRmax( Mesh &myMesh){ //Compute residual distances and keep the greater

    double rMaxTemp;

    multimap<double, int> dist2eig;
    multimap<double, int>::iterator myIt;

    GenEigen myGenEigen;

    myGenEigen.CopyEig(myMesh); //Get main EigenVector

    dist2eig = Distance2Eigen1(myMesh, myGenEigen); //Compute residual distances

    myIt = dist2eig.end();

    myIt--; //Iterator to greater distance

    return rMaxTemp = myIt->first; //Return greater distance

}


vector<Vector3d> ProjectNodeCenters(const vector<Vector3d> &centers, const Mesh &myMesh){ //Project Node centers onto mesh EigenSpace

    vector<Vector3d> projectedNodeCenters;

    Matrix3d tempEigenVec; //Declare feature matrix

    tempEigenVec.row(0) = myMesh.eigenVec[0]; //Set EigenVectors as rows of the feature matrix
    tempEigenVec.row(1) = myMesh.eigenVec[1];
    tempEigenVec.row(2) = myMesh.eigenVec[2];

    Vector3d tempCenter;

    for (int i=0;i<centers.size();i++){ //For each center

        tempCenter = centers[i] - myMesh.eigenMean; //Substract Mean

        tempCenter = tempEigenVec * tempCenter; //Multiply feature matrix and center coordinates

        projectedNodeCenters.push_back(tempCenter); //Store projected point

    }

    return projectedNodeCenters;

}


void First_Part(const Mesh &Original, Mesh &Crem, Mesh &Q, Node &myNode, const int max_iter, const int k, const int depth){
    //Algorithm 2 : Octree + LMS

    int i=0;

    Mesh tempQ,tempCrem; //Temporary mesh Q & Crem

    double rMin = numeric_limits<double>::max();

    double rHalf;

    int medDist;

    vector<int> tempVec;
    vector<int> finalVec;

    vector<Vector3d> nodeCenters;

    GenEigen myGenEigen;

    multimap<double, int> dist2eig;
    multimap<double, int>::iterator myIt;

    nodeCenters = myNode.initNode(Original,depth); //Construct the Octree

    while(i++<=max_iter){ //For max_iter times

        tempCrem = Original; //tempCrem set as the original mesh

        tempQ.ClearAll();

        tempVec = RandomKPoints(myNode,tempCrem,k); //Rand K points

        RemainingPoints(tempCrem,tempQ,tempVec); //Exctract previous k points from tempCrem and store them within tempQ

        tempQ.Meshpca(); //Compute PCA over previous k points

        myGenEigen.CopyEig(tempQ); //Copy EigenVectors and Mean

        dist2eig = Distance2Eigen1(myNode, nodeCenters, myGenEigen); //Compute residual distances

        medDist = dist2eig.size()/2; //Get index of median distance

        myIt = dist2eig.begin();

        advance(myIt, medDist);

        rHalf = myIt->first;

        if (rHalf<rMin){

            rMin = rHalf;

            finalVec = tempVec;

            Q = tempQ;

            Crem = tempCrem;

        }

    }

    cout<<"Point selected index : "<<finalVec[0]<<" "<<finalVec[1]<<" "<<finalVec[2]<<" "<<finalVec[3]<<endl<<endl;

}


void Second_Part(Mesh& Crem, Mesh& Q, const int MAX_Iter, const int m){

    multimap<double, int> dist2eig;

    vector<int> closestpoints;

    GenEigen myGenEigen;

    int iter = 0;

    int mpts = 0;

    double alpha = 3;

    double rMax = SetRmax(Q);

    rMax *= alpha;

    cout<<"rMax : "<<rMax<<endl<<endl;

    Q.Meshpca();

    do{
        myGenEigen.CopyEig(Q);

        dist2eig = Distance2Eigen1(Crem, myGenEigen);//check

        closestpoints = GetClosestPoint(dist2eig,m,rMax);

        RemainingPoints(Crem,Q,closestpoints);//check

        mpts = closestpoints.size();

        Q.Meshpca();

    }while (iter++ < MAX_Iter && mpts==m);

}
