#include "XkClient.h"
#include <Poco/Bugcheck.h>
#include <MessageIdentifiers.h>
#include "XkDebugStr.h"
#include "XkTimer.h"

namespace Xk {//namespace Xk begin

Client::Client(MsgCallback* pCallback) : m_pCallback(pCallback)
{
    m_peer = RakNet::RakPeerInterface::GetInstance();
    poco_assert(m_peer != NULL);

    RakNet::SocketDescriptor des(0, 0);
    des.socketFamily = AF_INET;
    RakNet::StartupResult result = m_peer->Startup(2, &des, 1);
    poco_assert(result == RakNet::RAKNET_STARTED);
}

Client::~Client()
{
    DBG_BEGIN_END0();
    Disconnect();
    Stop();
    m_peer->Shutdown(100);
    RakNet::RakPeerInterface::DestroyInstance(m_peer);
    m_peer = NULL;
}

bool Client::Connect(const char* szAddress,
        const char* szPasswd,
        int nPort,
        int nTimeout)
{
    using namespace RakNet;
    RakNet::ConnectionAttemptResult result = m_peer->Connect(szAddress,
        nPort, szPasswd, strlen(szPasswd) );
    if(result != RakNet::CONNECTION_ATTEMPT_STARTED) {
        return false;
    }

    Timer timer;
    MSG_PACKET msg;
    while(!timer.IsUp(nTimeout)) {
        if(GetMsg(msg)) {
            ReleaseMsg();

            switch(msg.nID){
            case ID_CONNECTION_ATTEMPT_FAILED:
                return false;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                m_strAddress = szAddress;
                m_strPasswd = szPasswd;
                m_nPort = nPort;
                return true;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                return false;
            }
        }else{
            Poco::Thread::sleep(50);
        }
    }

    return false;
}

bool Client::Connect(int nTimeout)
{
    return Connect(m_strAddress.c_str(),
        m_strPasswd.c_str(),
        m_nPort,
        nTimeout);
}

void Client::SetName(const std::string& strName)
{
    m_strName = strName;
    m_thread.setName(m_strName);
}

void Client::Disconnect()
{
    if(m_peer)
        m_peer->CloseConnection(m_peer->GetSystemAddressFromIndex(0), true);
}

bool Client::IsRunning()
{
    return m_thread.isRunning();
}

bool Client::IsConnected()
{
    RakNet::SystemAddress addr(m_strAddress.c_str(), m_nPort);

    RakNet::ConnectionState state = m_peer->GetConnectionState(addr);
    
    return state == RakNet::IS_CONNECTED;
}

void Client::Stop()
{
    DBG_BEGIN_END0();
    m_bRunning = false;
    Poco::Thread::sleep(200);
    if(m_thread.isRunning()) {
        try{
            m_thread.join(2000);
        }
        catch(Poco::TimeoutException&)
        {
            XkDebugString("Client::Stop wait thread quit failed!");
        }
    }
}

bool Client::Start()
{
    m_bRunning = true;
    m_thread.start(*this);

    return true;
}

bool Client::Send(int nID, const void* pBuf, unsigned int nLength)
{
    RakNet::SystemAddress sysaddr;
    char* pTmpBuf = (char*)alloca(nLength + 1);
    pTmpBuf[0] = static_cast<unsigned char>(nID);
    memcpy(&pTmpBuf[1], pBuf, nLength);

    uint32_t nRet= m_peer->Send(pTmpBuf,
        (const int)nLength+1,
        HIGH_PRIORITY,
        RELIABLE_ORDERED,
        0,
        sysaddr,
        true);

    return nRet != 0;
}

bool Client::GetMsg(MSG_PACKET& packet)
{
    m_packet = m_peer->Receive();
    if(m_packet == NULL) return false;

    packet.nID = static_cast<int>(m_packet->data[0]);
    packet.pBuf = &m_packet->data[1];
    packet.nLength = m_packet->length - 1;
    packet.guid = m_packet->guid.g;

    return true;
}

void Client::ReleaseMsg()
{
    if(m_packet) {
        m_peer->DeallocatePacket(m_packet);
        m_packet = NULL;
    }
}

void Client::run()
{
    DBGSTRING("Xk::Client::run [%s] begin", m_strName.c_str());

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
            m_bRunning = false;
            break;
        case ID_CONNECTION_LOST:
            m_pCallback->OnLostConnection(pack.guid);
            m_bRunning = false;
            break;
        default:
            m_pCallback->OnMsg(pack.guid, pack.nID, pack.pBuf, pack.nLength);
        }

        ReleaseMsg();
    }

    DBGSTRING("Xk::Client::run [%s] end", m_strName.c_str());
}

}//namespace Xk
