#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stddef.h> /* size_t */

/**
 *  @file HashMap.h
 *  @brief Generic Hash map of key-value pairs implemented with separate chaining using linked lists.
 *
 *  @details  The hash map (sometimes called dictionary or associative array)
 *  is a set of distinct keys (or indexes) mapped (or associated) to values.
 *  size of allocated table will be the nearest prime number greater than requestedcapacity
 *  Lists used for chaining will be allocated lazily.
 *
 *  @author Sivan Tel-Tzur   (tz.sivan@gmail.com)
 *
 *  @bug No known bugs.
 */

typedef struct HashMap HashMap;

typedef enum Map_Result
{
	MAP_SUCCESS = 0,
	MAP_UNINITIALIZED_ERROR, /**< Uninitialized map error 	*/
	MAP_KEY_NULL_ERROR,		 /**< Key was null 				*/
	MAP_KEY_DUPLICATE_ERROR, /**< Duplicate key error 		*/
	MAP_KEY_NOT_FOUND_ERROR, /**< Key not found 				*/
	MAP_ALLOCATION_ERROR,	 /**< Allocation error 	 		*/
	MAP_VALUE_NULL_ERROR,	 /**< value is null 				*/
	MAP_IS_FULL,			 /**< reached initial capacity 	*/
	MAP_IS_OUT_OF_BOUND,	 /**< if out of given size.		*/
	MAP_UNDERFLOW_ERROR		 /**< Low Capacity.				*/
} Map_Result;

typedef size_t (*HashFunction)(const void *_key);
typedef int (*EqualityFunction)(const void *_firstKey, const void *_secondKey);
typedef int (*KeyValueActionFunction)(const void *_key, void *_value, void *_context);
typedef void (*DestroyFunction1)(const void *_key);
typedef void (*DestroyFunction2)(void *_value);

/**
 * @brief Create a new hash map with given capcity and key characteristics.
 * @param[in] _capacity - Expected max capacity
 * 						  shall be rounded to nearest larger prime number.
 * @param[in] _hashFunc - hashing function for keys
 * @param[in] _keysEqualFunc - equality check function for keys.
 * @return newly created map or null on failure
 */
HashMap *HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc);

/**
 * @brief destroy hash map and set *_map to null
 * @param[in] _map : map to be destroyed
 * @param[optional] _keyDestroy : pointer to function to destroy keys
 * @param[optional] _valDestroy : pointer to function to destroy values
 * @details optionally destroy all keys and values using user provided functions
 */
void HashMap_Destroy(HashMap **_map, DestroyFunction1 _keyDestroy, DestroyFunction2 _valDestroy);

/**
 * @brief Adjust map capacity and rehash all key/value pairs
 * @param[in] _map - existing map
 * @param[in] _newCapacity - new capacity shall be rounded to nearest larger prime number.
 * @return MAP_SUCCESS or MAP_ALLOCATION_ERROR
 * @retval  MAP_UNINITIALIZED_ERROR if given map pointer NULL.
 */
Map_Result HashMap_Rehash(HashMap *_map, size_t _newCapacity);

/**
 * @brief Insert a key-value pair into the hash map.
 * @param[in] _map - Hash map adt to insert to, must be initialized
 * @param[in] _key - key to serve as index
 * @param[in] _value - The value to associate with the key
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_DUPLICATE_ERROR	if key already present in the map
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_UNINITIALIZED_ERROR
 * @retval  MAP_VALUE_NULL_ERROR
 * @retval  MAP_IS_FULL reached initial capacity
 * @retval  MAP_IS_OUT_OF_BOUND if out of given size.
 * @warning key must be unique and destinct
 */
Map_Result HashMap_Insert(HashMap *_map, const void *_key, void *_value);

/**
 * @brief Remove a key-value pair from the hash map.
 * @param[in] _map - Hash map to remove pair from, must be initialized
 * @param[in] _key - key to serve as index to locate _value
 * @param[in] _pKey  - pointer to variable to receive key from map if found
 * @param[in] _pValue - pointer to variable that will get the value if found.
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_KEY_NOT_FOUND_ERROR if key not found
 * @retval  MAP_UNINITIALIZED_ERROR
 *
 * @warning key must be unique and destinct
 */
Map_Result HashMap_Remove(HashMap *_map, const void *_searchKey, const void **_pKey, void **_pValue);

/**
 * @brief Find a value by key
 * @param[in] _map - Hash map to remove pair from, must be initialized
 * @param[in] _key - key to serve as index
 * @param[in] _pValue - pointer to variable that will get the value if found can't be NULL.
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_KEY_NOT_FOUND_ERROR if key not found
 * @retval  MAP_UNINITIALIZED_ERROR
 * @retval  MAP_VALUE_NULL_ERROR
 *
 * @warning key must be unique and destinct
 */
Map_Result HashMap_Find(const HashMap *_map, const void *_key, void **_pValue);

/**
 * @brief Get number of key-value pairs inserted into the hash map
 * @warning complexity can be O(?)
 */
size_t HashMap_Size(const HashMap *_map);

/**
 * @brief Iterate over all key-value pairs in the map.
 *                   Iteration will stop if _act returns a zero for a given pair
 * @details The user provided KeyValueActionFunction _action will be called for each element.
 * @param[in] _map - Hash map to iterate over.
 * @param[in] _act - User provided function pointer to be invoked for each element
 * @returns number of times the user functions was invoked
 */
size_t HashMap_ForEach(const HashMap *_map, KeyValueActionFunction _action, void *_context);

/**
 * This method is calculating the average number of collisions
 * @brief This method is calculating the average number of collisions
 * @details Statistics method
 * @param[in] _hash - Hash map to iterate over.
 * @returns average number of collisions per allcations
 */
double Hash_AverageCollisions(const HashMap *_hash);

#ifndef NDEBUG

typedef struct Map_Stats
{
	size_t numberOfBuckets;
	size_t numberOfChains;
	size_t maxChainLength;
	size_t averageChainLength;
} Map_Stats;

Map_Stats HashMap_GetStatistics(const HashMap *_map);

#endif /* NDEBUG */

#endif /* __HASHMAP_H__ */
