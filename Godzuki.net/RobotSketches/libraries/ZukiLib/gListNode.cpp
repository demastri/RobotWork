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
	gListNode *cur = FindNode( l );
	if( cur != 0 ) { // ensuring it's on the list...
		cur->prev->next = next;
		cur->next->prev = prev;
		delete cur;
		return true;
	}
	return false;
}
gListNode *gListNode::Find( void *o ) {  // objects can be duped on the list -> first one wins...
	gListNode *cur = this->next;
	while( (cur=cur->next) != this ) {
		if( cur->obj == o )
			return cur;
	} 
	return 0;
}
gListNode *gListNode::FindNode( gListNode *l ) {  // objects can be duped on the list -> first one wins...
	gListNode *cur = this->next;
	while( (cur=cur->next) != this ) {
		if( cur == l )
			return cur;
	} 
	return 0;
}
bool gListNode::isEmpty() {
	return next == prev && prev == this;
};

void *gListNode::PopFirst() {
	if( !isEmpty() ) {
		gListNode *cur = this->next;
		void *outObj = cur->obj;
		Remove( outObj );
		return outObj;
	}
	return 0;
}

