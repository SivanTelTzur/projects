#include "hashMap.h"
#include <stdio.h>
#include <stdlib.h> /*calloc*/
#include <string.h> /*memcpy*/


#define CAPACITY_ENLARGE(x) (((x) * 130 ) / 100)
#define HASH_INDEX(hashNumber, capacity) ((hashNumber) % (capacity))
#define EMPTY 0
#define ERASED 1
#define OUT_OF_RANGE(x) ((x)->m_capacity)
#define KEY(x) (_map->m_hashTable[x].m_key)

typedef enum
{
    FALSE, TRUE
} Bool;
typedef struct
{
	const void* m_key;
	void* m_value;
} Pair;

struct HashMap
{
	size_t m_capacity;
    size_t m_userCapacity;
	HashFunction m_hash;
	EqualityFunction m_isEqual;
	size_t m_nElements;
	Pair* m_hashTable;
    size_t m_maxInsertionSteps;
    size_t m_collision;
};

static size_t FindNextPrimeNumber(size_t _number);
static Bool isPrime(size_t _num);
static Map_Result InsertKey(HashMap* _map, size_t _HashIndex, const void* _key, void* _value);
static size_t FindEmptyPlace(HashMap* _map, const void* _key, size_t _hashIndex);
static Bool IsDuplicate(HashMap* _map, const void* _key, size_t _hashIndex);
static size_t FindKeyIndex(const HashMap* _map, const void* _key);
static void	HashMap_Rehash_ForEach(HashMap *_mapCur, HashMap *_newMap);
static void swapMap(HashMap *_map1, HashMap *_map2);

/***********************************************************************************************/


HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	HashMap* map;
	if (_hashFunc == NULL || _keysEqualFunc == NULL || _capacity < 1)
	{
		return NULL;
	}
	map = (HashMap*)malloc(sizeof(HashMap));
	if (map == NULL)
	{
		return NULL;
	}

    map->m_capacity = FindNextPrimeNumber(CAPACITY_ENLARGE(_capacity));
    map->m_hashTable = (Pair*)calloc(map->m_capacity, sizeof(Pair));
    if (map->m_hashTable == NULL)
	{
		free(map);
		return NULL;
	}

	map->m_userCapacity = _capacity;
	map->m_hash = _hashFunc;
	map->m_isEqual = _keysEqualFunc;
	map->m_nElements = 0;
	map->m_collision = 0;
    map->m_maxInsertionSteps = 0;
	return map;
}

void HashMap_Destroy(HashMap** _map, DestroyFunction1 _keyDestroy, DestroyFunction2 _valDestroy)
{
    size_t index;
    Pair* current;
    if (NULL == _map || NULL == *_map || NULL == (*_map)->m_hashTable)
    {
        return;
    }
    for(index = 0; index < (*_map)->m_capacity; ++index)
    {
     current = &(*_map)->m_hashTable[index] ;
     if (current->m_key != NULL && (size_t)current->m_key != ERASED)
     {
         if (_keyDestroy)
         {
             _keyDestroy(current->m_key);
         }
        if (_valDestroy)
         {
             _valDestroy(current->m_value);
         }
     }
    }
    free((*_map)->m_hashTable);
    (*_map)->m_hashTable = NULL;
    free(*_map);
	*_map = NULL;
}

Map_Result HashMap_Insert(HashMap* _map, const void* _key, void* _value)
{
    size_t hashIndex;
	if (_map == NULL )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
    if(_key == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    if(_value == NULL)
    {
        return MAP_VALUE_NULL_ERROR;
    }

    if (_map->m_userCapacity == _map->m_nElements)
    {
        return MAP_IS_FULL;
    }

    hashIndex = HASH_INDEX(_map->m_hash(_key), _map->m_capacity);
    if (IsDuplicate(_map, _key, hashIndex))
    {
        return MAP_KEY_DUPLICATE_ERROR;
    }
   return InsertKey(_map, hashIndex, _key, _value);
}

Map_Result HashMap_Find(const HashMap* _map, const void* _key, void** _pValue)
{
    size_t foundIndex;
	if (_map == NULL )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
    if(_key == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }
    if(_pValue == NULL)
    {
        return MAP_VALUE_NULL_ERROR;
    }

    foundIndex = FindKeyIndex(_map, _key);
    if (OUT_OF_RANGE(_map) == foundIndex)
    {
        *_pValue = NULL;
        return MAP_KEY_NOT_FOUND_ERROR;
    }
    *_pValue = _map->m_hashTable[foundIndex].m_value;
    return MAP_SUCCESS;
}

size_t HashMap_Size(const HashMap* _map)
{
    return (_map != NULL) ? _map->m_nElements : 0;
}

Map_Result HashMap_Remove(HashMap* _map, const void* _searchKey, const void** _pKey, void** _pValue)
{
    size_t foundIndex;

	if (_map == NULL )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
    if(_searchKey == NULL)
    {
        return MAP_KEY_NULL_ERROR;
    }

    foundIndex = FindKeyIndex(_map, _searchKey);
    if (foundIndex == OUT_OF_RANGE(_map))
    {
        return MAP_KEY_NOT_FOUND_ERROR;
    }

    if(_pKey != NULL || _pValue != NULL)
    {
        *_pValue = _map->m_hashTable[foundIndex].m_value;
        *_pKey = _map->m_hashTable[foundIndex].m_key;
    }
    _map->m_hashTable[foundIndex].m_key = (void*)ERASED;
    --_map->m_nElements;

    return MAP_SUCCESS;
}

double Hash_AverageCollisions(const HashMap *_hash)
{
    if (_hash == NULL )
	{
		return MAP_UNINITIALIZED_ERROR;
	}
    return (double)_hash->m_collision / (double)_hash->m_nElements;
}

Map_Result HashMap_Rehash(HashMap *_map, size_t _newCapacity)
{
	HashMap* newMap;

	if (_map == NULL)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
    if (_map->m_capacity > _newCapacity)
	{
        return MAP_UNDERFLOW_ERROR;
    }
	newMap = HashMap_Create(_newCapacity, _map->m_hash, _map->m_isEqual);
	if (newMap == NULL)
	{
		return MAP_ALLOCATION_ERROR;
	}
	HashMap_Rehash_ForEach(_map, newMap);
	swapMap(_map, newMap);
	HashMap_Destroy(&newMap, NULL, NULL);

	return MAP_SUCCESS;
}


size_t HashMap_ForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context)
{
	size_t index;
	size_t count = 0;
	if (_map == NULL || _action == NULL)
	{
		return count;
	}
	for (index = 0; index < _map->m_capacity; ++index)
	{
		if(KEY(index) != NULL && (size_t)KEY(index) != ERASED)
		{
            ++count;
            if (!_action(KEY(index), _map->m_hashTable[index].m_value, _context))
            {
                return count;
            }
		}
	}
	return count;
}



