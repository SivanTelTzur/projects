#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

struct Queue
{
	size_t m_maxSize;
	size_t m_head;
	size_t m_tail;
	size_t m_nItems;
	void *m_items[1];
};

Queue *Queue_Create(size_t _maxSize)
{
	Queue *newQueue = NULL;

	if (_maxSize == 0)
	{
		return NULL;
	}

	if (NULL == (newQueue = (Queue *)malloc(sizeof(Queue) + ((_maxSize - 1) * sizeof(void *)))))
	{
		return NULL;
	}

	newQueue->m_maxSize = _maxSize;
	newQueue->m_head = 0;
	newQueue->m_tail = 0;
	newQueue->m_nItems = 0;

	return newQueue;
}

void Queue_Destroy(Queue **_queue, void (*_elementDestroy)(void *_item))
{
	int i;

	if (NULL == _queue || NULL == *_queue)
	{
		return;
	}

	if (NULL != _elementDestroy)
	{
		for (i = 0; i < (*_queue)->m_maxSize; ++i)
		{
			_elementDestroy((*_queue)->m_items + i);
		}
	}

	free(*_queue);
	*_queue = NULL;
}

Queue_Result Queue_Enqueue(Queue *_queue, void *_item)
{

	if (NULL == _queue)
	{
		return QUEUE_UNINITIALIZED_ERROR;
	}

	if (NULL == _item)
	{
		return QUEUE_ITEM_NOT_EXIST;
	}

	if (_queue->m_nItems == _queue->m_maxSize)
	{
		return QUEUE_OVERFLOW;
	}

	_queue->m_items[_queue->m_tail] = _item;
	_queue->m_tail = (_queue->m_tail + 1) % _queue->m_maxSize;
	++_queue->m_nItems;

	return QUEUE_SUCCESS;
}

Queue_Result Queue_Dequeue(Queue *_queue, void **_pValue)
{
	if (NULL == _queue)
	{
		return QUEUE_UNINITIALIZED_ERROR;
	}

	if (_pValue == NULL)
	{
		return QUEUE_NULL_ELEMENT_ERROR;
	}

	if (0 == _queue->m_nItems)
	{
		return QUEUE_UNDERFLOW;
	}

	*_pValue = _queue->m_items[_queue->m_head];
	--_queue->m_nItems;
	_queue->m_head = (_queue->m_head + 1) % _queue->m_maxSize;

	return QUEUE_SUCCESS;
}

int Queue_IsEmpty(const Queue *_queue)
{
	return (NULL == _queue || 0 == _queue->m_nItems) ? 1 : 0;
}
