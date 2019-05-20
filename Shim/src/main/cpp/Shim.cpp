#include "Shim.h"

void FRC_NetworkCommunication_CANSessionMux_sendMessage(uint32_t messageID, const uint8_t *data, uint8_t dataSize, int32_t periodMs, int32_t *status){
  printf("Attempted to send CAN message\n");
}
void FRC_NetworkCommunication_CANSessionMux_receiveMessage(
     uint32_t *messageID, uint32_t messageIDMask, uint8_t *data,
     uint8_t *dataSize, uint32_t *timeStamp, int32_t *status){

     }
void FRC_NetworkCommunication_CANSessionMux_openStreamSession(
     uint32_t *sessionHandle, uint32_t messageID, uint32_t messageIDMask,
     uint32_t maxMessages, int32_t *status){

     }
void FRC_NetworkCommunication_CANSessionMux_closeStreamSession(
     uint32_t sessionHandle){

     }
void FRC_NetworkCommunication_CANSessionMux_readStreamSession(
     uint32_t sessionHandle, struct tCANStreamMessage *messages,
     uint32_t messagesToRead, uint32_t *messagesRead, int32_t *status){

     }
void FRC_NetworkCommunication_CANSessionMux_getCANStatus(
     float *percentBusUtilization, uint32_t *busOffCount, uint32_t *txFullCount,
     uint32_t *receiveErrorCount, uint32_t *transmitErrorCount, int32_t *status){

     }

uint32_t FRC_NetworkCommunication_nUsageReporting_report(uint8_t resource, uint8_t instanceNumber, uint8_t context, const char *feature){
  printf("Call to usage reporting\n");
}

int FRC_NetworkCommunication_sendError(int isError, int32_t errorCode, int isLVCode,
		const char *details, const char *location, const char *callStack){
      printf("Reporting error:\n");
      return 0;
    }

int FRC_NetworkCommunication_getControlWord(struct ControlWord_t *controlWord){
  return 0;
}

int FRC_NetworkCommunication_getWatchdogActive(void){
  return 0;
}
int FRC_NetworkCommunication_getAllianceStation(enum AllianceStationID_t *allianceStation){
  return 0;
}
int FRC_NetworkCommunication_getMatchInfo(char *eventName, enum MatchType_t *matchType, uint16_t *matchNumber, uint8_t *replayNumber,
    uint8_t *gameSpecificMessage, uint16_t *gameSpecificMessageSize){
      return 0;
    }
int FRC_NetworkCommunication_getMatchTime(float *matchTime){
  return 0;
}
int FRC_NetworkCommunication_getJoystickAxes(uint8_t joystickNum, struct JoystickAxes_t *axes, uint8_t maxAxes){
  return 0;
}
int FRC_NetworkCommunication_getJoystickButtons(uint8_t joystickNum, uint32_t *buttons, uint8_t *count){
  return 0;
}
int FRC_NetworkCommunication_getJoystickPOVs(uint8_t joystickNum, struct JoystickPOV_t *povs, uint8_t maxPOVs){
  return 0;
}
int FRC_NetworkCommunication_setJoystickOutputs(uint8_t joystickNum, uint32_t hidOutputs, uint16_t leftRumble, uint16_t rightRumble){
  return 0;
}
int FRC_NetworkCommunication_getJoystickDesc(uint8_t joystickNum, uint8_t *isXBox, uint8_t *type, char *name,
    uint8_t *axisCount, uint8_t *axisTypes, uint8_t *buttonCount, uint8_t *povCount){
      return 0;
    }

void FRC_NetworkCommunication_getVersionString(char *version){

}
int FRC_NetworkCommunication_observeUserProgramStarting(void){
  return 0;
}
void FRC_NetworkCommunication_observeUserProgramDisabled(void){

}
void FRC_NetworkCommunication_observeUserProgramAutonomous(void){

}
void FRC_NetworkCommunication_observeUserProgramTeleop(void){
  
}
void FRC_NetworkCommunication_observeUserProgramTest(void){

}