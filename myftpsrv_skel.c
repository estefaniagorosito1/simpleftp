#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <err.h>

#include <netinet/in.h>

#define BUFSIZE 512
#define CMDSIZE 4
#define PARSIZE 100

#define MSG_220 "220 srvFtp version 1.0\r\n"
#define MSG_331 "331 Password required for %s\r\n"
#define MSG_230 "230 User %s logged in\r\n"
#define MSG_530 "530 Login incorrect\r\n"
#define MSG_221 "221 Goodbye\r\n"
#define MSG_550 "550 %s: no such file or directory\r\n"
#define MSG_299 "299 File %s size %ld bytes\r\n"
#define MSG_226 "226 Transfer complete\r\n"

/**
 * function: receive the commands from the client
 * sd: socket descriptor
 * operation: \0 if you want to know the operation received
 *            OP if you want to check an especific operation
 *            ex: recv_cmd(sd, "USER", param)
 * param: parameters for the operation involve
 * return: only usefull if you want to check an operation
 *         ex: for login you need the seq USER PASS
 *             you can check if you receive first USER
 *             and then check if you receive PASS
 **/
bool recv_cmd(int sd, char *operation, char *param) {
    char buffer[BUFSIZE], *token;
    int recv_s;

    // receive the command in the buffer and check for errors



    // expunge the terminator characters from the buffer
    buffer[strcspn(buffer, "\r\n")] = 0;

    // complex parsing of the buffer
    // extract command receive in operation if not set \0
    // extract parameters of the operation in param if it needed
    token = strtok(buffer, " ");
    if (token == NULL || strlen(token) < 4) {
        warn("not valid ftp command");
        return false;
    } else {
        if (operation[0] == '\0') strcpy(operation, token);
        if (strcmp(operation, token)) {
            warn("abnormal client flow: did not send %s command", operation);
            return false;
        }
        token = strtok(NULL, " ");
        if (token != NULL) strcpy(param, token);
    }
    return true;
}

/**
 * function: send answer to the client
 * sd: file descriptor
 * message: formatting string in printf format
 * ...: variable arguments for economics of formats
 * return: true if not problem arise or else
 * notes: the MSG_x have preformated for these use
 **/
bool send_ans(int sd, char *message, ...){
    char buffer[BUFSIZE];

    va_list args;
    va_start(args, message);

    vsprintf(buffer, message, args);
    va_end(args);
    // send answer preformated and check errors




}

/**
 * function: RETR operation
 * sd: socket descriptor
 * file_path: name of the RETR file
 **/

void retr(int sd, char *file_path) {
    FILE *file;    
    int bread;
    long fsize;
    char buffer[BUFSIZE];

    // check if file exists if not inform error to client

    // send a success message with the file length

    // important delay for avoid problems with buffer size
    sleep(1);

    // send the file

    // close the file

    // send a completed transfer message
}
/**
 * funcion: check valid credentials in ftpusers file
 * user: login user name
 * pass: user password
 * return: true if found or false if not
 **/
bool check_credentials(char *user, char *pass) {
    FILE *file;
    char *path = "./ftpusers", *line = NULL, cred[100];
    size_t len = 0;
    bool found = false;

    // make the credential string
    strcpy(cred, user);
    strcat(cred, ":");
    strcat(cred, pass);
    strcat(cred, "\n");
    
    // check if ftpusers file it's present
    file = fopen(path, "r");
    if (file == NULL) {
        printf(MSG_550);
    }
    
    // search for credential string
    line = (char *)malloc(sizeof(char)*100);
    while(feof(file) == 0) {
        fgets(line, 100, file);
    }
    if (strcmp(file, line) == 0) {
        found = true;
    } 
    
    // close file and release any pointes if necessary
    fclose(file);
    
    // return search status
    return found;
}

/**
 * function: login process management
 * sd: socket descriptor
 * return: true if login is succesfully, false if not
 **/
bool authenticate(int sd) {
    char user[PARSIZE], pass[PARSIZE];

    // wait to receive USER action

    // ask for password

    // wait to receive PASS action

    // if credentials don't check denied login

    // confirm login
}

/**
 *  function: execute all commands (RETR|QUIT)
 *  sd: socket descriptor
 **/

void operate(int sd) {
    char op[CMDSIZE], param[PARSIZE];

    while (true) {
        op[0] = param[0] = '\0';
        // check for commands send by the client if not inform and exit


        if (strcmp(op, "RETR") == 0) {
            retr(sd, param);
        } else if (strcmp(op, "QUIT") == 0) {
            // send goodbye and close connection




            break;
        } else {
            // invalid command
            // furute use
        }
    }
}

/**
 * Run with
 *         ./mysrv <SERVER_PORT>
 **/
int main (int argc, char *argv[]) {

    // arguments checking
    if(argc != 2){
        printf("%s <SERVER_PORT>\n", argv[0]);
        exit(1);
	}

    // reserve sockets and variables space
    typedef struct sockaddr *addr;
    int sockfd, socksend;
    struct sockaddr_in serv, peer_addr;
    socklen_t peeraddrsize = sizeof(struct sockaddr_in);
    
    // create server socket and check errors
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(argv[1]););
    serv.sin_addr.s_addr = INADDR_ANY;

    if(sockfd < 0){
	    printf("Socket could not create\n");
	    exit(1);
    }
    print("Socket created\n");
    
    // bind master socket and check errors
    if((bind(sockfd, (struct sockaddr *)&serv, peeraddrsize)) < 0) {
	    printf("Error in binding\n");
	    exit(1);
    }
    
    // make it listen
    if(!(listen(sockfd, 4))) {
        printf("Error in listening\n");
	    exit(1);
    } else {
	    printf("Socket listening\n");
    }
    
    // main loop
    while (true) {
        // accept connectiones sequentially and check errors
        if((socksend = accept(sockfd, (struct sockaddr *)&peer_addr, peeraddrsize)) < 0) {
	        printf("Error in accept function\n");
	        exit(1);
	    }
        
        // send hello
        if(!send_ans(socksend, MSG_220)) {
	        printf("Error in hello message\n");
        } else {
	        printf("Hello message was sent\n");
        }
        
        // operate only if authenticate is true
        if(!authenticate(socksend)) {
	        send_ans(sock_send, MSG_530);
	    }
    }

    // close server socket
    close(socksend);
    
    return 0;
}
