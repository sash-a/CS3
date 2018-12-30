//
// Created by sasha on 2018/04/08.
//
#include "HuffmanNode.h"

abrsas::HuffmanNode::HuffmanNode(const char &letter, const int &freq, const bool &isLeaf)
{
    this->isLeaf = isLeaf;
    this->letter = letter;
    this->freq = freq;
    left;
    right;
}

abrsas::HuffmanNode::~HuffmanNode() noexcept
{
    left.reset();
    right.reset();
}

// Move
abrsas::HuffmanNode::HuffmanNode(abrsas::HuffmanNode &&other) noexcept
        : right(std::move(other.right)),
          left(std::move(other.left)),
          letter(other.letter),
          freq(other.freq),
          isLeaf(other.isLeaf)
{}


//Copy
abrsas::HuffmanNode::HuffmanNode(const abrsas::HuffmanNode &other)
{
    this->right = other.right;
    this->left = other.left;
    this->freq = other.freq;
    this->letter = other.letter;
    this->isLeaf = other.isLeaf;
}

abrsas::HuffmanNode::HuffmanNode()
{
    left;
    right;
    letter = '#';
    freq = 0;
    isLeaf = false;
}

abrsas::HuffmanNode &abrsas::HuffmanNode::operator=(const abrsas::HuffmanNode &other)
= default;

