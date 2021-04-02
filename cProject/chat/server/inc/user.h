#ifndef __USER_H__
#define __USER_H__

/**
 *  @file user.h
 *  @brief user module for single user, holds login data and live groups the client participate in when active .
 *
 *  @details  The user a struct of user saving it's details.
 *  save data of all the groups that the client participate in.
 * 
 *  @warning all given names to functions must be strings
 * 
 *  @author Sivan Tel-Tzur   (tz.sivan@gmail.com)
 *
 *  @bug No known bugs.
 */

typedef struct User User;

typedef enum
{
    FALSE = 0,
    TRUE
} Bool;

typedef int (*UserLogOutActionFunction)(void* _element, void* _context);
typedef enum User_Result
{
    USER_OK,
    USER_NOT_INITIALIZED,
    USER_GROUP_NOT_FOUND,
    USER_ALLOCATION_FAILED,
    USER_ALREADY_IN_GROUP
} User_Result;

/**
 * @brief Create a new User.
 * @param[in] user name
 * @param[in] user password
 * @return newly created User or NULL on failure.
 */
User* UserCreate(const char* _userName, const char* _password);

/**
 * @brief destroy User and set , and all group he was in.
 * @param[in] _user : User to be destroyed
 */
void UserDestroy(User* _user);

/**
 * @brief set user active flag on.
 * @param[in] User : User to be activate.
 * @warning User MUST be existed in chat
 */
void UserActivate(User* _user);

/**
 * @brief set user flag off.
 * @param[in] _user : User to be deactivate.
 * @warning User MUST be existed in chat
 */
void UserDeActivate(User* _user);

/**
 * @brief Create a new user and return message on success or failure. add him to list of groups of the user.
 * @param[in] _user - user pointer.
 * @param[in] _groupName - group name for the new group - string 
 * @return Success indicator
 * @retval  USER_OK	on success.
 * @retval  USER_NOT_INITIALIZED if input not initialize
 * @retval  USER_ALREADY_IN_GROUP	if user already have this group chat.
 * @retval  USER_ALLOCATION_FAILED on allocation user fail.
 * @warning groupName must be a string
 */
User_Result UserAddGroup(User* _user, const char* _groupName);

/**
 * @brief search if the user have the group on his groups list.
 * @param[in] _user - user pointer.
 * @param[in] _groupName - group name to search for must be a string .
 * @return True if exists , false if not exists.
 */
Bool UserIsInGroup(User* _user, const char* _groupName);

/**
 * @brief return the user name.
 * @param[in] _user - user pointer.
 * @return pointer to the name of user if user not initialize returns NULL.
 */
const void* UserGiveName(User* _user);

/**
 * @brief return the user password.
 * @param[in] _user - user pointer.
 * @return pointer to the password of user if user not initialize returns NULL.
 */
void UserGivePass(User* _user, char* _pass);

/**
 * @brief check if given user is active or not.
 * @param[in] _user - user pointer.
 * @return TRUE if user active FALSE if not .
 */
Bool UserIsActive(User* _user);



/**
 * @brief remove the group from list, if found.
 * @param[in] _user - user pointer.
 * @param[in] _groupName - group name for the new group
 * @return Success indicator
 * @retval  USER_OK	on success.
 * @retval  USER_GROUP_NOT_FOUND	if user do not have the group.
 */
User_Result UserRemoveGroup(User* _user, const char* _groupName);


/**
 * @brief remove the groups list of the player, disables him, GroupMng remove count of users in which the user was in.
 * @param[in] _user - user pointer.
 * @param[in] _removeGroupFunc - function from the server manager.
  * @param[in] _context - pointer transfer the given function .
 * @return Success indicator
 * @retval  USER_OK	on success.
 */
User_Result UserLogOut(User* _user, UserLogOutActionFunction _removeGroupFunc, void* _context);



#endif /*  __USER_H__ */