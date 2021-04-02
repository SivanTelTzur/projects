#include "clientMng.h"
#include "client_net.h"
#include "ui.h"
#include "protocol.h"
#include "vector.h"
#define _POSIX_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h> /*fd_set*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> /*malloc*/
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#define KEYBOARD_LISTEN 0
#define MAX_SOCKETS 1024
#define MAX_USERNAME 25
#define MAX_PASSWORD 10
#define TRUE 1
#define EXIT_FLAG 2
#define INITIAL_SIZE 100
#define BLOCK_SIZE 10
#define EMPTY -2

typedef struct GroupNamePid GroupNamePid;
struct GroupNamePid
{
    char m_groupname[MAX_USERNAME];
    int m_pidSender;
    int m_pidListener;
};

struct ClientMng
{
    int m_clientsocket;
    char m_username[MAX_USERNAME];
    char m_groupname[MAX_USERNAME];
    Vector *m_gruops;
};

static void CkeckClientNamePass(char *_name, char *_pass);
static void SwapVector(ClientMng *_vector, size_t _index);
static int LeaveGroupForEach(void *_elem, size_t _index, void *_cont);

/*-------------for menu ----------------------------------*/
static void ShowMenuCompareToServerReply(int _status, int *_flagStatus);

/************deals with server replys**********/
static int CheckServerReply(ClientMng *_clientMng, char _serverMessaeg[], int *_flagStatus);
static int CheckTagStatus(ClientMng *_clientMng, int _tag, char _buffer[], char _serverMessaeg[], int *_flagStatus);
static int RegReply(char buffer[]);
static int LoginReply(char buffer[]);
static int CreateGroupReply(ClientMng *_clientMng, char buffer[]);
static int JoinGroupReply(ClientMng *_clientMng, char buffer[]);
static int JoinGroupIpReply(ClientMng *_clientMng, char _buffer[]);
static int LeaveGroupReply(ClientMng *_clientMng, char _buffer[]);

/************deals with user selections**********/
static int CheckUserSelect(ClientMng *_clientMng, int _select, int *_flagStatus);
static int DealWithFlagZero(ClientMng *_clientMng, int _select, int* _flagStatus);
static int DealWithFlagOne(ClientMng *_clientMng, int _select, int *_flagStatus);

/*******starts chat for client-----------*/
static int OpenChat(GroupNamePid *_newGroupVec, char *_username, char *_ip, char *_port);

/***************check user group select------*/
static int CheckUserName(void *_elem, size_t _index, void *_cont);

/********for log out-------------*/
static void ForKillPid(void *_item);
static void CloseAllChats(ClientMng *_clientMng);
static int Logout(ClientMng *_clientMng, char _buffer[], int *_flagStatus);

/********for delete protocol vector-------------*/
static void ForDeleteProtocolVector(void *_item);

ClientMng *ClientMngCreate()
{
    ClientMng *newClinet = (ClientMng *)malloc(sizeof(ClientMng));
    newClinet->m_clientsocket = Client_Create();
    return newClinet;
}

ClientMng_Result ClientMngRun(ClientMng *_clientMng)
{
    fd_set master, temp;
    int activity, flagRegister = 0, status, userselect;
    char serverMessaeg[MAX_BUFFER];
    FD_ZERO(&master);
    FD_SET(KEYBOARD_LISTEN, &master);
    FD_SET(_clientMng->m_clientsocket, &master);
    _clientMng->m_gruops = Vector_Create(INITIAL_SIZE, BLOCK_SIZE);
    while (TRUE && flagRegister != EXIT_FLAG)
    {
        temp = master;
        activity = select(MAX_SOCKETS, &temp, NULL, NULL, NULL);
        if (activity < 0)
        {
            printf("select error");
            /* return CLIENT_MGN_CONNECTION_FAILED; */
        }
        if (FD_ISSET(KEYBOARD_LISTEN, &temp))
        {
            scanf("%d", &userselect);
            getchar();
            CheckUserSelect(_clientMng, userselect, &flagRegister);
        }
        if (FD_ISSET(_clientMng->m_clientsocket, &temp))
        {
            status = CheckServerReply(_clientMng, serverMessaeg, &flagRegister);
            ShowMenuCompareToServerReply(status, &flagRegister);
        }
    }
    if (flagRegister == EXIT_FLAG)
    {
        Vector_Destroy(&_clientMng->m_gruops, NULL);
        free(_clientMng);
    }
    return CLIENT_MGN_OK;
}

static void ShowMenuCompareToServerReply(int _status, int *_flagStatus)
{
    if (_status == 1)
    {
        *_flagStatus = 1;
    }
    if (*_flagStatus == 0)
    {
        MenuWelcome();
    }
    else if (*_flagStatus == 1)
    {
        MenuStart();
    }
}

