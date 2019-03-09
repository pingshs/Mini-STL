#include<algorithm>
#include<string>
#include<list>
#include<vector>
#include<stdexcept>
#include<memory>
#include"iterator_traits.h"
#include"pair.h"

#ifndef _UNORDERED_SET_H_
#define _UNORDERED_SET_H_

namespace MiniSTL{

template<typename T>
class hash{
public:
	size_t operator()(const T& x){
		return static_cast<size_t>(x);
	}
};
template<>
class hash<std::string>{
public:
	size_t operator()(const std::string& x){
		size_t hashval = 0;
	
		for(char ch : x)
			hashval = 37*hashval + ch;
		return hashval;
	}
};

template<typename KeyType>
class unordered_set{
friend class iterator;
public:

	void print(){
		for(auto & a: table_){
			for(auto &b : a)
				std::cout << b << " ";
			std::cout << std::endl;
		}
			
	}

	class iterator{
	friend class unordered_set<KeyType>;
	public:
		typedef forward_iterator_tag iterator_category;

		const KeyType& operator*() const{
			return *cur_;
		}

		iterator operator++(){
	
			KeyType k = *(cur_);
//
			size_t index = pset_->myhash(k);

			++cur_;
			if(cur_ == (*pset_).table_[index].end()){
				index++;
				while(index < (*pset_).table_.size() &&
					(*pset_).table_[index].size() == 0)
					index++;
				if(index == (*pset_).table_.size())
					cur_ = (*pset_).table_[--index].end();
				else
					cur_ = (*pset_).table_[index].begin();
			}
			return iterator(cur_, pset_);
		}

		iterator operator++(int){
			auto old(*this);
			++(*this);
			return old;
		}

//TODO
/*		iterator operator--(){
			
			if(cur_ == table_[index_].begin() || 
				table_[index_].size() == 0){

				while(index_  >= 0 && 
					table_[index_].size() == 0)	
					index_--;
				if(index_ < 0)
					throw std::out_of_range("unordered_set iterator out of range");
				cur_ = --(table_[index_].end());
			}else{
				--cur_;
			}
			return iterator(index_, cur_);
		}

		iterator operator--(int){
			auto old(*this);
			--(*this);
			return old;
		}

		iterator operator--(int);
*/

		bool operator==(const iterator& rhs) const{
			return cur_ == rhs.cur_;
		}
		bool operator!=(const iterator& rhs) const{
			return !(*this == rhs);
		}
			
	private:
		iterator(typename std::list<KeyType>::const_iterator c, unordered_set<KeyType>* p)
			: cur_(c), pset_(p){}
		unordered_set<KeyType> *pset_;
		typename std::list<KeyType>::const_iterator cur_;
	};

	iterator begin() {
		if(size_ == 0)
			throw std::out_of_range("unordered_set is empty()");
			
		int i;
		for(i = 0; i < table_.size() && table_[i].size() == 0; ++i)	
				;
		if(i != table_.size())
			return iterator(table_[i].begin(), this);
	}
	iterator end() {
		return iterator(table_[table_.size()-1].end(), this);
	}

	unordered_set():size_(0), table_(7){}
	
	unordered_set(const std::initializer_list<KeyType>& lst):size_(0), table_(7){
		for(auto &a : lst)
			insert(a);
	}
	
	unordered_set(const unordered_set& rhs){}
	unordered_set& operator=(const unordered_set& rhs){}
	~unordered_set(){}
	unordered_set(unordered_set&& rhs){}
	unordered_set& operator=(unordered_set&& rhs){}


	iterator find(const KeyType& x){
		for(auto &lst : table_){
			auto itr = std::find(lst.begin(), lst.end(), x);
			if(itr != lst.end())
				return iterator(itr, this);
		}
		return iterator(end(), this);
	}
	pair<iterator, bool> insert(const KeyType& x);

	int remove(const KeyType& x);
private:
	int size_;
	std::vector<std::list<KeyType>> table_;

	void rehash(int prime){
		std::vector<std::list<KeyType>> old(std::move(table_));	
		for(auto& lst : table_)
			lst.clear();
		table_.resize(prime);

		for(auto& lst : old)
			for(auto &a : lst){
				insert(a);
			}
	}

	size_t myhash(const KeyType& x){
		static hash<KeyType> hf;
		size_t tmp  = hf(x);
		return tmp % table_.size();
	}

	int NextPrime(int m){
		int prime = m;
		while(1){
			int i;
			for(i = 2; i*2 < prime && prime % i != 0; i++)	
				;
			if(i*2 > prime)
				break;
			prime++;
		}
		return prime;
	}

};

}

namespace MiniSTL{

//
/*
template<typename KeyType>
typename unordered_set<KeyType>::iterator unordered_set<KeyType>::iterator::operator++(){
	++cur_;
	if(cur_ == unordered_set<KeyType>::table_[index_].end()){
		index_++;
		while(index_ < table_.size() &&
			table_[index_].size() == 0)
			index_++;
		if(index_ == table_.size())
			cur_ = table_[--index_].end();
		else
			cur_ = table_[index_].begin();
	}
	return iterator(index_, cur_);
}
template<typename KeyType>
typename unordered_set<KeyType>::iterator unordered_set<KeyType>::iterator::operator++(int){
	auto old(*this);
	++(*this);
	return old;
}

template<typename KeyType>
typename unordered_set<KeyType>::iterator unordered_set<KeyType>::iterator::operator--(){
	
	if(cur_ == table_[index_].begin() || 
		table_[index_].size() == 0){

		while(index_  >= 0 && 
			table_[index_].size() == 0)	
			index_--;
		if(index_ < 0)
			throw std::out_of_range("unordered_set iterator out of range");
		cur_ = --(table_[index_].end());
	}else{
		--cur_;
	}
	return iterator(index_, cur_);
}

template<typename KeyType>
typename unordered_set<KeyType>::iterator unordered_set<KeyType>::iterator::operator--(int){
	auto old(*this);
	--(*this);
	return old;
}
*/

template<typename KeyType>
pair<typename unordered_set<KeyType>::iterator, bool> unordered_set<KeyType>::insert(const KeyType& x){
	size_t hashvalue = myhash(x);
	auto& lst = table_[hashvalue];
	auto itr = std::find(lst.begin(), lst.end(), x);
	if(itr != lst.end())
		return pair<iterator, bool>(iterator(itr, this), false);
	lst.push_front(x);
	
	if(++size_ > table_.size())	{
		rehash(NextPrime(table_.size()*2+1));
		hashvalue = myhash(x);
	}

	return pair<iterator, bool>(iterator(lst.begin(), this), true);
}
template<typename KeyType>
int unordered_set<KeyType>::remove(const KeyType& x){
	auto& lst = table_[myhash(x)];
	auto itr = std::find(lst.begin(), lst.end(), x);
	if(itr == lst.end())
		return 0;
	lst.erase(itr);
	size_ --;
	return 1;
}

}

#endif
