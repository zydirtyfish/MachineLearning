#include "machine_learning.h"

/*machine learning algorithm parents class*/
class MLA{

private:
    AlgorithmType at;
    vector <struct train_record *> mydataset;
    struct train_record *tr;

public:/*algorithm basic operation*/
    MLA(){}
    
    void get_data_set(const char * filename)
    {
        FILE *fp = fopen(filename,"r");
        while(!feof(fp))
        {
            tr = (struct train_record *)malloc(sizeof(struct train_record));
            fscanf(fp, "%ld,%ld,%lf,%lf,%lf,%d\n", &tr->read_timestamp, &tr->write_timestamp, &tr->read_ratio, &tr->read_size, &tr->write_size, &tr->freq);
            mydataset.push_back(tr);
            tr = NULL;
        }
        fclose(fp);
    }

    virtual void training(){};

    virtual Classes predict(struct train_record *tr){};

    void predict_many(const char *filename)
    {
        FILE *fp = fopen(filename, "r");
        while (!feof(fp))
        {
            tr = (struct train_record *)malloc(sizeof(struct train_record));
            fscanf(fp, "%ld,%ld,%lf,%lf,%lf,%d\n", &tr->read_timestamp, &tr->write_timestamp, &tr->read_ratio, &tr->read_size, &tr->write_size,&tr->freq);
            cout << predict(tr) << endl;;
            free(tr);
            tr = NULL;
        }
        fclose(fp);
    }

    virtual void persistent(){};
    virtual void load_model(){};
};