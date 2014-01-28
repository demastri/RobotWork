#ifndef G_COMMAND_OBJECT_H
#define G_COMMAND_OBJECT_H

class gCommandObject {
	// RouteCommand becomes -- 
	//   void RouteCommand( CommandObject ) rather than
	//   CommandObject RouteCommand( dev, inst, cmd, param )
	//   where the return object now contains the response (if any) for that command)
	//   for a recurring command, the command object can be routed as normal (should this be the default?)
	// the router doesn't know where the command came from, except that it was from a LOCAL object
	//  it knows how to route between those objects only.  
	//  this is fine for local objects, but one source of commands are external devices/objects
	// the source for commands from external objects are 
	//  serial ports (radio or USB) or buttons or other types of async triggers (camera IR availability, etc)
	//  buttons are a kind of special case because their only real purpose is to source command events
	// the gComms/gInput classes now acts as pure status and definitional/key mapping classes
	//  where they really should act as pure comms hub devices
	// gComms should simply translate between strings and commands (serialize on command objects?)
	//  apps/objects requesting replies should be able to tell their gComms object, and it should talk to the world on its behalf
	//	routers that don't know how to route objects locally should ask their gComms objects to route them externally to see who replies
	//	partner gComms objects should reply with a "hey, I know who that is" so folks know it was routed properly
	//	this is actually good enough, but once a gComms finds a partner, it should register that dev/inst with the router 
	//		so it is "found" on the next pass and not broadcast again
	// gInputs should remember where source Devices / Instances route from and return results to the right port
	// Examples:
	//  Button press (say Single Distance Reading):
	//		on setup, there must be a gComms object established for each serial port
	//			these objects can register as targets of CNC_APP commands if they want to receive Status Text command replies
	//			(need to route commands to multiple defaults if no specific inst sent? in all cases?)
	//		gInputs reads the button press
	//		direct Routing of command internally to gDistanceSensor object
	//		response with OK status and sensor reading returned to gInput object
	//		gInput routes a "Display Status Text" reply with text to appropriately registered CNC_APP devices
	//		gComms devices that are registered will display the status text message as plain text on the appropriate port
	//  CNC_APP command (say Recurring Distance Reading with a timer...):
	//		on setup, CNC_APP devices can register to receive commands if they want to receive Status Text command replies (or not)
	//		CNC_APP issues a well formed command by serial port to the mesh (with appropriate target device IDs)
	//			(do we need robot ID / target node ID in the command object?)
	//		gComms reads the command string and turns it into an actionable local command object
	//		gComms registers this CNC_APP object and port with the router object for responses for this command
	//		gComms direct Routes the command internally to gDistanceSensor object
	//		gDistanceSensor response with OK status received by the gComms block call directly
	//			gComms packages up the response and sends it back over the same serial port to the calling CNC_APP object
	//			gDistanceSensor may also send a text status message reply to all listening CNC_APP objects...handled as above
	//		gDistanceSensor also registers a timer event to actually take sensor objects
	//			...tick...
	//		gRouter eventually issues a "get distance reading" to gDistanceSensor and results are returned
	//		gRouter looks for objects registered for get distance reading responses
	//			if the object (actually this should be for any command) has a port set for a particular gComms object
	//			the command response is packaged and sent to that gComms object, which puts it "otw" appropriately
	//	the result is that on receipt of a reply to any command, gComms will look to see who's registered for command replies
	//		if a registration exists, if it's local, call processCommandReply directly, if not, route through the right gComms for that port
	//  things that issue commands generally shouldn't know where the targets exist
	//		a simplifying assumption is that devices don't move off the physical command processor they're connected to
	//			example - Motor controller 4 will be commected to a particular robot for all time
	//		we can either 
	//			a) listen and log where devices are as they communicate, 
	//			b) keep a static list, or 
	//			c) we can ask then to register on startup - probably best
	//		each input manager has a mesh endpoint ID and "knows" what devices are available on its side of the mesh
	//			on startup, it should post the list of known devices to it's mesh - (as a command - to start a dialog)
	//			any input managers listening on that mesh should reply with its own list of "known" devices (as a reply to not start a storm)
	//			if any devices are added dynamically, when they're registered, this cycle can repeat (as replies to not storm the link...)
	//		actually, in this way, the gComms object can register a generic comms handler for remote commands/replies they see
	//			they route normally and the command router is unaware that the routing is causing i/o
	//			since the assumption is that there's only one object per outgoing port, this removes the need for routing tables.
	//			if this gComms obj requested it, and it registers for the reply, the reply will eventually find its target on the mesh...
	//			synchronous is easy - send back to the same port you're on.  It's the asynch ones that require any thought.
public:
	gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData );
	gCommandObject( int srcdev, int srcinst, int dev, int inst, int cmd );
	gCommandObject( int dev, int inst, int cmd, int param );
	gCommandObject( int dev, int inst, int cmd );
	gCommandObject( gCommandObject *rhs);

	int targetDeviceID;
	int targetInstanceID;
	int commandID;
	int parameter;

	int sourceDeviceID;
	int sourceInstanceID;
	bool isReply;

	gCommandObject *nextEntry;
	gCommandObject *prevEntry;

	gCommandObject *InitReply( unsigned char status, long paySize, void *payData );
	void print();

	unsigned char rtnStatus;
	long payloadSize;
	void *payloadData;

private:
	void Init( int srcdev, int srcinst, int dev, int inst, int cmd, int param, long paySize, void *payData );


	//virtual char *Serialize();
};
#endif