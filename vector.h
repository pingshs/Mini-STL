#include<algorithm>
#include<stdexcept>
#include"iterator_traits.h"

#ifndef _VECTOR_H
#define _VECTOR_H

namespace MiniSTL{


template<typename T>
class vector{
public:
	typedef T* iterator;
	typedef const T* const_iterator;

	vector():size_(0), capacity_(0), element_(nullptr){}
	vector(const std::initializer_list<T>& lst):
		size_(lst.size()),capacity_(size_+5),element_(new T[capacity_]){
		int i = 0;
		for(auto& a : lst){
			element_[i] = a;
			i++;
		}
	}
	vector(const vector& rhs):
		size_(rhs.size()),capacity_(size_),element_(new T[capacity_]){
		for(int i = 0; i < size_; i++)
			element_[i] = rhs.element_[i];
	}
	vector& operator=(const vector& rhs){
		auto tmp(rhs);
		std::swap(size_, tmp.size_);
		std::swap(capacity_, tmp.capacity_);
		std::swap(element_, tmp.element_);
		return *this;
	}
	vector(vector&& rhs): vector(){
		std::swap(*this, rhs);
	}
	vector& operator=(vector&& rhs){
		std::swap(*this, rhs);
		return *this;
	}
	~vector(){
		if(element_ != nullptr)
			delete[] element_;
	}

	int size() const {
		return size_;
	}
	int empty() const {
		return size() == 0;
	}
	void clear() {
		size_ = 0;
	}

	T& operator[](int i){
		return element_[i];
	}
	const T& operator[](int i) const {
		return element_[i];
	}
	T& at(int i){
		if(i < 0 || i >= size_)
			throw std::out_of_range("vector out of rangle");
		return element_[i];
	}
	const T& at(int i) const{
		if(i < 0 || i >= size_)
			throw std::out_of_range("vector out of rangle");
		return element_[i];
	}
	T& front(){
		return element_[0];
	}
	const T& front() const {
		return element_[0];
	}
	T& back(){
		return element_[size_-1];
	}
	const T& back() const {
		return element_[size_-1];
	}
	void push_back(const T& x){
		if(size_ >= capacity_)
			resize(capacity_*2+1);
		element_[size_] = x;
		size_++;
	}
	void pop_back(){
		size_--;
	}

	iterator begin() {
		return element_;
	}
	const_iterator begin() const{
		return element_;
	}
	iterator end() {
		return element_+size_;
	}
	const_iterator end() const{
		return element_+size_;
	}

	iterator insert(iterator pos, const T& x);
	iterator erase(iterator pos);

	template<typename IterT>
	iterator insert(iterator iter, const IterT& beg, const IterT& end);
	template<typename IterT>
	typename vector<T>::iterator insert(iterator iter, 
	  const IterT& ibeg, const IterT& iend, random_access_iterator_tag);
	template<typename IterT>
	typename vector<T>::iterator insert(iterator iter, 
	  const IterT& ibeg, const IterT& iend, forward_iterator_tag);

	iterator erase(iterator beg, iterator end);

	void resize(int newsize);
	
private:
	int size_;
	int capacity_;
	T* element_;
};
	

}
	

namespace MiniSTL{

template<typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos, const T& x){
	if(size_ >= capacity_)
		resize(capacity_*2+1);
	auto itr = end();
	for(; itr != pos; itr--)
		*itr = *(itr-1);
	*itr = x;
	size_++;
	return itr;
}
template<typename T>
typename vector<T>::iterator vector<T>::erase(iterator pos){
	if(pos == end())
		return pos;
	auto itr = pos;
	for(; itr < end() - 1; itr ++)
		*itr = *(itr+1);
	size_--;
	return pos;
}

template<typename T>
template<typename IterT>
typename vector<T>::iterator vector<T>::insert(iterator iter, const IterT& beg, const IterT& end){
	return 
	insert(iter, beg, end, typename iterator_traits<IterT>::iterator_category());
}

template<typename T>
template<typename IterT>
typename vector<T>::iterator vector<T>::insert(iterator iter, 
	const IterT& ibeg, const IterT& iend, random_access_iterator_tag){
		int length = iend - ibeg;
		if(size_ + length > capacity_){
			if(size_+length > capacity_*2+1)
				resize(size_+length);
			else
				resize(capacity_*2+1);
		}
		size_ = size_+length;	
		for(auto itr = end() -1; itr >= iter; itr--){
			*(itr+length) = *itr;
		}

		auto insert_begin = iter;
		for(auto itr = ibeg; itr != iend; itr++ ){
			*(insert_begin++) =  *itr;
		}
		return iter;
}
template<typename T>
template<typename IterT>
typename vector<T>::iterator vector<T>::insert(iterator iter, 
	const IterT& ibeg, const IterT& iend, forward_iterator_tag){
		int length = 0;
		for(auto itr = ibeg; itr != iend; itr++){
			length ++;
		}
		if(size_ + length > capacity_){
			if(size_+length > capacity_*2+1)
				resize(size_+length);
			else
				resize(capacity_*2+1);
		}
		size_ = size_+length;	
		for(auto itr = end() -1; itr >= iter; itr--){
			*(itr+length) = *itr;
		}

		auto insert_begin = iter;
		for(auto itr = ibeg; itr != iend; itr++ ){
			*(insert_begin++) =  *itr;
		}
		return iter;
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(iterator beg, iterator end){
	int deleted_length = end - beg;
	iterator itr = beg;

	while(end != this->end()){
		*end = *beg;
		end++;
		beg++;
	}
	size_ = size_ - deleted_length;
	return itr;
}

template<typename T>
void vector<T>::resize(int newsize){
	if(newsize <= capacity_)
		return;
	T* old = element_;
	element_ = new T[newsize];
	for(int i = 0; i < size_; i++)
		element_[i] = std::move(old[i]);
	if(old != nullptr)
		delete[] old;
	capacity_ = newsize;
}

}

#endif
