#ifndef HKOGRE_TIMERLISTENER_H
#define HKOGRE_TIMERLISTENER_H

namespace HkOgre
{

class TimeListener
{
public:
    virtual void advance(float fTime) = 0;
};

}//namespace HkOgre

#endif