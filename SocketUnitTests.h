/************************************************************************
 * Modification history :
 * 10/03/2012 - Rudhir
 ************************************************************************/

#include "TraciSocket.h"
#include <string>


using namespace std;

class SocketUnitTests
{
private:
	enum mode {undefined = -1, client, server};
	mode currentMode;
	TraciSocket* serverSocket;
	TraciSocket* clientSocket;

public:

	SocketUnitTests();
	void openConnection();
	void receiveStorage();
	void sendStorage();
	void closeConnection();
};
