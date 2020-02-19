#ifndef ETHERNETCLIENT_H
#define ETHERNETCLIENT_H

/*
 * Author: Mattis Jaksch
 *
 * Ethernet Transceiver
 * Creates a socket and connects to a server
 *
 */

#define RETRY_TIMEOUT 10
#define CONNECTION_TIMEOUT 5
#define READ_BUFFER_SIZE 1024

#include <netinet/in.h>
#include <string>

class EthernetClient {
public:
    EthernetClient(uint port, const std::string &server);
	virtual ~EthernetClient();

	bool write(const std::string &message);
	std::string query(const std::string message);

    bool retry();
    bool connectionOk() const {return connection_ok;}

private:
    bool socket_ok, connection_ok;
	struct sockaddr_in server_address;
    uint port;
    int sock = 0;
	std::string server;

	bool create_socket();
	bool connect_to_server();
    void disconnect_from_server();
};

#endif // ETHERNETCLIENT_H
