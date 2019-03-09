#include<algorithm>
#include<iostream>
#include<queue>
#include"iterator_traits.h"
#include"pair.h"

namespace MiniSTL{

#ifndef _MAP_H
#define _MAP_H

/*template<typename KeyType, typename MappedType>
struct pair{
	KeyType first;
	MappedType second;

	pair(const KeyType& k = KeyType(), const MappedType& m = MappedType())
		:first(k), second(m){}
	pair(KeyType&& k, MappedType&& m)
		:first(std::move(k)), second(std::move(m)){}

	bool operator<(const pair& rhs)const {
		return first < rhs.first;
	}
	bool operator==(const pair& rhs)const {
		return first == rhs.first;
	}
	bool operator!=(const pair& rhs) const {
		return !(*this== rhs);
	}
};
*/
template<typename KeyType, typename MappedType>
class map{
friend class iterator;
	//struct node;
	enum {RED, BLACK};
	struct node {
		pair<KeyType, MappedType> element;
		node *parent;
		node *left, *right;
		int color;

		node(const pair<KeyType,MappedType>& e=pair<KeyType,MappedType>(), node *p = nullptr, node *l = nullptr, node *r = nullptr, int c = BLACK)
		: element(e), parent(p), left(l), right(r), color(c){}
		node(pair<KeyType,MappedType>&& e, 
			node *p = nullptr, node *l = nullptr, node *r = nullptr, int c = BLACK)
		: element(std::move(e)), parent(p), left(l), right(r), color(c){}

	};
public:

	map(const pair<KeyType, MappedType>& negInf): size_(0){
		nullnode_ = new node();
		nullnode_->parent = nullnode_->left = nullnode_->right = nullnode_;
	
		header_ = new node(negInf);
		header_->parent = header_->left = header_->right = nullnode_;
	}
	map(const map& rhs){
		nullnode_ = new node;
		nullnode_->parent = nullnode_->left_ = nullnode_->right = nullnode_;
		header_ = new node(rhs.header_->element);
		header_->parent = header_->left_ = nullnode_;
		header_->right = clone(rhs.header_->right, header_);
	}
	map(map&& rhs): map(rhs.header_->element){
		std::swap(size_, rhs.size_);
		std::swap(header_, rhs.header_);
		std::swap(nullnode_, rhs.nullnode_);
	}
	map& operator=(const map& rhs){
		map tmp(rhs);
		std::swap(size_, tmp.size_);
		std::swap(header_, tmp.header_);
		std::swap(nullnode_, tmp.nullnode_);
		return *this;	
	}
	map& operator=(map&& rhs){
		std::swap(size_, rhs.size_);
		std::swap(header_, rhs.header_);
		std::swap(nullnode_, rhs.nullnode_);
		return *this;	
	}
	~map(){
		clear();
		delete nullnode_;
		delete header_;
	}
	void clear(){
		if(header_->right != nullnode_)
			clear(header_->right);
	}

	class iterator{
	friend class map;
	public:
		typedef bidirectional_iterator_tag iterator_category;

		pair<KeyType, MappedType>& operator*(){
			return cur->element;
		}
		iterator& operator++(){
			increment();
			return *this;
		}
		iterator& operator++(int){
			iterator old = *this;;
			increment();
			return old;
		}
		iterator& operator--(){
			decrement();
			return *this;
		}
		iterator& operator--(int){
			iterator old = *this;;
			decrement();
			return old;
		}

		bool operator==(const iterator& rhs)const {
			return cur == rhs.cur;
		}
		bool operator!=(const iterator& rhs)const {
			return !(*this == rhs);
		}

	private:
		map::node* cur;
		map* pmap;

		iterator(node* c, map* p): cur(c), pmap(p){ }


		void increment(){
			if(cur->right != pmap->nullnode_){
				cur = cur->right;
				while(cur->left != pmap->nullnode_)
					cur = cur->left;
			}else{
				node* parent = cur->parent;
				while(cur == parent->right){
					cur = parent;
					parent = parent->parent;
				}
				if(cur != pmap->header_)
					cur = parent;
			}
		}

		void decrement(){
			if(cur == header_)
 				cur = findMax(header_->right) ;
			
			else if(cur->left != nullnode_){
				cur = cur->left;
				while(cur->right != nullnode_)
					cur = cur->right;
			}else {
				node *parent = cur->parent;
				while(cur == parent->left){
					cur = parent;
					parent = parent->parent;
				}

				cur = parent;
			}
		}
	};
	iterator begin(){
		return iterator(findMin(header_->right), this);
	}
	iterator end(){
		return iterator(header_, this);
	}

	int size(){
		return size_;
	}
	bool empty(){
		return size() == 0;
	}

