#include "user.h"
#include "list_functions.h"
#include <stdlib.h>
#include <string.h>

#define ACTIVE 1
#define NOT_ACTIVE 0
struct User
{
    const char *m_userName;
    const char *m_password;
    List *m_groups;
    int m_nOfGroups;
    Bool m_isActive;
};

/*Functions to send list */

/*compare between two users name*/
static int EqualFunc(void *_a, void *_b);
/*destroy each struct that exist in list*/
static void DestroyForList(void *_item);

/*****************************************************************/

User *UserCreate(const char *_userName, const char *_password)
{
    char *name;
    char *password;
    User *newUser = (User *)malloc(sizeof(User));
    if (newUser == NULL)
    {
        return NULL;
    }

    name = malloc(strlen(_userName) + 1);
    strcpy(name, _userName);
    password = malloc(strlen(_password) + 1);
    strcpy(password, _password);

    newUser->m_userName = name;
    newUser->m_password = password;
    newUser->m_isActive = FALSE;
    newUser->m_groups = List_Create();
    return newUser;
}

void UserDestroy(User *_user)
{
    if (_user == NULL)
    {
        return;
    }
    List_Destroy(&_user->m_groups, DestroyForList);
    free(_user);
}

void UserActivate(User *_user)
{
    if (_user != NULL)
    {
        _user->m_isActive = ACTIVE;
    }
}

void UserDeActivate(User *_user)
{
    if (_user != NULL)
    {
        _user->m_isActive = NOT_ACTIVE;
    }
}

User_Result UserAddGroup(User *_user, const char *_groupName)
{
    char *newGroup;
    if (_user == NULL || _groupName == NULL)
    {
        return USER_NOT_INITIALIZED;
    }
    if (UserIsInGroup(_user, _groupName))
    {
        return USER_ALREADY_IN_GROUP;
    }
    newGroup = (char *)malloc(strlen(_groupName) + 1);
    if (newGroup == NULL)
    {
        return USER_ALLOCATION_FAILED;
    }

    strcpy(newGroup, _groupName);
    List_PushHead(_user->m_groups, newGroup);
    return USER_OK;
}

Bool UserIsInGroup(User *_user, const char *_groupName)
{
    ListItr listBegin = ListItr_Begin(_user->m_groups);
    ListItr listEnd = ListItr_End(_user->m_groups);
    ListItr temp;
    temp = ListItr_FindFirst(listBegin, listEnd, EqualFunc, (void *)_groupName);
    if (temp != listEnd)
    {
        return TRUE;
    }
    return FALSE;
}

const void *UserGiveName(User *_user)
{
    if (_user == NULL)
    {
        return NULL;
    }
    return _user->m_userName;
}

User_Result UserRemoveGroup(User *_user, const char *_groupName)
{
    ListItr listBegin = ListItr_Begin(_user->m_groups);
    ListItr listEnd = ListItr_End(_user->m_groups);
    ListItr tempCurr;
    char *list;

    tempCurr = ListItr_FindFirst(listBegin, listEnd, EqualFunc, (void *)_groupName);
    if (tempCurr == listEnd)
    {
        return USER_GROUP_NOT_FOUND;
    }
    list = ListItr_Remove(tempCurr);
    free(list);
    return USER_OK;
}

User_Result UserLogOut(User *_user, UserLogOutActionFunction _removeGroupFunc, void *_context)
{
    void *groupName;

    while (List_PopTail(_user->m_groups, &groupName) != LIST_UNDERFLOW)
    {
        _removeGroupFunc(groupName, _context);
        free(groupName);
    }
    UserDeActivate(_user);
    return USER_OK;
}

void UserGivePass(User *_user, char *_pass)
{
    if (_user != NULL)
    {
        strcpy(_pass, _user->m_password);
    }
}

Bool UserIsActive(User *_user)
{
    if (_user != NULL)
    {
        return (_user->m_isActive);
    }
    return FALSE;
}

/*******************************************/

static int EqualFunc(void *_a, void *_b)
{
    int status = strcmp((char *)_a, (char *)_b);
    if (status == 0)
    {
        return TRUE;
    }

    return FALSE;
}

static void DestroyForList(void *_item)

{
    free(_item);
}