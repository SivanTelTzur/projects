#include "protocol.h"
#include <string.h> /* strlen */
#include <stdio.h>  /* print TODO delete? */
#include <stdlib.h>/* CALLOC */

#define DAVINCHI 134
#define DAVINCHI_2 37

void Encrypt(char *_buffer)
{
    char i;
    size_t size = strlen(_buffer);
    printf("buffer before enc = *%s*\n",_buffer);
    /* starting from 1 for not destroying the tag */
    for(i = 1; i< size;i++)
    {
       if (i % 2 == 0)
       {
           *(_buffer + i) = *(_buffer + i) ^ (char)DAVINCHI;
       }
       else
       {
           *(_buffer + i) = *(_buffer + i) ^ (char)DAVINCHI_2;
       }     
    }
    printf("buffer after enc = *%s*\n",_buffer);
}

int ProtocolGetTag(char* _data)
{
    return (int) *_data;
}

/*REGISTRATE 
================================
|TAG=01|T_L|L|USER_N|L|PASSWORD|
================================
*/
int ProtocolPackRegistrate(ClientNameAndPass *_user, char *_buffer)
{
    char* indexBegin = _buffer;
    size_t size = strlen(_user->m_userName) + strlen(_user->m_userPass) + 2; /* 2 for value and pass length */
    *_buffer = REGISTRATION;
    ++_buffer;
    *_buffer = (char)size;
    ++_buffer;
    *_buffer = (char)(strlen(_user->m_userName));
     ++_buffer;
    strcpy(_buffer, _user->m_userName);
    _buffer = _buffer + (char)strlen(_user->m_userName);
    *_buffer = (char)(strlen(_user->m_userPass));
     ++_buffer;
    strcpy(_buffer, _user->m_userPass);
    _buffer = _buffer + strlen(_user->m_userPass);
    *_buffer = '\0';
    Encrypt(indexBegin);
    return (int)size;
}

void ProtocolUnPackRegistrate(char *_data, ClientNameAndPass *_struct)
{
    char temp1 [MAX_NAME]={'\0'};
    char temp2 [MAX_NAME]={'\0'};
    size_t passLength;
    size_t nameLength;
    Encrypt(_data);
    nameLength = (size_t)(*(_data + 2));
 /*    printf("name length = %lu", nameLength); TODO DELETE */
    
    strncpy(temp1, _data + 3, nameLength);
    strncpy(_struct->m_userName, temp1, nameLength+1);

    passLength = (size_t)(*(_data + 2 + 1 + nameLength));
    strncpy(temp2, _data + 2 + 1 + 1 + nameLength, passLength);
    strncpy(_struct->m_userPass, temp2, passLength+1);
}

/*REGREPLY
==============================
|TAG=02|T_L=1|okOrFail|
================================
*/
int ProtocolPackRegReply(int _kOrFail, char* _buffer)
{
    int size = 1;
    *_buffer++ = REG_REPLY;
    *_buffer++ = (char)size; 
    *_buffer++ = (char)_kOrFail;

    return size;
}

int ProtocolUnPackRegReply(char* _data)
{
    return *(_data+2);
}

/*LOGIN 
================================
|TAG=03|T_L|L|USER_N|L|PASSWORD|
================================
*/
int ProtocolPackLogin(ClientNameAndPass *_user, char *_buffer)
{
    char* indexBegin = _buffer;
    size_t size = strlen(_user->m_userName) + strlen(_user->m_userPass) + 2; /* 2 for value and pass length */
    *_buffer = LOGIN; 
    ++_buffer;
    *_buffer = (char)size;
    ++_buffer;
    *_buffer = (char)(strlen(_user->m_userName));
     ++_buffer;
    strcpy(_buffer, _user->m_userName);
    _buffer = _buffer + (char)strlen(_user->m_userName);
    *_buffer = (char)(strlen(_user->m_userPass));
     ++_buffer;
    strcpy(_buffer, _user->m_userPass);
    _buffer = _buffer + strlen(_user->m_userPass);
    *_buffer = '\0';
    Encrypt(indexBegin);
    return (int)size;
}

void ProtocolUnPackLogin(char *_data, ClientNameAndPass *_struct)
{
    char temp1 [MAX_NAME]={'\0'};
    char temp2 [MAX_NAME]={'\0'};
    size_t passLength;
    size_t nameLength; 
    Encrypt(_data);
    nameLength = (size_t)(*(_data + 2));
 /*    printf("name length = %lu", nameLength); TODO DELETE */
    
    strncpy(temp1, _data + 3, nameLength);
    strncpy(_struct->m_userName, temp1, nameLength+1);

    passLength = (size_t)(*(_data + 2 + 1 + nameLength));
    strncpy(temp2, _data + 2 + 1 + 1 + nameLength, passLength);
    strncpy(_struct->m_userPass, temp2, passLength+1);
}

