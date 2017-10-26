#include <iostream>
#include "lexanalyzer.h"


using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Not Arguments" << endl;
        return -1;
    }
    if(analyze(argv[1]) < 0) {
        cerr << "File couldn't be analyze" << endl;
        return -2;
    }

    return 0;
}
