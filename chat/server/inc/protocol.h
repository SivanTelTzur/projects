#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/** 
 * @brief Full chat protocol.
 * @author SOS
 */

#include "vector.h"
#include <stddef.h> /* size_t */

/*CHAT MESSAGE STRUCTURE 
==========================
|USER_NAME|" : "|"MESSAGE|
==========================
*/

#define MAX_NAME 20
#define MAX_BUFFER 255
#define MAX_PASS 10
#define LEN_PASS_NAME 30
#define OK 1
#define FAIL 0

typedef enum
{
    REGISTRATION = 1,
    REG_REPLY,/* 2 */
    LOGIN, /* 3 */
    LOGIN_REPLY, 
    LOG_OUT,
    CREATE_GROUP, /* 6 */
    REPLY_CREATE_GROUP,
    JOIN_GROUP_SHOW, /* 8 */
    JOIN_GROUP_REPLY_SHOW,
    JOIN_GROUP_SELECTED, /* 10 */
    JOIN_GROUP_IP,
    LEAVE_GROUP, /* 12 */
    LEAVE_GROUP_REPLY,
    LOG_OUT_REPLY /* 14 */
} Tag_Action;

typedef struct ClientLoginReq ClientLOginReq;
typedef struct ClientLogOut ClientLogOut;
typedef struct ServerGroupSendIpAndPort ServerGroupSendIpAndPort;
typedef struct ClientCreateGruop ClientCreateGruop;
typedef struct ClientJoinGroup ClientJoinGroup;
typedef struct ServerShowGroups ServerShowGroups;
typedef struct ServerJoinGroupShow ServerJoinGroupShow;
typedef struct ServerJoinByIp ServerJoinByIp;
typedef struct LeaveGroupClient LeaveGroupClient;
typedef struct ClientLeaveGroup ClientLeaveGroup;
typedef struct ClientJoinSelected ClientJoinSelected;
typedef struct ClientNameAndPass ClientNameAndPass;

struct ClientLeaveGroup
{
    char m_userName[MAX_NAME];
    char m_groupName[MAX_NAME];
};

struct ClientJoinSelected
{
    char m_userName[MAX_NAME];
    char m_groupName[MAX_NAME];
};

struct ClientNameAndPass
{
    char m_userName[MAX_NAME];
    char m_userPass[MAX_PASS];
};

struct ClientCreateGruop
{
    char m_userName[MAX_NAME];
    char m_groupName[MAX_NAME];
};

struct ServerJoinGroupShow
{
    Vector *m_vectorNames;
};

struct ServerGroupSendIpAndPort
{
    char m_ipAddress[MAX_NAME];
    char m_port[MAX_NAME];
};

struct ClientLogOut
{
    char m_userName[MAX_NAME];
};

struct ClientJoinGroup
{
    char m_groupName[MAX_NAME];
};

struct ServerShowGroups
{
    char m_groupName[MAX_NAME];
};

struct ServerJoinByIp
{
    char m_groupName[MAX_NAME];
};

struct LeaveGroupClient
{
    char m_groupName[MAX_NAME];
};

/**  
 * @brief Return TAG_ACTION
 * @param[in] char* according to protocol message structure
 * @return Tag action by int
 * @warning not checking for NULL.
 */
int ProtocolGetTag(char *_data);

/**  
 * @brief Transfer structure into char message
 * @param[in] char*
 * @param[in] structure with userName and password.
 * @return Total len
 * @warning not checking for NULL.
 * @warning Encrtypt the data using ^.
    ================================
    |TAG=01|T_L|L|USER_N|L|PASSWORD|
    ================================
 */
int ProtocolPackRegistrate(ClientNameAndPass *_user, char *_buffer);
/*Transfer opposite direction*/
void ProtocolUnPackRegistrate(char *_data, ClientNameAndPass *_struct);

/**  
 * @brief Transfer OK message into char message
 * @param[in] char*
 * @param[in] "BOOL" int .
 * @return len=1.
 * @warning not checking for NULL.
    ==============================
    |TAG=02|T_L=1|okOrFail|
    =================================
 */

int ProtocolPackRegReply(int _kOrFail, char *_buffer);
/*return OK or FAIL*/
int ProtocolUnPackRegReply(char *_data);

/**  
 * @brief Transfer structure into char message
 * @param[in] char*
 * @param[in] structure with userName and password.
 * @return Total len
 * @warning not checking for NULL.
 * @warning Encrypted the data using ^.
================================
|TAG=03|T_L|L|USER_N|L|PASSWORD|
================================
 */
int ProtocolPackLogin(ClientNameAndPass *_user, char *_buffer);
/*Transfer opposite direction*/
void ProtocolUnPackLogin(char *_data, ClientNameAndPass *_struct);

/**  
 * @brief Transfer OK message into char message
 * @param[in] char*
 * @param[in] "BOOL" int .
 * @return len=1.
 * @warning not checking for NULL.
    =======================
    |TAG=04|T_L=1|okOrFail|
    =======================
 */
