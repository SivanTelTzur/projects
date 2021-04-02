#ifndef __CLIENTMNG_H__
#define __CLIENTMNG_H__

typedef struct ClientMng ClientMng;

typedef enum WelcomeMenuClient
{
    LOGINN = 2,
    EXIT
} WelcomeMenuClient;

typedef enum SecondClient
{
    CREATEGROUP =1,
    JOINGROUP,
    LEAVEGROUP,
    LOGOUT
} SecondClient;

typedef enum ClientMng_Result
{
    CLIENT_MGN_OK,
    CLIENT_MGN_CONNECTION_FAILED,
    CLIENT_MGN_NOT_INITIALIZED,
    CLIENT_MGN_ALLOCATION_FAILED
} ClientMng_Result;

/**  
 * @brief Creates client meneger , with vector and socket ready to use. e 
 * @return ClientMng* pointer - on success / NULL on fail 
 *
 */
ClientMng *ClientMngCreate();

/**  
 * @brief Runs the client until he choose to logout. 
 * @param[in] pointer to ClientMng
 * @return CLIENT_MGN_OK on success logout.
 *
 * @warning if _blockSize is 0 the vector will be of fixed size. 
 * @warning 
 */
ClientMng_Result ClientMngRun(ClientMng *_clientMng);

/**  
 * @brief destroy client meneger , the vector and the items inside of it.  
 * @param[in] pointer to ClientMng.
 * @return void.
 * */
void ClientMngDestroy(ClientMng **_clientMng);

#endif