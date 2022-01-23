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
    double getWCSS(const DataSet &dataset, const Tag &tag, const Clusters &clusters);
private:
    double getDistance2(const Vector &a, const Vector &b);
    void assignment(const DataSet &dataset, Tag &tag, const Clusters &clusters);
    void update(const DataSet &dataset, const Tag &tag, Clusters &clusters);

    size_t getClusterOfMinDis(const Vector &vec, const Clusters &clusters);
    size_t getClusterOfMinDisElkan(const Vector &vec, const Clusters &clusters,
                                   const std::vector <std::vector <Data>> &dis_2_between_clusters);
};

// K-Means.cpp
// #include "K-Means.h"
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <set>
#include <utility>
#include <algorithm>

KMeans::Clusters KMeans::cluster(const DataSet &dataset, Tag &tag, size_t k) {
    /* TODO: Use SA to optimize the selection of initial cluster */
    /* to avoid just in locally optimal solution. */

    /* DONE: Forgy: Randomly select k vectors as the initial cluster. */
    srand(time(NULL));

    /* SA variables. */
    double T = 100.0;
    constexpr double rate = 0.9;

    Clusters final_clusters;
    Tag temp_tag = tag;
    while(fabs(T) > EPS) {
        Clusters temp_clusters;
        temp_clusters.push_back(dataset[rand() % dataset.size()]);
        
        std::vector <std::pair <double, size_t>> select_helper(dataset.size());
        while(temp_clusters.size() < k) {
            for(int i = 0; i < dataset.size(); i++) {
                size_t index = getClusterOfMinDis(dataset[i], temp_clusters);
                select_helper[i] = { getDistance2(dataset[i], temp_clusters[index]), i };
            }
            std::sort(select_helper.begin(), select_helper.end(), 
                    [](const std::pair <double, size_t> &a, const std::pair <double, size_t> &b)->bool {
                        return (fabs(a.first - b.first) <= EPS) ? (a.second < b.second) : (a.first > b.first);
                    });
            size_t datasize = dataset.size();
            for(int i = 0; i < datasize; i++) {
                if(fabs(select_helper[i].first) <= EPS) {
                    temp_clusters.push_back(dataset[select_helper[0].second]);
                    break;
                }
                if(rand() % (datasize * datasize / 2) < (datasize - i)) {
                    temp_clusters.push_back(dataset[select_helper[i].second]);
                    break;
                }
            }
        }

        /* If not SA, it could be 5e4. */
        size_t iteration_count = 5e2;
        double last_wcss = 0;
        while(0 != iteration_count--) {
            assignment(dataset, temp_tag, temp_clusters);
            update(dataset, temp_tag, temp_clusters);

            if(fabs(last_wcss - getWCSS(dataset, temp_tag, temp_clusters)) <= EPS) {
                break;
            }
        }

        if(final_clusters.empty()) {
            final_clusters = temp_clusters, tag = temp_tag;
        }
        else {
            double delta = getWCSS(dataset, temp_tag, temp_clusters) - getWCSS(dataset, tag, final_clusters);
            if(delta <= EPS) {
            final_clusters = temp_clusters, tag = temp_tag;
            }
            else if(exp(-delta / T) * RAND_MAX > rand()) {
                final_clusters = temp_clusters, tag = temp_tag;
            }
        }

        T *= rate;
    }
    
    return final_clusters;
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

size_t KMeans::getClusterOfMinDisElkan(const Vector &vec, const Clusters &clusters,
                                       const std::vector <std::vector <Data>> &dis_2_between_clusters) {
    double min_dis_2 = getDistance2(vec, clusters[0]);
    size_t index = 0;
    for(int i = 1; i < clusters.size(); i++) {
        /* XXX: I don't know why this is 'i - index', it should be 'i - index - 1', I guess. */
        if(dis_2_between_clusters[index][i - index] >= 2 * min_dis_2) {
            continue;
        }
        double temp_dis_2 = getDistance2(vec, clusters[i]);
        if(temp_dis_2 < min_dis_2) {
            min_dis_2 = temp_dis_2, index = i;
        }
    }
    return index;
}

void KMeans::assignment(const DataSet &dataset, Tag &tag, const Clusters &clusters) {
    std::vector <std::vector <double>> dis_2_between_clusters(clusters.size());
    if(clusters.size() <= dataset.size()) {
        for(int i = 0; i < clusters.size(); i++) {
            // for(int j = 0; j < i; j++) {
            //     dis_2_between_clusters[i].push_back(dis_2_between_clusters[j][i]);
            // }
            // dis_2_between_clusters[i].push_back(0);
            for(int j = i + 1; j < clusters.size(); j++) {
                dis_2_between_clusters[i].push_back(getDistance2(clusters[i], clusters[j]));
            }
        }
        for(int i = 0; i < dataset.size(); i++) {
            tag[i] = getClusterOfMinDisElkan(dataset[i], clusters, dis_2_between_clusters);
        }
    }
    else {
        for(int i = 0; i < dataset.size(); i++) {
            tag[i] = getClusterOfMinDis(dataset[i], clusters);
        }
    }
}

void KMeans::update(const DataSet &dataset, const Tag &tag, Clusters &clusters) {
    std::vector <size_t> count(clusters.size(), 0);
    int data_len = dataset[0].size();
    for(int i = 0; i < clusters.size(); i++) {
        for(auto &data: clusters[i]) {
            data = 0;
        }
    }
    for(int i = 0; i < dataset.size(); i++) {
        for(int j = 0; j < data_len; j++) {
            clusters[tag[i]][j] += dataset[i][j];
        }
        count[tag[i]]++;
    }
    for(int i = 0; i < clusters.size(); i++) {
        for(int j = 0; j < data_len; j++) {
            clusters[i][j] /= count[i];
        }
    }
}