//
// Created by sasha on 2018/04/08.
//

#ifndef HUFFMANCOMPRESSION_HUFFMANTREE_H
#define HUFFMANCOMPRESSION_HUFFMANTREE_H

#include <memory>
#include <queue>
#include <unordered_map>
#include <string>
#include <memory>
#include "HuffmanNode.h"

namespace abrsas
{
    class Compare
    {
    public:
        bool operator()(const HuffmanNode &a, const HuffmanNode &b);
    };

    class HuffmanTree
    {
    public:
        std::shared_ptr<HuffmanNode> root;
        std::unordered_map<char, std::string> bitstring;

        HuffmanTree(std::priority_queue<abrsas::HuffmanNode, std::vector<abrsas::HuffmanNode>, abrsas::Compare> nodes);

        HuffmanTree();

        HuffmanTree(const HuffmanTree &);

        HuffmanTree(HuffmanTree &&);

        ~HuffmanTree();

        void genTree(std::priority_queue<abrsas::HuffmanNode, std::vector<abrsas::HuffmanNode>, abrsas::Compare> nodes);

    private:
        void genBitstrings(std::string bits, std::shared_ptr<HuffmanNode> node);
    };
}


#endif //HUFFMANCOMPRESSION_HUFFMANTREE_H
