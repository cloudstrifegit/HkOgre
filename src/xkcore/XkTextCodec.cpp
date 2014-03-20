#include "XkTextCodec.h"
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#ifdef WIN32
#include <Windows.h>
#endif

namespace Xk {

int TextCodec::asciiToUnicode(const char* szInput, wchar_t* wszOut, int nOutSize)
{
#ifdef WIN32
    //int nLen = ::MultiByteToWideChar(CP_ACP, 0, szInput, -1, 0, 0);
    int nLen = ::MultiByteToWideChar(CP_ACP, 0, szInput, strlen(szInput), wszOut, nOutSize);
    return nLen;
#else
    char* locname = NULL;
    locname = setlocale(LC_ALL, LC_NAME_zh_CN_GBK);
    if(NULL == locname)
        printf("setlocale with %s failed!\n", LC_NAME_zh_CN_GBK);

    size_t nLen = mbstowcs(wszOut, szInput, strlen(szInput));
    return nLen;
#endif
}

int TextCodec::utf8ToUnicode(const char* szInput, wchar_t* wszOut, int nOutSize)
{
#ifndef WIN32
    char* locname = NULL;
    locname = setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);
    if(NULL == locname)
        printf("setlocale with %s failed!\n", LC_NAME_zh_CN_GBK);

    size_t nLen = wcstombs(wszOut, wszInput, wcslen(wszInput));
    return nLen;
#else
    int nLen = ::MultiByteToWideChar(CP_UTF8, 
        0,
        szInput,
        -1,
        (LPWSTR)wszOut,
        nOutSize);
    
    return nLen;
#endif
}

int TextCodec::unicodeToAscii(const wchar_t* wszInput, char* szOut, int nOutSize)
{
#ifdef WIN32
    int nLen = ::WideCharToMultiByte(CP_ACP,
        0,
        wszInput,
        wcslen(wszInput),
        szOut,
        nOutSize,
        NULL, NULL);
    return nLen;
#else

#endif
}

int TextCodec::unicodeToUft8(const wchar_t* wszInput, char* szOut, int nOutSize)
{
#ifdef WIN32
    int nLen = ::WideCharToMultiByte(CP_UTF8, 
        0,
        wszInput,
        wcslen(wszInput),
        szOut,
        nOutSize,
        NULL, NULL);
    return nLen;
#else

#endif
}

int TextCodec::utf8ToAscii(const char* szInput, char* szOut, int nOutSize)
{
#ifdef WIN32
    int nNeedLen = ::MultiByteToWideChar(CP_UTF8, 0, szInput, -1, NULL, NULL);
    wchar_t* wszBuf = (wchar_t*)alloca(nNeedLen*sizeof(wchar_t));
    memset(wszBuf, 0, sizeof(wchar_t)*nNeedLen);
    utf8ToUnicode(szInput, wszBuf, nNeedLen);

    //ÔÙ×ª»»Unicodeµ½Ascii
    int nLen = unicodeToAscii(wszBuf, szOut, nOutSize);
    return nLen;
#else

#endif
}

int TextCodec::utf8ToAscii(std::string& strInput)
{
#ifdef WIN32
    std::string strTmp;
    int nNeedLen = ::MultiByteToWideChar(CP_UTF8, 0, strInput.c_str(), -1, NULL, NULL);
    wchar_t* wszBuf = (wchar_t*)alloca(nNeedLen*sizeof(wchar_t));
    memset(wszBuf, 0, sizeof(wchar_t)*nNeedLen);

    int nLen = utf8ToUnicode(strInput.c_str(), wszBuf, nNeedLen);
    
    nNeedLen = ::WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, NULL, 0, NULL, NULL);
    printf("nNeedLen: %d\n", nNeedLen);
    strTmp.resize(nNeedLen-1);

    nLen = unicodeToAscii(wszBuf, (char*)strTmp.data(), nNeedLen-1);
    
    strInput = strTmp;
    return nLen;
#endif
}

int TextCodec::asciiToUft8(const char* szInput, char* szOut, int nOutSize)
{
#ifdef WIN32
    int nNeedLen = ::MultiByteToWideChar(CP_ACP, 0, szInput, -1, 0, 0);
    wchar_t* wszBuf = (wchar_t*)alloca(nNeedLen * sizeof(wchar_t));
    memset(wszBuf, 0, sizeof(wchar_t) * nNeedLen);

    int nLen = asciiToUnicode(szInput, wszBuf, nNeedLen);
    return unicodeToUft8(wszBuf, szOut, nOutSize);
#else

#endif
}

int TextCodec::asciiToUft8(std::string& strInput)
{
#ifdef WIN32
    int nNeedLen = ::MultiByteToWideChar(CP_ACP, 0, strInput.c_str(), -1, 0, 0);
    wchar_t* wszBuf = (wchar_t*)alloca(nNeedLen * sizeof(wchar_t));
    memset(wszBuf, 0, sizeof(wchar_t) * nNeedLen);

    int nLen = asciiToUnicode(strInput.c_str(), wszBuf, nNeedLen);

    nNeedLen = ::WideCharToMultiByte(CP_UTF8, 0, wszBuf, -1, 0, 0, 0, 0);
    strInput.resize(nNeedLen-1);

    nLen = unicodeToUft8(wszBuf, (char*)strInput.data(), nNeedLen -1);

    return nLen;

#else

#endif
}

}//namespace Xk