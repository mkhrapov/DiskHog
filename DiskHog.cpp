//
// Created by Max Khrapov on 6/28/25.
//

#include "DiskHog.h"

#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>


void DiskHog::traverse() {
    DIR *dir;
    struct dirent *entry;
    struct stat info;

    dir = opendir(dirname.c_str());

    if (!dir) {
        cerr << "Could not open directory: " << dirname << endl;
        return;
    }

    //cout << "Traversing: " << dirname << '\n';

    while ((entry = readdir(dir)) != NULL) {
        string filename(entry->d_name);
        if (filename == ".") {
            //cout << filename << '\n';
        }
        else if (filename == "..") {
            //cout << filename << '\n';
        }
        else {
            string path = dirname + "/" + filename;
            lstat(path.c_str(), &info);
            if (S_ISLNK(info.st_mode)) {
                //cout << "Symlink: " << path << '\n';
            }
            else {
                stat(path.c_str(), &info);
                if (S_ISDIR(info.st_mode)) {
                    //cout << "Directory: " << path << '\n';

                    DiskHog* dh = new DiskHog(path);
                    dh->traverse();
                    totalDirSize += dh->totalDirSize;
                    subdirs.push_back(dh);


                } else if (S_ISREG(info.st_mode)) {
                    totalDirSize += info.st_size;
                    //cout << "Regular file: " << path << '\n';
                } else {
                    //cout << "Not a regular file: " << path << '\n';
                }
            }
        }
    }
    closedir(dir);

    sort(subdirs.begin(), subdirs.end(), [](const DiskHog* lhs, const DiskHog* rhs) {
        return lhs->totalDirSize > rhs->totalDirSize;
    });
}


vector<string> DiskHog::display() const {
    vector<string> result(0);
    result.push_back(minidisp(""));

    if (subdirs.size() > 0) {
        vector<string> first_dir = subdirs[0]->display();
        for(const string& s : first_dir) {
            result.push_back("    " + s);
        }
    }

    if (subdirs.size() > 1) {
        for(int i = 1; i < subdirs.size(); i++) {
            if (subdirs[i]->totalDirSize >= 10'000'000) {
                result.push_back(subdirs[i]->minidisp("    "));
            }
        }
    }

    return result;
}


string DiskHog::minidisp(string prefix) const {
    ostringstream oss;
    string suffix = " ";
    double humanReadableDirSize = 0.0;
    if (totalDirSize >= 1'000'000'000.0) {
        humanReadableDirSize = totalDirSize / 1'000'000'000.0;
        suffix = " G ";
    }
    else if (totalDirSize >= 1'000'000.0) {
        humanReadableDirSize = totalDirSize / 1'000'000.0;
        suffix = " M ";
    }
    else if (totalDirSize >= 1'000.0) {
        humanReadableDirSize = totalDirSize / 1'000.0;
        suffix = " K ";
    }
    else {
        humanReadableDirSize = totalDirSize;
    }

    oss << prefix << fixed << setprecision(1) << humanReadableDirSize << suffix << dirname;
    return oss.str();
}


DiskHog::~DiskHog() {
    for(DiskHog* dh : subdirs) {
        delete dh;
    }
}