/*LOGINREPLY
=======================
|TAG=04|T_L=1|okOrFail|
=======================
*/
int ProtocolPackLoginReply(int _kOrFail, char* _buffer)
{
    int size = 1;
    *_buffer++ = LOGIN_REPLY;
    *_buffer++ = (char)size; 
    *_buffer++ = (char)_kOrFail;

    return size;
}

int ProtocolUnPackLoginReply(char* _data)
{
    return *(_data+2);
}

/*LOGOUT
===================
|TAG=05|TL|USERNAME|
===================
*/
int ProtocolPackLogOut(ClientLogOut *_struct,char* _buffer)
{
    size_t size = strlen(_struct->m_userName);
    *_buffer = LOG_OUT;
    ++_buffer;
    *_buffer = (char)size;
    ++_buffer;
    strcpy(_buffer, _struct->m_userName);
    _buffer = _buffer + strlen(_struct->m_userName);
    *_buffer = '\0';
    return (int)size;
}

void ProtocolUnPackLogOut(char* _data, ClientLogOut *_struct)
{
    char temp [MAX_NAME]={'\0'};
    size_t userNameLength = (size_t)(*(_data + 1));
    
    strncpy(temp, _data + 2, userNameLength);
    strncpy(_struct->m_userName, temp, userNameLength+1);/*adding '\0' */
    
}

/*CreateGroup
=================================================
|TAG=06|TL|L_User|UserName|gNameLength|groupName|
=================================================
*/
int ProtocolPackCreateGroup(ClientCreateGruop *_group, char *_buffer)
{
    size_t size = strlen(_group->m_groupName) + strlen(_group->m_userName) +2;
    *_buffer = CREATE_GROUP;
    ++_buffer;
    *_buffer = (char)size;
    ++_buffer;
    
    *_buffer = (char)strlen(_group->m_userName);
    ++_buffer;
    strcpy(_buffer, _group->m_userName);
    _buffer = _buffer + strlen(_group->m_userName);

    *_buffer = (char)strlen(_group->m_groupName);
    ++_buffer;
    strcpy(_buffer, _group->m_groupName);
    _buffer = _buffer + strlen(_group->m_groupName);
    
    *_buffer = '\0';
    return (int)size;
}

void ProtocolUnPackCreateGroup(char *_data, ClientCreateGruop *_struct)
{
    char temp [MAX_NAME]={'\0'};
    char temp2 [MAX_NAME]={'\0'};
    size_t groupNameLength;
    size_t userNameLength = (size_t)(*(_data + 2));
   /* printf("userNameLength = %lu", userNameLength); TODO DELETE */
    strncpy(temp, _data + 3, userNameLength);
    strncpy(_struct->m_userName, temp, userNameLength+1);/*adding '\0' */
    _data = _data +3  +userNameLength;

    groupNameLength = (size_t)*(_data);
    /*printf("\ngroupname length = %lu\n", groupNameLength); TODO DELETE */
    strncpy(temp2, _data + 1, groupNameLength);
    strncpy(_struct->m_groupName, temp2, groupNameLength+1);/*adding '\0' */
}

/*ReplyCreateGroup
=============================================
|TAG=07|totalLen|IP_LEN|IP|portLen|portValue|
=============================================
*/
int ProtocolPackReplyCreateGroup(ServerGroupSendIpAndPort *_group, char *_buffer)
{
    size_t size = strlen(_group->m_ipAddress) + strlen(_group->m_port) + 2; /* 2 for value and pass length */
    *_buffer = REPLY_CREATE_GROUP;
    ++_buffer;
    *_buffer = (char)size;
    ++_buffer;
    *_buffer = (char)strlen(_group->m_ipAddress);
    ++_buffer;
    strcpy(_buffer, _group->m_ipAddress);
    _buffer = _buffer + strlen(_group->m_ipAddress);
    *_buffer = (char)strlen(_group->m_port);
    ++_buffer;
    strcpy(_buffer, _group->m_port);
    _buffer = _buffer + strlen(_group->m_port);
    *_buffer = '\0';
    return (int)size;
}

void ProtocolUnPackReplyCreateGroup(char *_data, ServerGroupSendIpAndPort *_struct)
{
    char temp [MAX_NAME] = {'\0'};
    char temp2 [MAX_NAME] = {'\0'};
    size_t portLength;
    size_t IPLength = (size_t)(*(_data + 2));
    /*printf("name length = %lu", IPLength); TODO DELETE */
    strncpy(temp, _data + 3, IPLength);
    strncpy(_struct->m_ipAddress, temp, IPLength+1);/*adding '\0' */
    
    portLength = (size_t)(*(_data + 2 + 1 + IPLength));
    strncpy(temp2, _data + 2 + 2 + IPLength, portLength);
    strncpy(_struct->m_port, temp2, portLength+1);/*adding '\0' */
}

