
#include "serverMng.h"
#include "protocol.h"
#include "server.h"
#include "usersMng.h"
#include "groupMng.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 100

struct ServerMng
{
    Server *m_netServer;
    UsersMng *m_users;
    GroupMng *m_groups;
};

/*Tags Main Functions */
static int Registration(ClientNameAndPass *_userDetails, ServerMng *_serverMng);
static int Login(ClientNameAndPass *_userDetails, ServerMng *_serverMng);
static void CreatNewGroup(ClientCreateGruop *_newGroup, ServerMng *_serverMng, ServerGroupSendIpAndPort *_newGroupDetails);
static void JoinGroupListOfGroups(ServerJoinGroupShow *_groupsList, ServerMng *_serverMng);
static void JoinSelectedGroup(ClientJoinSelected *_userJoinGroup, ServerGroupSendIpAndPort *_groupIpPortReply, ServerMng *_serverMng);
static int LeaveGroup(ClientLeaveGroup *userLeaveGroup, ServerMng *_serverMng);
static int IsUserNameOK(char *_userName);
static int IsGroupNameOK(char *_groupName);
static int IsPasswordOK(char *_pass);
static Bool CheckInputNameGroup(char *_name, char *_groupName);

/**************************************************************************************************************/

/*Server Net function*/
int ReceiveFromServer(char *_buffer, int _sock, int _size, void *_context);
void NewClient(int _sock, void *_context);
void ClientClose(const int _sock, void *_context);
/*Log Out User Group Function*/
int GroupLeave(void *_groupName, void *_groupMng);

/*************************************API_FUNCTIONS*********************************************/

ServerMng *ServerMngCreate(size_t _maxClients)
{
    Server *serverNet = NULL;
    UsersMng *usersMng = NULL;
    GroupMng *groupMng = NULL;
    ServerMng *serverMng = NULL;

    serverMng = (ServerMng *)malloc(sizeof(ServerMng));
    if (!serverMng)
    {
        return NULL;
    }
    serverNet = Server_Create((int)_maxClients, ReceiveFromServer, NewClient, ClientClose, NULL, serverMng);
    usersMng = UsersMngCreate(_maxClients);
    groupMng = GroupMngCreate();

    if (serverNet == NULL || usersMng == NULL || groupMng == NULL)
    {
        DestroyServer(&serverNet);
        UsersMngDestroy(&usersMng);
        GroupMngDestroy(&groupMng);
        return NULL;
    }

    serverMng->m_groups = groupMng;
    serverMng->m_netServer = serverNet;
    serverMng->m_users = usersMng;
    return serverMng;
}

/*********************************************************/
ServerMng_Result ServerMngRun(ServerMng *_serverMng)
{
    Server_Run(_serverMng->m_netServer);
    return SERVER_MGN_OK;
}

/*********************************************************/

void ServerMngDestroy(ServerMng **_serverMng)
{
    if (!_serverMng || !*_serverMng)
    {
        return;
    }

    DestroyServer(&(*_serverMng)->m_netServer);
    UsersMngDestroy(&(*_serverMng)->m_users);
    GroupMngDestroy(&(*_serverMng)->m_groups);
    free(*_serverMng);
    *_serverMng = NULL;
}

/**********************************SERVER_NET_TRIGGER_FUNCTION*************************************/

/* when a client packet received server net activate this function - this function assign actions to each of given tags from user*/
int ReceiveFromServer(char *_buffer, int _sock, int _size, void *_context)
{
    char replyBuffer[MAX_BUFFER];

    int tag;
    ClientNameAndPass user;
    ClientCreateGruop newGroup;
    ServerGroupSendIpAndPort groupIpPortReply;
    ServerJoinGroupShow groupsList;
    ClientJoinSelected userJoinGroup;
    ClientLeaveGroup userLeaveGroup;
    ClientLogOut userLogOut;
    ServerMng *serverMng = (ServerMng *)_context;

    tag = ProtocolGetTag(_buffer);
    printf("tag received : %d\n", tag);
    switch (tag)
    {
    case REGISTRATION:
        ProtocolUnPackRegistrate(_buffer, &user);
        ProtocolPackRegReply(Registration(&user, serverMng), replyBuffer);
        break;

    case LOGIN:
        ProtocolUnPackLogin(_buffer, &user);
        ProtocolPackLoginReply(Login(&user, serverMng), replyBuffer);
        break;

    case LOG_OUT:
        ProtocolUnPackLogOut(_buffer, &userLogOut);
        ProtocolPackLogOutReply(UsersMngUserLogOut(serverMng->m_users, userLogOut.m_userName, GroupLeave, serverMng->m_groups), replyBuffer);
        break;

    case CREATE_GROUP:
        ProtocolUnPackCreateGroup(_buffer, &newGroup);
        CreatNewGroup(&newGroup, serverMng, &groupIpPortReply);
        ProtocolPackReplyCreateGroup(&groupIpPortReply, replyBuffer);
        break;

    case JOIN_GROUP_SHOW:
        JoinGroupListOfGroups(&groupsList, serverMng);
        ProtocolPackReplyJoinGroupShow(&groupsList, replyBuffer);
        break;

    case JOIN_GROUP_SELECTED:
        ProtocolUnPackJoinSelect(_buffer, &userJoinGroup);
        JoinSelectedGroup(&userJoinGroup, &groupIpPortReply, serverMng);
        ProtocolPackReplyJoinGroupByIP(&groupIpPortReply, replyBuffer);
        break;

    case LEAVE_GROUP:
        ProtocolUnPackLeaveGroup(_buffer, &userLeaveGroup);
        ProtocolPackLeaveGroupReply(LeaveGroup(&userLeaveGroup, serverMng), replyBuffer);
        break;
    }
    Server_Send_Message(replyBuffer, sizeof(replyBuffer), _sock);
    return 0;
}

