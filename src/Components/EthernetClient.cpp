#include "EthernetClient.h"

#include <cstdlib>
#include <cstring>

EthernetClient::EthernetClient(uint port, const std::string &server) : port(port), server(server) {
    connection_ok = false;

    socket_ok = create_socket();

    if(socket_ok)
        connection_ok = connect_to_server();
}

EthernetClient::~EthernetClient() {
    close(sock);
}

bool EthernetClient::retry() {
    static long retry_clock = time(NULL);

    if(time(NULL) - retry_clock < 10)
        return false;

    if(!socket_ok)
        socket_ok = create_socket();

    if(socket_ok)
        connection_ok = connect_to_server();

    retry_clock = time(NULL);

    return connection_ok;
}

bool EthernetClient::create_socket() {
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
		//exit(EXIT_FAILURE);
		return false;
	}

	memset(&server_address, '0', sizeof(server_address));

	server_address.sin_family = AF_INET;
    server_address.sin_port = htons(uint16_t(port));

    //Add send/receive timeout
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

	return true;
}

bool EthernetClient::connect_to_server() {
	if(inet_pton(AF_INET, server.c_str(), &server_address.sin_addr) < 0) {
        perror("Invalid address");
		//exit(EXIT_FAILURE);
		return false;
	}

	if(connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
		perror("Connection failed");
		//exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

bool EthernetClient::write(const std::string &message) {
    ssize_t data_bytes = 0;

	std::string tmp = message + "\r\n";

    if(socket_ok && connection_ok)
        data_bytes = send(sock, tmp.c_str(), strlen(tmp.c_str()), MSG_CONFIRM);

    return data_bytes != -1 && size_t(data_bytes) == tmp.size();
}

std::string EthernetClient::query(const std::string message) {
    if(!write(message))
        return "";

    char output[256];
    std::string buffer = "";

    ssize_t data_bytes = ssize_t(recv(sock, output, 256, 0));

    if(data_bytes == -1) {
        disconnect();
        return "";
    }

    output[data_bytes] = '\0';
    buffer += output;

    return buffer;
}

void EthernetClient::disconnect() {
    connection_ok = false;
    socket_ok = false;
    close(sock);
}
