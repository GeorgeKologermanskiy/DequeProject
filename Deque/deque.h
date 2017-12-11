#pragma once
#include <iostream>
#include <iterator>
#include <algorithm>

template<typename T>
class Deque {
private:
	struct Node {
		T value_;
		int pos_;
		bool isEmpty_;
		Node() {}
		Node(const int & pos) :
			pos_(pos),
			isEmpty_(true) {}
		Node(const T & value, const int & pos) :
			value_(value),
			pos_(pos),
			isEmpty_(false) {}
		Node(const Node & b) :
			value_(b.value_),
			pos_(b.pos_),
			isEmpty_(b.isEmpty_) {}
		Node(const Node * b) :
			value_(b->value_),
			pos_(b->pos_),
			isEmpty_(b->isEmpty_) {}
		~Node() {}
		Node& operator=(const Node & b) {
			if (this == &b)return *this;
			value_ = b.value_;
			pos_ = b.pos_;
			return *this;
		}
	};
	typedef Node* pNode;
	typedef const Deque<T>* pDeque;
	pNode *a_;
	int Head_, Tail_;
	int aSize_;
	int RealSize_;
	int Next(int value)const {
		return (value + 1) % aSize_;
	}
	int Prev(int value)const {
		return (value - 1 + aSize_) % aSize_;
	}
	void Upgrade() {
		if (RealSize_ + 1 != aSize_)return;
		pNode * NewArr = new pNode[aSize_ + aSize_];
		for (int i = aSize_; i < aSize_ + aSize_; ++i) {
			NewArr[i] = new Node(i);
		}
		for (int i = 0; i < aSize_; ++i) {
			NewArr[i] = a_[(i + Tail_ + 1) % aSize_];
			NewArr[i]->pos_ = i;
		}
		delete[] a_;
		a_ = new pNode[aSize_ + aSize_];
		Head_ = aSize_ - 1;
		aSize_ += aSize_;
		Tail_ = aSize_ - 1;
		for (int i = 0; i < aSize_; ++i)
			a_[i] = NewArr[i];
		delete[] NewArr;
	}
	void Degrade() {
		if (RealSize_ * 4 > aSize_)
			return;
		pNode * NewArr = new pNode[RealSize_ + 2];
		for (int i = 0; i < RealSize_ + 2; ++i) {
			NewArr[i] = a_[Tail_];
			if (NewArr[i] != 0) {
				NewArr[i]->pos_ = i;
			}
			Tail_ = Next(Tail_);
		}
		aSize_ = RealSize_ + 2;
		delete[] a_;
		a_ = new pNode[aSize_];
		for (int i = 0; i < aSize_; ++i)
			a_[i] = NewArr[i];
		Head_ = aSize_ - 1;
		Tail_ = 0;
		delete[] NewArr;
	}

	template<bool R>
	class NormalIterator;
	template<bool R>
	class ConstIterator :public std::iterator<std::random_access_iterator_tag, T> {
	public:
		pDeque Where_;
		pNode ptr_;
		ConstIterator<R>() : ptr_(0) {}
		ConstIterator<R>(const ConstIterator<R> & b) :
			Where_(b.Where_),
			ptr_(b.ptr_) {}
		ConstIterator<R>(pDeque where, pNode pos) :
			Where_(where),
			ptr_(pos) {}

		ConstIterator<R>& operator=(const ConstIterator<R> b) {
			if (this == &b)return *this;
			Where_ = b.Where_;
			ptr_ = b.ptr_;
			return *this;
		}

		bool operator==(const ConstIterator<R> & b)const {
			return b.ptr_->pos_ == ptr_->pos_;
		}
		bool operator!=(const ConstIterator<R> & b)const {
			return !(*this == b);
		}
		bool operator<(const ConstIterator<R> & b)const {
			if (Tail_ <= Head_)return R ^ (ptr_->pos_ < b->ptr_->pos_);
			if (ptr_->pos_ <= Head_ && Tail_ <= b->ptr_->pos_)
				return R;
			if (b->ptr_->pos_ <= Head_ && Tail_ <= ptr_->pos_)
				return R ^ true;
			return R ^ (ptr_->pos_ < b->ptr_->pos_);
		}
		bool operator<=(const ConstIterator<R> & b)const {
			return (*this < b || *this == b);
		}
		bool operator>(const ConstIterator<R> & b)const {
			return !(*this <= b);
		}
		bool operator>=(const ConstIterator<R> & b)const {
			return !(*this < b);
		}

		ConstIterator<R>& operator++() {
			if (R) {
				ptr_ = Where_->a_[Where_->Prev(ptr_->pos_)];
			}
			else {
				ptr_ = Where_->a_[Where_->Next(ptr_->pos_)];
			}
			return *this;
		}
		ConstIterator<R> operator++(int) {
			ConstIterator ret = *this;
			++*this;
			return ret;
		}
		ConstIterator<R>& operator--() {
			if (R) {
				ptr_ = Where_->a_[Where_->Next(ptr_->pos_)];
			}
			else {
				ptr_ = Where_->a_[Where_->Prev(ptr_->pos_)];
			}
			return *this;
		}
		ConstIterator<R> operator--(int) {
			ConstIterator<R> ret = *this;
			--*this;
			return ret;
		}

