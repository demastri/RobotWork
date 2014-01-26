#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include "RoutingDeviceIDs.h"
#include "CommandRouter.h"

class SimpleObject {
public:
  static const int DEVICE_ID = SIMPLE_OBJECT_DEVICE_ID;

  static const int COMMAND_ID_DO_SOMETHING       = 0x01;
  static const int COMMAND_ID_DO_SOMETHING_ELSE  = 0x02;

  SimpleObject();
  void setup(int thisID, CommandRouter *router);
  void teardown(CommandRouter *router);


private:
  int instanceID;

  void setupCommandListener( CommandRouter &router );
  void teardownCommandListener( CommandRouter &router );

  CMD_METHOD_DEFINE(processCommand); // catchall method...

  CMD_METHOD_DEFINE(doSomething);
  CMD_METHOD_DEFINE(doSomethingElse);
};

#endif
