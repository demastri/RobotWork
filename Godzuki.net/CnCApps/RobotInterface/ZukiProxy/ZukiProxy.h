// ZukiProxy.h

#pragma once

#include "..\..\..\RobotSketches\MainApplications\godzuki\gCommandRouter.h"
#include "..\..\..\RobotSketches\MainApplications\godzuki\gComms.h"

using namespace System;

namespace Godzuki {

	public delegate void ResponseHandler(int cmd, int status, int size, String^ data);

	void receiveCommands( void *objRef, gCommandObject *cmdObj );

	public ref class SensorBot
	{
		// TODO: Add your methods for this class here.
	public:
		static void HelloWorld();
		static void setup();
		static void routeCommand(int cmdID);
		static void setClientHandler(ResponseHandler^ clientHandler);
		static ResponseHandler^ setHandler;
		static void loop();
		
	private:
	};
};
