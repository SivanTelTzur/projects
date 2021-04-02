#include "usersMng.h"
#include "hashMap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define USER_NAME 100
#define PASSWORD 20
struct UsersMng
{
    HashMap *m_users;
};

static Bool IsUserExist(UsersMng *_usersMng, const char *_userName);
size_t HashUserName(const void *_userName);
int IsEqualName(const void *_givenName, const void *_hashCurrentName);
void DestroyClient(const void *_key);
void DestroyClientGroupList(const void *_GroupList);
void DestroyUser(void* _user);

/*************************************API_FUNCTIONS*********************************************/

UsersMng *UsersMngCreate(size_t _maxClients)
{
    UsersMng *newUsersMng = NULL;
    HashMap *hash = NULL;
    FILE *fp;
    User *user;
    char userName[USER_NAME];
    char password[PASSWORD];

    newUsersMng = malloc(sizeof(UsersMng));
    if (newUsersMng == NULL)
    {
        return NULL;
    }

    hash = HashMap_Create(_maxClients, HashUserName, IsEqualName);
    if (!hash)
    {
        UsersMngDestroy(&newUsersMng);
        return NULL;
    }

    fp = fopen("registeredClients.txt", "r");
    if (fp != NULL)
    {
        fscanf(fp, "%s %s", userName, password);

        while (!feof(fp))
        {
            user = UserCreate(userName, password);
            HashMap_Insert(hash, UserGiveName(user), user);
            fscanf(fp, "%s %s", userName, password);
        }
        fclose(fp);
    }
    newUsersMng->m_users = hash;
    return newUsersMng;
}

/****************************************************************/
void UsersMngDestroy(UsersMng **_usersMng)
{
    if (!_usersMng || !*_usersMng)
    {
        return;
    }

    HashMap_Destroy(&(*_usersMng)->m_users, NULL, DestroyUser);
    free(*_usersMng);
    *_usersMng = NULL;
}

/****************************************************************/

UsersMng_Result UsersMngRegisterUser(UsersMng *_usersMng, const char *_userName, const char *_password)
{
    User *newUser = NULL;
    FILE *fp;

    if (_userName == NULL || _usersMng == NULL || _password == NULL)
    {
        return USERS_MGN_NOT_INITIALIZED;
    }
    if (IsUserExist(_usersMng, _userName))
    {
        return USERS_MGN_USER_EXIST;
    }

    if (NULL == (newUser = UserCreate(_userName, _password)))
    {
        return USERS_MGN_FAIL_CREATE_USER;
    }

    fp = fopen("registeredClients.txt", "a");
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
    }
    HashMap_Insert(_usersMng->m_users, UserGiveName(newUser), newUser);
    UserActivate(newUser);
    fprintf(fp, "%s %s\n", _userName, _password);
    fflush(fp);
    fclose(fp);
    return USERS_MGN_OK;
}

/****************************************************************/

UsersMng_Result UsersMngUserLogin(UsersMng *_usersMng, const char *_userName, const char *_password)
{
    User *user;
    char pass[PASSWORD];

    if (_userName == NULL || _usersMng == NULL || _password == NULL)
    {
        return USERS_MGN_NOT_INITIALIZED;
    }

    if (!IsUserExist(_usersMng, _userName))
    {
        return USERS_MGN_USER_NOT_FOUND;
    }

    HashMap_Find(_usersMng->m_users, _userName, (void **)&user);
    UserGivePass(user, pass);

    if (0 != strcmp(pass, _password))
    {
        return USERS_MNG_FAIL_LOGIN;
    }

    UserActivate(user);
    return USERS_MGN_OK;
}

/****************************************************************/

void UsersMngUserAddGroup(UsersMng *_usersMng, const char *_userName, const char *_groupName)
{
    User *user;
    HashMap_Find(_usersMng->m_users, _userName, (void **)&user);
    UserAddGroup(user, _groupName);
}

/****************************************************************/

void UserMngLeaveGroup(UsersMng *_usersMng, const char *_userName, const char *_groupName)
{
    User *user;
    HashMap_Find(_usersMng->m_users, _userName, (void **)&user);
    UserRemoveGroup(user, _groupName);
}

/****************************************************************/

int UsersMngUserLogOut(UsersMng *_usersMng, const char *_userName, UserLogOutActionFunction _leaveGroup, void *_context)
{
    User *user;

    if (_userName == NULL || _usersMng == NULL || _leaveGroup == NULL || _context == NULL)
    {
        return FALSE;
    }

    if (MAP_SUCCESS == HashMap_Find(_usersMng->m_users, _userName, (void **)&user))
    {
        if (UserIsActive(user))
        {
            UserLogOut(user, _leaveGroup, _context);
            return TRUE;
        }
    }
    return FALSE;
}
/****************************************************************/

int UsersMngIsUserInGroup(UsersMng *_usersMng, const char *_userName, const char *_groupName)
{
    User *user;

    if (_userName == NULL || _usersMng == NULL || _groupName == NULL)
    {
        return FALSE;
    }

    if (MAP_SUCCESS != HashMap_Find(_usersMng->m_users, _userName, (void **)&user))
    {
        return FALSE;
    }
    return UserIsInGroup(user, _groupName);
}

int UsersMngIsUserActive(UsersMng *_usersMng, const char *_userName)
{
    User *user;

    if (_userName == NULL || _usersMng == NULL)
    {
        return FALSE;
    }
    if (MAP_SUCCESS != HashMap_Find(_usersMng->m_users, _userName, (void **)&user))
    {
        return FALSE;
    }
    return (UserIsActive(user)) ? TRUE : FALSE;
}

/****************************************************************/

/*check if a given user name is registered to the chat*/
static Bool IsUserExist(UsersMng *_usersMng, const char *_userName)
{
    int *temp;
    return (MAP_SUCCESS == HashMap_Find(_usersMng->m_users, _userName, (void **)&temp)) ? TRUE : FALSE;
}

/*calc index for hash*/
size_t HashUserName(const void *_userName)
{
    int SumAscii = 0;
    int i;

    for (i = 0; i < strlen((char *)_userName); ++i)
    {
        SumAscii += ((char *)_userName)[i];
    }
    return (size_t)SumAscii;
}

/*calc equalization  for username in hash */
int IsEqualName(const void *_givenName, const void *_hashCurrentName)
{
    return (!strcmp((char *)_givenName, (char *)_hashCurrentName)) ? TRUE : FALSE;
}

/*destroy the user */
void DestroyUser(void* _user)
{
    UserDestroy(_user);
}