	pair<iterator, bool> insert(const pair<KeyType, MappedType>& x){
		node *cur = header_;		
		node *parent = header_;;
		
		while(cur != nullnode_ && x != cur->element){
			parent = cur;
			cur = x < cur->element ? cur->left : cur->right;	

			if(cur->left->color == RED && cur->right->color == RED)
				HandleReorient(cur);
		}
		if(cur != nullnode_)
			return pair<iterator, bool>({cur, this},  false);
		
		cur = new node(x, parent, nullnode_, nullnode_);
		if(cur->element < parent->element)
			parent->left = cur;
		else
			parent->right = cur;

		HandleReorient(cur);

		return pair<iterator, bool>({cur, this}, true);
	}
	int erase(const pair<KeyType, MappedType>& x ){
		if(header_->right == nullnode_)
			return 0;

		pair<KeyType, MappedType> item(x);

		node* proot = header_->right;	
		if(proot->left->color == BLACK && proot->right->color == BLACK){
			proot->color = RED;	
			down(item, proot);
		}
		node* cur = proot;

		while(cur->color == BLACK){
			if(cur->left->color==BLACK && cur->right->color == BLACK){
				node* parent= cur->parent;
				node* brother = (cur == parent->left) ?
					 cur->right : cur->left;

				parent->color = BLACK;
				cur->color = RED;
				if(brother->left->color == BLACK && 
					brother->right->color == BLACK){
					brother->color = RED;
				}else if(brother->left->color == RED){
					if(parent->element < brother->left->element ){
						node* brother_left = brother->left;
						rotate(brother_left->element,parent);
						rotate(brother_left->element,parent->parent);
					}else{
						brother->color = RED;
						brother->left->color = BLACK;
						rotate(brother->element,parent->parent);
					}
				}else{
					if(brother->right->element < parent->element){
						node* brother_right = brother->right;
						rotate(brother_right->element,parent);
						rotate(brother_right->element,parent->parent);
					}else{
						brother->color = RED;
						brother->right->color = BLACK;
						rotate(brother->element,parent->parent);
					}
	
				}
				down(item, cur);
			}else{
				down(item, cur);
				if(cur->color == RED){
					down(item, cur);
				}else{
					cur->parent->color = RED;
					cur->parent->left->color = BLACK;
					cur->parent->right->color = BLACK;

					if(cur->parent->left == cur)
						rotate(cur->parent->right->element, cur->parent->parent);
					else
						rotate(cur->parent->left->element, cur->parent->parent);
				}
			}
		}
		header_->right->color = BLACK;		

		if(cur == cur->parent->left)
			cur->parent->left = nullnode_;
		else
			cur->parent->right = nullnode_;

		delete cur;
		
	}

	void PrintLevel(){
		if(header_->right == nullnode_)
			return ;
		std::queue<node*> que;
		que.push(header_->right);

		while(!que.empty()){
			node* pnode = que.front();
			que.pop();

			std::cout << pnode->element.first;
			if(pnode->color == RED)
				std::cout << "R";
			else
				std::cout << "B";
			printf("\t");
				 
			if(pnode->left != nullnode_)
				que.push(pnode->left);
			if(pnode->right != nullnode_)
				que.push(pnode->right);
			}
		}

	void PrintInorder_R(){
		if(header_->right != nullnode_)
			PrintInorder_R(header_->right);
	}

private:

	int size_;
	node *nullnode_;
	node *header_;

	node* clone(node *proot, node *parent){
		if(proot == nullnode_)
			return nullnode_;
		node* pnode = new node(proot->element, parent);
		pnode->left = clone(proot->left, pnode);
		pnode->right = clone(proot->right, pnode);
		pnode->color = proot->color;
		return pnode;
	}
	
	void clear(node *proot){
		if(proot != nullnode_){
			clear(proot->left);
			clear(proot->right);
			delete proot;
		}
	}

	node* findMin(node *proot) const {
		if(proot != nullnode_){
			while(proot->left != nullnode_)
				proot = proot->left;
		}
		return proot;
	}

	node* findMax(node *proot) const {
		if(proot != nullnode_){
			while(proot->right != nullnode_)
				proot = proot->right;
		}
		return proot;
	}

	void LeftRotate(node* &proot){
		node* pnode = proot;
		node* right = pnode->right;

		right->parent = pnode->parent;
		pnode->parent = right;
		right->left->parent = pnode;

		pnode->right = right->left;
		right->left = pnode;
		proot = right;
	}
	void RightRotate(node* &proot){
		node* pnode = proot;
		node* left = pnode->left;

		left->parent = pnode->parent;
		pnode->parent = left;
		left->right->parent = pnode;

		pnode->left = left->right;
		left->right = pnode;
		proot = left;
	}

	node* rotate(const pair<KeyType, MappedType>& item, node* parent){
		if(item < parent->element){
			item < parent->left->element ?
				RightRotate(parent->left):
				LeftRotate(parent->left);
			return parent->left;
		}else{
			item < parent->right->element ?
				RightRotate(parent->right):
				LeftRotate(parent->right);
			return parent->right;
		}
	}

	void HandleReorient(node* cur){
		cur->color = RED;
		cur->left->color = BLACK;
		cur->right->color = BLACK;

		if(cur->parent->color == RED){
			cur->parent->parent->color = RED;

			node* pnode;
			if(cur->element < cur->parent->parent->element != 
					cur->element < cur->parent->element) {
				rotate(cur->element, cur->parent->parent);
				pnode = rotate(cur->element, cur->parent->parent);
			}else
				pnode = rotate(cur->element, cur->parent->parent->parent);
			pnode->color = BLACK;
		}
		header_->right->color = BLACK;
	}

	void down(pair<KeyType, MappedType>& item, node* &proot){
		if(item == proot->element){
			if(proot->right != nullnode_){
				proot->element = findMin(proot->right)->element;
				item = proot->element;
				proot = proot->right;
			}else if(proot->left != nullnode_){
				proot->element = findMax(proot->left)->element;
				item = proot->element;
				proot = proot->left;
			}else{
////////
			        return;		
			}
		}else if(item < proot->element){
			proot = proot->left;
		}else{
			proot = proot->right;
		}
	}

	void PrintInorder_R(node* proot){
		if(proot != nullnode_){
			PrintInorder_R(proot->left);

			std::cout << proot->element.first;
			if(proot->color == RED)
				std::cout << "R";
			else
				std::cout << "B";
					 
			printf("\t");

			PrintInorder_R(proot->right);
		}
	}
};

#endif
}
