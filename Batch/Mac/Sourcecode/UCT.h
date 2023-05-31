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
private:
    /*------------MEMBERS--------*/
    int ** board_;
    double value;
    int row;          // M
    int column;       // N
    int * top_;
    bool isleaf;
    int n;            // number of visiting
    int r;            // result of the node
    int lastx,lasty;  // last position that opponent put (not used)
    int x,y;
    int no_x,no_y;    // banned position
    NodePosi parent;
    NodePosi * children;
    int expandables;        // for expand, also can track how many times this node has been expanded
    int* expandable_nodes;  // for expand
    /*----------GENERATORS & ------------*/
public:
    Node() {}
    Node(int ** board,const int M, const int N, int* top,const int noX, const int noY, bool is_leaf = true)
    : value(0.0),row(M), column(N),isleaf(is_leaf), n(0),r(0)
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

            // initiate array 'children' with nullptrs
            children = new NodePosi[column]();
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
    //------------ getters-------------
    const int& get_x() {return x;}
    const int& get_y() {return y;}
    int& get_n() {return n;}
    int& get_r() {return r;}
    void set_value() {value = (double)r / (double)n ;}
    double get_value() {return value;}
    NodePosi get_parent() { return parent;}
    void set_parent(NodePosi to_be_parent) {parent = to_be_parent;}

    /*-----------FUNCTIONS------------*/
    NodePosi tree_policy();
    int rollout();
    NodePosi expand();
    void backup(int delta);
    NodePosi bestchild(const double c=C);
};

NodePosi Node::tree_policy(){
    // @todo : 
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
    //         simulate game whose initial state is this board

};

NodePosi Node::expand(){
    // @TODO : select yet explored node,
    /*
        - only called when 'this' node is expandable 
        - children array is initialized with nullptr at the first site,
    */
    for(int i = 0; i < column; i++){
        if(top_[i] > 0){
            if(children[i] == nullptr){
                // will put on (top_[i]-1,i)
                board_[top_[i]-1][i] = 2;
                children[i] = new Node(board_,row,column,top_,no_x,no_y,true);
                children[i]->set_parent(this);
                board_[top_[i]-1][i] = 0;
                return children[i];
            }
        }
    }
};

void Node::backup(int delta){
    // @TODO : backpropagate till root 
    //          consider tie!!
    NodePosi v = this;
    while(v){
        v->get_n() += 1;
        v->get_r() += delta;
        v->set_value();
        delta = -1*delta;
        v = v->get_parent();
    }
};

NodePosi Node::bestchild(const double c=C){ 
    // @TODO : traverse all children of 'this' 
    //          and find maximum value
    //          b_child won't be null 
    NodePosi b_child{nullptr};
    double res = 0;
    for(int i=0; i< column ; i++){
        if(children[i] == nullptr) continue;
        NodePosi child = children[i];
        if(child->get_n() == 0)
            return child;
        double value_ = child->get_value() + ((double)C)*(log((double)this->get_n()/(double)child->get_n()));
        if( res < value_){
            res = value_;
            b_child = child;
        }
    }
    return b_child;
};

#endif
