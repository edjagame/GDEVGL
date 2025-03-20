#include <iostream>
#include <random>
#include <vector>

// change this to your desired window attributes
#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 900
#define WINDOW_TITLE  "Hello Transform (use WASDQE and arrow keys; 1 and 2 to select object)"
#define PI 3.14159265358979323846
float fov   =  75.0f;


using namespace std;
// define a vertex array to hold our vertices
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

vector<float> loadVerts(const string& filePath);
float* tailsVertices2 = loadVerts("./vertx.txt").data();
    
int main () {

    for(int i = 0; i < 3; i++) {
        cout << tailsVertices2[i] << endl;
    }

    return 1;
}

vector<float> loadVerts(const string& filePath) {
    vector<float> data;
    ifstream file(filePath);
    string line;

    while (getline(file, line)) {
        float value = stof(line);
        data.push_back(value);
    }

    return data;
}

