#include "mla.h"

class DecisionTree : public MLA
{
private:

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
};