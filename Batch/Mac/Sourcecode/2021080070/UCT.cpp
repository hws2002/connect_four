#include "UCT.h"

bool Node::is_terminal(){
    if(this->get_parent() == nullptr) return false;
    if(userWin(x,y,row,column,board_))
        return true;
    if(machineWin(x,y,row,column,board_))
        return true;
    if(isTie(column,top_))
        return true;
    return false;
}

/**
 * @brief Check if a point one step up from (p_x,p_y) is banned or not
 * 
 * @param p_x 
 * @param p_y 
 * @return true : if the point is banned
 * @return false : if the point is free
 */
bool Node::is_banned(const int p_x, const int p_y){
    return (p_x == get_no_x() && p_y == get_no_y()) ? true : false;
}

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

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param row 
 * @param column 
 * @param board 
 * @param top 
 * @param chance 
 * @return int 
 */

int is_finished(int x, int y, int row, int column, int ** board,int * top, int chance){
    switch (chance) {
        case true:
            if(userWin(x,y,row,column,board))
                return 1;
            if(isTie(column,top))
                return 0;
            break;
        case false:
            if(machineWin(x,y,row,column,board))
                return -1;
            if(isTie(column,top))
                return 0;
            break;
        default:
            return -2;
            break;
    }
    return -2;
}

/**
 * @brief playout
 * 
 * @return int 
 */
int Node::rollout(){
    //prepare new board
    int** _board = new int*[row];
    for(int i=0; i<row;i++){
        _board[i] = new int[column];
        for(int j=0; j<column;j++){
            _board[i][j] = board_[i][j];
        }
    }

    //prepare new top
    int nonZeroCount = 0;
    int * _top = new int[column];
    for(int i=0; i< column; i++){
        _top[i] = top_[i];
        if(_top[i] > 0){
            nonZeroCount++;
        }
    }

    //prepare point where players will put
    int x_ = get_x();
    int y_ = get_y();
    bool chance_ = get_chance();
    int delta = -2;
    while(1){
        delta = is_finished(x_,y_,row,column,_board,_top,chance_);
        if(delta != -2){
            return delta;
        }
        // DETERMINE NEXT POINT
        switch(DEFAULT_POLICY){
            case 1: // random policy
                if(nonZeroCount>0){
                    int randomIndex = rand() & nonZeroCount;
                    int currentIndex = 0;
                    for (int i=0; i<column; ++i){
                        if(_top[i] > 0){
                            if(currentIndex == randomIndex){
                                x_ = --_top[i];
                                y_ = i;
                            break;
                            }
                        currentIndex++;
                        }
                    }
                }
                break;
            case 2: // middle the best
                break;
            case 3: // make-7
                break;
            default:
                break;
        }
        //check banned point
        if(Node::is_banned(x_-1,y_)){_top[y_]--;}
        //put stone
        _board[x_][y_] = chance_ ? 1 : 2; 
        if(_top[y_] == 0) nonZeroCount--;
        // change turn
        chance_ = chance_ ? false : true;
    }

    // delete board and top
    for (int i=0 ; i< row ;i++){
        delete[] _board[i];
    }
    delete[] _board;
    delete[] _top;
    return delta; // won't be run
};

/**
 * @brief select yet explored node
 * @note only called when 'this' node is expandable 
 *       children array is initialized with nullptr at the first site
 * @return NodePosi 
 */
NodePosi Node::expand(){
    bool chance_  = this->get_chance() ? false : true;
    for(int i = 0; i < column; i++){
        if(top_[i] > 0){
            if(children[i] == nullptr){ // if the node is not expanded yet, it will be nullptr
                // will put on (top_[i]-1,i)
                // check banned point
                board_[top_[i]-1][i] =  chance_? 2 : 1; 
                int step = 1;
                if( is_banned(top_[i]-2,i)) { // 2 step upper is banned position
                    step = 2;
                    top_[i] -= step;
                    // get expandables
                    children[i] = top_[i] == 0 ? new Node(row,column,top_,board_,top_[i]+1,i,no_x,no_y,get_expandables()-1,chance_) 
                                                : new Node(row,column,top_,board_,top_[i]+1,i,no_x,no_y,get_expandables(),chance_);
                } else {
                    top_[i] -= step;
                    children[i] = top_[i] == 0 ? new Node(row,column,top_,board_,top_[i],i,no_x,no_y,get_expandables()-1,chance_) 
                                                : new Node(row,column,top_,board_,top_[i],i,no_x,no_y,get_expandables(),chance_);
                }
                children[i]->set_parent(this);
                board_[top_[i]-1][i] = 0; top_[i]+= step;
                return children[i];
            }
        }
    }
};

/**
 * @brief backpropagate till root 
 * ! consider tie
 * @param delta 
 */
void Node::backup(int delta){
    NodePosi v = this;
    while(v != nullptr){
        v->set_n(v->get_n()+1);
        v->set_r(v->get_r()+delta);
        v->set_value();
        delta = -1*delta;
        v = v->get_parent();
    }
};

/**
 * @brief traverse all children and find maximum value
 * @note b_child won't be null 
 * @param c 
 * @return NodePosi 
 */

NodePosi Node::bestchild(const double c){ 
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
