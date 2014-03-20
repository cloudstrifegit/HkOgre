#include "XkServer.h"
#include <Poco/Bugcheck.h>
#include <MessageIdentifiers.h>
#include "XkDebugStr.h"
#include "XkTimer.h"

namespace Xk {

Server::Server(MsgCallback* pCallback) : m_pCallback(pCallback), m_packet(NULL) ,
    m_bRunning(true)
{
    m_peer = RakNet::RakPeerInterface::GetInstance();
    poco_assert(m_peer != NULL);
}

Server::~Server()
{
    Stop();
    m_peer->Shutdown(true);
    RakNet::RakPeerInterface::DestroyInstance(m_peer);
    m_peer = NULL;
}

void Server::SetName(const std::string& strName)
{
    m_strName = strName;
    m_thread.setName(m_strName);
}

bool Server::Start(const char* szAddress, const char* szPasswd, int nPort, int nMaxCount)
{
    RakNet::SocketDescriptor des;
    des.port = nPort;

    RakNet::StartupResult result = m_peer->Startup(nMaxCount,
        &des,
        1);

    if(result != RakNet::RAKNET_STARTED) {
        return false;
    }

    m_peer->SetMaximumIncomingConnections(nMaxCount);
    m_peer->SetIncomingPassword(szPasswd, strlen(szPasswd));

    m_thread.start(*this);

    m_strAddress = szAddress;
    m_strPasswd = szPasswd;
    m_nPort = nPort;
    m_nMaxCount = nMaxCount;

    return true;
}

bool Server::Disconnect(XK_GUID guid)
{
    RakNet::RakNetGUID rakGuid;
    rakGuid.g = guid;

    RakNet::SystemAddress addr =  m_peer->GetSystemAddressFromGuid(rakGuid);
    m_peer->CloseConnection(addr, true, 0, HIGH_PRIORITY);

    return true;
}

bool Server::IsRunning()
{
    return m_thread.isRunning();
}

void Server::Stop()
{
    m_bRunning = false;

    m_thread.join();
}

bool Server::Send(XK_GUID guid, int nID, const void* pBuf, unsigned int nLength)
{
    RakNet::RakNetGUID rakGuid;
    rakGuid.g = guid;

    RakNet::SystemAddress addr = m_peer->GetSystemAddressFromGuid(rakGuid);
    bool bBroadcast = true;
    if(addr != RakNet::UNASSIGNED_SYSTEM_ADDRESS) bBroadcast = false;

    char* pTmpBuf = (char*)alloca(nLength + 1);
    pTmpBuf[0] = static_cast<unsigned char>(nID);
    memcpy(&pTmpBuf[1], pBuf, nLength);

    uint32_t nRet= m_peer->Send(pTmpBuf,
        (const int)nLength + 1,
        HIGH_PRIORITY,
        RELIABLE_ORDERED,
        0,
        addr,
        bBroadcast);

    return nRet != 0;
}

bool Server::GetMsg(MSG_PACKET& packet)
{
    m_packet = m_peer->Receive();
    if(m_packet == NULL) return false;

    packet.nID = static_cast<int>(m_packet->data[0]);
    packet.pBuf = &m_packet->data[1];
    packet.nLength = m_packet->length - 1;
    packet.guid = m_packet->guid.g;

    return true;
}

void Server::ReleaseMsg()
{
    if(m_packet) {
        m_peer->DeallocatePacket(m_packet);
        m_packet = NULL;
    }
}

void Server::run()
{
    DBGSTRING("Xk::Server::run [%s] begin", m_strName.c_str());
    while(m_bRunning) {
        MSG_PACKET pack;
        if(!GetMsg(pack) || m_pCallback == NULL) {
            Poco::Thread::sleep(1);
            continue;
        }
        switch(pack.nID){
        case ID_NEW_INCOMING_CONNECTION:
            m_pCallback->OnNewConnection(pack.guid);
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            m_pCallback->OnDisconnect(pack.guid);
            break;
        case ID_CONNECTION_LOST:
            m_pCallback->OnLostConnection(pack.guid);
            break;
        default:
            m_pCallback->OnMsg(pack.guid, pack.nID, pack.pBuf, pack.nLength);
        }

        ReleaseMsg();
    }
    DBGSTRING("Xk::Server::run [%s] end", m_strName.c_str());
}

}//namespace Xk