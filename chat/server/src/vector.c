#include "vector.h"
#include "stdlib.h" /*malloc , free */

#define MAGIC 28645425
#define BLOAC_MIN_RESIZE 2

struct Vector
{
	int m_magic;
	void **m_items;
	size_t m_initialCapacity; /* original allocated space for items)  */
	size_t m_capacity;		  /* actual allocated space for items)*/
	size_t m_nItems;		  /* actual number of items */
	size_t m_blockSize;		  /* the chunk size to be allocated when no space*/
};

static int IsEnoughSpace(Vector *_vector);
static Vector_Result Vector_Resize(Vector *_vector, size_t _newSize);
/*****************************************************************************************/

Vector *Vector_Create(size_t _initialCapacity, size_t _blockSize)
{
	Vector *newVector = NULL;
	void *elemArr = NULL;

	if (0 == _initialCapacity && 0 == _blockSize)
	{
		return NULL;
	}

	newVector = (Vector *)malloc(sizeof(Vector));
	if (NULL == newVector)
	{
		return NULL;
	}

	elemArr = (void *)calloc(_initialCapacity, sizeof(void *));
	if (NULL == elemArr)
	{
		free(newVector);
		return NULL;
	}

	newVector->m_initialCapacity = _initialCapacity;
	newVector->m_blockSize = _blockSize;
	newVector->m_items = elemArr;
	newVector->m_nItems = 0;
	newVector->m_capacity = _initialCapacity;
	newVector->m_magic = MAGIC;

	return newVector;
}

void Vector_Destroy(Vector **_vector, void (*_elementDestroy)(void *_item))
{

	if (NULL == _vector || *_vector == NULL || MAGIC == (*_vector)->m_magic)
	{
		return;
	}

	if (!_elementDestroy)
	{
		while ((*_vector)->m_nItems--)
		{
			_elementDestroy((*_vector)->m_items[(*_vector)->m_nItems]);
		}
	}
	free((*_vector)->m_items);
	(*_vector)->m_magic = 0;
	free(*_vector);
	(*_vector) = NULL;
}

Vector_Result Vector_Append(Vector *_vector, void *_item)
{

	if (NULL == _vector || NULL == _item)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (!(IsEnoughSpace(_vector)))
	{
		if (Vector_Resize(_vector, _vector->m_capacity + _vector->m_blockSize) != VECTOR_SUCCESS)
		{
			return VECTOR_ALLOCATION_ERROR;
		}
	}

	_vector->m_items[_vector->m_nItems] = _item;
	++_vector->m_nItems;

	return VECTOR_SUCCESS;
}

Vector_Result Vector_Remove(Vector *_vector, void **_pValue)
{

	if (NULL == _vector || NULL == _pValue)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (0 == _vector->m_nItems)
	{
		return VECTOR_UNDERFLOW;
	}

	*_pValue = _vector->m_items[_vector->m_nItems - 1];
	_vector->m_items[--_vector->m_nItems] = 0; /*clean the address*/

	if (_vector->m_capacity > _vector->m_initialCapacity &&
		(_vector->m_capacity - _vector->m_nItems) > BLOAC_MIN_RESIZE * _vector->m_blockSize)
	{
		return (Vector_Resize(_vector, _vector->m_capacity - _vector->m_blockSize));
	}
	return VECTOR_SUCCESS;
}

Vector_Result Vector_Get(const Vector *_vector, size_t _index, void **_pValue)
{
	if (NULL == _vector || NULL == _pValue)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (0 == _vector->m_nItems)
	{
		return VECTOR_UNDERFLOW;
	}

	if (_index >= _vector->m_nItems)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}

	*_pValue = _vector->m_items[_index];
	return VECTOR_SUCCESS;
}

Vector_Result Vector_Set(Vector *_vector, size_t _index, void *_value)
{
	if (NULL == _vector || NULL == _value)
	{
		return VECTOR_UNITIALIZED_ERROR;
	}

	if (_index >= _vector->m_nItems)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}

	_vector->m_items[_index] = _value;
	return VECTOR_SUCCESS;
}

size_t Vector_Size(const Vector *_vector)
{
	return (_vector != NULL) ? _vector->m_nItems : 0;
}

size_t Vector_Capacity(const Vector *_vector)
{
	return (_vector != NULL) ? _vector->m_capacity : 0;
}

size_t Vector_ForEach(const Vector *_vector, VectorElementAction _action, void *_context)
{
	size_t i;

	if (NULL == _vector)
	{
		return 0;
	}

	for (i = 0; i < _vector->m_nItems; ++i)
	{
		if (_action(_vector->m_items[i], i, _context) == 0)
		{
			break;
		}
	}
	return i;
}

/************************************************************************/
/*Description: Check if more space is needed.
*Input:Vector* _vector pointer ;
*Output:true if there is space , false if there is no room
*/

static int IsEnoughSpace(Vector *_vector)
{
	if (_vector->m_capacity == _vector->m_nItems)
	{
		return 0;
	}
	return 1;
}

/*Description: Resize items Array by block size
*Input:Vector* _vector pointer ,_newSize is the new size of the array;
*Output:error code
*/

static Vector_Result Vector_Resize(Vector *_vector, size_t _newSize)
{
	void **new = NULL;

	if (0 == _vector->m_blockSize)
	{
		return VECTOR_ALLOCATION_ERROR;
	}

	new = (void **)realloc(_vector->m_items, _newSize * sizeof(void *));

	if (NULL == new)
	{
		return VECTOR_ALLOCATION_ERROR;
	}

	_vector->m_capacity = _newSize;
	_vector->m_items = new;
	return VECTOR_SUCCESS;
}
