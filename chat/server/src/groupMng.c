#include "groupMng.h"
#include "queue.h"
#include "hashMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*strcmp, strcpy*/

#define MAX_NAME 20
#define PORT_SIZE 7
#define FILE_NAME "queueIPFile.txt"
#define FALSE 0
#define TRUE 1
#define IP_MAX 16
#define PORT "1900"
#define BLOCK_SIZE 3
#define LAST_USER 1
#define FAIL 0
#define OK 1

struct GroupMng
{
    Queue *m_ipQue;
    HashMap *m_groups;
};

typedef struct Group
{
    char m_name[MAX_NAME];
    char m_port[PORT_SIZE];
    char m_ip[MAX_NAME];
    int m_usersCount;
} Group;

/*Destroy elements in DS held by Group */
static void DestroyForGroup(void *_group);
static void DestroyForQueue(void *_queue);

/*Hash functions*/
/*Creat a unique key from group name*/
static size_t ForUniqueKey(const void *_groupName);
/*compare between two groups names*/
static int EqualityFunctionHash(const void *_givenKey, const void *_hashKey);
/*Hash func to insert a group name into vector (used in ForEach) */
int AddGroupListToVec(const void *_groupName, void *_groupStruct, void *_groupsVec);
/**************************************************************************************/

/*Insert to a Queue 100 multicast IP address*/
static void InsertFromFile(Queue *_myQueue);

/*check if a given group name is exist in chat*/
static int IsGroupExist(GroupMng *_groupMng, const char *_groupName);

/*Fill all existed groups names into a vector*/
static void FillGroupsVector(GroupMng *_groupMng, Vector *_groupsVec);

/*return number of existed groups*/
static size_t NOfGroups(HashMap *_groups);
/***********************************************************************************************/
GroupMng *GroupMngCreate()
{
    GroupMng *newGroupMng = (GroupMng *)malloc(sizeof(GroupMng));
    if (newGroupMng == NULL)
    {
        return NULL;
    }
    newGroupMng->m_ipQue = Queue_Create(MAX_N_GROUP);
    InsertFromFile(newGroupMng->m_ipQue);
    newGroupMng->m_groups = HashMap_Create(MAX_N_GROUP, ForUniqueKey, EqualityFunctionHash);
    return newGroupMng;
}

void GroupMngDestroy(GroupMng **_groupMng)
{
    if (NULL == _groupMng || NULL == *_groupMng)
    {
        return;
    }
    Queue_Destroy(&(*_groupMng)->m_ipQue, DestroyForQueue);
    HashMap_Destroy(&(*_groupMng)->m_groups, NULL, DestroyForGroup);
    free(_groupMng);
    _groupMng = NULL;
}

Group_Result GroupMngCreateNewGroup(GroupMng *_groupMng, char *_groupName, char *_port, char *_ip)
{
    Map_Result status;
    Group *newGroupName;
    char *ip;

    if (_groupMng == NULL || _groupName == NULL || _port == NULL || _ip == NULL)
    {
        return GROUP_MNG_NOT_INITIALIZED;
    }
    strcpy(_port, "0");
    strcpy(_ip, "0");
    if (TRUE == IsGroupExist(_groupMng, _groupName))
    {
        return GROUP_MNG_EXIST;
    }
    newGroupName = (Group *)malloc(sizeof(Group));
    if (newGroupName == NULL)
    {
        return GROUP_MNG_ALLOCATION_FAILED;
    }

    Queue_Dequeue(_groupMng->m_ipQue, (void **)&ip);
    strcpy(_ip, ip);
    strcpy(newGroupName->m_ip, _ip);
    free(ip);
    strcpy(newGroupName->m_port, PORT);
    strcpy(_port, PORT);

    strcpy(newGroupName->m_name, _groupName);
    newGroupName->m_usersCount = 1;

    status = HashMap_Insert(_groupMng->m_groups, newGroupName->m_name, newGroupName);
    if (status != MAP_SUCCESS)
    {
        return GROUP_MNG_ALLOCATION_FAILED;
    }
    return GROUP_MNG_SUCCSESS;
}

