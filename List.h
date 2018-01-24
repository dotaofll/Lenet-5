#pragma once
#ifndef __LIST_H__
#define __LIST_H__
#include<cassert>
#include"ListNode.h"
#include<string>
typedef int Rank;

template<typename T>
class List
{
public:
	List();
	List(List<T> const& L);
	List(List<T> const& L, Rank r, int n);
	List(ListNode<T>* Cur_Node, int n);
	~List();
	int size() const;
	bool empty() const;
	T& operator[](Rank r) const;
	ListNode<T>* first() const;
	ListNode<T>* last() const;
	bool valid(ListNode<T>* Cur_Node) const;
	bool disordered() const;
	ListNode<T>* find(T const& e, int n, ListNode<T>* Cur_Node) const;
	ListNode<T>* find(T const& e) { return find(e, _size,this->trailer); }
	ListNode<T>* search(T const& e, int n, ListNode<T>* Cur_Node) const;
	ListNode<T>* search(T const& e) { return search(e, _size, trailer); }
	ListNode<T>* selectMax(ListNode<T>* Cur_Node, int n) const;
	ListNode<T>* insertAsFirst(T const& e);
	ListNode<T>* insertAsLast(T const& e);
	ListNode<T>* insertAfter(T const& e, ListNode<T>* Cur_Node);
	ListNode<T>* insertBefore(T const& e, ListNode<T>* Cur_Node);
	T remove(ListNode<T>* Cur_Node);
	void merge(ListNode<T>* &Cur_Node,int n,List<T> &L,ListNode<T>* slow,int m);
	void sort(ListNode<T>* Cur_Node, int n);
	void sort() { sort(this->first(), _size); }
	int deduplicate();
	int uniquify();
	void reverse();
	void traverse(void(* visit)(T&));
	template<typename VST>
	void traverse(VST& visit);
	//friend ListNode<T>* insertAsPred(T const &e);
	//friend ListNode<T>* insertAsSucc(T const &e);
	friend class ListNode<T>;
	std::string ListNodeToString(ListNode<T>* node);
protected:
	void init();
	int clear();
	void copyNodes(ListNode<T>* Copy_Node, int n);
	void mergeSort(ListNode<T>* &Cur_Node, int n);
	void selectionSort(ListNode<T>* Cur_Node, int n);
	void insertionSort(ListNode<T>* Cur_Node, int n);
private:
	int _size;
	ListNode<T>* header;
	ListNode<T>* trailer;
};


template<typename T>
std::string List<T>::ListNodeToString(ListNode<T>* node)
{
	if (node == NULL) {
		return "[]";
	}
	std::string result;
	while (node != NULL) {
		result += std::to_string(node->getData()) + ",";
		node = node->succ;
	}
	return "[" + result.substr(0, result.length() - 2) + "]";
}



template<typename T>
inline void List<T>::init()
{
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer;
	trailer->prec = header;
	header->prec = NULL;
	trailer->succ = NULL;
	_size = 0;
}

template<typename T>
inline int List<T>::clear()
{
	int oldsize = _size;
	while (_size>0)
	{
		remove(header->getSuccessor());
	}
	return oldsize;
}

template<typename T>
inline void List<T>::copyNodes(ListNode<T>* Copy_Node, int n)
{
	init();
	while (n-- > 0) {
		insertAsLast(Copy_Node->getData());
		Copy_Node = Copy_Node->getSuccessor();
	}
}

template<typename T>
inline void List<T>::mergeSort(ListNode<T>* &Cur_Node, int n)
{
	if (n < 2) return;
	int mi = n >> 1;
	ListNode<T>*  slow = Cur_Node;
	for (int i = 0; i < mi; i++) slow = slow->getSuccessor();
	mergeSort(Cur_Node, mi); mergeSort(slow, n - mi);
	merge(Cur_Node, mi, *this, slow, n - mi);
}

