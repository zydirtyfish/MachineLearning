#include "machine_learning.h"
#include "DecisionTree.h"
#include "Config.h"

int main(int argc, char *argv[])
{
    char config_file[500];
    strcpy(config_file, "config");
    Config *cf = new Config();
    char tmp[50];
    cf->Get(config_file, "AlgorithmType", tmp);
    MLA *dt;

    ATYPE a = C45;/*设置决策树算法的类别*/
    switch (atoi(tmp))
    {
        case 0:
            dt = new class DecisionTree(a);
            break;
        default:
            dt = new class DecisionTree(a);
            break;
    }

    dt->training("train.csv");
    dt->predict_many("test.csv");
    system("pause");
    return 0;
}