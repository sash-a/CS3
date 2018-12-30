//
// Created by abrsas002 on 2018/04/11.
//

#include <iostream>
#include <fstream>
#include "HuffmanNode.h"
#include "HuffmanTree.h"
#include <unordered_map>
#include <cstring>
#include "Driver.h"

using namespace abrsas;
using namespace std;

unordered_map<char, int> findFreq(const string &file)
{
    unordered_map<char, int> wordmap;
    wordmap.insert({'\n', 0});
    ifstream in(file);

    string line;
    while (getline(in, line))
    {
        for (char c : line)
        {
            if (wordmap.count(c) > 0)
            {
                wordmap[c] += 1;
            }
            else
            {
                wordmap.insert({c, 1});
                //wordmap[c] = 1;
            }
        }
        // new line
        wordmap['\n'] += 1;
    }

    if (wordmap['\n'] == 0)
    {
        wordmap.erase('\n');
    }

    in.close();
    return wordmap;
}

priority_queue<HuffmanNode, vector<HuffmanNode>, Compare> mapToQueue(const unordered_map<char, int> &wordmap)
{
    priority_queue<HuffmanNode, vector<HuffmanNode>, Compare> pq;
    for (auto pair: wordmap)
    {
        pq.push(HuffmanNode(pair.first, pair.second, true));
    }
    return pq;
}

string makeBuffer(const HuffmanTree &htree, const string &file)
{
    string buf;
    ifstream in(file);

    string line;
    while (getline(in, line))
    {
        for (char c : line)
        {
            buf += htree.bitstring.at(c);
        }
    }

    in.close();
    return buf;
}

void writeBuffer(const string &buf, const string &file)
{
    ofstream out(file);
    out.write(buf.c_str(), buf.length());
    out.close();
}

void writeCodeTable(const HuffmanTree &htree, const string &file)
{
    ofstream out(file + ".hdr");

    for (auto pair : htree.bitstring)
    {
        out << pair.first << ":" << pair.second << endl;
    }
    out.close();
}

bool parseArgs(int &argc, char *argv[], string &infile, string &outfile)
{
    if (argc != 3)
        return false;

    infile = argv[1];
    outfile = argv[2];
    return true;
}

/*
 * this assumes that each char in the buf string would take up one bit and that each char in the origional file takes up
 * size(char) bits
 */
double getCompressionRatio(string &infile, string &buf)
{
    ifstream in(infile);

    double filesize = 0;
    string line;
    while (getline(in, line))
    {
        filesize += line.size();
    }

    filesize *= sizeof(char);

    in.close();
    double bitSize = buf.size();

    return bitSize / filesize;
}

string decode(const string &file, const HuffmanTree &htree)
{
    ifstream in(file);
    string line;

    string decoded = "";
    // looping through lines
    while (getline(in, line))
    {
        string currentBits = "";
        // looping through chars
        for (char c : line)
        {
            currentBits += c;
            // finding value associated with bits
            for (auto pair : htree.bitstring)
            {
                if (pair.second == currentBits)
                {
                    decoded += pair.first;
                    currentBits = "";
                }
            }
        }
    }
    return decoded;
}


