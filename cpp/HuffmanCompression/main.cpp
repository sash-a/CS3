#include <iostream>
#include <fstream>
#include "HuffmanNode.h"
#include "HuffmanTree.h"
#include <unordered_map>
#include <cstring>
#include "Driver.h"

using namespace abrsas;
using namespace std;

int main(int argc, char *argv[])
{
    string infile;
    string outfile;

    if (!parseArgs(argc, argv, infile, outfile))
        return 0;

    unordered_map<char, int> wordmap = findFreq(infile);
    priority_queue<HuffmanNode, vector<HuffmanNode>, Compare> pq = mapToQueue(wordmap);

    HuffmanTree htree(pq);

    string buf = makeBuffer(htree, infile);

    cout << "Ideal compression ratio:" << getCompressionRatio(infile, buf) << endl;

    writeBuffer(buf, outfile);
    writeCodeTable(htree, outfile);

    writeBuffer(decode(outfile, htree), "decoded.txt");

    return 0;
}
