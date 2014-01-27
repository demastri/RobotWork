#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include "../../RobotSketches/godzuki/gRoutingDeviceIDs.h"
#include "../../RobotSketches/godzuki/gCommandRouter.h"
#include "../../RobotSketches/godzuki/gComms.h"

class SimpleObject {
public:
  static const int DEVICE_ID = SIMPLE_OBJECT_DEVICE_ID;

  static const int COMMAND_ID_DO_SOMETHING       = 0x01;
  static const int COMMAND_ID_DO_SOMETHING_ELSE  = 0x02;

  SimpleObject();
  void setup(int thisID, gCommandRouter *router);
  void teardown(gCommandRouter *router);


private:
  int instanceID;

  void setupCommandListener( gCommandRouter &router );
  void teardownCommandListener( gCommandRouter &router );

  CMD_METHOD_DEFINE(processCommand); // catchall method...

  CMD_METHOD_DEFINE(doSomething);
  CMD_METHOD_DEFINE(doSomethingElse);
};

#endif
