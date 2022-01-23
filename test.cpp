/* DONE: Write a test for the first version. */

#include "K-Means.cpp"

#include <iostream>
using namespace std;

int main() {
    vector <vector <double>> dataset, clusters;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            dataset.push_back({ 1.0 * i, 1.0 * j });
        }
    }

    for(int i = 6; i < 9; i++) {
        for(int j = 6; j < 9; j++) {
            dataset.push_back({ 1.0 * i, 1.0 * j });
        }
    }

    for(int i = 10; i < 13; i++) {
        for(int j = 0; j < 3; j++) {
            dataset.push_back({ 1.0 * i, 1.0 * j });
        }
    }

    KMeans kmeans;
    vector <size_t> tag(dataset.size());
    clusters = kmeans.cluster(dataset, tag, 3);
    for(auto &vec: clusters) {
        for(auto x: vec) {
            cout << x << ' ';
        }
        cout << endl;
    }
}