template<typename T>
void List<T>::selectionSort(ListNode<T>* Cur_Node, int n)
{
	ListNode<T>* head = Cur_Node->prec; ListNode<T>* tail = Cur_Node;
	for (int i = 0; i < n; i++) tail = tail->succ;
	while (1 < n) {
		ListNode<T>* mx = selectMax(head->succ, n);
		insertBefore(tail, remove(mx));
		tail = tail->prec; n--;
	}
}

template<typename T>
void List<T>::insertionSort(ListNode<T>* Cur_Node, int n)
{
	for (int r = 0; r < n; r++) {
		insertAfter(Cur_Node->data, search(Cur_Node->data, r, Cur_Node));
		Cur_Node = Cur_Node->succ; remove(Cur_Node->prec);
	}
}

template<typename T>
inline List<T>::List()
{
	init();
}

template<typename T>
inline List<T>::List(List<T> const & L)
{
	copyNodes(L.first(), L._size);
}

template<typename T>
inline List<T>::List(List<T> const & L, Rank r, int n)
{
	assert(r + n < L._size);
	copyNodes(L[r], n);
}

template<typename T>
inline List<T>::List(ListNode<T>* Cur_Node, int n)
{
	assert(valid(Cur_Node) && n < this->_size);
	copyNodes(Cur_Node, n);
}

template<typename T>
inline List<T>::~List()
{
	clear(); delete header; delete trailer;
}

template<typename T>
inline int List<T>::size() const
{
	return _size;
}

template<typename T>
inline bool List<T>::empty() const
{
	if (_size == 0) return true;
	else return false;
}

template<typename T>
inline T & List<T>::operator[](Rank r) const
{
	assert(r <= _size);
	ListNode<T>* head = first();
	while (0 < r--) {
		head = head->getSuccessor();
	}
	return head->getData();
}

template<typename T>
inline ListNode<T>* List<T>::first() const
{
	return header->getSuccessor();
}

template<typename T>
inline ListNode<T>* List<T>::last() const
{
	return trailer->getPrecursor();
}

template<typename T>
inline bool List<T>::valid(ListNode<T>* Cur_Node) const
{
	return Cur_Node && (header != Cur_Node) && (trailer != Cur_Node);
}

template<typename T>
inline bool List<T>::disordered() const
{
	ListNode<T>* head = first();
	ListNode<T>* prec = head;
	head = head->getSuccessor();
	while (head != trailer) {
		if (prec->getData() > head.getData())
			return false;
		else head=head->getSuccessor();
	}
	return false;
}

template<typename T>
inline ListNode<T>* List<T>::find(T const & e, int n, ListNode<T>* Cur_Node) const
{
	assert(0 < n < _size);
	while (0 < n--) {
		if (e == (Cur_Node = Cur_Node->getPrecursor())->getData())
			return Cur_Node;
	}
	return NULL;
}

template<typename T>
inline ListNode<T>* List<T>::search(T const & e, int n, ListNode<T>* Cur_Node) const
{
	assert(0 <= n < _size);
	while (0 < n--) { 
		if ((Cur_Node = Cur_Node->getPrecursor())->getData() <= e)
			break;
	}
	return Cur_Node;
}

template<typename T>
inline ListNode<T>* List<T>::selectMax(ListNode<T>* Cur_Node, int n) const
{
	ListNode<T>* mx = first();
	ListNode<T>* Cur = mx;
	while (0 < n--) {
		if (mx->getData() < (Cur = Cur->getSuccessor())->getData())
			mx = Cur;
	}
	return mx;
}

template<typename T>
inline ListNode<T>* List<T>::insertAsFirst(T const & e)
{
	_size++;
	return header->insertAsSucc(e);
}

template<typename T>
inline ListNode<T>* List<T>::insertAsLast(T const & e)
{
	_size++;
	return trailer->insertAsPred(e);
}

