#include "listStructs.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>

#define MAGIC 0xDEADBEEF


static void DestroyNode(Node* _node);
static List_Result InsertBefore ( void* _item, Node* _refPoint);
static List_Result RemoveAndDestroyNode ( Node* _node , void** _pItem);

/*****************************************************/

List* List_Create(void)
{
	List* newList=NULL;
	
	if (NULL == (newList= (List*) malloc (sizeof(List))))
	{
		return NULL;
	}

	newList->m_head.m_next = &newList->m_tail;
	newList->m_tail.m_prev = &newList->m_head;
	newList->m_head.m_prev = NULL;
	newList->m_tail.m_next = &newList->m_tail;
 	newList->m_head.m_data = (void*)MAGIC;
	newList->m_tail.m_data = (void*)MAGIC;
	return newList;
}

void List_Destroy(List** _pList, void (*_elementDestroy)(void* _item))
{
	Node* next = NULL;
	List* list = NULL;
	
	if (NULL == _pList || NULL == *_pList)
	{
		return;
	}
	
	list = *_pList;
	next = list->m_head.m_next; 
	
	while ( next !=  &list->m_tail)
	{
		next = next->m_next;
		if (_elementDestroy != NULL)
		{
			_elementDestroy(next->m_prev->m_data);
		}
		DestroyNode(next->m_prev);
	}
	
	free(list);
	*_pList = NULL;
}



List_Result List_PushHead(List* _list, void* _item)
{

	if (NULL == _list)
	{
		return LIST_UNINITIALIZED_ERROR;
	}

	if (NULL == _item )
	{
		return LIST_NULL_ELEMENT_ERROR;
	}

	return  InsertBefore(_item, _list->m_head.m_next);
}


List_Result List_PushTail(List* _list, void* _item)
{

	if (NULL == _list)
	{
		return LIST_UNINITIALIZED_ERROR;
	}

	if (NULL == _item )
	{
		return LIST_NULL_ELEMENT_ERROR;
	}

	return	InsertBefore(_item, &_list->m_tail);

}

List_Result List_PopHead(List* _list, void** _pItem)
{

	if (NULL == _list )
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	
	if (_list->m_head.m_next == &_list->m_tail )
	{
		return LIST_UNDERFLOW;
	}
	
	
	return RemoveAndDestroyNode(_list->m_head.m_next, _pItem);
}
	
	 
	
	
	
List_Result List_PopTail(List* _list, void** _pItem)
{
	if (NULL == _list )
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	
	if (_list->m_head.m_next == &_list->m_tail )
	{
		return LIST_UNDERFLOW;
	}

	return RemoveAndDestroyNode(_list->m_tail.m_prev, _pItem);
}


size_t List_Size(const List* _list)
{
	size_t count=0;
	Node* next=NULL;
	
	if (NULL == _list )
	{
		return count ;
	}
	
	next= _list->m_head.m_next;
	
	while (next != &_list->m_tail)
	{
		next= next->m_next;
		++count;
	}
	return count;
}



static List_Result InsertBefore ( void* _item, Node* _refPoint)
{
	Node* newNode=NULL;
	
	if (NULL == (newNode = (Node*) malloc (sizeof(Node) )))
	{
		return LIST_ALLOCATION_ERROR;
	}

	newNode->m_data = _item;
	
	
	newNode->m_prev= _refPoint->m_prev;
	newNode->m_next= _refPoint;
	_refPoint->m_prev->m_next= newNode;
	_refPoint->m_prev= newNode;
	
	return LIST_SUCCESS;
}
	

	
static List_Result RemoveAndDestroyNode ( Node* _node , void** _pItem)
{
	if (NULL == _pItem)
	{
		return LIST_NULL_ELEMENT_ERROR;
	}
	
	*_pItem = _node->m_data;
	
	_node->m_next->m_prev = _node->m_prev;
	_node->m_prev->m_next = _node->m_next;
	
	DestroyNode(_node);
	return LIST_SUCCESS;
}



static void DestroyNode(Node* _node)
{
	if (NULL == _node )
	{
		return ;
	}

	free(_node);

}





	

	

