#pragma once
#ifndef __LISTNODE_H__
#define __LISTNODE_H__
#include<cassert>


template<typename T>
class ListNode
{
public:
	ListNode() {}
	ListNode(ListNode<T> &node) { data = node->data; prec = node->prec; succ = node->succ; }
	ListNode(T e, ListNode<T>* prec = NULL, ListNode<T>* succ = NULL) :data(e), prec(prec), succ(succ) {}
	ListNode<T>* getPrecursor()  { return this->prec; }
	ListNode<T>* getSuccessor() { return this->succ; }
	T& getData() { return this->data; }
	ListNode<T>* insertAsPred(T const &e);
	ListNode<T>* insertAsSucc(T const &e);
	T data;
	ListNode<T>* prec;
	ListNode<T>* succ;
};
template<typename T>
inline ListNode<T>* ListNode<T>::insertAsPred(T const & e)
{
	ListNode<T>* new_ListNode = new ListNode(e, prec, this);
	prec->succ = new_ListNode; this->prec =new_ListNode ;
	return new_ListNode;
}

template<typename T>
inline ListNode<T>* ListNode<T>::insertAsSucc(T const & e)
{
	ListNode<T>* new_ListNode = new ListNode(e, this, succ);
	succ->prec = new_ListNode; this->succ = new_ListNode;
	return new_ListNode;
}

#endif //!__LISTNODE_H__