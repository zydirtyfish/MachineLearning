//////////////////////////////////////////////////////////////////////////
// DecisionTree.h
// Author: Yu Zhang
// Date: 2018-08
// Machine learning Decision Tree Algorithm

#include "mla.h"
#include <cmath>

#ifndef MN
#define MN
#define MAXNODE 30
#endif

#ifndef NODE__
#define NODE__
struct TreeNode
{ /*tree node*/
    int label = 0;
    int neg;                     /*当前节点下正例个数*/
    int pos;                     /*当前节点下负例个数*/
    double CUT;                  /*是否剪枝*/
    int attr;                    /*节点特征*/
    vector<int> attr_value;      /*节点分支依据*/
    TreeNode *children[MAXNODE] = {NULL};   /*有多个孩子*/
};
#endif

class DecisionTree : public MLA
{
private:
    vector<int> attr; /*特征集合*/
    vector<int> index; /*数据集索引*/
    TreeNode * root; /*根节点*/
    
    ATYPE algorithm_type;

public:
    /*initialize*/
    DecisionTree(ATYPE at)
    {
        for(int i = 0 ; i < ATTR_NUM ; i++)
        {/*init attr set*/
            attr.push_back(i);
        }
        algorithm_type = at;
        root = new TreeNode;
    }

    ~DecisionTree()
    {}

    void training(const char *filename)
    {
        /*get the training data set*/
        get_data_set(filename);
        for(int i = 0 ; i < length ; i++)
        {/*初始化数据集索引，每一项代表训练集中的一条记录*/
            index.push_back(i);
        }
        recursive(root, index, algorithm_type);
        show(root);
        //eT(root);   /*遍历树，标记可以剪枝的节点*/
        //ergo(root); /*遍历树并剪枝。*/
    }

    Classes predict(struct train_record *tr)
    {
        
        return Hot;
    }
    
    void persistent(const char *filename)
    {}
    
    void load_model(const char *filename)
    {}

    void show(TreeNode *tn)
    {
        cout << tn->attr << " : ";
        for (auto it = tn->attr_value.begin(); it != tn->attr_value.end(); it++)
        {
            cout << *it << " ";
        }
        cout << endl;
        for (auto it = tn->attr_value.begin(); it != tn->attr_value.end(); it++)
        {
            show(tn->children[*it]);
        }
        
    }

  private:
    void recursive(TreeNode *p, vector<int> index,ATYPE at)
    {
        int attr_chosen = getAttr(Data,index, at);
        vector<vector<int> > subsets = get_subsets(attr_chosen,index);
        p->attr = attr_chosen;
        for(int i = 0 ; i < attr.size() ; i++) /*特征集中删除当前属性*/
            if(attr[i] == attr_chosen)
                attr.erase(attr.begin()+i, attr.begin()+ i +1);
        for(auto it = Attr_value[attr_chosen].begin(); it != Attr_value[attr_chosen].end() ; it++)
        {
            p->attr_value.push_back(*it);
        }

        /*给节点pos和neg赋值*/
        int pos = 0, neg = 0;
        for(int i = 0 ; i < index.size() ; i++)
        {
            if(Data[index[i]][ATTR_NUM] == 1) pos++;
            else neg++;
        }
        p->pos=pos;p->neg=neg;

        auto it = Attr_value[attr_chosen].begin();
        for(int i = 0 ; i < subsets.size() ; i++,it++)
        {/*节点分支和边界处理*/
            TreeNode *subnode = new TreeNode;
            p->children[*it] = subnode;
            if(meet_with_bound(subsets[i]))
            { /*可优化 如果下一个节点是递归结束，下个节点属性则在当前结点处理 */
                if(subsets[i].size() == 0)
                {
                    if(p->pos >= p->neg) subnode->label = 1;
                    else subnode->label = -1;
                    subnode->pos = 0;
                    subnode->neg = 0;
                    continue;
                }
                
                else
                {
                    pos = 0;neg = 0;
                    for(int j = 0; j < subsets[i].size() ; j++)
                    {
                        if(Data[subsets[i][j]][ATTR_NUM] == 1) pos++;
                        else neg++;
                    }
                    if(pos >= neg) subnode->label = 1;
                    else  subnode->label = -1;
                    subnode->pos = pos;
                    subnode->neg = neg;
                    continue;
                }
            }
            recursive(subnode,subsets[i],at);
        }
        attr.push_back(attr_chosen);
        subsets.clear();
        return;
    }