template<typename T>
inline ListNode<T>* List<T>::insertAfter(T const & e, ListNode<T>* Cur_Node)
{
	assert(valid(Cur_Node));
	_size++;
	return Cur_Node->insertAsSucc(e);
}

template<typename T>
inline ListNode<T>* List<T>::insertBefore(T const & e, ListNode<T>* Cur_Node)
{
	assert(valid(Cur_Node));
	_size++;
	return Cur_Node->insertAsPred(e);
}

template<typename T>
inline T List<T>::remove(ListNode<T>* Cur_Node)
{	
	assert(valid(Cur_Node));
	T e = Cur_Node->data;
	Cur_Node->prec->succ = Cur_Node->succ;
	Cur_Node->succ->prec = Cur_Node->prec;
	delete Cur_Node; _size--;
	return e;
}

template<typename T>
inline void List<T>::merge(ListNode<T>* &Cur_Node, int n, List<T>& L, ListNode<T>* slow, int m)
{
	ListNode<T>* prev = Cur_Node->getPrecursor();
	while (0 < m) {
		if ((0 < n) && (Cur_Node->getData() <= slow->getData())) {
			if (slow == (Cur_Node = Cur_Node->getSuccessor()))
				break;
			else n--;
		}
		else {
			insertBefore(L.remove((slow = slow->getSuccessor())->getPrecursor()), Cur_Node); m--;
		}
	}
	Cur_Node = prev->getSuccessor();
}

template<typename T>
inline void List<T>::sort(ListNode<T>* Cur_Node, int n)
{
	mergeSort(Cur_Node, n);
}

template<typename T>
inline int List<T>::deduplicate()
{
	if (_size < 2) return 0;
	int oldsize = _size;
	ListNode<T>* cur = header->getSuccessor();
	Rank r = 1;
	while (trailer != (cur = cur->getSuccessor())) {
		ListNode<T>* temp = find(cur->getData(), r, cur);
		temp ? remove(temp) : r++;
	}
	return oldsize-_size;
}

template<typename T>
inline int List<T>::uniquify()
{
	if (_size < 2) return 0;
	int oldsize = _size;
	ListNode<T>* cur = this->first();
	ListNode<T>* prev = cur;
	cur = cur->getSuccessor();
	while (trailer != (cur = cur->getSuccessor())) {
		(cur->data = prev->getData()) ? remove(prev) : prev = cur;
	}
	return oldsize-_size;
}

template<typename T>
inline void List<T>::reverse()
{
	//Version1:reverse the ptr
	if (_size < 2) return;
	ListNode<T>* head; ListNode<T>* succ;
	for (head = header, succ = head->succ; head != trailer; head = succ, succ = head->succ) {
		head->prec = succ;
	}
	trailer->prec = NULL;
	for (head = header, succ = head->prec; head != trailer; head = succ, succ = head->prec) {
		succ->succ = head;
	}
	header->succ = NULL;
	std::swap(header, trailer);
	/*

	Version2:


	if(_size<2) return;
	ListNode<T>* head;
	for(head=header;head;head=head->prec){
		std::swap(head->prec,head->succ);
	}
	std::swap(header,trailer);


	Version3:


	ListNode<T>* head=header;
	ListNode<T>* trail=trailer;
	for(int i=1;i<_size;i+=2){
		std::swap((head=head->succ)->data,(trail=trail->prec)->data);
	}
	
	*/
}

template<typename T>
inline void List<T>::traverse(void(* visit)(T&))
{
	ListNode<T>* cur = header->getSuccessor();
	for (cur; cur != trailer; cur = cur->getSuccessor()) {
		visit(cur->getData());
	}
}

template<typename T>
template<typename VST>
inline void List<T>::traverse(VST & visit)
{
	ListNode<T>* cur = header->getSuccessor();
	for (cur; cur != trailer; cur = cur->getSuccessor()) {
		visit(cur->getData());
	}
}
#endif // !__LIST_H__