/////////////////////////// Student Work /////////////////////////////////

#ifndef TREENODE_H_INCLUDED
#define TREENODE_H_INCLUDED


class Node{

    public :

    //Constructor
    Node();
    Node(Vector3d ,Vector3d , int);

    //Destructor
    ~Node();

    //Node points
    vector<Vector3d> pts;

    //Pointers to subNodes
    Node *subNode[8];

    //List of points which belong to Node
    multimap<int, int> indCenter2pts;

    //Node gravity center
    Vector3d center;

    //Leafs cdf
    map<int,int> nodeCdf;

    //Node leaf
    bool leaf;

    //Node first root
    bool root;

    ////Node functions

    //Compute surrounding box diagonal
    vector<Vector3d> TreeNodeDim(const Mesh&);

    //Initialise octree
    vector<Vector3d> initNode(const Mesh& ,int);

    //Get number of leafs
    int size();

    //Draw leafs
    void Draw(bool);

    //Display leafs center value
    void Disp();

    //Draw boundaries nodes
    void DrawBound(bool,vector<Vector3d>& ,multimap<int, int>&);

    //Reset treenode
    void ClearAll();

    //Exctract leafs centers
    void GetCenters(vector<Vector3d>&);

    //Compute distances from each point to each node center
    //and store node index and points indexes that it contains
    bool SetWeight(const Mesh&, vector<Vector3d>&);

    //Compute the cumulative density function
    bool SetCdf();

};

/*  Node Points

         1---5
        /|  /|
       0---4 |
       | 2-|-6
       |/  |/
       3---7

*/


#endif // TREENODE_H_INCLUDED
