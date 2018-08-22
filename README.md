# MachineLearning

To realize some classical machine learning algorithm by C++ language

#### data set explanation

```
struct dataset{
    read_timestamp;
    write_timestamp;
    read_ratio;
    read_size;
    write_size;
    read_freq;
    write_freq;
}
```

#### data pre_processing
To do the following operation and make sure all the data items are integers between 1 and 30 
```
timestamp -> log(timestamp)
read_ratio -> read_ratio / 5
io_size -> log(io_size)
frequency -> log(frequency)
```
