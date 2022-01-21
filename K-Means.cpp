// K-Means.h
#include <utility>
#include <vector>

class KMeans {
    typedef     double                  Data;
    typedef     std::vector <Data>      Vector;
    typedef     std::vector <Vector>    DataSet;
    typedef     std::vector <Vector>    Clusters;
    typedef     std::vector <size_t>    Tag;

    constexpr static double EPS = 1e-6;
public:
    KMeans() = default;
    Clusters cluster(const DataSet &dataset, Tag &tag, size_t k);
private:
    double getDistance2(const Vector &a, const Vector &b);
    double getWCSS(const DataSet &dataset, const Tag &tag, const Clusters &clusters);
    void assignment(const DataSet &dataset, Tag &tag, const Clusters &clusters);
    void update(const DataSet &dataset, const Tag &tag, Clusters &clusters);

    size_t getClusterOfMinDis(const Vector &vec, const Clusters &clusters);
};

// K-Means.cpp
// #include "K-Means.h"
#include <ctime>
#include <cstdlib>
#include <cmath>
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

    size_t iteration_count = 100;
    double last_wcss = 0;
    while(0 != iteration_count--) {
        assignment(dataset, tag, clusters);
        update(dataset, tag, clusters);

        if(fabs(last_wcss - getWCSS(dataset, tag, clusters)) <= EPS) {
            break;
        }
    }

    return clusters;
}

// a.size() must be equal to b.size().
double KMeans::getDistance2(const Vector &a, const Vector &b) {
    double dis = 0;
    for(int i = 0; i < a.size(); i++) {
        dis += (a[i] - b[i]) * (a[i] - b[i]);
    }

    return dis;
}

double KMeans::getWCSS(const DataSet &dataset, const Tag &tag, const Clusters &clusters) {
    /* DONE: Calculate the wcss, i.e. Within-Cluster Sum of Square. */
    double wcss = 0;
    for(int i = 0; i < dataset.size(); i++) {
        wcss += getDistance2(dataset[i], clusters[tag[i]]);
    }
    return wcss;
}

size_t KMeans::getClusterOfMinDis(const Vector &vec, const Clusters &clusters) {
    double min_dis_2 = getDistance2(vec, clusters[0]);
    size_t index = 0;
    for(int i = 1; i < clusters.size(); i++) {
        double temp_dis_2 = getDistance2(vec, clusters[i]);
        if(temp_dis_2 < min_dis_2) {
            min_dis_2 = temp_dis_2, index = i;
        }
    }
    return index;
}

void KMeans::assignment(const DataSet &dataset, Tag &tag, const Clusters &clusters) {
    for(int i = 0; i < dataset.size(); i++) {
        tag[i] = getClusterOfMinDis(dataset[i], clusters);
    }
}

void KMeans::update(const DataSet &dataset, const Tag &tag, Clusters &clusters) {

}