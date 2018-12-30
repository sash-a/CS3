//
// Created by abrsas002 on 2018/04/11.
//

#include "HuffmanNode.h"
#include "HuffmanTree.h"
#include "Driver.h"

#define CATCH_CONFIG_MAIN //So that catch will define a main method

#include "catch.hpp"

using namespace std;
using namespace abrsas;

// Node test cases
TEST_CASE("Huffman node", "Testing constructors")
{
    // Default constructor
    HuffmanNode n;

    REQUIRE(n.freq == 0);
    REQUIRE(n.letter == '#');
    REQUIRE(n.left == nullptr);
    REQUIRE(n.right == nullptr);

    // Non default constructor
    char c = 'a';
    int freq = 1;
    HuffmanNode node(c, freq, true);

    REQUIRE(node.freq == freq);
    REQUIRE(node.letter == c);
    REQUIRE(node.left == nullptr);
    REQUIRE(node.right == nullptr);

    // Assigning pointers
    shared_ptr<HuffmanNode> left;
    shared_ptr<HuffmanNode> right;

    node.left = (left);
    node.right = (right);

    REQUIRE(node.left == left);
    REQUIRE(node.right == right);

    // Copy constructor
    HuffmanNode copy = node;
    REQUIRE(copy.right == node.right);
    REQUIRE(copy.left == node.left);
    REQUIRE(copy.letter == node.letter);
    REQUIRE(copy.freq == node.freq);

    // Move constructor
    HuffmanNode move(std::move(node));
    REQUIRE(move.freq == freq);
    REQUIRE(move.letter == c);
    REQUIRE(move.left == copy.left);
    REQUIRE(move.right == copy.right);
}

TEST_CASE("HuffmanTree", "Constructors")
{
    priority_queue<HuffmanNode, vector<HuffmanNode>, Compare> pq;
    pq.push(HuffmanNode('a', 1, true));
    pq.push(HuffmanNode('b', 2, true));
    pq.push(HuffmanNode('c', 3, true));
    pq.push(HuffmanNode('d', 4, true));

    // Default constructor
    HuffmanTree defaultContr;
    REQUIRE(defaultContr.root == nullptr);

    defaultContr.genTree(pq);
    REQUIRE(defaultContr.root->freq == 10);
    REQUIRE(defaultContr.root->left->letter == 'd');

    // Non default constructor
    HuffmanTree nonDefault(pq);
    REQUIRE(nonDefault.root->freq == 10); // checking root
    REQUIRE(nonDefault.root->left->letter == 'd'); // checking letter
    // checking bitstring
    REQUIRE(nonDefault.bitstring['a'] == "110");
    REQUIRE(nonDefault.bitstring.at('b') == "111");
    REQUIRE(nonDefault.bitstring.at('d') == "0");
    REQUIRE(nonDefault.bitstring.at('c') == "10");

    // Copy
    HuffmanTree copy = nonDefault;
    REQUIRE(copy.root == nonDefault.root);
    REQUIRE(copy.bitstring == nonDefault.bitstring);

    HuffmanTree move = std::move(copy);
    REQUIRE(move.root == nonDefault.root);
    REQUIRE(move.bitstring == nonDefault.bitstring);
}

TEST_CASE("Main", "Methods in main")
{
    // test.txt is just: abbcccdddd
    unordered_map<char, int> freqMap = findFreq("test.txt");
    REQUIRE(freqMap['a'] == 1);
    REQUIRE(freqMap['b'] == 2);
    REQUIRE(freqMap['c'] == 3);
    REQUIRE(freqMap['d'] == 4);

    priority_queue<HuffmanNode, vector<HuffmanNode>, Compare> pq = mapToQueue(freqMap);

    HuffmanTree htree(pq);

    REQUIRE(pq.top().letter == 'a');
    pq.pop();
    REQUIRE(pq.top().letter == 'b');
    pq.pop();
    REQUIRE(pq.top().letter == 'c');
    pq.pop();
    REQUIRE(pq.top().letter == 'd');
    pq.pop();

    string buf = makeBuffer(htree, "test.txt");
    REQUIRE(buf == "1101111111010100000");

}