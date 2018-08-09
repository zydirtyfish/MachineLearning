#include "machine_learning.h"
#define MAX_LEN 10000000

/*machine learning algorithm parents class*/
class MLA{

protected:
    AlgorithmType at;
    vector<int> Data[MAX_LEN];
    struct train_record *tr;
    set<int> Attr_value[ATTR_NUM]; /*每个属性的值的集合，每个属性有多个值*/
    int length; /*样本数*/

public:/*algorithm basic operation*/
    MLA(){}
    
    void get_data_set(const char * filename)
    {
        FILE *fp = fopen(filename,"r");
        int m = 0;
        while(!feof(fp))
        {
            tr = (struct train_record *)malloc(sizeof(struct train_record));
            fscanf(fp, "%ld,%ld,%lf,%lf,%lf,%d,%d,%d\n", &tr->read_timestamp, &tr->write_timestamp, &tr->read_ratio, &tr->read_size, &tr->write_size, &tr->readfreq,&tr->writefreq,&tr->label);
            Data[m] = pre_pro_data(tr); /*add label to the training record*/
            Data[m].push_back(tr->label);
            for(int k = 0 ; k < ATTR_NUM ; k++)
            {/*update the attr set values*/
                Attr_value[k].insert(Data[m][k]);
            }
            m++;
            tr = NULL;
        }
        length = m;
        fclose(fp);
    }

    vector<int> pre_pro_data(struct train_record *train_r)
    {
        vector<int> result;
        int tmp = 0;
        /*时间处理*/
        tmp = log(train_r->read_timestamp + 1) / log(2);
        if(tmp > 25) tmp = 25;
        result.push_back(tmp);
        tmp = log(train_r->write_timestamp + 1) / log(2);
        if (tmp > 25) tmp = 25;
        result.push_back(tmp);

        /*读写频次处理*/
        tmp = log(train_r->readfreq + 1) / log(2);
        if(tmp > 20) tmp = 20;
        result.push_back(tmp);
        tmp = log(train_r->writefreq + 1) / log(2);
        if(tmp > 20) tmp = 20;
        result.push_back(tmp);

        /*读写比例处理*/
        result.push_back(train_r->read_ratio / 5);

        /*读写大小处理*/
        tmp = log(train_r->read_size + 1) / log(2);
        if(tmp > 20) tmp = 20;
        result.push_back(tmp);
        tmp = log(train_r->write_size + 1) / log(2);
        if(tmp > 20) tmp = 20;
        result.push_back(tmp);

        return result;
    }

    virtual void training(const char *filename)=0;

    virtual Classes predict(struct train_record *tr)=0;

    void predict_many(const char *filename)
    {
        FILE *fp = fopen(filename, "r");
        while (!feof(fp))
        {
            tr = (struct train_record *)malloc(sizeof(struct train_record));
            fscanf(fp, "%ld,%ld,%lf,%lf,%lf,%d,%d\n", &tr->read_timestamp, &tr->write_timestamp, &tr->read_ratio, &tr->read_size, &tr->write_size,&tr->readfreq,&tr->writefreq);
            cout << predict(tr) << endl;;
            free(tr);
            tr = NULL;
        }
        fclose(fp);
    }

    virtual void persistent(const char * filename)=0;
    virtual void load_model(const char * filename)=0;
};