/*JoinGroupShow
=================
|TAG=08|totalLen|
=================
*/
int ProtocolPackJoinGroupShow(char* _buffer)
{
    int size = 0;
    *_buffer = JOIN_GROUP_SHOW;
    ++_buffer;
    *_buffer = 0; 
    return size;
}

int ProtocolUnPackJoinGroupShow(char* _data)
{
    return *(_data);
}

/*ReplyJoinGroupShow
============================================================================
|TAG=09|totalLen|numOfGroups|groupName_LEN|groupName|groupName_LEN|groupName .......
============================================================================
*/
int ProtocolPackReplyJoinGroupShow(ServerJoinGroupShow *_struct, char *_buffer)
{
    char* sizeIndex;
    char* numOfGroupIndex;
    char* tempName;
    size_t size = 0,numOfGroups = 0;
    *_buffer = JOIN_GROUP_REPLY_SHOW;
    ++_buffer;
    sizeIndex = _buffer;
    ++ _buffer;
    numOfGroupIndex = _buffer;
    ++ _buffer;
    while(Vector_Remove(_struct->m_vectorNames,(void**)&tempName) == VECTOR_SUCCESS)
    {
        size = size + strlen(tempName);
        *_buffer = (char)strlen(tempName);
        ++_buffer;
        strcpy(_buffer, tempName);
        _buffer = _buffer + strlen(tempName);
        ++numOfGroups;
        free(tempName);
    }
    *numOfGroupIndex = (char)numOfGroups;
    *sizeIndex = (char)(size + numOfGroups);
    *_buffer = '\0';
    Vector_Destroy(&(_struct->m_vectorNames), NULL); 
    return (int)size;
}

 /*TODO change to int and send 1 if more then 1 packet */

void ProtocolUnPackReplyJoinGroupShow(char *_data, ServerJoinGroupShow *_struct)
{
    /* char* tempDelete; */
    /*int i_delete = 0; TODO DELETE */
    char* tempName;
    size_t numOfGroups = (size_t)(*(_data+2));
    char nameSize;
    _data = _data + 3;
   /* printf("numOfGroups = %lu\n",numOfGroups);  TODO DELETE */
    while(numOfGroups > 0)
    {
        nameSize = *_data;
        ++_data;
       /* printf("nameSize = %d\n",nameSize);  TODO DELETE */
        tempName = (char*) calloc ((size_t)nameSize+1,sizeof(char));
        strncpy(tempName,_data,(size_t)nameSize);
        _data = _data + nameSize;
        /* printf("temp = %s\n",tempName); */
        Vector_Append(_struct->m_vectorNames,tempName);
        /*Vector_Get(_struct->m_vectorNames,i_delete++,(void**)&tempDelete);  TODO DELETE */
       /*  printf("tempDelete = %s\n",tempDelete); */
        --numOfGroups;
    }
}

/*JOIN SELECTED
=================================================
|TAG=10|TL|L_User|UserName|gNameLength|groupName|
=================================================
*/

int ProtocolPackJoinSelect(ClientJoinSelected *_group, char *_buffer)
{
    size_t size = strlen(_group->m_groupName) + strlen(_group->m_userName) +2;
    *_buffer = JOIN_GROUP_SELECTED;
    ++_buffer;
    *_buffer = (char)size;
    ++_buffer;
    
    *_buffer = (char)strlen(_group->m_userName);
    ++_buffer;
    strcpy(_buffer, _group->m_userName);
    _buffer = _buffer + strlen(_group->m_userName);

    *_buffer = (char)strlen(_group->m_groupName);
    ++_buffer;
    strcpy(_buffer, _group->m_groupName);
    _buffer = _buffer + strlen(_group->m_groupName);
    
    *_buffer = '\0';
    return (int)size;
}

void ProtocolUnPackJoinSelect(char *_data, ClientJoinSelected *_struct)
{
    char temp [MAX_NAME]={'\0'};
    char temp2 [MAX_NAME]={'\0'};
    size_t groupNameLength;
    size_t userNameLength = (size_t)(*(_data + 2));
   /* printf("userNameLength = %lu", userNameLength); TODO DELETE */
    strncpy(temp, _data + 3, userNameLength);
    strncpy(_struct->m_userName, temp, userNameLength+1);/*adding '\0' */
    _data = _data +3  +userNameLength;

    groupNameLength = (size_t)*(_data);
    /*printf("\ngroupname length = %lu\n", groupNameLength); TODO DELETE */
    strncpy(temp2, _data + 1, groupNameLength);
    strncpy(_struct->m_groupName, temp2, groupNameLength+1);/*adding '\0' */
}

