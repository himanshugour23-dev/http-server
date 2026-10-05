#ifndef SERVER_HPP
#define SERVER_HPP

class Server {
    public : 
        Server(int port) ; 
        ~Server() ; 
        void start() ;
    private : 
        int port ;    
        int serverFd_ ;
        void createSocket() ;
        void bindSocket() ;
        void startListening() ;
        void handleClient(int clientFd) ;
};

#endif 