#include "RBBasedDictionary.h"
//#include <string>
#include <iostream>


int main()
{
	std::cout << "Meow" << std::endl;
	/*RedBlackTree<int>* intTree = new RedBlackTree<int>();
	// 
	//intTree->insert(10);
	//intTree->insert(20);
	//intTree->insert(5);
	//intTree->insert(4);*/
	//RedBlackTree<int> *intTree = new RedBlackTree<int>();

	//intTree->insertValue(5);
	//intTree->insertValue(4);
	//intTree->insertValue(10);
	//intTree->insertValue(20);
	//


	//intTree->inorder();

	RBBasedDictionary<int, int>* dictionary = new RBBasedDictionary<int, int>();
	dictionary->insertValue(10, 120);
	dictionary->insertValue(13, 30);
	dictionary->insertValue(15, 40);
	dictionary->inorder();
	std::cout << "Search result for existing key: " << dictionary->find(13) << std::endl;
	dictionary->deleteValue(10);
	dictionary->inorder();


}