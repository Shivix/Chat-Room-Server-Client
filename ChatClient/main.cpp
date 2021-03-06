#include <iostream>
#include "chatClient.hpp"

int main(){

    try{
        chatClient client{};
        std::cout << "Type exit() to quit the program\n"
                     /*"Type ! followed by a chat room name to join a specific chatroom\n"*/
                     "Type / followed by a username to send a private message" << std::endl;
        
        client.connectToServer();

        bool isRunning{true};
        while (isRunning){
            int numberSet{poll(client.fdSet.data(), client.fdSet.size(), -1)};

            if(numberSet == -1){
                throw std::runtime_error("Error occurred while polling file descriptors");
            }
            if(client.fdSet[1].revents != 0){ // checks if the file descriptor for the listening socket has been set
                client.receiveMessage();
            }
            if (client.fdSet[0].revents != 0){
                std::string message;
                std::getline(std::cin, message);
                if (message == "exit()"){ // provides a way for the user to exit the program by typing exit()
                    isRunning = false;
                }
                /*else if (message.starts_with('!')){
                    client.activeChatRoom = message.substr(1, message.size() - 1);
                }*/
                else if (message.starts_with('/')){
                    client.sendMessage(messageProtocol::messageType::direct, message.substr(message.find(' ') + 1, message.back()),
                                       message.substr(1, message.find(' ') - 1));
                }
                else{
                    client.sendMessage(messageProtocol::messageType::chatRm, message, client.activeChatRoom);
                }
            }
        }
    }
    catch(std::exception& exception){
        std::cerr << exception.what() << std::endl;
        return -1;
    }
    return 0;
}
