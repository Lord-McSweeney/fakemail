#include <stdio.h>
#include <strings.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#define sendersize 512

char ipaddr[64];

char data[65536];

char message[131072];

char message2[131072];

char message3[131072];

char message4[131072];

char message5[131072];

char message6[131072];

char message7[131072];

int i = 0;

char useless[65536];

char currenttime[512];

/*
HELO [[sender-email-provider]]
MAIL FROM: <[[sender-name]]@[[sender-email-provider]]>
RCPT TO: <[[recipient-name]]@[[recipient-email-provider]]>
DATA
Date: [[date]]
From: [[sender-real-name]] <[[sender-name]]@[[sender-email-provider]]>
To: [[recipient-real-name]] <[[recipient-name]]@[[recipient-email-provider]]>
Subject: [[subject]]
Message-Id: <[[random-data]]>

[[contents]]
.


*/

void strcut(char *string, char delimiter, char **result) {
  int i;
  int encountereddel = 0;
  int res;
  int j;
  for (i = 0; i < strlen(string); i ++) {
    if (encountereddel) {
      result[1][j] = string[i];
    } else {
      if (string[i] != delimiter) {
        result[0][i] = string[i];
      } else {
        j = -1;
        encountereddel = 1;
      }
    }
    j ++;
  }
}

void getTime(char *result) {
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  strftime(result, 512, "%a, %d %b %Y %H:%M:%S %Z", &tm);
}

int sendmessage(char *address, int portnum, char *senddata, char *senddata2, char *senddata3, char *senddata4, char *senddata5, char *senddata6, char *senddata7, char *towrite, int amount) {
    int socket_desc;
    struct sockaddr_in server;
    char *message;
    char *server_reply = (char *) malloc(amount);

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Error while creating socket. Error code: \"%d\"\n", errno);
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(address);
    server.sin_family = AF_INET;
    server.sin_port = htons(portnum);
    puts("Server socket created");

    if (connect(socket_desc, (struct sockaddr *) & server, sizeof(server)) < 0) {
        printf("Error while connecting to remote server. Error code: \"%d\" ", errno);
        if (errno == 111) {
            printf("(ERR_CONNECTION_REFUSED)");
        }
        if (errno == 101) {
            printf("(ERR_NETWORK_UNREACHABLE)");
        }
        printf("\n");
        return 1;
    }
    puts("Server socket connected");

    if (recv(socket_desc, server_reply, amount, 0) < 0) {
        printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Response: %s\n", server_reply);

    if (send(socket_desc, senddata, strlen(senddata), 0) < 0) {
        printf("Error while sending data to remote server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Sent message to server (%s)\n", senddata);
    if (recv(socket_desc, server_reply, amount, 0) < 0) {
        printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Response: %s\n", server_reply);
    if (send(socket_desc, senddata2, strlen(senddata2), 0) < 0) {
        printf("Error while sending data to remote server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Sent second message to server (%s)\n", senddata2);
    if (recv(socket_desc, server_reply, amount, 0) < 0) {
        printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Response: %s\n", server_reply);
    if (send(socket_desc, senddata3, strlen(senddata3), 0) < 0) {
        printf("Error while sending data to remote server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Sent third message to server (%s)\n", senddata3);
    if (recv(socket_desc, server_reply, amount, 0) < 0) {
        printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Response: %s\n", server_reply);
    if (send(socket_desc, senddata4, strlen(senddata4), 0) < 0) {
        printf("Error while sending data to remote server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Sent fourth message to server (%s)\n", senddata4);
    if (recv(socket_desc, server_reply, amount, 0) < 0) {
        printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Response: %s\n", server_reply);
    if (send(socket_desc, senddata5, strlen(senddata5), 0) < 0) {
        printf("Error while sending data to remote server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Sent fifth message to server (%s)\n", senddata5);/*
    if (recv(socket_desc, server_reply, amount, 0) < 0) {
        printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
        return 1;
    }*/
    // printf("Response: %s\n", server_reply);
    if (send(socket_desc, senddata6, strlen(senddata6), 0) < 0) {
        printf("Error while sending data to remote server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Sent sixth message to server (%s)\n", senddata6);/*
    if (recv(socket_desc, server_reply, amount, 0) < 0) {
        printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
        return 1;
    }*/
    printf("Response: %s\n", server_reply);
    if (send(socket_desc, senddata7, strlen(senddata7), 0) < 0) {
        printf("Error while sending data to remote server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Sent seventh message to server (%s)\n", senddata7);
    if (recv(socket_desc, server_reply, amount, 0) < 0) {
        printf("Error while recieving response from server. Error code: \"%d\"\n", errno);
        return 1;
    }
    printf("Response: %s\n", server_reply);
    strcpy(towrite, server_reply);
    close(socket_desc);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 7) {
      printf("Usage: fakemail [sender] [real sender name] [recepient] [real recipient name] [subject] [file]\n");
      return 1;
    }
    printf("Creating message...\n");
    char *sender = argv[1];
    char **cutbuffer = (char**)malloc(2*sizeof(char*));
    int i;
    for (i = 0; i < 2; i++) {
        cutbuffer[i] = (char*)malloc(sendersize*sizeof(char));
    }
    strcut(sender, '@', cutbuffer);
    char *SENDER_EMAIL_PROVIDER = cutbuffer[1];
    char *SENDER_NAME = cutbuffer[0];

    char *reciever = argv[3];
    char **cutbuffer2 = (char**)malloc(2*sizeof(char*));
    for (i = 0; i < 2; i++) {
        cutbuffer2[i] = (char*)malloc(sendersize*sizeof(char));
    }
    strcut(reciever, '@', cutbuffer2);
    char *RECIEVER_EMAIL_PROVIDER = cutbuffer2[1];
    char *RECIEVER_NAME = cutbuffer2[0];
    FILE *fstr = fopen(argv[6], "r");
    unsigned int c = fgetc(fstr);
    i = 0;
    while(c != EOF && i < 65536) {
      data[i] = c;
      i ++;
      c = fgetc(fstr);
    }
    getTime(currenttime);
    sprintf(message, "HELO %s\n", RECIEVER_EMAIL_PROVIDER);
    sprintf(message2, "MAIL FROM: <%s>\n", sender);
    sprintf(message3, "RCPT TO: <%s>\n", reciever);
    sprintf(message4, "DATA\n");
    sprintf(message5, "From: %s <%s>\nTo: %s <%s>\n", argv[2], sender, argv[4], reciever);
    sprintf(message6, "Subject: %s\nDate: %s\n\n%s", argv[5], currenttime, data);
    sprintf(message7, ".\n\n");
    printf("Done!\n");
    printf("Sending message...\n");
    sendmessage("74.125.137.27", 25, message, message2, message3, message4, message5, message6, message7, useless, 65536);
    // rwsocket("127.0.0.1", 4444, message, useless, 65536);
    // printf("%s", message);
    printf("Sent!\n");
    printf("%s", useless);
    return 0;
}
