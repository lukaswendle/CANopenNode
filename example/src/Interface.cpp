#include "CAN_Write.h"

#ifdef __cplusplus
extern "C" {
#endif

   void Send(struct_s_Message* sMessage)
   {
      
      CAN_WriteMsg::Write WriteObj{};
       WriteObj.WriteMessage(sMessage);
   }


}