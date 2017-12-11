#include "deque.h"
#include "gtest\gtest.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <queue>

int getRand(int l, int r) {
	return l + rand() % (r - l + 1);
}

int RandInt(int l, int r) {
	return getRand(l, r);
}

std::string RandString(int minSize, int maxSize) {
	int Size = getRand(minSize, maxSize);
	std::string res = std::string(Size, ' ');
	for (int i = 0; i < Size; ++i)
		res[i] = (char)getRand(0, 255);
	return res;
}

template<typename T>
void generateRandomDeque(Deque<T> & q, int minSize, int maxSize, T(*generator)(int, int)) {
	int Size = RandInt(minSize, maxSize);
	for (int i = 0; i < Size; ++i) {
		q.push_back(generator(1, 100));
	}
}

TEST(ClassicDequetest, simpleTest) {
	int n = RandInt(1000000, 2000000);
	Deque<int> q;
	std::deque<int> qq;
	for (int i = 0; i < n; ++i) {
		int type = RandInt(1, 5), value;
		ASSERT_EQ(qq.size(), q.size());
		ASSERT_FALSE(qq.empty() ^ q.empty());
		while (qq.size() == 0 && type > 2)
			type = RandInt(1, 2);
		if (type == 1) {
			value = RandInt(-10000, 10000);
			q.push_back(value);
			qq.push_back(value);
		}
		if (type == 2) {
			value = RandInt(-10000, 10000);
			q.push_front(value);
			qq.push_front(value);
		}
		if (type == 3) {
			q.pop_back();
			qq.pop_back();
		}
		if (type == 4) {
			q.pop_front();
			qq.pop_front();
		}
		if (type == 5) {
			int pos = RandInt(0, qq.size() - 1);
			ASSERT_EQ(q[pos], qq[pos]);
		}
	}
}

TEST(firstIteratorTest, IteratorFor) {
	Deque<std::string> q;
	generateRandomDeque<std::string>(q, 1, 1000000, RandString);
	Deque<std::string> qq;
	qq = q;
	for (Deque<std::string>::iterator it = q.begin(); it != q.end(); ++it) {
		ASSERT_EQ(*it, qq.front());
		qq.front() += "abc";
		ASSERT_EQ(*it + "abc", qq.front());
		qq.pop_front();
	}
	qq = q;
	for (Deque<std::string>::reverse_iterator it = q.rbegin(); it != q.rend(); ++it) {
		ASSERT_EQ(*it, qq.back());
		qq.back() += "abc";
		ASSERT_EQ(*it + "abc", qq.back());
		qq.pop_back();
	}
	qq = q;
	for (Deque<std::string>::const_iterator it = q.cbegin(); it != q.cend(); ++it) {
		ASSERT_EQ(*it, qq.front());
		qq.front() += "abc";
		ASSERT_EQ(*it + "abc", qq.front());
		qq.pop_front();
	}
	qq = q;
	for (Deque<std::string>::const_reverse_iterator it = q.crbegin(); it != q.crend(); ++it) {
		ASSERT_EQ(*it, qq.back());
		qq.back() += "abc";
		ASSERT_EQ(*it + "abc", qq.back());
		qq.pop_back();
	}


}

TEST(IteratorDistancetest, IteratorDist) {
	for (int it = 0; it < 1000; ++it) {
		Deque<std::string> q;
		generateRandomDeque<std::string>(q, 10, 100, RandString);
		ASSERT_FALSE(q.empty());
		ASSERT_EQ(q.size(), q.end() - q.begin());
		ASSERT_EQ(q.size(), q.rend() - q.rbegin());
		int pos1 = RandInt(0, q.size() - 1);
		int pos2 = RandInt(0, q.size() - 1);
		if (it % 4 == 0) {
			Deque<std::string>::iterator it1 = q.begin() + pos1;
			Deque<std::string>::iterator it1Ok = q.begin();
			for (int i = 0; i < pos1; ++i)++it1Ok;
			Deque<std::string>::iterator it2 = q.begin() + pos2;
			Deque<std::string>::iterator it2Ok = q.begin();
			for (int i = 0; i < pos2; ++i)++it2Ok;
			ASSERT_EQ(it1, it1Ok);
			ASSERT_EQ(it2, it2Ok);
			ASSERT_EQ(pos1 - pos2, it1 - it2);
		}
		if (it % 4 == 1) {
			Deque<std::string>::reverse_iterator it1 = q.rbegin() + pos1;
			Deque<std::string>::reverse_iterator it1Ok = q.rbegin();
			for (int i = 0; i < pos1; ++i)++it1Ok;
			Deque<std::string>::reverse_iterator it2 = q.rbegin() + pos2;
			Deque<std::string>::reverse_iterator it2Ok = q.rbegin();
			for (int i = 0; i < pos2; ++i)++it2Ok;
			ASSERT_EQ(it1, it1Ok);
			ASSERT_EQ(it2, it2Ok);
			ASSERT_EQ(pos1 - pos2, it1 - it2);
		}
		if (it % 4 == 2) {
			Deque<std::string>::const_iterator it1 = q.cbegin() + pos1;
			Deque<std::string>::const_iterator it1Ok = q.cbegin();
			for (int i = 0; i < pos1; ++i)++it1Ok;
			Deque<std::string>::const_iterator it2 = q.cbegin() + pos2;
			Deque<std::string>::const_iterator it2Ok = q.cbegin();
			for (int i = 0; i < pos2; ++i)++it2Ok;
			//			ASSERT_EQ(it1, it1Ok);
			//			ASSERT_EQ(it2, it2Ok);
			ASSERT_EQ(pos1 - pos2, it1 - it2);
		}
		if (it % 4 == 3) {
			Deque<std::string>::const_reverse_iterator it1 = q.crbegin() + pos1;
			Deque<std::string>::const_reverse_iterator it1Ok = q.crbegin();
			for (int i = 0; i < pos1; ++i)++it1Ok;
			Deque<std::string>::const_reverse_iterator it2 = q.crbegin() + pos2;
			Deque<std::string>::const_reverse_iterator it2Ok = q.crbegin();
			for (int i = 0; i < pos2; ++i)++it2Ok;
			ASSERT_EQ(it1, it1Ok);
			ASSERT_EQ(it2, it2Ok);
			ASSERT_EQ(pos1 - pos2, it1 - it2);
		}
	}
}

