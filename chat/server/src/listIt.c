#include "listStructs.h"
#include "list_itr.h"
#include <stdio.h>
#include <stdlib.h>

#define CAST_TO_ITR(a) (ListItr)((Node*)a)
#define CAST_NODE(a) ((Node*)(a))

#define SUB(a,b) (a) - (b)


/***********************************************************/
ListItr ListItr_Begin(const List* _list)
{
	if (_list == NULL)
	{
		return NULL;
	}

	return (ListItr)_list->m_head.m_next;
}

/***********************************************************/
ListItr ListItr_End(const List* _list)
{
	if (_list == NULL)
	{
		return NULL;
	}

	return (ListItr)&_list->m_tail;
}

/***********************************************************/

int ListItr_Equals(const ListItr _a, const ListItr _b)
{
	return !((Node*)_a == (Node*)_b);
}

/***********************************************************/

ListItr ListItr_Next(ListItr _itr)
{
	if (_itr == NULL)
	{
		return NULL;
	}
	
	return (ListItr)((Node*)_itr)->m_next;
}

/***********************************************************/

ListItr ListItr_Prev(ListItr _itr)
{
	if (_itr == NULL)
	{
		return NULL;
	}
	
	return (((Node*)_itr)->m_prev->m_prev == NULL) ? _itr : CAST_TO_ITR(_itr)->m_prev;
}

/***********************************************************/

void* ListItr_Get(ListItr _itr)
{
	return (CAST_NODE(_itr) == CAST_NODE(_itr)->m_next) ? NULL : CAST_NODE(_itr)->m_data;
}

/***********************************************************/

void* ListItr_Set(ListItr _itr, void* _element)
{
	void* oldData = CAST_NODE(_itr)->m_data;
	
	if (CAST_NODE(_itr) == CAST_NODE(_itr)->m_next) /*if iterator is end*/
	{
		return NULL;
	}
	
	CAST_NODE(_itr)->m_data = _element;
	return oldData;	 
}

/***********************************************************/

ListItr ListItr_InsertBefore(ListItr _itr, void* _element)
{

	Node* newItr=NULL;
	
	if (_itr == NULL || NULL == (newItr = (Node*) malloc (sizeof(Node) )))
	{
		return NULL;
	}

	newItr->m_data = _element;
	
	
	newItr->m_prev= CAST_NODE(_itr)->m_prev;
	newItr->m_next= CAST_NODE(_itr);
	CAST_NODE(_itr)->m_prev->m_next= newItr;
	CAST_NODE(_itr)->m_prev= newItr;	
	
	return (ListItr)newItr;		
}

/***********************************************************/

void* ListItr_Remove(ListItr _itr)
{
	void* data = NULL;
	if (_itr == NULL || CAST_NODE(_itr) == CAST_NODE(_itr)->m_next)
	{
		return NULL;
	}
	data = CAST_NODE(_itr)->m_data;	
	CAST_NODE(_itr)->m_next->m_prev = CAST_NODE(_itr)->m_prev;
	CAST_NODE(_itr)->m_prev->m_next = CAST_NODE(_itr)->m_next;
	
	free(_itr);
	return data;
}