/*this function handle user request to leave group - it is also check if user name and group is valid*/
static int LeaveGroup(ClientLeaveGroup *userLeaveGroup, ServerMng *_serverMng)
{
    /*Check if values received from client are valid*/
    if (CheckInputNameGroup(userLeaveGroup->m_groupName, userLeaveGroup->m_userName) &&
        UsersMngIsUserInGroup(_serverMng->m_users, userLeaveGroup->m_userName, userLeaveGroup->m_groupName))
    {
        if (GroupMngUserLeaveGroup(_serverMng->m_groups, userLeaveGroup->m_groupName) == FAIL)
        {
            return FAIL;
        }
        UserMngLeaveGroup(_serverMng->m_users, userLeaveGroup->m_userName, userLeaveGroup->m_groupName);
        return OK;
    }
    return FAIL;
}

static Bool CheckInputNameGroup(char *_name, char *_groupName)
{
    if (IsGroupNameOK(_groupName) && IsUserNameOK(_name))
    {
        return TRUE;
    }
    return FALSE;
}

static void JoinSelectedGroup(ClientJoinSelected *_userJoinGroup, ServerGroupSendIpAndPort *_groupIpPortReply, ServerMng *_serverMng)
{
    /*Check if values received from client are valid*/
    if (!IsGroupNameOK(_userJoinGroup->m_groupName) || !IsUserNameOK(_userJoinGroup->m_userName))
    {
        return;
    }

    /*check if client Allready In group*/
    if (UsersMngIsUserInGroup(_serverMng->m_users, _userJoinGroup->m_userName, _userJoinGroup->m_groupName) != FAIL)
    {
        *_groupIpPortReply->m_port = '0';
        return;
    }
    /*Join group - check if group exist*/
    GroupMngJoinGroup(_serverMng->m_groups, _userJoinGroup->m_groupName, _groupIpPortReply->m_port, _groupIpPortReply->m_ipAddress);
    if (*_groupIpPortReply->m_port == '0')
    {
        return;
    }
    UsersMngUserAddGroup(_serverMng->m_users, _userJoinGroup->m_userName, _userJoinGroup->m_groupName);
}

static void CreatNewGroup(ClientCreateGruop *_newGroup, ServerMng *_serverMng, ServerGroupSendIpAndPort *_newGroupDetails)
{
    /*Check if values received from client are valid*/
    if (!IsGroupNameOK(_newGroup->m_groupName) || !IsUserNameOK(_newGroup->m_userName))
    {
        return;
    }

    GroupMngCreateNewGroup(_serverMng->m_groups, _newGroup->m_groupName, _newGroupDetails->m_port, _newGroupDetails->m_ipAddress);
    if (*_newGroupDetails->m_port == '0')
    {
        return;
    }
    UsersMngUserAddGroup(_serverMng->m_users, _newGroup->m_userName, _newGroup->m_groupName);
}

static int Registration(ClientNameAndPass *_userDetails, ServerMng *_serverMng)
{
    UsersMng_Result status;
    /*Check if values received from client are valid*/
    if (IsUserNameOK(_userDetails->m_userName) && IsPasswordOK(_userDetails->m_userPass))
    {
        status = UsersMngRegisterUser(_serverMng->m_users, _userDetails->m_userName, _userDetails->m_userPass);
        if (status == USERS_MGN_OK)
        {
            return OK;
        }
    }
    return FAIL;
}

static int Login(ClientNameAndPass *_userDetails, ServerMng *_serverMng)
{
    UsersMng_Result status;

    /*Check if values received from client*/
    if ((IsUserNameOK(_userDetails->m_userName) && IsPasswordOK(_userDetails->m_userPass)) &&
        !UsersMngIsUserActive(_serverMng->m_users, _userDetails->m_userName))
    {
        status = UsersMngUserLogin(_serverMng->m_users, _userDetails->m_userName, _userDetails->m_userPass);
        if (status == USERS_MGN_OK)
        {
            return OK;
        }
    }
    return FAIL;
}

void NewClient(int _sock, void *_context)
{
    char *hello = "welcome to our App\n";

    printf("New client been created sock Number : %d\n", _sock);
    Server_Send_Message(hello, sizeof(hello), _sock);
}

void ClientClose(const int _sock, void *_context)
{
    printf("Client %d has been closed by server\n", _sock);
}

int IsUserNameOK(char *_userName)
{
    return (_userName == NULL || strlen(_userName) >= MAX_NAME) ? FAIL : OK;
}

int IsGroupNameOK(char *_groupName)
{
    return (_groupName == NULL || strlen(_groupName) >= MAX_NAME) ? FAIL : OK;
}

int IsPasswordOK(char *_pass)
{
    return (_pass == NULL || strlen(_pass) >= MAX_NAME) ? FAIL : OK;
}

static void JoinGroupListOfGroups(ServerJoinGroupShow *_groupsList, ServerMng *_serverMng)
{
    _groupsList->m_vectorNames = GroupMngAllGroups(_serverMng->m_groups);
}

int GroupLeave(void *_groupName, void *_groupMng)
{
    GroupMngUserLeaveGroup((GroupMng *)_groupMng, (char *)_groupName);
    return 0;
}