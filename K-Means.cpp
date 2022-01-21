// K-Means.h
#include <utility>
#include <vector>

class KMeans {
    typedef     unsigned long long      Data;
    typedef     std::vector <Data>      Vector;
    typedef     std::vector <Vector>    DataSet;
    typedef     std::vector <Vector>    Cluster;
    typedef     std::vector <size_t>    Tag;
public:
    KMeans() = default;
    Cluster cluster(const DataSet& dataset, Tag &tag, size_t k);
private:
    size_t getDistance(const Vector &a, const Vector &b);
};

// K-Means.cpp
// #include "K-Means.h"
#include <ctime>
#include <cstdlib>

KMeans::Cluster KMeans::cluster(const DataSet& dataset, Tag &tag, size_t k) {
    srand(time(NULL));
    /* TODO: Randomly select k vectors as the initial cluster. */
}