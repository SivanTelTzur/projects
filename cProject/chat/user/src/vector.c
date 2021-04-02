#include <stdio.h>
#include<stdlib.h> /*malloc , free*/
#include "vector.h"

#define MAGIC 82734023

struct Vector
{
	size_t m_magic;
	void* *m_items;
	size_t m_originalSize;
	size_t m_size;
	size_t m_nitems;
	size_t m_blockSize;
};


Vector* Vector_Create(size_t _initialCapacity, size_t _blockSize)
{
	Vector *newVector = (Vector*)malloc(sizeof(Vector));
	if (newVector == NULL)
	{
		return NULL;
	}

	newVector->m_magic = MAGIC;	
	newVector->m_items = NULL;
	newVector->m_originalSize = _initialCapacity;
	newVector->m_size = _initialCapacity;
	newVector->m_nitems = 0;
	newVector->m_blockSize = _blockSize;

	newVector->m_items = (void **)malloc(sizeof(void*) *_initialCapacity);
	if (newVector->m_items == NULL)
	{
		free(newVector);
		return NULL;
	}
	return newVector;
}

void Vector_Destroy(Vector** _vector, void (*_elementDestroy)(void* _item))
{
	int i;

	if (_vector == NULL || (*_vector)->m_magic != MAGIC)
	{
		return;
	}

	if (_elementDestroy != NULL)
	{
		for (i=0;i < (*_vector)->m_nitems;i++)
		{
			_elementDestroy((*_vector)->m_items[i])	;
		}
	} 

	free((*_vector)->m_items);
	(*_vector)->m_magic=0;
	free(*_vector);
	*_vector=NULL;
}

Vector_Result Vector_Append(Vector* _vector, void* _item)
{
	if (_vector == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	
	if (_vector->m_nitems>=_vector->m_size)
	{
		if (_vector->m_blockSize == 0)
		{
			return VECTOR_UNDERFLOW_ERROR;
		}
		_vector->m_size+=_vector->m_size+_vector->m_blockSize;
		_vector->m_items=(void*)realloc(_vector->m_items,sizeof(void*)*_vector->m_size);
	}
	if (_vector->m_items==NULL)
	{
		return VECTOR_ALLOCATION_ERROR;
	}
	_vector->m_items[_vector->m_nitems]=_item;
	_vector->m_nitems++;
	return VECTOR_SUCCESS;
}

Vector_Result Vector_Remove(Vector* _vector, void** _pValue)
{
	if (_vector == NULL || _pValue == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
		
	if (_vector->m_nitems == 0)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;/*NO ITEMS TO DELETE*/
	}

	*_pValue=_vector->m_items[_vector->m_nitems-1];
	_vector->m_nitems--;

	if (_vector->m_nitems < _vector->m_size-2*_vector->m_blockSize && _vector->m_size - _vector->m_originalSize)
	{
		_vector->m_size=_vector->m_size - _vector->m_blockSize;
		_vector->m_items = (void**)realloc(_vector->m_items,sizeof(void*)*_vector->m_size);/*if failed what happens to old vector*/
	}

	if (_vector->m_items == NULL)
	{
		return VECTOR_ALLOCATION_ERROR;
	}
	return VECTOR_SUCCESS;	
}

Vector_Result Vector_Set(Vector* _vector, size_t _index, void*  _value)
{
	if (_vector==NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if (_vector->m_nitems == 0 || _vector->m_nitems < _index)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;		
	}

	_vector->m_items[_index]=_value;
	return VECTOR_SUCCESS;
}

Vector_Result Vector_Get(const Vector* _vector, size_t _index, void** _pValue)
{
	if (_vector == NULL)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}
	if(_vector->m_nitems == 0 || _vector->m_nitems < _index)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}

	*_pValue=_vector->m_items[_index];
	return VECTOR_SUCCESS;
}

size_t Vector_Size(const Vector* _vector)
{
	if (_vector == NULL)
	{
		return 0;
	}
	return _vector->m_nitems;
}

size_t Vector_Capacity(const Vector* _vector)
{
	if (_vector == NULL)
	{
		return 0;
	}
	return _vector->m_size;
}

size_t Vector_ForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
	size_t i;	
	for(i = 0; i < Vector_Size(_vector); ++i)
	{
        if (_action(_vector->m_items[i], i, _context) == 0)
		{
			break;
		}	
    }
	return i;
}

