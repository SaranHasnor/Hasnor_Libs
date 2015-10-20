#include "network_utils.h"

extern void setupNetwork(long worryTime, long timeoutTime);
extern void shutdownNetwork(void);

void SV_initServer(int maxConnections, unsigned short port, socketProtocol_t protocol, networkStatus_t *status);
int SV_checkForNewClients(void);
void SV_kickClient(int clientID);
void SV_closeServer(networkStatus_t *status);

void SV_sendMessage(int targetID, bytestream_t message);
void SV_update(networkUpdate_t *update);
