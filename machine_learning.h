#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>
#include <set>
#include <iterator>
using namespace std;

enum AlgorithmType
{
    DecisionTree = 0,           //decision tree
    LogisticRegression,     //logistic regression
    BayesianClassification, //bayesian classification
    RandomForest            //random forest
};

enum Classes
{
    Hot = 0,
    Cold
};

struct train_record
{
    long read_timestamp;
    long write_timestamp;
    double read_ratio;
    double read_size;
    double write_size;
};