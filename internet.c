#include <stdio.h>
#include <strings.h>
#include <time.h>
#include "resources/sockets.h"
#define sendersize 512

char ipaddr[64];

char data[65536];

char message[131072];

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

int main(int argc, char **argv) {
    if (argc < 7) {
      printf("Usage: fakemail [sender] [real sender name] [recepient] [real recipient name] [subject] [file]\n");
      return 1;
    }
    printf("Creating message...\n");
    char *sender = argv[1];
    char **cutbuffer = (char**)malloc(2*sizeof(char*));
    int i;
    for(i = 0; i < 2; i++){
        cutbuffer[i] = (char*)malloc(sendersize*sizeof(char));
    }
    strcut(sender, '@', cutbuffer);
    char *SENDER_EMAIL_PROVIDER = cutbuffer[1];
    char *SENDER_NAME = cutbuffer[0];

    char *reciever = argv[3];
    char **cutbuffer2 = (char**)malloc(2*sizeof(char*));
    for(i = 0; i < 2; i++){
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
    sprintf(message, "HELO %s\nMAIL FROM: <%s>\nRCPT TO: <%s>\nDATA\nFrom: %s <%s>\nTo: %s <%s>\nSubject: %s\nDate: %s\n\n%s.", RECIEVER_EMAIL_PROVIDER, sender, reciever, argv[2], sender, argv[4], reciever, argv[5], currenttime, data);
    printf("Done!\n");
    printf("Sending message...\n");
    rwsocket("142.250.141.27", 25, message, useless, 65536);
    printf("Sent!\n");
    printf("%s", useless);
    return 0;
}
