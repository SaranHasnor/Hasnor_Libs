#include "network_client.h"
#include "network_internal.h"

void CL_connectToServer(const char *address, unsigned short port, bytestream_t clientInfo, socketProtocol_t protocol, networkStatus_t *status)
{
	if (currentNetworkMode() != NETWORK_MODE_LOCAL)
	{
		if (status)
		{
			status->error = NETWORK_ERROR_UNAUTHORIZED_ACTION;
			status->socketError = 0;
		}
		return;
	}

	if (maxConnections() == 0)
	{ // setup the socket
		if (!createSocket(address, port, protocol, status))
		{
			return;
		}
	}

	if (tryToConnect(clientInfo, status))
	{
		if (status)
		{
			status->error = NETWORK_ERROR_NONE;
			status->socketError = 0;
		}
	}
}

void CL_disconnectFromServer(networkStatus_t *status)
{
	if (currentNetworkMode() != NETWORK_MODE_CLIENT)
	{
		if (status)
		{
			status->error = NETWORK_ERROR_UNAUTHORIZED_ACTION;
			status->socketError = 0;
		}
		return;
	}

	disconnect();
}

bool CL_connected(void)
{
	return currentNetworkMode() == NETWORK_MODE_CLIENT;
}

void CL_sendMessage(int targetID, bytestream_t message)
{
	sendMessage(NETWORK_MESSAGE_CUSTOM, -1, targetID, message);
}
