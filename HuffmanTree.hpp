#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <string>
#include <map>
#include <stack>

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"

class HuffmanTree : public HuffmanTreeBase
{
    public:
        HuffmanTree()
        {
            nodeRoot = new HuffmanNode('\0', 0);
        }
        ~HuffmanTree();

        std::string compress(const std::string inputStr);
        std::string serializeTree() const;
        std::string decompress(const std::string inputCode, const std::string serializedTree);

    private:
        HuffmanNode* nodeRoot;
};

void prefixSerialize(std::map < char, std::string >* prefixCode, HuffmanNode* root, std::string str);
const std::string postfixSerialize(HuffmanNode* root);
void removeTree(HuffmanNode* root);

#endif /* HUFFMANTREE_H */ 