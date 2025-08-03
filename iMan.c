#include "headers/headers.h"

void html_tag_remover(char * str)
{
    bool tag_flag = 0;
    for(int i = 0 ; i < strlen(str) ; i++)
    {
        if(str[i] == '<')
        {
            tag_flag = 1;
        }else if(str[i] == '>')
        {
            tag_flag = 0;
        }
        else if(tag_flag == 0)
        {
            printf("%c",str[i]);
        }
    }
    return ;
}

void getting_man_pages(char **commands)
{
    if(commands[1] == NULL)
    {
        printf("\033[31mError: No command entered\033[0m\n");
        return ;
    }

    int socketfd;
    char *request = (char *)malloc(MAX_SIZE*sizeof(char));
    if(request == NULL)
    {
        printf("\033[31mMemory allocation failed\033[0m\n");
        return ;
    }

    char *response = (char *)malloc(BUFFER_SIZE*sizeof(char));
    if(response == NULL)
    {
        printf("\033[31mMemory allocation failed\033[0m\n");
        free(request);
        return ;
    }

    char *hostname = (char*)malloc(MAX_SIZE*sizeof(char));
    if(hostname == NULL)
    {
        printf("\033[31mMemory allocation failed\033[0m\n");
        free(request);
        free(response);
        return ;
    }
    strcpy(hostname,"man.he.net");
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0)
    {
        printf("\033[31mError in creating socket\033[0m\n");
        free(request);
        free(response);
        free(hostname);
        return ;
    }

    struct sockaddr_in server_address;
    struct hostent *server;

    server = gethostbyname(hostname);
    if(server == NULL)
    {
        printf("\033[31mError in finding host\033[0m\n");
        free(request);
        free(response);
        free(hostname);
        return ;
    }

    // bzero((char *) &server_address, sizeof(server_address));
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    // bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    memcpy((char *)&server_address.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    server_address.sin_port = htons(80);

    if(connect(socketfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("\033[31mError in connecting to server\033[0m\n");
        free(request);
        free(response);
        free(hostname);
        return ;
    }
// http://man.he.net/?topic=ls&section=all

    snprintf(request, MAX_SIZE, 
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: iMan/1.0\r\n"
             "Connection: close\r\n\r\n", commands[1], hostname);

    if(send(socketfd,request,strlen(request),0) < 0)
    {
        printf("\033[31mError in sending request\033[0m\n");
        free(request);
        free(response);
        free(hostname);
        close(socketfd);
        return ;
    }
    // printf("\033[32mRequest sent successfully\033[0m\n");
    printf("\033[32m%s\033[0m\n\n",commands[1]);
    int received, header_end = 0;
    int error_flag = 0 ;
    char error_output[MAX_SIZE] ;
    snprintf(error_output , MAX_SIZE , "No matches for \"%s\"", commands[1]);
    // printf("%s",error_output) ;
    while ((received = recv(socketfd, response,  BUFFER_SIZE -1 , 0)) > 0) {
        printf("Received %d bytes\n", received);
        response[received] = '\0';
        printf("%s\n",response);

        // Error checking
        char *error = strstr(response , error_output);
        if(error != NULL) error_flag = 1;
        // Find the end of the HTTP headers
        char *header_end_ptr = strstr(response, "<html>");
        if (header_end_ptr != NULL) {
            header_end = header_end_ptr - response + 6;
            // printf("%s", header_end_ptr + 6);
            html_tag_remover(header_end_ptr + 6);
        }else{
            // printf("%s", response);
            html_tag_remover(response);
        }
    }
    

    if (received < 0) {
        printf("\033[031mError in getting respomse\033[03m\n");
        free(request);
        free(response);
        free(hostname);
        close(socketfd);
        return;
    }
    if(error_flag)
    {
        printf("\033[031mError : No matches were found for \"%s\" command.\033[0m\n", commands[1]);
    }

    free(request);
    free(response);
    free(hostname);
    close(socketfd);
    return;

}