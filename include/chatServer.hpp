#ifndef CHATSERVER_CHATSERVER_HPP
#define CHATSERVER_CHATSERVER_HPP


#include <sys/socket.h>
#include <netinet/in.h>
#include <array>
#include <netdb.h>
#include <vector>
#include <poll.h>
#include "chatClient.hpp"
#include "messageProtocol.hpp"
#include <optional>


class chatServer{ 
    
    static constexpr uint16_t port{12345};
    static constexpr size_t maxMessageSize{2048};
    int serverSocket{socket(PF_INET, SOCK_STREAM, 0)}; // endpoint for all incoming and outgoing data -- TCP to avoid partial messages being received

    std::string_view serverIP{"127.0.0.1"}; // currently localhost 
    sockaddr_in serverAddress{}; // struct with address info to bind the socket

    std::vector<chatClient> clientList;

    pollfd listenFD; // file descriptor to store events associated with the listening socket
    
public:
    chatServer();
    ~chatServer();
    chatServer(const chatServer&) = delete;
    chatServer& operator=(chatServer) = delete;

    void run();
    
private:
    void addClient();
    void removeClient(const chatClient&);
    void relayMessage(const chatClient&);
    std::optional<messageProtocol> receivePayload(const chatClient&);
};


#endif //CHATSERVER_CHATSERVER_HPP