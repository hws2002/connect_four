/********************************************************
*	Node.h : Node类                                     *
*	含有说                                               *
*	hwooseok123@gmail.com                               *
*	2023.5                                              *
*********************************************************/

#ifndef UCT_H_
#define UCT_H_
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#include <cmath>
#define C (sqrt(2)) // define constant whatever you want 
using NodePosi = Node*;
struct Node{
    /*------------MEMBERS--------*/
    int ** board_;
    double value;
    int row;          // M
    int column;       // N
    int * top_;
    bool isleaf;
    int n;            // number of visiting
    int w;            // number of win
    int lastx,lasty; // last position that opponent put
    int no_x,no_y;    // banned position
    int expandables;
    NodePosi parent;
    NodePosi * children;
    int* expandable_nodes;
    /*----------GENERATORS & ------------*/
    Node() {}
    Node(int ** board,const int M, const int N, int* top,const int noX, const int noY, bool is_leaf = true)
    : value(0.0),row(M), column(N),isleaf(is_leaf), n(0),w(0)
        ,lastx(-1),lasty(-1),no_x(noX),no_y(noY),expandables(true)
        ,parent(nullptr){
            // copy board
            board_ = new int*[row];
            for(int i=0; i<row;i++){
                board_[i] = new int[column];
                for(int j=0; j< column; j++){
                    board_[i][j] = board[i][j];
                }
            }
            // copy top
            top_ = new int[column];
            for (int i=0; i<column; i++){
                top_[i] = top[i];
            }

            // initiate children list
            children = new NodePosi[column];
            // create expandable nodes list
            expandable_nodes = new int[N];

            // prepare expandable_nodes list

        };
    ~Node(){
        // delete board
        for(int i=0; i < row; i++){
            delete[] board_[i];
        }
        delete[] board_;
        // delte children
        for(int i=0; i < column; i++){
            if(children[i]) delete children[i];
        }
        delete[] children;
        // delete top
        delete[] top_;
        delete[] expandable_nodes;
    }

    /*-----------INTERFACES-----------*/
    bool& is_leaf() {return isleaf;}
    bool is_expandable() {return expandables > 0;}
    int& get_n() { return n;}
    int& get_w() { return w;}
    void set_value(){value = (double)w / (double)n ;}
    NodePosi get_parent() { return parent;};

    /*-----------FUNCTIONS------------*/
    NodePosi tree_policy();
    int rollout();
    NodePosi expand();
    void backup(int delta);
    Node* bestchild(const double c=C);
};

NodePosi Node::tree_policy(){
    NodePosi v = this;
    while(!v->is_leaf()){
        if(v->is_expandable()){
            return v->expand();
        } else {
            v = v->bestchild();
        }
    }
    return v;
};

int Node::rollout(){
    // @TODO : rollout = default policy
    // ????????????????????????????????
};

NodePosi Node::expand(){
    // @TODO : select yet explored node,
};

void Node::backup(int delta){
    // @TODO : backpropagate till root 
    NodePosi v = this;
    while(v){
        v->get_n() += 1;
        v->get_w() += delta;
        v->set_value();
        delta = 1 - delta;
        v = v->get_parent();
    }

};

Node* Node::bestchild(const double c=C){ 
    // @TODO : traverse all children of 'this' 
    //          and find maximum value
    NodePosi b_child{nullptr};
    double res = 0;
    // for{
        double value_ = value + C*(log(this->get_parent()->get_n()/get_parent()));
        res = MAX(res,value_);
    // }

};

/*-------------------------------------------------------*/
// class Node{
// public:
//     // int ** _board;
//     double win_rate;
//     int row;          // M
//     int column;       // N
//     bool isleaf;
//     int n;            // number of visiting
//     int w;            // number of win
//     int lastx,lasty; // last position that opponent put
//     int no_x,no_y;    // banned position
//     bool expandable;
//     /*------------------------*/
//     Node * parent{NULL};
//     Node ** children;
// // Constructor
//     Node() : isleaf(true){
//     }
//     Node(const int M, const int N, const int noX, const int noY)
//     : win_rate(0.0),row(M), column(N),isleaf(true), n(0),w(0)
//         ,lastx(-1),lasty(-1),no_x(noX),no_y(noY),expandable(true){
//     };

//     Node* tree_policy();
//     void bestchild();
// };

// Node* Node::tree_policy(){

// };
// void Node::bestchild(){
//     // @TODO : traverse all children of 'this' 
//     //          and find maximum value
//     double res = 0;
// };
#endif
