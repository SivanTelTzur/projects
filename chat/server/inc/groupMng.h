#ifndef __GROUPMNG_H__
#define __GROUPMNG_H__

#include "vector.h" /*vector struct*/

/**
 *  @file groupMng.h
 *  @brief group manager module handle all existed groups and handle creation and destroy of single group.
 *
 *  @details  The GroupMng handle all aspect of groups in a chat . GroupMng creates groups(supply an IP number and port).
 *  Num of groups is limited to MAX_N_GROUP.
 *  In case of user log out all groups that the user is the last user will be closed and destroy also when user leave single group
 * 
 *  @warning group name given must be a string
 *  @author Sivan Tel-Tzur   (tz.sivan@gmail.com)
 *
 *  @bug No known bugs.
 */

typedef struct GroupMng GroupMng;

#define MAX_N_GROUP 100

typedef enum
{
    GROUP_MNG_SUCCSESS = 0,
    GROUP_MNG_ALLOCATION_FAILED, /**< Allocation error 	 	    	*/
    GROUP_MNG_NOT_INITIALIZED,   /**< Uninitialized pointer error 	*/
    GROUP_MNG_NOT_EXIST,         /**< Group not found 				*/
    GROUP_MNG_EXIST              /**< Group allready exist			*/
} Group_Result;

/**
 * @brief Create a new groupMng.
 * @param[in] _Non
 * @return newly created groupMng or NULL on failure.
 */
GroupMng *GroupMngCreate();

/**
 * @brief destroy groupMng and set *_groupMng to NULL - destroy all existed Groups and IP's.
 * @param[in] _groupMng : groupMng to be destroyed
 */
void GroupMngDestroy(GroupMng **_groupMng);

/**
 * @brief Create a new group and return an multicasting IP and port to connect.
 * user is automaticaly join group
 * @param[in] _groupMng - group manager module adt to insert to.
 * @param[in] _groupName - group name for the new group
 * @param[in] _port - string pointer to return port on fail return this parameter as 0.
 * @param[in] _ip - string pointer to return IP for the new group
 * @return Success indicator
 * @retval  GROUP_MNG_SUCCSESS	on success
 * @retval  GROUP_MNG_ALLOCATION_FAILED	if fail to allocate group
 * @retval  GROUP_MNG_NOT_INITIALIZED
 * @warning if group allready exist returns port 0
 */
Group_Result GroupMngCreateNewGroup(GroupMng *_groupMng, char *_groupName, char *_port, char *_ip);

/**
 * @brief add user to an existing group update the  multicasting IP and port to connect the group.
 * @param[in] _groupMng - group manager struct.
 * @param[in] _groupName - group name for the group chosen group
 * @param[in] _port - string pointer to return port on fail return this parameter as 0.
 * @param[in] _ip - string pointer to return IP for the new group
 * @return Success indicator
 * @retval  GROUP_MNG_SUCCSESS	on success
 * @retval  GROUP_MNG_NOT_INITIALIZED
 * @retval  GROUP_MNG_NOT_EXIST if chosen group name does not exist
 * @warning on fail returns port 0 
 */
Group_Result GroupMngJoinGroup(GroupMng *_groupMng, char *_groupName, char *_port, char *_ip);

/**
 * @brief Insert all existing groups in chat.
 * @param[in] _groupMng -group manager struct.
 * @return Vector pointer with all existed groups names
 * @retval  NULL if _groupMng not initialize, vector pionter on success
 * @warning when empty returns empty vector
 */
Vector *GroupMngAllGroups(GroupMng *_groupMng);

/**
 * @brief remove a user from a given group name
 * @param[in] _groupMng - group manager module struct 
 * @param[in] _groupName - group name for the group to leave 
 * @return OK on success FAIL  
 * @retval  OK	on success
 * @retval  FAIL if group not exist or NULL input
 * @retval  GROUP_MNG_NOT_INITIALIZED
 * @warning  0
 */
int GroupMngUserLeaveGroup(GroupMng *_groupMng, char *_groupname);

#endif