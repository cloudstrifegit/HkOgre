#include "XkMessageCodec.h"
#include <Poco/Foundation.h>

#ifdef linux
#include <arpa/inet.h> //ntohl,htonl
#include <stdlib.h> //memcpy
#else
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

namespace Xk {

const int MsgHeaderLen = sizeof(Poco::Int32);

google::protobuf::Message* MessageCodec::createMessage(const std::string& type_name)
{
    google::protobuf::Message* message = NULL;
    const google::protobuf::Descriptor* descriptor = 
        google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
    if(descriptor) {
        const google::protobuf::Message* prototype = 
            google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
        if(prototype) {
            message = prototype->New();
        }
    }

    return message;
}

std::string MessageCodec::encode(const google::protobuf::Message& message)
{
    std::string strResult;
    strResult.resize(MsgHeaderLen);

    const std::string& strTypeName = message.GetTypeName();
    Poco::Int32 nNameLen = static_cast<Poco::Int32>(strTypeName.size() + 1);
    Poco::Int32 be32 = ::htonl(nNameLen);
    strResult.append(reinterpret_cast<char*>(&be32), sizeof be32);
    strResult.append(strTypeName.c_str(), nNameLen);

    bool bSuccessed = message.AppendToString(&strResult);

    if(bSuccessed) {
        const char* begin = strResult.c_str() + MsgHeaderLen;
        Poco::Int32 len = ::htonl(strResult.size() - MsgHeaderLen);
        std::copy(reinterpret_cast<char*>(&len),
            reinterpret_cast<char*>(&len) + sizeof len,
            strResult.begin());
    }else {
        strResult.clear();
    }

    return strResult;
}

namespace {
    Poco::Int32 asInt32(const char* buf)
    {
        Poco::Int32 be32 = 0;
        ::memcpy(&be32, buf, sizeof(be32));
        return ::ntohl(be32);
    }
}

google::protobuf::Message* MessageCodec::decode(const std::string& buf)
{
    google::protobuf::Message* result = NULL;
    Poco::Int32 len = static_cast<Poco::Int32>(buf.size());

    if(len > 10) {
        Poco::Int32 nameLen = asInt32(buf.c_str() + MsgHeaderLen);
        if(nameLen >= 2 && nameLen <= len - 2*MsgHeaderLen) {
            std::string typeName(buf.begin() + 2*MsgHeaderLen, buf.begin() + 
                2*MsgHeaderLen + nameLen - 1);
            google::protobuf::Message* message = createMessage(typeName);

            if(message) {
                const char* data = buf.c_str() + 2*MsgHeaderLen + nameLen;
                Poco::Int32 dataLen = len - nameLen - 2*MsgHeaderLen;
                if(message->ParseFromArray(data, dataLen)) {
                    result = message;
                }else {
                    //parse error
                    delete message;
                }
            }else {
                //createMessage error
            }
        }else {
            //nameLen error
        }
    }

    return result;
}

std::string MessageCodec::getTypeName(void* buf, unsigned int size)
{
    if(size <= 0) return "";

    std::string strRet;

    Poco::Int32 nameLen = asInt32((char*)buf + MsgHeaderLen);
    if(nameLen >= 2 && nameLen <= size - 2*MsgHeaderLen) {
        std::string typeName((char*)buf + 2*MsgHeaderLen, (char*)buf + 
                2*MsgHeaderLen + nameLen - 1);

        strRet = typeName;
    }

    return strRet;
}

}//namespace Xk