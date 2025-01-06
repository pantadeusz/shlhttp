# shlhttp Documentation

## Project Overview
**shlhttp** is a single-header HTTP server written in C++. This lightweight server is designed to be easy to integrate into other projects without the need for additional dependencies.

## Installation
### Prerequisites
- A C++ compiler supporting C++11 or later.
- [Git](https://git-scm.com/) for cloning the repository.

### Steps
1. Clone the repository:
```sh
git clone https://github.com/pantadeusz/shlhttp.git
cd shlhttp
```

2. Include the `shttp.hpp` header file in your project.

## Usage
### Basic Example
To start a basic HTTP server, include `shttp.hpp`. You can start with the following example:

```cpp
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
```

Compile and run your program. The server will be accessible at `http://localhost:8080`.

## API Reference

TODO

## Contributing
### Guidelines
1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Make your changes and commit them with clear messages.
4. Push your changes to your fork and create a pull request.

### Running Tests
To run tests, include the `catch.hpp` file and compile with the test files:

```sh
g++ -o tests test.cpp -std=c++11
./tests
```

## License
This project is licensed under the MIT License. See the [LICENSE](https://github.com/pantadeusz/shlhttp/blob/master/LICENSE) file for more details.

## Contact
For issues, please open a ticket on the [GitHub Issues](https://github.com/pantadeusz/shlhttp/issues) page.

For direct contact, reach out to the repository owner through GitHub.
