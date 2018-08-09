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
#include <cmath>

using namespace std;

#ifndef ATTRN
#define ATTRN
#define ATTR_NUM 9
#endif

#ifndef ALGORITHMT
#define ALGORITHMT
enum AlgorithmType
{
    DecisionTree__ = 0,           //decision tree
    LogisticRegression__,     //logistic regression
    BayesianClassification__, //bayesian classification
    RandomForest__            //random forest
};
#endif

#ifndef CLASSES
#define CLASSES
enum Classes
{
    Hot = 0,
    Cold
};
#endif

#ifndef ATTR
#define ATTR
enum Attribute
{
    FREQ = 0,
    READTIME,
    WRTIETIME,
    READRATIO,
    READSIZE,
    WRITESIZE
};
#endif

#ifndef TRAIN_RE
#define TRAIN_RE
struct train_record
{
    int readfreq;
    int writefreq;
    int label; /*标记类别*/
    long read_timestamp;
    long write_timestamp;
    double read_ratio;
    double read_size;
    double write_size;
};
#endif