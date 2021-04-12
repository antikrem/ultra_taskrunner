#include "pch.h"
#include "CppUnitTest.h"

#include "../ultra_taskrunner/atomic_queue.hpp"

#include "delayed_adder.h"
#include <random>
#include <set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ultrataskrunnertests
{
	TEST_CLASS(AtomicQueueTests)
	{
	public:

		TEST_METHOD(WhenIntAdded_ReturnsAdded)
		{
			AtomicQueue<int> sut;
			sut.Add(10);
			
			int v;

			Assert::AreEqual(true, sut.Get(&v));
			Assert::AreEqual(10, v);
		}

		TEST_METHOD(WhenStopped_AlwaysReturnsFalse)
		{
			AtomicQueue<int> sut;
			sut.Add(10);

			sut.Stop();

			int v;

			Assert::AreEqual(false, sut.Get(&v));
		}

		TEST_METHOD(WhenAdded_BlockUntilAdded)
		{
			AtomicQueue<int> sut;
			DelayedAdder<int> adder(&sut);

			auto tic = adder.Tic();
			adder.DelayedAdd(100, 10);
			
			int v;

			Assert::AreEqual(true, sut.Get(&v));

			auto toc = adder.Toc(tic);

			Assert::AreEqual(true, toc >= 100);
		}

		TEST_METHOD(WhenAdded_ReturnsInOrderRecieved)
		{
			AtomicQueue<int> sut;
			DelayedAdder<int> adder(&sut);

			auto tic = adder.Tic();
			adder.DelayedAdd(100, 1);
			adder.DelayedAdd(200, 2);
			adder.DelayedAdd(300, 3);

			int v;

			Assert::AreEqual(true, sut.Get(&v));
			Assert::AreEqual(1, v);
			Assert::AreEqual(true, sut.Get(&v));
			Assert::AreEqual(2, v);
			Assert::AreEqual(true, sut.Get(&v));
			Assert::AreEqual(3, v);

			sut.Stop();
			Assert::AreEqual(false, sut.Get(&v));
		}
		template <class T>
		std::set<T> VecToSet(std::vector<T> vec)
		{
			std::set<T> set;

			for (auto& i : vec)
			{
				set.insert(i);
			}

			return set;
		}

		TEST_METHOD(StressTest_Kagiyama)
		{
			std::vector<int> samples;
			for (int i = 0; i < 1000; i++) 
			{
				samples.push_back(i);
			}

			AtomicQueue<int> sut;
			DelayedAdder<int> adder(&sut);

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> distr(1000, 5000);

			for (auto i : samples)
			{
				adder.DelayedAdd(distr(gen), i);
			}

			adder.DelayedStop(50000);

			std::vector<int> sampled;
			int value;
			while (sut.Get(&value))
			{
				sampled.push_back(value);;
			};

			Sleep(60000);

			Assert::AreEqual(true, VecToSet(sampled) == VecToSet(samples));
		}
	};
}