Group_Result GroupMngJoinGroup(GroupMng *_groupMng, char *_groupName, char *_port, char *_ip)
{
    Group existedGroup;
    Group *pExistedGroup = &existedGroup;

    strcpy(_port, "0");
    strcpy(_ip, "0");
    if (_groupMng == NULL || _groupName == NULL || _port == NULL || _ip == NULL)
    {
        return GROUP_MNG_NOT_INITIALIZED;
    }

    if (FALSE == IsGroupExist(_groupMng, _groupName))
    {
        return GROUP_MNG_NOT_EXIST;
    }

    HashMap_Find(_groupMng->m_groups, _groupName, (void **)&pExistedGroup);
    strcpy(_ip, pExistedGroup->m_ip);
    strcpy(_port, pExistedGroup->m_port);
    ++pExistedGroup->m_usersCount;

    return GROUP_MNG_SUCCSESS;
}

Vector *GroupMngAllGroups(GroupMng *_groupMng)
{
    Vector *groupsVec;
    size_t nOfGroups = 0;

    if (_groupMng == NULL)
    {
        return NULL;
    }

    nOfGroups = NOfGroups(_groupMng->m_groups);
    groupsVec = Vector_Create(nOfGroups, BLOCK_SIZE);
    FillGroupsVector(_groupMng, groupsVec);
    return groupsVec;
}


int AddGroupListToVec(const void *_groupName, void *_groupStruct, void *_groupsVec)
{
    char *sendGroupName;

    sendGroupName = malloc(strlen((char *)_groupName) + 1);
    strcpy(sendGroupName, _groupName);
    if (VECTOR_SUCCESS != Vector_Append((Vector *)_groupsVec, sendGroupName))
    {
        return FAIL;
    }
    return OK;
}


int GroupMngUserLeaveGroup(GroupMng *_groupMng, char *_groupname)
{
    Group *leavingGroup;
    Map_Result status;
    char *ip;

    if (_groupMng == NULL || _groupname == NULL)
    {
        return FAIL;
    }

    status = HashMap_Find(_groupMng->m_groups, _groupname, (void **)&leavingGroup);
    if (status == MAP_SUCCESS)
    {

        if (leavingGroup->m_usersCount == LAST_USER)
        {
            HashMap_Remove(_groupMng->m_groups, _groupname, NULL, NULL);
            ip = (char *)malloc(strlen(leavingGroup->m_ip) + 1);
            strcpy(ip, leavingGroup->m_ip);
            Queue_Enqueue(_groupMng->m_ipQue, (void *)ip);
            DestroyForGroup(leavingGroup);
            leavingGroup = NULL;
        }
        else
        {
            leavingGroup->m_usersCount--;
        }
        return OK;
    }
    return FAIL;
}
/**************************************************************************************/

static size_t NOfGroups(HashMap *_groups)
{
    return HashMap_Size(_groups);
}

static void DestroyForGroup(void *_group)
{
    if (_group == NULL)
    {
        return;
    }
    free(_group);
    _group = NULL;
}

static void DestroyForQueue(void *_pIp)
{
    if (_pIp == NULL)
    {
        return;
    }
    free(_pIp);
    _pIp = NULL;
}

static int IsGroupExist(GroupMng *_groupMng, const char *_groupName)
{
    int *temp;
    if (_groupMng == NULL || _groupName == NULL)
    {
        return -1;
    }
    return (MAP_SUCCESS == HashMap_Find(_groupMng->m_groups, _groupName, (void **)&temp)) ? TRUE : FALSE;
}

static void FillGroupsVector(GroupMng *_groupMng, Vector *_groupsVec)
{
    if (_groupMng == NULL)
    {
        return;
    }
    HashMap_ForEach(_groupMng->m_groups, AddGroupListToVec, _groupsVec);
}


static size_t ForUniqueKey(const void *_groupName)
{
    int SumAscii = 0;
    int i;

    if (_groupName == 0)
    {
        return 0;
    }
    for (i = 0; i < strlen((char *)_groupName); ++i)
    {
        SumAscii += ((char *)_groupName)[i];
    }
    return (size_t)SumAscii;
}

static int EqualityFunctionHash(const void *_givenKey, const void *_hashKey)
{
    int status;
    status = strcmp((char *)_givenKey, (char *)_hashKey);
    if (status == 0)
    {
        return TRUE;
    }
    return FALSE;
}

static void InsertFromFile(Queue *_myQueue)
{
    FILE *fp;
    char *myPtr;
    char data[20];

    if (_myQueue == 0)
    {
        return;
    }
    fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
    {
        return;
    }

    fscanf(fp, "%s", data);
    while (!feof(fp))
    {
        myPtr = (char *)malloc(strlen(data) + 1);
        strcpy(myPtr, data);
        Queue_Enqueue(_myQueue, (void *)myPtr);
        fscanf(fp, "%s", data);
    }
    fclose(fp);
}