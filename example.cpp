#include "shttp.hpp"
#include <iostream>

const std::string GET_RESPONSE_TEXT = R"(<!DOCTYPE html>
<html>
<head>
<meta charset='utf-8'>
<title>SHTTP</title>
</head>
<body>
<h2>Hello shttp</h2>
 <FORM action="/"
       enctype="multipart/form-data"
       method="post">
   <P>
   What is your name? <INPUT type="text" name="submit-name"><BR>
   What files are you sending? <INPUT type="file" name="files"><BR>
   <INPUT type="submit" value="Send"> <INPUT type="reset">
 </FORM>
</body>
</html>
)";

int main() {
  using namespace shttp;
  int lsocket = listen_socket();
  do {
    auto [acc_socket, client_host, client_port] = accept_on_socket(lsocket);
    std::cout << acc_socket << " " << client_host << " " << client_port
              << std::endl;
    auto request = receive_request(acc_socket);
    std::cout << request << std::endl;
    response_t response = response_of(200,"ok",GET_RESPONSE_TEXT);
    send_response(acc_socket, response);
    close(acc_socket);
  } while (true);
  close(lsocket);
  return 0;
}