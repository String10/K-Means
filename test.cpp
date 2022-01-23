/* DONE: Write a test for the first version. */

#include "K-Means.cpp"

#include <iostream>
using namespace std;

int main() {
    vector <vector <double>> dataset, clusters;
    for(int i = 0; i < 10; i++) {
        dataset.push_back({ 1.0 * i, 1.0 * i });
    }
    KMeans kmeans;
    vector <size_t> tag(10);
    clusters = kmeans.cluster(dataset, tag, 3);
    for(auto &vec: clusters) {
        for(auto x: vec) {
            cout << x << ' ';
        }
        cout << endl;
    }
}