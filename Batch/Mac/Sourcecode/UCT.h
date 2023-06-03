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
#include <chrono>
#include "Strategy.h"

auto start_time = std::chrono::high_resolution_clock::now();
double duration = 0.0;

#define C (std::sqrt(2)) // define constant whatever you want 
using NodePosi = Node*;
struct Node{
private:
    /*------------MEMBERS--------*/
    int ** board_;
    double value;
    int row;          // M
    int column;       // N
    int * top_;
    bool isterminal;
    int n;            // number of visiting
    int r;            // result of the node
    int lastx,lasty;  // last position that opponent put (not used)
    int x,y;
    int no_x,no_y;    // banned position
    NodePosi parent;
    NodePosi * children;
    int expandables;        // for expand, also can track how many times this node has been expanded
    bool chance;
    // int* expandable_nodes;  // for expand
    /*----------GENERATORS & ------------*/
public:
    Node() {}
    Node(int ** board,const int M, const int N, int* top,const int noX, const int noY, bool is_terminal = false, bool chance_ = false)
    : value(0.0),row(M), column(N),isterminal(is_terminal),chance(chance_),n(0),r(0)
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
            // expandable_nodes = new int[N];

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
        // delete[] expandable_nodes;
    }

    /*-----------INTERFACES-----------*/
    bool& is_terminal() {return isterminal;}
    bool is_expandable() {return expandables > 0;}
    //------------ getters-------------
    const int& get_x() {return x;}
    const int& get_y() {return y;}
    const int& get_no_x() {return no_x;}
    const int& get_no_y() {return no_y;}
    int& get_n() {return n;}
    int& get_r() {return r;}
    void set_value() {value = (double)r / (double)n ;}
    double get_value() {return value;}
    NodePosi get_parent() { return parent;}
    bool& get_chance() {return chance;}
    void set_parent(NodePosi to_be_parent) {parent = to_be_parent;}

    /*-----------FUNCTIONS------------*/
    NodePosi tree_policy();
    int rollout();
    NodePosi expand();
    void backup(int delta);
    NodePosi bestchild(const double c=C);
};

NodePosi Node::tree_policy(){
    NodePosi v = this;
    while(!v->is_terminal()){
        if(v->is_expandable()){
            return v->expand();
        } else {
            v = v->bestchild();
        }
    }
    return v;
};

int Node::rollout(){ // Default policy
    if(this->is_terminal()){// if node is terminal state
        
    } else { // have to simulate

    }
};

NodePosi Node::expand(){
    bool chance_  = this->get_chance() ? false : true;
    int turn = chance_? 2 : 1;
    for(int i = 0; i < column; i++){
        if(top_[i] > 0){
            if(children[i] == nullptr){ // if the node is not expanded yet, it will be nullptr
                // will put on (top_[i]-1,i)
                // check banned point
                board_[top_[i]-1][i] = turn; 
                int step = 1;
                if( i == get_no_y() && top_[i]-2 == get_no_x()) step=2;
                top_[i] -= step;
                children[i] = new Node(board_,row,column,top_,no_x,no_y,false,chance_);
                children[i]->set_parent(this);
                board_[top_[i]-1][i] = 0; top_[i]+= step;
                return children[i];
            }
        }
    }
};

void Node::backup(int delta){
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
    NodePosi b_child{nullptr};
    double res = -1e16; double dist = 0;
    for(int i=0; i< column ; i++){
        if(children[i] == nullptr) continue; //
        NodePosi child = children[i];
        if(child->get_n() == 0) //  actually, this part of the code won't be run 
            return child;
        double value_ = child->get_value() + ((double)C)*(std::log((double)this->get_n()/(double)child->get_n()));
        if( res <= value_){
            if(res == value_){ // compare dist
                double dist_ = abs((((double)column)-1)/2 - i);
                if(dist_ < dist) {
                    b_child = child;
                    dist = dist_;
                }
            } else {
                res = value_;
                b_child = child;
                dist = abs((((double)column)-1)/2 - i);
            }
        }
    }
    return b_child;
};
#endif
