#ifndef LUMINES_PLAYER_HAND_H
#define LUMINES_PLAYER_HAND_H

#include <tnl.h>
#include <tnlNetObject.h>

class NetLuHand : public NetObject
{
public:
      typedef NetObject Parent;
      TNL_DECLARE_CLASS(NetLuHand);

      enum States {
          PosMask = BIT(0),
          RotMask = BIT(1),
      };

};



#endif//LUMINES_PLAYER_HAND_H
