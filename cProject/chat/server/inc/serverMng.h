#ifndef __SERVER_MNG_H__
#define __SERVER_MNG_H__
#include <stddef.h>

typedef struct ServerMng  ServerMng;

typedef enum ServerMng_Result
{
    SERVER_MGN_OK,
    SERVER_MGN_NOT_INITIALIZED,
    SERVER_MGN_ALLOCATION_FAILED
} ServerMng_Result;


/**
 * @brief Create a new ServerMng.
 * @param[in] _maxClients for hash map of users and groups.
 * @return newly created ServerMng or NULL on failure.
 */
ServerMng* ServerMngCreate(size_t  _maxClients);


/**
 * @brief Runs the server_net function.
 * user is automaticaly join group
 * @param[in] _serverMng - server manager .
 * @return Success indicator
 * @retval  SERVER_MGN_OK	on success

 */
ServerMng_Result ServerMngRun(ServerMng* _serverMng);


/**
 * @brief destroy ServerMng and set *ServerMng to NULL - destroy all existed Groups , users and the server sockets.
 * @param[in] _groupMng : ServerMng to be destroyed
 */
void ServerMngDestroy(ServerMng** _serverMng);


#endif /*  __SERVER_MNG_H__ */