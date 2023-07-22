#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

//Simple c++ server that allows you to manage file access requests with a GET 
//on http://localhost:8080/file.extension.
//The server prints the contents of the requests.

struct header {
  char * n;
  char * v;
}
h[100];

int main() {
  int port = 8080; //Port used, IP address: 127.0.0.1

  struct sockaddr_in addr, remote_addr;
  int c, i, j, k, s, s2, ka;
  socklen_t len;
  FILE * fin,fin2;
  int yes = 1;
  char * commandline, * method, * path, * ver;
  char request[5000], response[5000], command[100];

  //Create a socket which will then be the one that will handle the arrival of connections
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) {
    perror("Socket Failed");
    return 1;
  }

  //Create the structure addr
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = 0;

  //Set the socket options
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof(int)) == -1) {
    perror("Setsockopt Failed");
    return 1;
  }

  //Assigns the address specified by addr to the socket referred
  if (bind(s, (struct sockaddr * ) & addr, sizeof(struct sockaddr_in)) == -1) {
    perror("Bind Failed");
    return 1;
  }

  //Marks the socket referred to by sockfd as a passive socket
  if (listen(s, 225) == -1) {
    perror("Listen Failed");
    return 1;
  }
  len = sizeof(struct sockaddr_in);

  while (1) {

    //New socket that accept first connection form s list
    s2 = accept(s, (struct sockaddr * ) & remote_addr, & len);

    //Create a new thread for each connection. if fork return 0 is the child process and 
    //process to the following code, else is the parent process and go to the beginning 
    //of the loop
    if (fork()) continue;

    do {
      if (s2 == -1) {
        perror("Accept Failed");
        return 1;
      }

      //Parsing of request headers
      bzero(h, 100 * sizeof(struct header * ));
      commandline = h[0].n = request;
      for (j = 0, k = 0; read(s2, request + j, 1); j++) {
        printf("%c", request[j]);
        if (request[j] == ':' && (h[k].v == 0)) {
          request[j] = 0;
          h[k].v = request + j + 1;
        } else if ((request[j] == '\n') && (request[j - 1] == '\r')) {
          request[j - 1] = 0;
          if (h[k].n[0] == 0) break;
          h[++k].n = request + j + 1;
        }
      }
      printf("\n");

      //Verify request expect a keep-alive connection
      ka = 1;
      for (i = 1; i < k; i++) {
        if (strcmp(h[i].n, "Connection") == 0) {
          ka = !strcmp(h[i].v, " keep-alive");
        }
      }

      //Parsing command line, example: "GET /chunks/manifest.m3u8 HTTP/1.1"
      //method = GET
      //path = /chunks/manifest.m3u8
      //ver = HTTP/1.1
      method = commandline;
      for (i = 0; commandline[i] != ' '; i++) {}
      commandline[i] = 0;
      path = commandline + i + 1;
      for (i++; commandline[i] != ' '; i++);
      commandline[i] = 0;
      ver = commandline + i + 1;

      //Try to open file requested
      if ((fin = fopen(path + 1, "rt")) == NULL) {
        sprintf(response, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n");
        write(s2, response, strlen(response));
      } else {

        //Undestand file size
        FILE * fin2 = fopen(path + 1, "rt");
        for (i = 0; fgetc(fin2) != EOF; i++);
        fclose(fin2);

        //Send command line response
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", i);
        write(s2, response, strlen(response));

        //Send body response
        while ((c = fgetc(fin)) != EOF)
          write(s2, & c, 1);
        fclose(fin);
      }
    } while (ka && recv(s2, NULL, 1, MSG_PEEK | MSG_DONTWAIT) != 0);
    close(s2);
  }
}