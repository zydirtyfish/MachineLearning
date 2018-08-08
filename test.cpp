#include "machine_learning.h"
#include "DecisionTree.h"

int main(int argc,char *argv[])
{
    cout << "123" << endl;
    MLA *dt = new class DecisionTree();
    dt->training("train.cvs");
    dt->predict_many("test.cvs");

    system("pause");
    return 0;
}