/*************************************************************************************************************/
static void swapMap(HashMap *_map1, HashMap *_map2)
{
	HashMap temp;

	memcpy(&temp, _map1, sizeof(HashMap));
	memcpy(_map1, _map2, sizeof(HashMap));
	memcpy(_map2, &temp, sizeof(HashMap));
}

static void	HashMap_Rehash_ForEach(HashMap *_mapCur, HashMap *_newMap)
{
    size_t index, newIndex;
    void* key;
    for (index = 0; index < _mapCur->m_capacity; ++index)
    {
        key = (void*) _mapCur->m_hashTable[index].m_key;
        if( key != NULL && (size_t)key != ERASED)
        {
            newIndex = HASH_INDEX(_mapCur->m_hash(key), _newMap->m_capacity);
            _newMap->m_hashTable[newIndex] = _mapCur->m_hashTable[index];
        }
    }
}


static size_t FindKeyIndex(const HashMap* _map, const void* _key)
{
    size_t currIndex , hashIndex = HASH_INDEX(_map->m_hash(_key), _map->m_capacity);

    currIndex = hashIndex;
    while (currIndex <= (hashIndex + _map->m_maxInsertionSteps) % _map->m_capacity)
    {
        if (KEY(currIndex) == NULL)
        {
            return OUT_OF_RANGE(_map);
        }
        if ((size_t)KEY(currIndex) != ERASED && _map->m_isEqual(KEY(currIndex), _key))
        {
            return currIndex;
        }
        currIndex = (currIndex + 1) % _map->m_capacity;
    }
    return OUT_OF_RANGE(_map);
}



static size_t FindEmptyPlace(HashMap* _map, const void* _key, size_t _hashIndex)
{
    size_t currIndex = _hashIndex;

    while (currIndex < _map->m_capacity && KEY(currIndex) != NULL)
    {
        if ((size_t)KEY(currIndex) == ERASED)
        {
            return currIndex;
        }
        currIndex = (currIndex + 1) % _map->m_capacity;
    }
    return currIndex;
}


static Map_Result InsertKey(HashMap* _map, size_t _HashIndex, const void* _key, void* _value)
{
    size_t index, steps;

    index = FindEmptyPlace(_map, _key, _HashIndex);

    KEY(index) = _key;
    _map->m_hashTable[index].m_value = _value;

    ++_map->m_nElements;
    if((steps = index - _HashIndex) > _map->m_maxInsertionSteps)
    {
        _map->m_maxInsertionSteps = steps;
    }
    _map->m_collision+= steps;
    return MAP_SUCCESS;
}

static Bool IsDuplicate(HashMap* _map, const void* _key, size_t _hashIndex)
{
        size_t index = _hashIndex;

        do
        {
            if (KEY(index) == NULL )
            {
                return FALSE;
            }
            if (_map->m_isEqual(_key, KEY(index) ))
            {
               return TRUE;
            }
            ++index;
        } while (index <= (_hashIndex + _map->m_maxInsertionSteps));
        return FALSE;
}

static Bool isPrime(size_t _num)
{
	size_t i;
    if(_num % 2 == 0)
    {
        return FALSE;
    }
	for(i = 3; i * i <= _num; i += 2)
	{
		if(_num % i == 0)
		{
			return FALSE;
		}
	}
	return TRUE;
}

static size_t FindNextPrimeNumber(size_t _number)
{
	while(isPrime(_number) != TRUE)
	{
		_number++;
	}
	return _number;
}