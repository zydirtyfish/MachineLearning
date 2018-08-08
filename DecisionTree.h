#include "mla.h"
#include <cmath>

#ifndef NODE__
#define NODE__
struct TreeNode
{ /*tree node*/
    int label = 0;
    int neg;                     /*当前节点下正例个数*/
    int pos;                     /*当前节点下负例个数*/
    int attr;                    /*节点特征*/
    vector<int> attr_value;      /*节点分支依据*/
    TreeNode *children[50] = {NULL};   /*有多个孩子*/
};
#endif

class DecisionTree : public MLA
{
public:
    /*initialize*/
    DecisionTree()
    {}

    ~DecisionTree()
    {}

    void training(const char *filename)
    {
        /*get the training data set*/
        get_data_set(filename);
    }

    Classes predict(struct train_record *tr)
    {
        
        return Hot;
    }
    
    void persistent(const char *filename)
    {

    }
    
    void load_model(const char *filename)
    {

    }

  private:
    int getAttr(vector<int> *data, vector<int> index, vector<int> attr)
    { /*返回熵增益最大的属性，使用C4_5算法*/
        double pos = 0, neg = 0;
        int len = index.size();
        for (int i = 0; i < len; i++)
        { /*计算数据集中正例与反例的个数*/
            int a = index[i];
            if (data[a][data[a].size() - 1] == 1)
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
            double attr_pos[50] = {0};
            double attr_neg[50] = {0};
            for (int j = 0; j < len; j++)
            { /*计算各分支的正例与反例的个数*/
                int a = index[j];
                if (data[a][data[a].size() - 1] == 1)
                    attr_pos[data[a][i]]++;
                else
                    attr_neg[data[a][i]]++;
            }
            gain[k] = 0;
            splitinfo[k] = 0;
            for (int j = 0; j < 50; j++)
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
            if (gain[i] > MAX)
            {
                MAX = gain[i];
                position = attr[i];
            }
        }
        return position;
    }

    double xlog2(double a)
    {
        if (a == 0)
            return 0;
        return a * log(a) / log(2);
    }

    int getAttr2(vector<int> *data, vector<int> index, vector<int> attr)
    { /*返回熵增益最大的属性，使用CART算法的基尼系数*/
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
                if (data[a][data[a].size() - 1] == 1)
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

    int getAttr3(vector<int> *data, vector<int> index, vector<int> attr)
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
};

