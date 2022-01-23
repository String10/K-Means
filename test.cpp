// #define TESTBENCH_MAKE

#include "K-Means.cpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    /* DONE: Writing a test to compare KMeans and KMeans-with-SA. */
    srand(time(NULL));
    constexpr int datasize = 1e3, datarange = 1e4;
#ifdef TESTBENCH_MAKE
    ofstream of_data("data.txt");

    for(int i = 0; i < datasize; i++) {
        of_data << rand() % datarange << ' ' << rand() % datarange << endl;
    }
    of_data.close();
#endif // TESTBENCH_MAKE

    vector <vector <double>> dataset, clusters;
    ifstream if_data("data.txt");
    for(int i = 0; i < datasize; i++) {
        int a, b;
        if_data >> a >> b;
        dataset.push_back({ 1.0 * a, 1.0 * b });
    }
    if_data.close();

    KMeans kmeans;
    vector <size_t> tag(dataset.size());
    clusters = kmeans.cluster(dataset, tag, 5);

    ofstream of_log("log1.txt");
    for(auto &vec: clusters) {
        for(auto x: vec) {
            of_log << x << ' ';
        }
        of_log << endl;
    }
    of_log << "SA version." << endl;
    of_log << "The final WCSS is :" << kmeans.getWCSS(dataset, tag, clusters) << endl;
    of_log.close();

    cout << "Test Finished!" << endl;
}