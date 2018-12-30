//
// Created by sasha on 2018/04/08.
//

#ifndef HUFFMANCOMPRESSION_HUFFMANNODE_H
#define HUFFMANCOMPRESSION_HUFFMANNODE_H

#include <memory>

namespace abrsas
{
    class HuffmanNode
    {
    public:
        char letter;
        int freq;
        bool isLeaf;

        std::shared_ptr<HuffmanNode> left;
        std::shared_ptr<HuffmanNode> right;

        HuffmanNode();

        HuffmanNode(const char &letter, const int &freq, const bool &isLeaf);

        ~HuffmanNode() noexcept;

        HuffmanNode(HuffmanNode &&other) noexcept;

        HuffmanNode &operator=(const HuffmanNode &other);

        HuffmanNode(const HuffmanNode &other);

    private:
    };
}


#endif //HUFFMANCOMPRESSION_HUFFMANNODE_H
