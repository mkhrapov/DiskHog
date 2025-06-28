//
// Created by Max Khrapov on 6/28/25.
//

#ifndef DISKHOG_H
#define DISKHOG_H

#include <vector>
#include <string>

using namespace std;


class DiskHog {
private:
    const string dirname;
    double totalDirSize;
    vector<DiskHog *> subdirs;

    string minidisp(string prefix) const;

public:
    DiskHog(string dirname_) : dirname(dirname_), totalDirSize(0.0), subdirs{} {};
    ~DiskHog();

    void traverse();
    vector<string> display() const;
};

#endif //DISKHOG_H
