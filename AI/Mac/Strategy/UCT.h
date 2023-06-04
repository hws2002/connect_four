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
#include "Judge.h"

// define deafult policy for simulation. Will be used in rollout
#define DEFAULT_POLICY 1 // 1: random , 2 : middle, 3: make-7
#define C (std::sqrt(2)) // define constant whatever you want 
struct Node;
using NodePosi = Node*;
struct Node{
private:
    /*------------MEMBERS--------*/
    int ** board_;
    double value;
    int row;          // M
    int column;       // N
    int * top_;
    int n;            // number of visiting
    int r;            // result of the node
    // int lastx,lasty;  // last position that opponent put (not used)
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
    Node(const int M, const int N, const int* top,int** board, const int current_x, const int current_y,const int noX, const int noY, const int expandables_, bool chance_ = false)
    : value(0.0),row(M), column(N),chance(chance_),n(0),r(0),
        x(current_x),y(current_y),no_x(noX),no_y(noY),expandables(expandables_),parent(nullptr)
        {
            // copy board
            board_ = new int*[row];
            for(int i=0; i<row;i++){
                board_[i] = new int[column];
                for(int j=0; j< column; j++){
                    board_[i][j] = board[i][j];
                }
            }
            // copy top
            int zeroCounter = 0;
            top_ = new int[column];
            for (int i=0; i<column; i++){
                top_[i] = top[i];
                if(top_[i] == 0) zeroCounter++;
            }
            expandables = column - zeroCounter;

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
    bool is_expandable() {return expandables > 0;}
    int get_expandables(){return expandables;}
    void set_expandables(int num) {expandables = num;}
    //------------ getters-------------
    const int get_x() {return x;}
    const int get_y() {return y;}
    const int get_no_x() {return no_x;}
    const int get_no_y() {return no_y;}
    int get_n() {return n;}
    int get_r() {return r;}
    void set_value() {value = (double)r / (double)n ;}
    double get_value() {return value;}
    NodePosi get_parent() { return parent;}
    bool get_chance() {return chance;}
    //------------ setters-------------
    void set_n(int n_) {n = n_;}
    void set_r(int r_) {r = r_;}
    void set_parent(NodePosi to_be_parent) {parent = to_be_parent;}
    /*-----------FUNCTIONS------------*/
    
    NodePosi tree_policy();
    int rollout();
    NodePosi expand();
    void backup(int delta);
    NodePosi bestchild(const double c=C);
    bool is_terminal();
    bool is_banned(const int p_x, const int p_y);
};

class UCT{
    private :
    int row, column;
    int ban_x,ban_y;
    bool expanded{true};
    NodePosi root;
public:

    /*-----------INTERFACES-----------*/
    //-----------Getters---------------
    const Node* get_root() const { return root; }
    const int& get_row() { return row; }
    const int& get_column() { return column; }
    const int& get_ban_x() { return ban_x; }
    const int& get_ban_y() { return ban_y; }
    bool& get_expanded() { return expanded; }
    int* weight{NULL};
    int full_weight{0};

    UCT(int M, int N, int* top, int** board,int noX, int noY, bool expanded = true)
        : row(M),column(N),ban_x(noX),ban_y(noY){
            root = new Node(M,N,top,board,-1,-1,noX,noY,N,expanded);
            weight = new int[N];
            for(int i=0; i<N; i++){
                weight[i] = 0;
            }
        }

    ~UCT(){ delete this->root; delete[] this->weight;}
};

#endif