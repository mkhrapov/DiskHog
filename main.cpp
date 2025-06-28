#include <iostream>
#include "DiskHog.h"


using namespace std;


void runDiskHog(string dir) {
    DiskHog dh(dir);
    dh.traverse();
    for(const auto& s : dh.display()) {
        cout << s << '\n';
    }
}


int main(int argc, char* argv[]) {
    if (argc == 1) {
        runDiskHog(".");
    }
    else {
        runDiskHog(string(argv[1]));
    }
    return 0;
}
