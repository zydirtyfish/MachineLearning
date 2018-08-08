#include "machine_learning.h"
#include "DecisionTree.h"
#include "Config.h"

int main(int argc,char *argv[])
{
    char config_file[500];
	strcpy(config_file,"config");
    Config *cf = new Config();
    char tmp[50];
    cf->Get(config_file, "trace_type", tmp);
    MLA *dt;
    switch (atoi(tmp))
    {
        case 0:
            cout << "123" << endl;
            dt = new class DecisionTree();
            break;
        default:
            break;
    }
    dt->training("train.cvs");
    dt->predict_many("test.cvs");
    system("pause");
    return 0;
}