    bool meet_with_bound(vector<int> index)
    {
        int len = index.size();
        int pos = 0, neg = 0;
        if(len == 0) return true; /*当前数据集为空*/
        for(int i = 0 ; i < len ; i++)
        {
            if(Data[index[i]][ATTR_NUM] == 1) pos++;
            else neg++;
        }
        if(neg == 0 || pos == 0) return true; /*当前数据集标记一致*/
        if(attr.size() == 0) return true; /*当前特征集为空*/
        for(int i=0;i<attr.size();i++)
        {/*判断特征集取值是否相同*/
            int a = Data[index[0]][attr[i]];
            for(int j = 1; j < len ;j++)
            {
                if(Data[index[j]][attr[i]] != a) return false;
            }
        }
        return true;
    }

    vector<vector<int> > get_subsets(int attr, vector<int> index)
    {
        int col = attr;
        vector<vector<int> > result;
        vector<int> index_set[MAXNODE];

        int m = 0 ;
        for(auto it = Attr_value[col].begin() ; it != Attr_value[col].end(); it++)
        {
            for(int i = 0 ; i < index.size() ; i++)
            {
                if(*it == Data[index[i]][col])
                {
                    index_set[m].push_back(index[i]);
                }
            }
            result.push_back(index_set[m]);
            index_set[m].clear();
            m++;
        }
        return result;
    }

    int getAttr(vector<int> *data, vector<int> index, ATYPE algorithmtype)
    {
        switch(algorithmtype)
        {
            case C45:
                return getAttr1(data, index);
            case CART:
                return getAttr2(data, index);
            case ID3:
                return getAttr3(data, index);
            default:
                return getAttr1(data, index);
        }
    }

    int getAttr1(vector<int> *data, vector<int> index)
    { /*返回熵增益最大的属性，使用C4_5算法*/
        double pos = 0, neg = 0;
        int len = index.size();
        for (int i = 0; i < len; i++)
        { /*计算数据集中正例与反例的个数*/
            int a = index[i];
            if (data[a][ATTR_NUM] == 1)
                pos++;
            else
                neg++;
        }
        /*计算数据集熵*/
        double H = -xlog2(pos / (len * 1.0)) - xlog2(neg / (len * 1.0));
        /*计算各属性的信息增益*/
        double *gain = new double[attr.size()];
        double *splitinfo = new double[attr.size()];
        for (int k = 0; k < attr.size(); k++)
        {
            int i = attr[k];
            double attr_pos[MAXNODE] = {0};
            double attr_neg[MAXNODE] = {0};
            for (int j = 0; j < len; j++)
            { /*计算各分支的正例与反例的个数*/
                int a = index[j];
                if (data[a][ATTR_NUM] == 1)
                    attr_pos[data[a][i]]++;
                else
                    attr_neg[data[a][i]]++;
            }
            gain[k] = 0;
            splitinfo[k] = 0;
            for (int j = 0; j < MAXNODE; j++)
            { /*计算信息增益*/
                if (attr_pos[j] != 0 || attr_neg[j] != 0)
                {
                    double p_sum = attr_pos[j] + attr_neg[j];
                    splitinfo[k] += -xlog2(p_sum / (len * 1.0));
                    gain[k] += p_sum / (len * 1.0) * (-xlog2(attr_pos[j] / p_sum) - xlog2(attr_neg[j] / p_sum));
                }
            }
            /*计算增益率*/
            gain[k] = (H - gain[k]) / splitinfo[k];
        }
        double MAX = -99999;
        int position;
        for (int i = 0; i < attr.size(); i++)
        {
            cout << gain[i] << endl;
            if (gain[i] > MAX)
            {
                MAX = gain[i];
                position = attr[i];
            }
        }
        //cout << position << endl;
        return position;
    }

    double xlog2(double a)
    {
        if (a == 0)
            return 0;
        return a * log(a) / log(2);
    }

