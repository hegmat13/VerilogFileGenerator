#include <iostream>
#include <fstream>
using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

string readFile(string textName) {
    //input variables and stream
    ifstream inFile(textName);
    string oneLine("");
    string theText{""};

    while (!inFile.eof()) {

    }

    return theText;
}