		ConstIterator<R>& operator+=(int n) {
			int size = 0;
			int go = n * (R ? -1 : 1);
			if (go < 0) {
				if (ptr_->pos_ >= Where_->Tail_) {
					size = ptr_->pos_ - Where_->Tail_;
				}
				else {
					size += ptr_->pos_;
					size += Where_->aSize_ - Where_->Tail_;
				}
			}
			else {
				if (ptr_->pos_ <= Where_->Head_) {
					size = Where_->Head_ - ptr_->pos_;
				}
				else {
					size += Where_->Head_;
					size += Where_->aSize_ - ptr_->pos_;
				}
			}
			int NewPos = (ptr_->pos_ + go + Where_->aSize_) % Where_->aSize_;
			ptr_ = Where_->a_[NewPos];
			return *this;
		}
		ConstIterator<R>& operator-=(int n) {
			return *this += -n;
		}

		ConstIterator<R> operator+(int n)const {
			ConstIterator<R> A = *this;
			return A += n;
		}
		ConstIterator<R> operator-(int n)const {
			ConstIterator<R> A = *this;
			return A -= n;
		}

		int operator-(const ConstIterator<R> & b)const {
			int sign = (R ? -1 : 1);
			if (Where_->Tail_ <= Where_->Head_) {
				return sign * (ptr_->pos_ - b.ptr_->pos_);
			}
			if (ptr_->pos_ <= Where_->Head_ && Where_->Tail_ <= b.ptr_->pos_) {
				return sign * (ptr_->pos_ + (Where_->aSize_ - b.ptr_->pos_));
			}
			if (b.ptr_->pos_ <= Where_->Head_ && Where_->Tail_ <= ptr_->pos_) {
				return -sign * (b.ptr_->pos_ + (Where_->aSize_ - ptr_->pos_));
			}
			return sign * (ptr_->pos_ - b.ptr_->pos_);
		}

		const T operator*()const {
			return ptr_->value_;
		}
		const T* operator->()const {
			return ptr_->value_;
		}
		const T operator[](int n)const {
			return *(*this + n);
		}

	};
	template<bool R>
	class NormalIterator :public ConstIterator<R> {
	protected:
		NormalIterator<R>() : ConstIterator<R>() {}
		NormalIterator<R>(const ConstIterator<R> & b) : ConstIterator<R>(b) {}
		explicit NormalIterator<R>(const NormalIterator<R> & b) : ConstIterator<R>(b.Where_, b.ptr_) {}
		explicit NormalIterator<R>(pDeque where, pNode pos) : ConstIterator<R>(where, pos) {}
	public:
		NormalIterator<R>& operator=(const NormalIterator<R> & b) {
			if (this == &b)return *this;
			ConstIterator<R>::ptr_ = b.ptr_;
			ConstIterator<R>::Where_ = b.Where_;
			return *this;
		}

		T& operator*() {
			return ConstIterator<R>::ptr_->value_;
		}
		T* operator -> () {
			return &ConstIterator<R>::ptr_->value_;
		}
	};
public:
	class iterator : public NormalIterator<false> {
	public:
		iterator() : NormalIterator<false>() {}
		iterator(NormalIterator<false> b) :NormalIterator<false>(b) {}
		iterator(ConstIterator<false> b) :NormalIterator<false>(b) {}
		explicit iterator(pDeque where, pNode pos) : NormalIterator<false>(where, pos) {}

		iterator& operator=(iterator b) {
			if (this == &b)return *this;
			ConstIterator<false>::ptr_ = b.ptr_;
			ConstIterator<false>::Where_ = b.Where_;
			return *this;
		}
	};
	class const_iterator : public ConstIterator<false> {
	public:
		const_iterator() : ConstIterator<false>() {}
		const_iterator(ConstIterator<false> b) : ConstIterator<false>(b) {}
		explicit const_iterator(pDeque where, pNode pos) : ConstIterator<false>(where, pos) {}

		const_iterator& operator=(const_iterator b) {
			if (this == &b)return *this;
			ConstIterator<false>::ptr_ = b.ptr_;
			ConstIterator<false>::Where_ = b.Where_;
			return *this;
		}
	};
	class reverse_iterator : public NormalIterator<true> {
	public:
		reverse_iterator() : NormalIterator<true>() {}
		reverse_iterator(NormalIterator<true> b) :NormalIterator<true>(b) {}
		reverse_iterator(ConstIterator<true> b) :NormalIterator<true>(b) {}
		explicit reverse_iterator(pDeque where, pNode pos) : NormalIterator<true>(where, pos) {}

