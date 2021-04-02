#ifndef __QUEUE_H__
#define __QUEUE_H__
/** 
 * @brief Create a Generic Queue data type of a given size
 * that stores pointer to user provided elements of generic type
 * The Queue is heap allocated and wraps around
 * 
 * @author Author SivanTelTzur(tz.sivan@gmail.com)
 */

#include <stddef.h> /* size_t */

typedef struct Queue Queue;

typedef enum Queue_Result
{
	QUEUE_SUCCESS,
	QUEUE_UNINITIALIZED_ERROR, /**< Uninitialized queue error 	*/
	QUEUE_OVERFLOW,			   /**< Queue is full for insert operation */
	QUEUE_UNDERFLOW,		   /**< Queue is empty for remove operation */
	QUEUE_ITEM_NOT_EXIST,
	QUEUE_NULL_ELEMENT_ERROR

	/* Add more as needed by your implementation */
} Queue_Result;

/**  
 * @brief Dynamically create a new queue object of given capacity and  
 * @param[in] _maxSize - maximal number of elements that can be stored in the queue
 * @return Queue * - on success / NULL on fail 
 *
 * @warning if _maxSize is zero function will return NULL.
 */
Queue *Queue_Create(size_t _maxSize);

/**  
 * @brief Dynamically deallocate a previously allocated vector  
 * @param[in] _queue - Vector to be deallocated.
 * @params[in] _elementDestroy : A function pointer to be used to destroy all
 * 					 remaining elements in the queue or a null if no such destroy is required
 * @return void
 */
void Queue_Destroy(Queue **_queue, void (*_elementDestroy)(void *_item));

/**  
 * @brief Enqueue an Item to the tail of the Queue.  
 * @param[in] _queue - Queue to operate on.
 * @param[in] _item - Item to enqueue.
 * @return success or error code 
 * @retval 		QUEUE_SUCCESS on success 
 * @retval QUEUE_UNINITIALIZED_ERROR if one of the Queue is NULL. 
 * @retval  QUEUE_OVERFLOW if the queue is full.
 * @warning _item can't be null. this will be assertion violation
 */
Queue_Result Queue_Enqueue(Queue *_queue, void *_item);

/**  
 * @brief Dequeue (Remove and obtain) item at head of queue or return an error if queue is empty.  
 * @param[in] _queue - Queue to operate on.
 * @param[out] _pValue - pointer to variable that will receive dequeued item
 * @return success or error code 
 * @retval QUEUE_SUCCESS on success 
 * @retval QUEUE_UNINITIALIZED_ERROR if one of the Queue is NULL. 
 * @retval  QUEUE_UNDERFLOW if the queue is empty.   
 */
Queue_Result Queue_Dequeue(Queue *_queue, void **_pValue);

/**  
 * @brief Return true if the queue is empty.
 * @param[in] _queue - Queue to check.
 * @return  1 if empty 0 otherwise
 */
int Queue_IsEmpty(const Queue *_queue);

#endif /* __QUEUE_H__ */
