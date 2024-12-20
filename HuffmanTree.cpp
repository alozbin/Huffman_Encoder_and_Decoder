#include "HuffmanTree.hpp"
#include <exception>
#include <stack>

HuffmanTree::HuffmanTree() : root(nullptr) {}		// constructor

HuffmanTree::~HuffmanTree()		// destructor
{
	clearTree(root);
}

void HuffmanTree::clearTree(HuffmanNode* node)		// used in destructor
{
	if (node)
	{
		clearTree(node->left);
		clearTree(node->right);
		delete node;
	}
}

// function compresses string depending on frequency of each character
std::string HuffmanTree::compress(const std::string inputStr)
{
	std::map<char, size_t> frequency;		// map used to store each character and its frequency
	for (char c : inputStr)
	{
		frequency[c]++;
	}

	HeapQueue<HuffmanNode*, HuffmanNode::Compare> frequencyQueue;
	for (std::map<char, size_t>::iterator it = frequency.begin(); it != frequency.end(); it++)		// iterator adds value and frequency of each map element to queue
	{
		HuffmanNode* node = new HuffmanNode(it->first, it->second);
		frequencyQueue.insert(node);
	}

	while (frequencyQueue.size() > 1)	// loop executes until one element left in queue
	{
		HuffmanNode* newLeft = frequencyQueue.min();		// new left leaf node is min value from queue
		frequencyQueue.removeMin();

		HuffmanNode* newRight = frequencyQueue.min();	// new right leaf node is next min value from queue
		frequencyQueue.removeMin();

		size_t sumFrequencies = newLeft->getFrequency() + newRight->getFrequency();		// sum of leaf frequencies used for intermediate node
		HuffmanNode* intermediate = new HuffmanNode(' ', sumFrequencies);		// intermediate node is parent to leaf nodes in binary tree
		intermediate->left = newLeft;
		intermediate->right = newRight;

		newLeft->parent = intermediate;
		newRight->parent = intermediate;

		frequencyQueue.insert(intermediate);		// intermediate node put back into priority queue
	}

	if (frequencyQueue.size() != 1)		// throws if queue size not exactly one
	{
		throw std::out_of_range("Error: queue operation failed.");
	}

	root = frequencyQueue.min();		// root of binary tree is final node from queue

	std::map<char, std::string> codeMap;
	std::string code;
	encodePreorder(root, codeMap, code);	// map populated with codes corresponding to characters

	std::string encoded;
	for (char c : inputStr)		// original string is iterated over to place character codes in correct order
	{
		encoded += codeMap[c];
	}

	return encoded;
}

// function serializes nodes of binary tree into string
std::string HuffmanTree::serializeTree() const
{
	std::string serialized;
	serializePostorder(root, serialized);	// tree traversed and leaf/branch nodes serialized
	return serialized;
}

std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree)
{
	// Rebuild the Huffman tree from the serialized string
	root = deserializeTree(serializedTree);

	std::string decoded;
	HuffmanNode* currentNode = root;

	for (char c : inputCode)
	{
		if (c == '0')        // move current node left when bit is 0
		{
			currentNode = currentNode->left;
		}
		else        // move current node right when bit is 1
		{
			currentNode = currentNode->right;
		}
		if (currentNode->isLeaf())
		{
			decoded += currentNode->getCharacter();        // adding character from leaf node to decoded string and moving currentnode back to root for next string of bits
			currentNode = root;
		}
	}
	return decoded;
}

HuffmanNode* HuffmanTree::deserializeTree(const std::string& serializedTree)
{
	std::stack<HuffmanNode*> stack;

	// iterating across serializedTree string
	for (size_t i = 0; i < serializedTree.length(); ++i)
	{
		if (serializedTree[i] == 'L')
		{
			stack.push(new HuffmanNode(serializedTree[++i], 0));    // pushing leaf node to stack with no frequency (freq not needed to decompress)
		}
		else if (serializedTree[i] == 'B')
		{
			// setting right and left child to top of stack and then popping
			HuffmanNode* right = stack.top(); stack.pop();
			HuffmanNode* left = stack.top(); stack.pop();
			HuffmanNode* branch = new HuffmanNode(' ', 0);    // empty node for branch

			// setting branch left and right child
			branch->left = left;
			branch->right = right;
			stack.push(branch);
		}
	}
	HuffmanNode* root = stack.top();
	return root;
}

// recursive function creates codes for characters from binary tree
void HuffmanTree::encodePreorder(HuffmanNode* node, std::map<char, std::string>& codeMap, std::string& prevCode)
{
	if (node == nullptr)	// base case when node points to null
	{
		return;
	}

	if (node->isLeaf())		// code and character of leaf node added to code map
	{
		codeMap[node->getCharacter()] = prevCode;
		return;
	}
	
	// traverses nodes in preorder, adding 0 for left and 1 for right to code
	std::string codeLeft = prevCode + "0";
	encodePreorder(node->left, codeMap, codeLeft);
	std::string codeRight = prevCode + "1";
	encodePreorder(node->right, codeMap, codeRight);
}

// recursive function serializes nodes in binary tree
void HuffmanTree::serializePostorder(HuffmanNode* node, std::string& serialized) const
{
	if (node == nullptr)	// base case when node points to null
	{
		return;
	}

	// child nodes traversed in postorder
	serializePostorder(node->left, serialized);
	serializePostorder(node->right, serialized);

	if (node->isLeaf())		// adds L and character to serialized string if leaf node
	{
		serialized += "L";
		serialized += node->getCharacter();
	}
	else if (node->isBranch())		// adds B to serialized string if internal node
	{
		serialized += "B";
	}
}