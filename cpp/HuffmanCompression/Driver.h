//
// Created by abrsas002 on 2018/04/11.
//

#ifndef HUFFMAN_DRIVER_H
#define HUFFMAN_DRIVER_H

#include <iostream>
#include <fstream>
#include "HuffmanNode.h"
#include "HuffmanTree.h"
#include <unordered_map>
#include <cstring>


std::unordered_map<char, int> findFreq(const std::string &file);

std::priority_queue <abrsas::HuffmanNode, std::vector<abrsas::HuffmanNode>, abrsas::Compare>
mapToQueue(const std::unordered_map<char, int> &wordmap);

std::string makeBuffer(const abrsas::HuffmanTree &htree, const std::string &file);

void writeBuffer(const std::string &buf, const std::string &file);

void writeCodeTable(const abrsas::HuffmanTree &htree, const std::string &file);

bool parseArgs(int &argc, char *argv[], std::string &infile, std::string &outfile);

double getCompressionRatio(std::string &infile, std::string &outfile);

std::string decode(const std::string &file, const abrsas::HuffmanTree &htree);
#endif //HUFFMAN_DRIVER_H