/*JOIN_GROUP_IP
=================================================
|TAG=11|totalLen|IP_LEN|IP|portLen|portValue|
=================================================
*/
int ProtocolPackReplyJoinGroupByIP(ServerGroupSendIpAndPort *_group, char *_buffer)
{
    size_t size = strlen(_group->m_ipAddress) + strlen(_group->m_port) + 2; /* 2 for value and pass length */
    *_buffer = JOIN_GROUP_IP; 
    ++_buffer;
    *_buffer = (char)size;
    ++_buffer;
    *_buffer = (char)strlen(_group->m_ipAddress);
    ++_buffer;
    strcpy(_buffer, _group->m_ipAddress);
    _buffer = _buffer + strlen(_group->m_ipAddress);
    *_buffer = (char)strlen(_group->m_port);
    ++_buffer;
    strcpy(_buffer, _group->m_port);
    _buffer = _buffer + strlen(_group->m_port);
    *_buffer = '\0';
    return (int)size;
}

void ProtocolUnPackReplyJoinGroupByIP(char *_data, ServerGroupSendIpAndPort *_struct)
{
    char temp [MAX_NAME] = {'\0'};
    char temp2 [MAX_NAME] = {'\0'};
    size_t portLength;
    size_t IPLength = (size_t)(*(_data + 2));
    /*printf("name length = %lu", IPLength); TODO DELETE */
    strncpy(temp, _data + 3, IPLength);
    strncpy(_struct->m_ipAddress, temp, IPLength+1);/*adding '\0' */
    
    portLength = (size_t)(*(_data + 2 + 1 + IPLength));
    strncpy(temp2, _data + 2 + 2 + IPLength, portLength);
    strncpy(_struct->m_port, temp2, portLength+1);/*adding '\0' */
}

/*LEAVE GROUP
=================================================
|TAG=12|TL|L_User|UserName|gNameLength|groupName|
=================================================
*/
int ProtocolPackLeaveGroup(ClientLeaveGroup *_group, char *_buffer)
{
    size_t size = strlen(_group->m_groupName) + strlen(_group->m_userName) +2;
    *_buffer = LEAVE_GROUP;
    ++_buffer;
    *_buffer = (char)size;
    ++_buffer;
    
    *_buffer = (char)strlen(_group->m_userName);
    ++_buffer;
    strcpy(_buffer, _group->m_userName);
    _buffer = _buffer + strlen(_group->m_userName);

    *_buffer = (char)strlen(_group->m_groupName);
    ++_buffer;
    strcpy(_buffer, _group->m_groupName);
    _buffer = _buffer + strlen(_group->m_groupName);
    
    *_buffer = '\0';
    return (int)size;
}

void ProtocolUnPackLeaveGroup(char *_data, ClientLeaveGroup *_struct)
{
    char temp [MAX_NAME]={'\0'};
    char temp2 [MAX_NAME]={'\0'};
    size_t groupNameLength;
    size_t userNameLength = (size_t)(*(_data + 2));
   /* printf("userNameLength = %lu", userNameLength); TODO DELETE */
    strncpy(temp, _data + 3, userNameLength);
    strncpy(_struct->m_userName, temp, userNameLength+1);/*adding '\0' */
    _data = _data +3  +userNameLength;

    groupNameLength = (size_t)*(_data);
    /*printf("\ngroupname length = %lu\n", groupNameLength); TODO DELETE */
    strncpy(temp2, _data + 1, groupNameLength);
    strncpy(_struct->m_groupName, temp2, groupNameLength+1);/*adding '\0' */
}

/*LEAVE_GROUP_REPLY
=================================================
|TAG=13|T_L=1|okOrFail|
=================================================
*/
int ProtocolPackLeaveGroupReply(int _kOrFail, char* _buffer)
{
    int size = 1;
    *_buffer++ = LEAVE_GROUP_REPLY;
    *_buffer++ = (char)size; 
    *_buffer++ = (char)_kOrFail;

    return size;
}

int ProtocolUnPackLeaveGroupReply(char* _data)
{
    return *(_data+2);
}

/*REGREPLY
==============================
|TAG=014|T_L=1|okOrFail|
================================
*/
int ProtocolPackLogOutReply(int _kOrFail, char* _buffer)
{
    int size = 1;
    *_buffer++ = LOG_OUT_REPLY;
    *_buffer++ = (char)size; 
    *_buffer++ = (char)_kOrFail;

    return size;
}

int ProtocolUnPackLogOutReply(char* _data)
{
    return *(_data+2);
}