int ProtocolPackLoginReply(int _kOrFail, char *_buffer);
/*return OK or FAIL*/
int ProtocolUnPackLoginReply(char *_data);

/**  
 * @brief Transfer structure into char message
 * @param[in] char*
 * @param[in] structure with userName.
 * @return Total len of package
 * @warning not checking for NULL.
===================
|TAG=05|TL|USERNAME|
===================
*/
int ProtocolPackLogOut(ClientLogOut *_struct, char *_buffer);
/*Transfer opposite direction*/
void ProtocolUnPackLogOut(char *_data, ClientLogOut *_struct);

/**  
 * @brief Transfer structure into char message
 * @param[in] char*
 * @param[in] structure with userName and group name.
 * @return Total len of package
 * @warning not checking for NULL.
=================================================
|TAG=06|TL|L_User|UserName|gNameLength|groupName|
=================================================
*/
int ProtocolPackCreateGroup(ClientCreateGruop *_group, char *_buffer);
/*Transfer opposite direction*/
void ProtocolUnPackCreateGroup(char *_data, ClientCreateGruop *_struct);

/*
 * @brief Transfer structure into char message
 * @param[in] char*
 * @param[in] structure with IP and portValue.
 * @return Total len of package
 * @warning not checking for NULL.
=============================================
|TAG=07|totalLen|IP_LEN|IP|portLen|portValue|
=============================================
*/
int ProtocolPackReplyCreateGroup(ServerGroupSendIpAndPort *_group, char *_buffer);
/*Transfer opposite direction*/
void ProtocolUnPackReplyCreateGroup(char *_data, ServerGroupSendIpAndPort *_struct);

/*
* @brief insert tag = 8 and len = 0 into char message
 * @param[in] char*
 * @return Total len of package
 * @warning not checking for NULL.
=================
|TAG=08|totalLen|
=================
*/
int ProtocolPackJoinGroupShow(char *_buffer);
/*return int tag = 8*/
int ProtocolUnPackJoinGroupShow(char *_data);

/*
 * @brief Transfer vector into char message with all the groups names.
 *        free all vector elements and destroy the vector.
 * @param[in] char*
 * @param[in] structure with vector full of groups name.
 * @return Total len of package
 * @warning not checking for NULL.
 * @warning elements must be char *
============================================================================
|TAG=09|totalLen|numOfGroups|groupName_LEN|groupName|groupName_LEN|groupName .......
============================================================================
*/
int ProtocolPackReplyJoinGroupShow(ServerJoinGroupShow *_struct, char *_buffer);

/*
 * @brief Transfer char messag into vector with all the groups names.
 *        create malloc for each element.
 * @param[in] char* with group names
 * @param[in] structure with vector for inserting the names.
 * @warning user need to take care of the vector memory.
*/
void ProtocolUnPackReplyJoinGroupShow(char *_data, ServerJoinGroupShow *_struct);

/*JOIN SELECTED  
 * @brief Transfer structure into char message
 * @param[in] char*
 * @param[in] structure with userName and group name.
 * @return Total len of package
 * @warning not checking for NULL.
=================================================
|TAG=10|TL|L_User|UserName|gNameLength|groupName|
=================================================
*/
int ProtocolPackJoinSelect(ClientJoinSelected *_group, char *_buffer);
/*Transfer opposite direction*/
void ProtocolUnPackJoinSelect(char *_data, ClientJoinSelected *_struct);

/*JOIN_GROUP_IP
=================================================
|TAG=11|totalLen|IP_LEN|IP|portLen|portValue|
=================================================
*/
int ProtocolPackReplyJoinGroupByIP(ServerGroupSendIpAndPort *_group, char *_buffer);
void ProtocolUnPackReplyJoinGroupByIP(char *_data, ServerGroupSendIpAndPort *_struct);

/*LEAVE GROUP
 * @brief Transfer structure into char message
 * @param[in] char*
 * @param[in] structure with userName and group name.
 * @return Total len of package
 * @warning not checking for NULL.
=================================================
|TAG=12|TL|L_User|UserName|gNameLength|groupName|
=================================================
*/
int ProtocolPackLeaveGroup(ClientLeaveGroup *_group, char *_buffer);
/*Transfer opposite direction*/
void ProtocolUnPackLeaveGroup(char *_data, ClientLeaveGroup *_struct);

/*LEAVE_GROUP_REPLY
=================================================
|TAG=13|T_L=1|okOrFail|
=================================================
*/
int ProtocolPackLeaveGroupReply(int _kOrFail, char *_buffer);
int ProtocolUnPackLeaveGroupReply(char *_data);

/*LOG_OUT_REPLY
* @brief Transfer OK message into char message
 * @param[in] char*
 * @param[in] "BOOL" int .
 * @return len=1.
 * @warning not checking for NULL.
=================================================
|TAG=14|T_L=1|okOrFail|
=================================================
*/
int ProtocolPackLogOutReply(int _kOrFail, char *_buffer);
/*return OK or FAIL*/
int ProtocolUnPackLogOutReply(char *_data);

#endif /*__PROTOCOL_H__*/
