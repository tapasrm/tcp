#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
  int socket_desc, client_sock, client_size;
  struct sockaddr_in server_addr, client_addr;
  char server_message[2000], client_message[2000];

  // Clean buffers
  memset(server_message, '\0', sizeof(server_message));
  memset(client_message, '\0', sizeof(client_message));

  // Create Socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_desc < 0) {
    printf("Error while creating socket.\n");
    return -1;
  }
  printf("Socket Created Succesfully !!\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Bind to the set port and IP
  if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    printf("Couldn't bind to the port.\n");
    return -1;
  }
  printf("Done with binding\n");

  // Listen for clients:
  if (listen(socket_desc, 1) < 0) {
    printf("Error while listening\n");
    return -1;
  }
  printf("\nListening for incoming connections...\n");

  // Accept an incoming connection.
  client_size = sizeof(client_addr);
  client_sock =
      accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

  if (client_sock < 0) {
    printf("Can't accept!\n");
    return -1;
  }

  printf("Client connected at IP: %s and port: %i\n",
         inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  // Recieve Message
  if (recv(client_sock, client_message, sizeof(client_message), 0) < 0) {
    printf("Can't Send");
    return -1;
  }

  printf("Msg from Client: %s\n", client_message);

  // Respond to Client
  strcpy(server_message, "Hello from the Server");

  if (send(client_sock, server_message, strlen(server_message), 0) < 0) {
    printf("Can't Send.\n");
    return -1;
  }

  // Closing the Connection
  close(client_sock);
  close(socket_desc);

  return 0;
}