		reverse_iterator& operator=(reverse_iterator b) {
			if (this == &b)return *this;
			ConstIterator<true>::ptr_ = b.ptr_;
			ConstIterator<true>::Where_ = b.Where_;
			return *this;
		}
	};
	class const_reverse_iterator : public ConstIterator<true> {
	public:
		const_reverse_iterator() : ConstIterator<true>() {}
		const_reverse_iterator(ConstIterator<true> b) : ConstIterator<true>(b) {}
		explicit const_reverse_iterator(pDeque where, pNode pos) : ConstIterator<true>(where, pos) {}

		const_reverse_iterator& operator=(const_reverse_iterator b) {
			if (this == &b)return *this;
			ConstIterator<true>::ptr_ = b.ptr_;
			ConstIterator<true>::Where_ = b.Where_;
			return *this;
		}
	};

	Deque<T>() :
		Head_(0),
		Tail_(1),
		aSize_(2),
		RealSize_(0)
	{
		a_ = new pNode[2];
		a_[0] = new Node(0);
		a_[1] = new Node(1);
	}
	Deque<T>(const Deque<T> & b) :
		Head_(b.Head_),
		Tail_(b.Tail_),
		aSize_(b.aSize_),
		RealSize_(b.RealSize_)
	{
		a_ = new pNode[aSize_];
		for (int i = 0; i < aSize_; ++i)
			a_[i] = new Node(b.a_[i]);
	}
	~Deque<T>() {
		delete[] a_;
	}
	Deque<T>& operator=(const Deque<T> & b) {
		if (this == &b)return *this;
		Head_ = b.Head_;
		Tail_ = b.Tail_;
		aSize_ = b.aSize_;
		RealSize_ = b.RealSize_;
		delete[] a_;
		a_ = new pNode[aSize_];
		for (int i = 0; i < aSize_; ++i)
			a_[i] = new Node(b.a_[i]);
		return *this;
	}
	bool operator==(const Deque<T> & b)const {
		return(this == &b);
	}
	bool operator!=(const Deque<T> & b)const {
		return !(*this == b);
	}
	bool empty()const {
		return RealSize_ == 0;
	}
	size_t size() const {
		return (size_t)RealSize_;
	}
	void clear() {
		Head_ = 0;
		Tail_ = 1;
		aSize_ = 2;
		RealSize_ = 0;
		delete[] a_;
		a_ = new pNode[2];
		a_[0] = new Node(0);
		a_[1] = new Node(1);
	}

	void push_back(const T & value) {
		delete a_[Head_];
		a_[Head_] = new Node(value, Head_);
		++RealSize_;
		Head_ = Next(Head_);
		Upgrade();
	}
	void pop_back() {
		Head_ = Prev(Head_);
		delete a_[Head_];
		a_[Head_] = new Node(Head_);
		--RealSize_;
		Degrade();
	}
	T& back() {
		return a_[Prev(Head_)]->value_;
	}
	const T back()const {
		return a_[Prev(Head_)]->value_;
	}

	void push_front(const T & value) {
		delete a_[Tail_];
		a_[Tail_] = new Node(value, Tail_);
		++RealSize_;
		Tail_ = Prev(Tail_);
		Upgrade();
	}
	void pop_front() {
		Tail_ = Next(Tail_);
		delete a_[Tail_];
		a_[Tail_] = new Node(Tail_);
		--RealSize_;
		Degrade();
	}
	T& front() {
		return a_[Next(Tail_)]->value_;
	}
	const T front()const {
		return a_[Next(Tail_)]->value_;
	}

	T& operator[](const int & index) {
		return a_[(Tail_ + index + 1) % aSize_]->value_;
	}
	T operator[](const int & index)const {
		return a_[(Tail_ + index + 1) % aSize_]->value_;
	}

	iterator begin() {
		return iterator(this, a_[Next(Tail_)]);
	}
	const_iterator begin() const {
		return const_iterator(this, a_[Next(Tail_)]);
	}
	const_iterator cbegin() const {
		return const_iterator(this, a_[Next(Tail_)]);
	}

	iterator end() {
		return iterator(this, a_[Head_]);
	}
	const_iterator end() const {
		return const_iterator(this, a_[Head_]);
	}
	const_iterator cend() const {
		return const_iterator(this, a_[Head_]);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(this, a_[Prev(Head_)]);
	}
	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(this, a_[Prev(Head_)]);
	}
	const_reverse_iterator crbegin() const {
		return const_reverse_iterator(this, a_[Prev(Head_)]);
	}

	reverse_iterator rend() {
		return reverse_iterator(this, a_[Tail_]);
	}
	const_reverse_iterator rend() const {
		return const_reverse_iterator(this, a_[Tail_]);
	}
	const_reverse_iterator crend() const {
		return const_reverse_iterator(this, a_[Tail_]);
	}
};