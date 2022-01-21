// K-Means.h
#include <utility>
#include <vector>

class KMeans {
    typedef     double                    Data;
    typedef     std::vector <Data>      Vector;
    typedef     std::vector <Vector>    DataSet;
    typedef     std::vector <Vector>    Clusters;
    typedef     std::vector <size_t>    Tag;
public:
    KMeans() = default;
    Clusters cluster(const DataSet &dataset, Tag &tag, size_t k);
private:
    double getDistance(const Vector &a, const Vector &b);
    double getWCSS(const DataSet &dataset, const Tag &tag, const Clusters &clusters);
    void assignment(const DataSet &dataset, Tag &tag, const Clusters &clusters);
    void update(const DataSet &dataset, const Tag &tag, Clusters &clusters);
};

// K-Means.cpp
// #include "K-Means.h"
#include <ctime>
#include <cstdlib>
#include <set>

KMeans::Clusters KMeans::cluster(const DataSet &dataset, Tag &tag, size_t k) {
    /* XXX: Randomly select k vectors as the initial cluster. */
    /* But this method may cause timeout, I think. */
    srand(time(NULL));
    std::set <size_t> selector;
    while(selector.size() < k) {
        selector.insert(rand() % dataset.size());
    }
    
    Clusters clusters;
    for(auto index: selector) {
        clusters.push_back(dataset[index]);
    }

    size_t iteration_count = 100, last_wcss;
    while(0 != iteration_count--) {
        assignment(dataset, tag, clusters);
        update(dataset, tag, clusters);
    }
}