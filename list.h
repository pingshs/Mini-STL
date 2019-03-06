
#include<algorithm>
#include"iterator_traits.h"

#ifndef _LIST_H_
#define _LIST_H_

namespace MiniSTL{


template<typename T>
class list{
	struct node {
		T element;
		node *prev;
		node *next;
	
		node(const T& e = T(), node* p = nullptr, node* n = nullptr)
			:element(e), prev(p), next(n){}
		node(T&& e, node* p = nullptr, node* n = nullptr)
			:element(e), prev(p), next(n){}
	};
public:
	list():size_(0), head_(new node), tail_(new node){
		head_->next = tail_;
		tail_->prev = head_;	
	}

	list(const std::initializer_list<T>& lst): list(){
		for(auto &a : lst)		
			insert(end(), a);;
	}
	list(const list& rhs):list(){
		for(auto &a : rhs)		
			insert(end(), a);;
	}
	list& operator=(const list& rhs){
		auto tmp(rhs);
		std::swap(size_, tmp.size_);
		std::swap(head_, tmp.head_);
		std::swap(tail_, tmp.tail_);
		return *this;
	}
	~list(){
		clear();
		delete[] head_;
		delete[] tail_;
	}
	list(list&& rhs):list(){
		std::swap(size_, rhs.size_);
		std::swap(head_, rhs.head_);
		std::swap(tail_, rhs.tail_);
	}
	list& operator=(list&& rhs){
		std::swap(size_, rhs.size_);
		std::swap(head_, rhs.head_);
		std::swap(tail_, rhs.tail_);
		return *this;
	}

	void clear(){
		auto itr = begin();
		while(itr != end())
			itr = erase(itr);
	}

	class const_iterator{
	friend class list<T>;
	public:
		typedef bidirectional_iterator_tag iterator_category;
		
		const T& operator*() const {
			return cur_->element;
		}
		const_iterator operator++(){
			cur_ = cur_->next;
			return cur_;
		}
		const_iterator operator++(int){
			auto old(cur_);
			cur_ = cur_->next;
			return old;
		}
		const_iterator operator--(){
			cur_ = cur_->prev;
			return cur_;
		}
		const_iterator operator--(int){
			auto old(cur_);
			cur_ = cur_->prev;
			return old;
		}

		bool operator==(const const_iterator& rhs) const{
			return cur_ == rhs.cur_;
		}
		bool operator!=(const const_iterator& rhs)const {
			return !(*this == rhs);
		}
	protected:
		node* cur_;
	
		const_iterator(node* c):cur_(c){}
	};
	class iterator : public const_iterator{
	friend class list<T>;
	public:
		T& operator*() {
			return this->cur_->element;
		}
		iterator operator++(){
			this->cur_ = this->cur_->next;
			return this->cur_;
		}
		iterator operator++(int){
			auto old(this->cur_);
			this->cur_ = this->cur_->next;
			return old;
		}
		iterator operator--(){
			this->cur_ = this->cur_->prev;
			return this->cur_;
		}
		iterator operator--(int){
			auto old(this->cur_);
			this->cur_ = this->cur_->prev;
			return old;
		}
	private:
		iterator(node* c):const_iterator(c){}
	};
	iterator begin(){
		return iterator(head_->next);
	}
	const_iterator begin() const{
		return const_iterator(head_->next);
	}
	const_iterator cbegin() const{
		return const_iterator(head_->next);
	}
	iterator end(){
		return iterator(tail_);
	}
	const_iterator end() const{
		return iterator(tail_);
	}
	const_iterator cend() const{
		return iterator(tail_);
	}


	iterator insert(iterator position, const T& x);
	iterator erase(iterator position);

	template<typename IterT>
	iterator insert(iterator iter, IterT ibeg, IterT iend);
		
	iterator erase(iterator& beg, iterator& end);
private:
	int size_;
	node *head_;
	node *tail_;
};

}

namespace MiniSTL{

template<typename T>
typename list<T>::iterator list<T>::insert(iterator iter, const T& x){
	node* pnext = iter.cur_;
	node* pnode = new node(x, pnext->prev, pnext);
	pnext->prev->next = pnode;
	pnext->prev = pnode;
	size_++;
	return iterator(pnode);
}

template<typename T>
typename list<T>::iterator list<T>::erase(iterator iter){
	node* pdeleted = iter.cur_;
	node* pnext = pdeleted->next;	
	pdeleted->prev->next = pnext;
	pnext->prev = pdeleted->prev;
	delete pdeleted;
	size_--;
	return iterator(pnext);
}

template<typename T>
template<typename IterT>
typename list<T>::iterator list<T>::insert(iterator iter, IterT ibeg, IterT iend){
	for(auto itr = iend - 1; itr >= ibeg; itr --)	
		iter = insert(iter, *itr);
	return iter;
}
		
template<typename T>
typename list<T>::iterator list<T>::erase(iterator& beg, iterator& end){
	auto itr = beg;
	while(itr != end)
		itr = erase(itr);
	return itr;
}

}

#endif
