#ifndef __USERS_MNG_H__
#define __USERS_MNG_H__

#include "user.h"
#include <stddef.h>

/**
 *  @file usersMng.h
 *  @brief users manager module handle all existed users (registered)).
 *
 *  @details  The usersMng handle all aspect of users in a chat . usersMng creates users(supply an userName and password).
 *  Num of users is limited to _maxClients given in initialization.
 *  In case of user log out all groups that the user is in will be updated
 *
 *  @author Sivan Tel-Tzur   (tz.sivan@gmail.com)
 *
 *  @bug No known bugs.
 */

typedef struct UsersMng UsersMng;

typedef enum UsersMng_Result
{
    USERS_MGN_OK = 0,
    USERS_MGN_NOT_INITIALIZED,  /**< Uninitialized pointer error 	*/
    USERS_MGN_USER_NOT_FOUND,   /**< User not found 				*/
    USERS_MGN_USER_EXIST,       /**< User allready exist			*/
    USERS_MGN_FAIL_CREATE_USER, /**< Allocation error 	 	    	*/
    USERS_MNG_FAIL_LOGIN        /**< User login failed	 	    	*/
} UsersMng_Result;

/**
 * @brief Create a new usersMng.
 * @param[in] _maxClients to create min DS size can't be 0 
 * @return newly created usersMng or NULL on failure.
 */
UsersMng *UsersMngCreate(size_t _maxClients);

/**
 * @brief destroy usersMng and set *_usersMng to NULL - destroy all existed users 
 * @param[in] _usersMng : usersMng to be destroyed
 */
void UsersMngDestroy(UsersMng **_usersMng);

/**
 * @brief Create a new user and return a success indicator
 * @param[in] _usersMng - user manager module adt to insert to.
 * @param[in] _userName - user name for the new user
 * @param[in] _password - string pointer of password.
 * @return Success indicator
 * @retval  USERS_MNG_SUCCSESS	on success
 * @retval  USERS_MGN_FAIL_CREATE_USER	if fail to allocate user
 * @retval  USERS_MGN_NOT_INITIALIZED
 * @retval  USERS_MGN_USER_EXIST if user allready exist
 * @warning if user allready exist returns port 0
 */
UsersMng_Result UsersMngRegisterUser(UsersMng *_usersMng, const char *_userName, const char *_password);

/**
 * @brief login a user a success indicator
 * @param[in] _usersMng - user manager module adt to insert to.
 * @param[in] _userName - user name for the new user
 * @param[in] _password - string pointer of password.
 * @return Success indicator
 * @retval  USERS_MNG_SUCCSESS	on success
 * @retval  USERS_MNG_FAIL_LOGIN	if password is wrong
 * @retval  USERS_MGN_NOT_INITIALIZED
 * @retval  USERS_MGN_USER_NOT_FOUND if user doesn't exist
 * @warning user must exit with logout
 */
UsersMng_Result UsersMngUserLogin(UsersMng *_usersMng, const char *_userName, const char *_password);

/**
 * @brief add group to an existing user .
 * @param[in] _usersMng - group manager struct.
 * @param[in] _userName - the given user name
 * @param[in] _groupName - group name for the group
 * @return No return value
 * @warning user must be login and not already in group 
 */
void UsersMngUserAddGroup(UsersMng *_usersMng, const char *_userName, const char *_groupName);

/**
 * @brief user leave group - removes group from user list.
 * @param[in] _usersMng - user manager struct.
 * @param[in] _userName - the given user name
 * @param[in] _groupName - group name for the group
 * @return No return value
 * @warning user must be login and not already in group 
 */
void UserMngLeaveGroup(UsersMng *_usersMng, const char *_userName, const char *_groupName);

/**
 * @brief user logout all groups and deactivate (0) - removes group from user list and reduce all groups that the user joined in by 1.
 * @param[in] _usersMng - user manager struct.
 * @param[in] _userName - the given user name
 * @param[in] __leaveGroup an UserLogOutActionFunction for update the groups that a user left group and context
 * @return If input not initialized, user not exist or not active return Fail(0) , return OK on success
 */
int UsersMngUserLogOut(UsersMng *_usersMng, const char *_userName, UserLogOutActionFunction _leaveGroup, void *_context);

/**
 * @brief check if user in a given group name
 * @param[in] _usersMng - users manager struct.
 * @param[in] _userName - the given user name
 * @param[in] _groupName a group name for the group
 * @return If input not initialized, user not exist or not in group Fail(0) , return OK on success
 */
int UsersMngIsUserInGroup(UsersMng *_usersMng, const char *_userName, const char *_groupName);

/**
 * @brief check if user is active
 * @param[in] _usersMng - users manager struct.
 * @param[in] _userName - the given user name
 * @return If input not initialized, user not exist or user not active Fail(0) , return OK on if user active
 */
int UsersMngIsUserActive(UsersMng *_usersMng, const char *_userName);

#endif /* __USERS_MNG_H__ */