#include "shlhttp.hpp"
#include <iostream>

int main() {
  using namespace shlhttp;
  int lsocket = listen_socket();
  do {
    auto [acc_socket, client_host, client_port] = accept_on_socket(lsocket);
    std::cout << acc_socket << " " << client_host << " " << client_port
              << std::endl;
    auto request = receive_request(acc_socket);
    response_t response = response_of(200,"well done","Witaj");
    send_response(acc_socket, response);
    close(acc_socket);
  } while (true);
  close(lsocket);
  return 0;
}