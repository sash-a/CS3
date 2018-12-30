#include <string>
#include <cstring>
#include <iostream>
#include "volimg.h"

using namespace std;

int main(int argc, char *argv[])
{
    string fileName = argv[1];
    auto *v = new abrsas::VolImage();
    if(!v->readImages(fileName))
    {
        return 0;
    }

    //TODO make an arg parser
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-d") == 0) //do diffmap
        {
            cout << "creating diffmap" << endl;
            v->diffmap(stoi(argv[i + 1]), stoi(argv[i + 2]), argv[i + 3]);
            break; // only do one operation
        }
        else if (strcmp(argv[i], "-x") == 0) //do extract
        {
            cout << "extracting slice" << endl;
            v->extract(stoi(argv[i + 1]), argv[i + 2]);
            break; // only do one operation
        }
        else if (strcmp(argv[i], "-g") == 0)
        {
            cout << "extracting row" << endl;
            v->extractRow(stoi(argv[i + 1]));
            break;
        }
    }
    if (argc <= 2)
    {
        cout << "Number of images: " << v->numImages << endl;
        cout << "Number of bytes: " << v->volImageSize() << endl;
    }

    delete v;
}