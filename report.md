
<font size = "6em" ><bold>**实验3: Connect4** &nbsp;&nbsp;</bold></font> <small>2021080070 计14 韩佑硕 </small> 

# **Pre requisite**
___
## Judge.h, Judge.cpp
userWin & computerWin : 判断谁赢
isTie 必须在调用userWin和computerWin之后

## **Point.h**
棋盘中左上角为坐标原点.纵向为x坐标轴,横向为y坐标轴

## **Strategy.h, Strategy.cpp**
策略模块中动态定义的对象必须由策略模块中的函数来释放


# **实现方法**
___
本人采用 Monte Carlo Tree Search + Upper Confidence Bounds。  
下面给MCTS的四大步骤给出了详细的解析。

## **1. Selection Strategy**
---
### **1-1. UCT**
已经说明了采用UCT方法，因此selection strategy 就是 UCB.
在UCT中，孩子节点k的选择是由这个方程决来进行的  
 $k \in argmax_{i\in I}(v_i + C \times \sqrt{ \ln n_p\over n_i})$
其中的参数设置如下
- $v_i$ : value of the node i → P_win
- $n_i$ : visit count of i,
- $n_p$ : visit count of p. (p is parent node of i)
- $C$: coefficient, which has to be tuned experimentally。 本人一开始的值设置成alphago 使用的常数 $\sqrt 2$

### **1-2. 代码实现 - bestchild**
___
horizontal lines in markdown except 
对应的代码部分为`UCT.h`中 `bestchild`部分,  
其中用到的节点成员为`children`。
实现方法很简单，但要注意孩子一次也没有被访问的情况。此时，就会访问那个节点。

## **2. Expansion Strategy**
---
首先，我选的expansion strategy是 “expand one node per simulated game.”
且是否可以扩展的指标为，在这次UCT中，是否还没存在从节点v可以走的棋盘。("The expanded node corresponds to the first position encountered that was not stored yet")  
如果所有的能走的棋盘存在，则 unexpandable, 否则 expandable。  

### **2-2. 代码实现**
具体在代码中实现呢，在`Node`中引入了`int expandables` 参数，用来记录可以走的棋盘的盘数。

#### **2-2-1. tree_policy**
`is_expandable`接口函数判断该节点能否扩展,
`is_leaf`接口函数判断该节点是否为叶节点。

#### **2-2-2. expand(v)**
需要注意的一点是，不是所有`Node`有`column`个可扩展的孩子节点，
事实上要检查top。

## **3. Rollout Policy(Simulation strategy, Default Policy)**
---
一开始所选取的default policy 是两个选手都随机放棋子。  
后来发现，一般而言，在棋盘中间放置的棋子对该选手获胜起到更积极的作用，因此给每列分配一定的权重，从而反应了这一点。  
下面为上述三种default policy 的代码实现。
### **3-2. 代码实现**

#### **3-2-1. rollout(random)**

#### **3-2-2. rollout(middle)**

#### **3-2-3. rollout(7)**

## **4. Back Propagation**
---
由于connect4 可以出现平局的情况，对`result`做了一些调整，
如果`simulation`比赛获胜$R_k$=1，平局则0，输则-1。  
节点的 `value` 使用该节点`result`的平均值。即，
$v_L = {(\sum_k R_k) \over n_L}$
### **4-2. 代码实现**
#### **4-2-1. backup**
用到的函数只是`Node`的接口函数而已。 

## **整体流程**
___
从如下基本的UCT框架得到启发，  
<img src ="./images/2023-05-31-17-18-54.png" alt = "UCT框架" width = "400" height ="500">
具体的流程图为如下
<!-- <img>... -->

# **总结与收获**


# **References**
___
* 教学团队提供的《蒙特卡洛博弈方法》
* Chaslot, Guillaume & Winands, Mark & Herik, H. & Uiterwijk, Jos & Bouzy, Bruno. (2008). Progressive Strategies for Monte-Carlo Tree Search. New Mathematics and Natural Computation. 04. 343-357. 10.1142/S1793005708001094. 
<https://www.researchgate.net/publication/23751563_Progressive_Strategies_for_Monte-Carlo_Tree_Search>


### TODO:
1. Frame

