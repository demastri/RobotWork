#pragma once

class gListNode
{
public:
	gListNode(void);
	~gListNode(void);

	gListNode *Add( void *o );
	bool Remove( void *o );
	bool RemoveNode( gListNode *l );
	gListNode *Find( void *o );
	gListNode *FindNode( gListNode *l );
	bool isEmpty();
	void *PopFirst();
	
	void *GetObject() { return obj; };
	gListNode *First() { return isEmpty() ? 0 : next; };
	gListNode *Next(gListNode *cur) { return cur->next == this ? 0 : cur->next; }

private:
	gListNode *next;
	gListNode *prev;
	void *obj;
};

typedef gListNode gList;
