#include "gListNode.h"


gListNode::gListNode(void)
{
	prev = next = this;
	obj = 0;
}


gListNode::~gListNode(void)
{
}

gListNode *gListNode::Add( void *o ) {  // this implements a queue - add this onto the end of the queue
	// the base node is a placeholder to the list->no data;
	gListNode *cur = new gListNode();
	if( cur != 0 ) {
		cur->next = this;
		cur->prev = prev;
		prev->next = cur;
		prev = cur;
		cur->obj = o;
	}
	return cur;
}
bool gListNode::Remove( void *o ) {
	return RemoveNode( Find( o ) );
}
bool gListNode::RemoveNode( gListNode *l ) {
	if( FindNode( l ) != 0 ) { // ensuring it's on the list...
		l->prev->next = l->next;
		l->next->prev = l->prev;
		delete l;
		return true;
	}
	return false;
}
gListNode *gListNode::Find( void *o ) {  // objects can be duped on the list -> first one wins...
	for( gListNode *n=First(); !isEmpty() && n != 0; n= Next(n) )
		if( n->obj == o )
			return n;
	return 0;
}
gListNode *gListNode::FindNode( gListNode *l ) {  // objects can be duped on the list -> first one wins...
	for( gListNode *n=First(); !isEmpty() && n != 0; n= Next(n) )
		if( n == l )
			return n;
	return 0;
}
bool gListNode::isEmpty() {
	return next == prev && prev == this;
};

void *gListNode::PopFirst() {
	if( !isEmpty() ) {
		void *outObj = next->obj;
		RemoveNode( next );
		return outObj;
	}
	return 0;
}

int gListNode::Size() {
	int i=0;
	for( gListNode *n=First(); !isEmpty() && n != 0; n= Next(n) )
		if( i++ > 50 )
			break;
	return i;
}


