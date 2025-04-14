#include "RBBasedDictionary.h"
#include <iostream>

int main()
{
	std::cout << "Meow" << std::endl;
	

	RBBasedDictionary<int, int>* dictionary = new RBBasedDictionary<int, int>();
	dictionary->insertValue(20, 120);
	dictionary->insertValue(21, 30);
	dictionary->insertValue(22, 40);
	dictionary->insertValue(10, 41);
	dictionary->inorder();
	std::cout << "Search result for existing key: " << dictionary->find(20) << std::endl;
	try 
	{
		dictionary->deleteValue(900);
	} 
	catch (const std::runtime_error& e)
	{
		std::cout << "Error " << e.what() << std::endl;
	}
	dictionary->inorder();
}