static int CheckUserSelect(ClientMng *_clientMng, int _select, int *_flagStatus)
{
    if (*_flagStatus == 0)
    {
        DealWithFlagZero(_clientMng, _select, _flagStatus);
    }
    if (*_flagStatus == 1)
    {
        DealWithFlagOne(_clientMng, _select, _flagStatus);
    }
    return 0;
}

static void ForKillPid(void *_item)
{
    GroupNamePid *test = (GroupNamePid *)_item;
    kill(test->m_pidListener, 15);
    kill(test->m_pidSender, 15);
    free(_item);
}

static void CloseAllChats(ClientMng *_clientMng)
{
    Vector_Destroy(&_clientMng->m_gruops, ForKillPid);
    _clientMng->m_gruops = NULL;
}

static int DealWithFlagOne(ClientMng *_clientMng, int _select, int *_flagStatus)
{
    size_t index;
    ClientLeaveGroup leavegroup;
    ClientCreateGruop structForProtocol;
    ClientLogOut LogOutProtocol;
    char buffer[MAX_BUFFER], *pointer;
    if (_select == LOGOUT)
    {
        strcpy(LogOutProtocol.m_userName, _clientMng->m_username);
        ProtocolPackLogOut(&LogOutProtocol, buffer);
        Send_Message(_clientMng->m_clientsocket, buffer, sizeof(buffer));
        return 0;
    }
    if (_select == CREATEGROUP)
    {
        GetNameOfGroup(structForProtocol.m_groupName);
        strcpy(structForProtocol.m_userName, _clientMng->m_username);
        strcpy(_clientMng->m_groupname, structForProtocol.m_groupName);
        ProtocolPackCreateGroup(&structForProtocol, buffer);
        return Send_Message(_clientMng->m_clientsocket, buffer, sizeof(buffer));
    }

    if (_select == JOINGROUP)
    {
        ProtocolPackJoinGroupShow(buffer);
        return Send_Message(_clientMng->m_clientsocket, buffer, sizeof(buffer));
    }
    if (_select == LEAVEGROUP)
    {
        if (Vector_Size(_clientMng->m_gruops) == 0)
        {
            DontHaveGroupsYet();
            return 0;
        }
        for (index = 0; index < Vector_Size(_clientMng->m_gruops); ++index)
        {
            Vector_Get(_clientMng->m_gruops, index, (void **)&pointer);
            UiShowGroups(pointer, index);
        }
        PressZeroToGoBack();
        GetNameOfGroup(leavegroup.m_groupName);
        while (index >= Vector_Size(_clientMng->m_gruops))
        {
            index = Vector_ForEach(_clientMng->m_gruops, CheckUserName, leavegroup.m_groupName);
            if (index >= Vector_Size(_clientMng->m_gruops) && *leavegroup.m_groupName != '0')
            {
                WrongGroupName();
                GetNameOfGroup(leavegroup.m_groupName);
            }
            if (*leavegroup.m_groupName == '0')
            {
                MenuStart();
                return 0;
            }
        }
        strcpy(leavegroup.m_userName, _clientMng->m_username);
        strcpy(_clientMng->m_groupname, leavegroup.m_groupName);
        ProtocolPackLeaveGroup(&leavegroup, buffer);
        return Send_Message(_clientMng->m_clientsocket, buffer, sizeof(buffer));
    }
    return 0;
}

static int DealWithFlagZero(ClientMng *_clientMng, int _select, int* _flagStatus)
{
    ClientNameAndPass structForProtocol;
    char buffer[MAX_BUFFER];
    if (_select == REGISTRATION)
    {
        getUsernamePassword(structForProtocol.m_userName, structForProtocol.m_userPass);
        CkeckClientNamePass(structForProtocol.m_userName, structForProtocol.m_userPass);
        strcpy(_clientMng->m_username, structForProtocol.m_userName);
        ProtocolPackRegistrate(&structForProtocol, buffer);
        return Send_Message(_clientMng->m_clientsocket, buffer, sizeof(buffer));
    }
    if (_select == LOGINN)
    {
        getUsernamePassword(structForProtocol.m_userName, structForProtocol.m_userPass);
        CkeckClientNamePass(structForProtocol.m_userName, structForProtocol.m_userPass);
        strcpy(_clientMng->m_username, structForProtocol.m_userName);
        ProtocolPackLogin(&structForProtocol, buffer);
        return Send_Message(_clientMng->m_clientsocket, buffer, sizeof(buffer));
    }
    if(_select == EXIT)
    {
        *_flagStatus = 2;
    }
    return 0;
}