    int getAttr2(vector<int> *data, vector<int> index)
    { /*返回熵增益最大的属性，使用CART算法的基尼系数*/
        double pos = 0, neg = 0;
        int len = index.size();
        for (int i = 0; i < len; i++)
        { //计算数据集中正例与反例的个数
            int a = index[i];
            if (data[a][ATTR_NUM] == 1)
                pos++;
            else
                neg++;
        }
        /*计算各属性的基尼系数*/
        double *gini = new double[attr.size()];
        for (int k = 0; k < attr.size(); k++)
        {
            int i = attr[k];
            double attr_pos[50] = {0};
            double attr_neg[50] = {0};
            for (int j = 0; j < len; j++)
            { //计算各分支的正例与反例的个数
                int a = index[j];
                if (data[a][ATTR_NUM] == 1)
                    attr_pos[data[a][i]]++;
                else
                    attr_neg[data[a][i]]++;
            }
            gini[k] = 0;
            for (int j = 0; j < 50; j++)
            { //计算各属性的基尼系数
                if (attr_pos[j] != 0 || attr_neg[j] != 0)
                {
                    double p_sum = attr_pos[j] + attr_neg[j];
                    gini[k] = p_sum / (len * 1.0) * (1 - pow(attr_pos[j] / p_sum, 2) - pow(attr_neg[j] / p_sum, 2));
                }
            }
        }
        double MIN = 99999;
        int position;
        for (int i = 0; i < attr.size(); i++)
        {
            if (gini[i] < MIN)
            {
                MIN = gini[i];
                position = attr[i];
            }
        }
        return position;
    }

    int getAttr3(vector<int> *data, vector<int> index)
    { /*返回熵增益最大的属性，使用ID3算法*/
        double pos = 0, neg = 0;
        int len = index.size();
        for (int i = 0; i < len; i++)
        { //计算数据集中正例与反例的个数
            int a = index[i];
            if (data[a][data[a].size() - 1] == 1)
                pos++;
            else
                neg++;
        }
        double H = -xlog2(pos / (len * 1.0)) - xlog2(neg / (len * 1.0));
        /*计算各属性的信息增益*/
        double *gain = new double[attr.size()];
        for (int k = 0; k < attr.size(); k++)
        {
            int i = attr[k];
            double attr_pos[50] = {0};
            double attr_neg[50] = {0};
            for (int j = 0; j < len; j++)
            { //计算各分支的正例与反例的个数
                int a = index[j];
                if (data[a][data[a].size() - 1] == 1)
                    attr_pos[data[a][i]]++;
                else
                    attr_neg[data[a][i]]++;
            }
            gain[k] = 0;
            //因为计算信息增益的时候，被减数信息熵相同，因此只计算减数，选择减数值越小的的属性
            for (int j = 0; j < 50; j++)
            { //计算信息增益
                if (attr_pos[j] != 0 || attr_neg[j] != 0)
                {
                    double p_sum = attr_pos[j] + attr_neg[j];
                    gain[k] += p_sum / (len * 1.0) * (-xlog2(attr_pos[j] / p_sum) - xlog2(attr_neg[j] / p_sum));
                }
            }
        }
        double MIN = 99999;
        int position;
        for (int i = 0; i < attr.size(); i++)
        { //选择
            cout << H - gain[i] << " ";
            if (gain[i] < MIN)
            {
                MIN = gain[i];
                position = attr[i];
            }
        }
        cout << endl;
        return position;
    }

    double eT(TreeNode *n)
    { //遍历 并标记哪个节点可以剪枝
        if (n->label != 0)
        {              //叶子节点
            double et; //计算叶子节点的et
            if (n->pos > n->neg)
                et = n->neg + 0.5;
            else if (n->pos < n->neg)
                et = n->pos + 0.5;
            else
                et = 0.0;
            return et;
        }
        double et;
        double ET = 0.0;
        if (n->pos >= n->neg)
            et = n->neg + 0.5;
        else
            et = n->pos + 0.5; //计算当前节点的et
        for (int i = 0; i < 50; i++)
        {
            if (n->children[i] != NULL)
                ET += eT(n->children[i]); //递归计算当前节点的ET
        }
        double p_sum = n->pos + n->neg;
        double SE = ET * (p_sum - ET) / p_sum;
        //cout<<p_sum<<" "<<ET<<endl;
        SE = sqrt(SE);
        if (et <= ET + SE)
            n->CUT = 1;
        return ET;
    }

    void cut(TreeNode *n)
    { //剪枝
        if (n->CUT == 1)
        {
            if (n->pos > n->neg)
                n->label = 1;
            else
                n->label = -1;
        }
    }

    void ergo(TreeNode *n)
    { //遍历
        if (n->label != 0)
            return;
        cut(n);
        if (n->label != 0)
            return;
        for (int i = 0; i < 50; i++)
            if (n->children[i] != NULL)
                ergo(n->children[i]);
    }

};

