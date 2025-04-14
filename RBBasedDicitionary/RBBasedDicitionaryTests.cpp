#include "pch.h"
#include "CppUnitTest.h"
#include "../RBBasedDictionary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RBBasedDicitionaryTest
{
	TEST_CLASS(RBBasedDicitionary)
	{
	public:
		
		TEST_METHOD(InsertExistingValue)
		{
			RBBasedDictionary<int, int>* dictionary = new RBBasedDictionary<int, int>();
			dictionary->insertValue(10, 120);
			dictionary->insertValue(13, 30);
			dictionary->insertValue(15, 40);
			dictionary->insertValue(15, 41);
			Assert::AreEqual(dictionary->find(15), 41);
		}

		TEST_METHOD(CorrectInsertionTest)
		{
			RBBasedDictionary<std::string, int>* dictionary = new RBBasedDictionary < std::string, int >();
			dictionary->insertValue("cat", 120);
			dictionary->insertValue("wolf", 30);
			dictionary->insertValue("dog", 40);
			dictionary->insertValue("mouse", 41);

			Assert::AreEqual(dictionary->find("mouse"), 41);
			Assert::AreEqual(dictionary->find("wolf"), 30);
		}
		TEST_METHOD(FindNonExistingValue)
		{
			RBBasedDictionary<int, int>* dictionary = new RBBasedDictionary<int, int>();
			dictionary->insertValue(10, 120);
			dictionary->insertValue(13, 30);
			dictionary->insertValue(15, 40);
			dictionary->insertValue(15, 41);
			Assert::ExpectException<std::runtime_error>(([&]() { dictionary->find(120); }));
		}

		TEST_METHOD(FindNonExistingValueEmptyDicitonary)
		{
			RBBasedDictionary<int, int>* dictionary = new RBBasedDictionary<int, int>();
			Assert::ExpectException<std::runtime_error>(([&]() { dictionary->find(120); }));
		}

		TEST_METHOD(DeleteNonExistingValue)
		{
			RBBasedDictionary<int, int>* dictionary = new RBBasedDictionary<int, int>();
			dictionary->insertValue(10, 120);
			dictionary->insertValue(13, 30);
			dictionary->insertValue(15, 40);
			dictionary->insertValue(15, 41);
			Assert::ExpectException<std::runtime_error>(([&]() { dictionary->deleteValue(20); }));
		}

		TEST_METHOD(DeleteNonExistingValueEmptyDiconary)
		{
			RBBasedDictionary<int, int>* dictionary = new RBBasedDictionary<int, int>();
			dictionary->insertValue(10, 120);
			dictionary->insertValue(13, 30);
			dictionary->insertValue(15, 40);
			dictionary->insertValue(15, 41);
			Assert::ExpectException<std::runtime_error>(([&]() { dictionary->deleteValue(20); }));
		}

		TEST_METHOD(Iterator)
		{
			RBBasedDictionary<int, int>* dictionary = new RBBasedDictionary<int, int>();
			dictionary->insertValue(10, 120);
			dictionary->insertValue(13, 30);
			dictionary->insertValue(15, 40);
			dictionary->insertValue(15, 41);
			std::vector<std::pair<int, int>>* keyValuePairs = dictionary->inorder();
			std::pair<int, int> pairToFind(10, 120);
			auto it = std::find(keyValuePairs->begin(), keyValuePairs->end(), pairToFind);
			Assert::IsTrue(it != keyValuePairs->end());
		}
		
	};
}