static void CkeckClientNamePass(char *_name, char *_pass)
{
    while (strlen(_name) > MAX_NAME)
    {
        TooBigName();
        getName(_name);
    }
    while (strlen(_pass) > MAX_PASS)
    {
        TooBigPass();
        getPassword(_pass);
    }
    return;
}

static int CheckServerReply(ClientMng *_clientMng, char _serverMessaeg[], int *_flagStatus)
{
    int tag;
    char buffer[MAX_BUFFER];
    Recv_Message(_clientMng->m_clientsocket, buffer, sizeof(buffer));
    tag = ProtocolGetTag(buffer);
    return CheckTagStatus(_clientMng, tag, buffer, _serverMessaeg, _flagStatus);
}

static int CheckTagStatus(ClientMng *_clientMng, int _tag, char _buffer[], char _serverMessaeg[], int *_flagStatus)
{

    if (_tag == REG_REPLY)
    {
        return RegReply(_buffer);
    }
    if (_tag == LOGIN_REPLY)
    {
        return LoginReply(_buffer);
    }
    if (_tag == REPLY_CREATE_GROUP)
    {
        return CreateGroupReply(_clientMng, _buffer);
    }
    if (_tag == JOIN_GROUP_REPLY_SHOW)
    {
        JoinGroupReply(_clientMng, _buffer);
    }
    if (_tag == JOIN_GROUP_IP)
    {
        JoinGroupIpReply(_clientMng, _buffer);
    }
    if (_tag == LEAVE_GROUP_REPLY)
    {
        LeaveGroupReply(_clientMng, _buffer);
    }
    if (_tag == LOG_OUT_REPLY)
    {
        Logout(_clientMng, _buffer, _flagStatus);
    }
    return 0;
}

static int Logout(ClientMng *_clientMng, char _buffer[], int *_flagStatus)
{
    int status;
    status = ProtocolUnPackLogOutReply(_buffer);
    if (status == OK)
    {
        CloseAllChats(_clientMng);
        *_flagStatus = 0;
        return OK;
    }
    if (status == FAIL)
    {
        LogOutReplyError();
        return FAIL;
    }
    return 0;
}

static int LeaveGroupForEach(void *_elem, size_t _index, void *_cont)
{
    GroupNamePid *_elemTemp = (GroupNamePid *)_elem;
    return strcmp(_elemTemp->m_groupname, (char *)_cont);
}

static int LeaveGroupReply(ClientMng *_clientMng, char _buffer[])
{
    GroupNamePid *delete;
    int status;
    size_t index;
    status = ProtocolUnPackLeaveGroupReply(_buffer);
    if (status == OK)
    {
        index = Vector_ForEach(_clientMng->m_gruops, LeaveGroupForEach, _clientMng->m_groupname);
        SwapVector(_clientMng, index);
        Vector_Remove(_clientMng->m_gruops, (void **)&delete);
        kill(delete->m_pidListener, 15);
        kill(delete->m_pidSender, 15);
        free(delete);
        SuccessToLeaveGroup();
    }
    if (status == FAIL)
    {
        WrongGroupNameLeave();
    }
    return 0;
}

static void SwapVector(ClientMng *_clientMng, size_t _index)
{
    GroupNamePid *delete, *temp;
    Vector_Get(_clientMng->m_gruops, Vector_Size(_clientMng->m_gruops) - 1, (void **)&temp);
    Vector_Get(_clientMng->m_gruops, _index, (void **)&delete);
    Vector_Set(_clientMng->m_gruops, Vector_Size(_clientMng->m_gruops) - 1, (void *)delete);
    Vector_Set(_clientMng->m_gruops, _index, (void *)temp);
}

static int JoinGroupIpReply(ClientMng *_clientMng, char _buffer[])
{
    GroupNamePid *newGroupVec;
    ServerGroupSendIpAndPort structForProtocol;
    ProtocolUnPackReplyJoinGroupByIP(_buffer, &structForProtocol);
    if (*structForProtocol.m_port == '0')
    {
        AlreadyInGroup();
        return 0;
    }
    newGroupVec = (GroupNamePid *)malloc(sizeof(GroupNamePid));
    strcpy(newGroupVec->m_groupname, _clientMng->m_groupname);
    Vector_Append(_clientMng->m_gruops, newGroupVec);
    OpenChat(newGroupVec, _clientMng->m_username, structForProtocol.m_ipAddress, structForProtocol.m_port);
    return 0;
}

