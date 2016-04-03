#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB\relations\Follows.h"
#include "PKB\relations\Modifies.h"
#include "PKB\relations\Parent.h"
#include "PKB\relations\Uses.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{		
	TEST_CLASS(FollowsTest)
	{
	public:
		
		TEST_METHOD(addNext)
		{
			Follows follows;
			follows.addNext(8, 9);

			Assert::IsTrue(follows.follows(8, 9));
		}

		TEST_METHOD(addPrev) {
			Assert::Fail();
		}

		TEST_METHOD(followsTrue) {
			Follows follows;
			follows.addNext(1, 2);
			follows.addNext(2, 3);
			follows.addNext(3, 4);
			follows.addNext(4, 5);
			follows.addNext(8, 9);

			Assert::IsTrue(follows.follows(4, 5));
		}

		TEST_METHOD(followsFalse) {
			Follows follows;
			follows.addNext(1, 2);
			follows.addNext(2, 3);
			follows.addNext(3, 4);
			follows.addNext(4, 5);
			follows.addNext(8, 9);

			Assert::IsFalse(follows.follows(3, 5));
		}

		TEST_METHOD(writeAll) {
			Assert::Fail();
		}

	};

	TEST_CLASS(ModifiesTEST) 
	{
	public:

		TEST_METHOD(add) {
			Assert::Fail();
		}

		TEST_METHOD(modifiesTrue) {
			Assert::Fail();
		}

		TEST_METHOD(modifiesFalse) {
			Assert::Fail();
		}

		TEST_METHOD(modifiesNoVarTrue) {
			Modifies modifies;
			modifies.add(10, 4);
			Assert::IsTrue(modifies.modifies(4, ""));
		}

		TEST_METHOD(modifiesNoVarFalse) {
			Modifies modifies;
			Assert::IsFalse(modifies.modifies(4, ""));
		}

		TEST_METHOD(writeAll) {
			Assert::Fail();
		}

	};
}