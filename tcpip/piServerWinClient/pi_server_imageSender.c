#include <unistd.h>                                                                                          
#include <stdio.h>                                                                                            
#include <sys/socket.h>                                                                                      
#include <stdlib.h>                                                                                          
#include <netinet/in.h>                                                                                      
#include <string.h>                                                                                          
#include <fcntl.h>                                                                                            
#define PORT 8080                                                                                            

int main(int argc, char const *argv[])                                                                        
{                                                                                                            
    int server_fd, new_socket, valread;                                                                      
    struct sockaddr_in address;                                                                              
    int opt = 1;                                                                                              
    int addrlen = sizeof(address);                                                                            
    char buffer[1024] = {0};                                                                                  
    char *hello = "Hello from server";                                                                        

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )                                                  
    {                                                                                                        
        perror("socket failed");                                                                              
        exit(EXIT_FAILURE);                                                                                  

    }                                                                                                        

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, & opt, sizeof(opt)))                    
    {                                                                                                        
        perror("setsockopt");                                                                                
        exit(EXIT_FAILURE);                                                                                  

    }                                                                                                        

    address.sin_family = AF_INET;                                                                            
    address.sin_addr.s_addr = INADDR_ANY;                                                                    
    address.sin_port = htons(PORT);                                                                          

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)                                      
    {                                                                                                        
        perror("bind failed");                                                                                
        exit(EXIT_FAILURE);                                                                                  

    }                                                                                                        
    if(listen(server_fd, 3)< 0)                                                                              
    {                                                                                                        
        perror("listen");                                                                                    
        exit(EXIT_FAILURE);                                                                                  

    }                                                                                                        
    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)                
    {                                                                                                        
        printf("server accept failed....\n");                                                                
        exit(0);                                                                                              

    }

    int flag = fcntl(new_socket, F_GETFL, 0);
    fcntl(new_socket, F_SETFL, flag | O_NONBLOCK);                                                            

    int read_size = 0;                                                                                        
    while(1)                                                                                                  
    {                                                                                                        

        read_size = read(new_socket, buffer, 1024);                                                          
        if(read_size > 0 )                                                                                    
        {                                                                                                    
            printf("%s\n",buffer);                                                                            
            write(new_socket, hello, strlen(hello));                                                          
            printf("Hello message sent\n");                                                                  

        }                                                                                                    

    }                                                                                                      
    return 0;                                                                                                

}  