static int JoinGroupReply(ClientMng *_clientMng, char _buffer[])
{
    size_t index;
    ClientJoinSelected forSelect;
    ServerJoinGroupShow vector;
    char *pointer;
    vector.m_vectorNames = Vector_Create(INITIAL_SIZE, BLOCK_SIZE);
    ProtocolUnPackReplyJoinGroupShow(_buffer, &vector);
    if (Vector_Size(vector.m_vectorNames) == 0)
    {
        DontHaveGroupsYetServer();
        return 0;
    }
    for (index = 0; index < Vector_Size(vector.m_vectorNames); ++index)
    {
        Vector_Get(vector.m_vectorNames, index, (void **)&pointer);
        UiShowGroups(pointer, index);
    }
    PressZeroToGoBack();
    GetNameOfGroup(forSelect.m_groupName);
    strcpy(forSelect.m_userName, _clientMng->m_username);
    while (index >= Vector_Size(vector.m_vectorNames))
    {
        index = Vector_ForEach(vector.m_vectorNames, CheckUserName, forSelect.m_groupName);
        if (index >= Vector_Size(vector.m_vectorNames) && *forSelect.m_groupName != '0')
        {
            WrongGroupName();
            GetNameOfGroup(forSelect.m_groupName);
        }
        if (*forSelect.m_groupName == '0')
        {
            Vector_Destroy(&vector.m_vectorNames, ForDeleteProtocolVector);
            return 0;
        }
    }
    strcpy(_clientMng->m_groupname, forSelect.m_groupName);
    ProtocolPackJoinSelect(&forSelect, _buffer);
    Send_Message(_clientMng->m_clientsocket, _buffer, MAX_BUFFER);
    Vector_Destroy(&vector.m_vectorNames, ForDeleteProtocolVector);
    return 0;
}

static void ForDeleteProtocolVector(void *_item)
{
    free(_item);
}

static int CheckUserName(void *_elem, size_t _index, void *_cont)
{
    return strcmp((char *)_elem, (char *)_cont);
}

static int CreateGroupReply(ClientMng *_clientMng, char buffer[])
{
    GroupNamePid *newGroupVec;
    ServerGroupSendIpAndPort structForProtocol;
    ProtocolUnPackReplyCreateGroup(buffer, &structForProtocol);
    if (*structForProtocol.m_port == '0')
    {
        GroupNameAlreadyExist();
        return 0;
    }
    newGroupVec = (GroupNamePid *)malloc(sizeof(GroupNamePid));
    strcpy(newGroupVec->m_groupname, _clientMng->m_groupname);
    Vector_Append(_clientMng->m_gruops, newGroupVec);
    OpenChat(newGroupVec, _clientMng->m_username, structForProtocol.m_ipAddress, structForProtocol.m_port);
    return 0;
}

static int RegReply(char buffer[])
{
    int status;
    status = ProtocolUnPackRegReply(buffer);
    if (status == OK)
    {
        return OK;
    }
    if (status == FAIL)
    {
        RegReplyFail();
        return FAIL;
    }
    return 0;
}

static int LoginReply(char buffer[])
{
    int status;
    status = ProtocolUnPackLoginReply(buffer);
    if (status == OK)
    {
        return OK;
    }
    if (status == FAIL)
    {
        LogginReplyFail();
        return FAIL;
    }
    return 0;
}

static int OpenChat(GroupNamePid *_newGroupVec, char *_username, char *_ip, char *_port)
{
    int listenPID = EMPTY;
    int senderPID = EMPTY;
    char fullSender[256] = {'\0'};
    char inputSender[255];
    char inputListener[255];
    char fullListener[256] = {'\0'};
    FILE *fptr;
    FILE *fptr2;
    /*clean the files*/
    fptr = fopen("pid.txt", "w");
    fclose(fptr);
    fptr2 = fopen("pid2.txt", "w");
    fclose(fptr2);
    strcpy(inputSender, _ip);
    strcat(inputSender, " ");
    strcat(inputSender, _port);
    strcat(inputSender, " ");
    strcat(inputSender, _username);
    strcat(inputSender, " ");
    strcat(inputSender, _newGroupVec->m_groupname);

    strcat(fullSender, "gnome-terminal -q -- ./sender.out ");
    strcat(fullSender, inputSender);
    strcpy(inputListener, _ip);
    strcat(inputListener, " ");
    strcat(inputListener, _port);
    strcat(inputListener, " ");
    strcat(inputListener, _newGroupVec->m_groupname);

    strcat(fullListener, "gnome-terminal -q -- ./listener.out ");
    strcat(fullListener, inputListener);

    system(fullSender);
    while (senderPID == EMPTY)
    {
        fptr = fopen("pid.txt", "r");
        fscanf(fptr, "%d", &senderPID);
        fclose(fptr);
    }
    system(fullListener);
    while (listenPID == EMPTY)
    {
        fptr2 = fopen("pid2.txt", "r");
        fscanf(fptr2, "%d", &listenPID);
        fclose(fptr2);
    }
    _newGroupVec->m_pidSender = senderPID;
    _newGroupVec->m_pidListener = listenPID;
    return 0;
}
