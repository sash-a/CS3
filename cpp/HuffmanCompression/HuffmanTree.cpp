//
// Created by sasha on 2018/04/08.
//

#include <iostream>
#include "HuffmanTree.h"

using namespace std;

abrsas::HuffmanTree::HuffmanTree(
        priority_queue<abrsas::HuffmanNode, vector<abrsas::HuffmanNode>, abrsas::Compare> nodes)
{
    genTree(nodes);
    genBitstrings("", root);
}

void abrsas::HuffmanTree::genBitstrings(std::string bits, shared_ptr<abrsas::HuffmanNode> node)
{
    if (node->isLeaf)
    {
        bitstring[node->letter] = bits;
        return;
    }

    genBitstrings(bits + "0", node->left);
    genBitstrings(bits + "1", node->right);
}

abrsas::HuffmanTree::HuffmanTree()
{
    root;
    bitstring;
}

abrsas::HuffmanTree::~HuffmanTree()
{
    root.reset();
}

// copy
abrsas::HuffmanTree::HuffmanTree(const abrsas::HuffmanTree &other)
{
    this->root = other.root;
    this->bitstring = other.bitstring;
}

// move
abrsas::HuffmanTree::HuffmanTree(abrsas::HuffmanTree &&other)
{
    this->root = move(other.root);
    this->bitstring = other.bitstring;
    //other.bitstring = unordered_map<char, string>;
}

void abrsas::HuffmanTree::genTree(
        std::priority_queue<abrsas::HuffmanNode, std::vector<abrsas::HuffmanNode>, abrsas::Compare> nodes)
{
    while (nodes.size() >= 2)
    {
        HuffmanNode left = nodes.top();
        nodes.pop();
        HuffmanNode right = nodes.top();
        nodes.pop();

        int totFreq = left.freq + right.freq;

        HuffmanNode parent('#', totFreq, false);
        parent.left = make_shared<HuffmanNode>(left);
        parent.right = make_shared<HuffmanNode>(right);

        nodes.push(parent);
    }

    root = make_shared<HuffmanNode>(nodes.top());
    nodes.pop();
}

bool abrsas::Compare::operator()(const abrsas::HuffmanNode &a, const abrsas::HuffmanNode &b)
{
    return a.freq > b.freq;
}
