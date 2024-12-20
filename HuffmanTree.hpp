#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <string>
#include <map>

class HuffmanTree : public HuffmanTreeBase
{
public:
	HuffmanTree();		// constructor
	~HuffmanTree();		// destructor

	std::string compress(const std::string inputStr) override;		// function compresses string depending on frequency of each character
	std::string serializeTree() const override;		// function serializes nodes of binary tree into string
	std::string decompress(const std::string inputCode, const std::string serializedTree) override;

private:
	HuffmanNode* root;

	void clearTree(HuffmanNode* node);		// deletes all tree nodes
	void encodePreorder(HuffmanNode* node, std::map<char, std::string>& codeMap, std::string& prevCode);	// recursive function creates codes for characters from binary tree
	void serializePostorder(HuffmanNode* node, std::string& serialized) const;		// recursive function serializes nodes in binary tree
	HuffmanNode* deserializeTree(const std::string& serializedTree);
};

#endif /* HUFFMANTREE_HPP */