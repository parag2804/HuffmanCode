#include "HuffmanTree.hpp"

void removeTree(HuffmanNode* root)
{
	if (!root->isLeaf())									//if root not a leaf
	{
		removeTree(root->left);
		removeTree(root->right);							//it will traverse in postfix way
	}
	delete root;											//deleting the root node
}

HuffmanTree::~HuffmanTree()									//to deconstruct the tree
{
	removeTree(nodeRoot);						
}

std::string HuffmanTree::compress(const std::string inStr)						//compression
{
	std::map<char, int> mapCharFreq;											//using map to make character frequency
	for(int i = 0; i < inStr.length(); i++)
	{
		if(mapCharFreq.count(inStr.at(i)) >= 1)									//conditional statement to check if the character is alredy there in map
    		mapCharFreq.at(inStr.at(i)) += 1;									//if condition passes, it will increment frequency by 1
		
		else
		{
			auto pair = std::make_pair(inStr.at(i), 1);
			mapCharFreq.insert(pair);											//else statement to insert the character 
		}
	}

	HeapQueue<HuffmanNode*, HuffmanNode::Compare> priorQueue;					//using HeapQueue to insert characters and their frequencies
	auto iterate = mapCharFreq.begin();
	while(iterate != mapCharFreq.end())
	{
		HuffmanNode* newNode = new HuffmanNode(iterate->first, iterate->second);
		priorQueue.insert(newNode);
		iterate++;
	}

	while(priorQueue.size() != 1)												//looping until only 1 elemnt left
	{
		auto firstPair = priorQueue.min();
		priorQueue.removeMin();													//removing pair once
		auto secondPair = priorQueue.min();
		priorQueue.removeMin();													//removing pair twice

		int parentNode = (firstPair->getFrequency() + secondPair->getFrequency());			//creating parent node equal to sum of children frequency pairs
		HuffmanNode* intrmdtNode = new HuffmanNode('\0', parentNode, nullptr, firstPair, secondPair);

		firstPair->parent = intrmdtNode; secondPair->parent = intrmdtNode;
		priorQueue.insert(intrmdtNode); 										//inserting the intermediate node back into the queue
	}

	nodeRoot = priorQueue.min();												//last pair
	std::string cipherChar = "";
	std::map<char, std::string> mapChar;										//Map with the char
	prefixSerialize(&mapChar, nodeRoot, cipherChar);

	for(char i : inStr) 
		cipherChar += mapChar.at(i);											//replacing with new length

	return cipherChar;
}

void prefixSerialize(std::map<char, std::string>* mapChar, HuffmanNode* root, std::string stringC)
{
	if (!root->isLeaf())														//condition if it's leaf or not
	{
		prefixSerialize(mapChar, root->left, stringC + '0'); 						// left add 0
		prefixSerialize(mapChar, root->right, stringC + '1'); 						// right add 1
	}
	else
	{
		auto codePair = std::make_pair(root->getCharacter(), stringC);
		mapChar->insert(codePair);
	}
	return;
}

std::string HuffmanTree::serializeTree() const
{ 
	return postfixSerialize(nodeRoot);
}

const std::string postfixSerialize(HuffmanNode* root)
{
	std::string serialString = "";
	if (root->isLeaf())														//if node is leaf, add L in addition to character
	{
			serialString += "L"; 
			serialString += root->getCharacter();
			return serialString;
	}
	return (postfixSerialize(root->left) + postfixSerialize(root->right) + 'B'); 				//add B to the serialized tree
}

std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree)			//decompressing
{ 
  std::string outputStr = "";
  std::stack<HuffmanNode*> huffmanStack; 

  for(int i = 0; i < serializedTree.length(); i++)												//looping through serializedTree
  {
    if(serializedTree[i] == 'L')
	{
      i++;
      HuffmanNode* t = new HuffmanNode(serializedTree[i], 0);
      huffmanStack.push(t);															//pushing node into stack
    }
    
	else if(serializedTree[i] == 'B')
	{ 
      HuffmanNode* right = huffmanStack.top();
      huffmanStack.pop();
      HuffmanNode* left = huffmanStack.top();
      huffmanStack.pop(); 

      HuffmanNode* parent = new HuffmanNode(0, 0, nullptr, left, right);
      huffmanStack.push(parent); 												//pushing the parent node back into the stack
    }
  }

HuffmanNode* root = huffmanStack.top(); 										//making root the top element
	int i = 0;
	HuffmanNode* t;																//temporary variable t
	bool isRoot = false;
	
	while(i < inputCode.length())
	{ 
		if (!isRoot)
		{ 
		t = root; 
		isRoot = true;
		}
		else if(isRoot)
		{
		if(t->isLeaf())
		{
			outputStr += t->getCharacter(); 									//concatenate the character to the string 
			isRoot = false;
		}
		else
		{
			if(inputCode[i] == '0')
			{
			t = t->left; 
			}
			else if(inputCode[i] == '1')
			{
			t = t->right;
			}
			i++;
		}
		}
	}
  
  	outputStr += (t->getCharacter());
  	return outputStr; 															//returning the output string
}