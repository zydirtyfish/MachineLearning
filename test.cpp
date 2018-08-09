#include "machine_learning.h"
#include "DecisionTree.h"
#include "Config.h"

int main(int argc, char *argv[])
{
    char config_file[500];
    strcpy(config_file, "config");
    Config *cf = new Config();
    char tmp[50];
    cf->Get(config_file, "trace_type", tmp);
    MLA *dt;

    switch (atoi(tmp))
    {
        ATYPE a = C45;
        case 0:
            dt = new class DecisionTree(a);
            break;
        default:
            dt = new class DecisionTree(a);
            break;
    }
    dt->training("train.csv");
    dt->predict_many("test.csv");
    return 0;
}