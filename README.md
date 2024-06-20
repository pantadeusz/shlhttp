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
To start a basic HTTP server, include `shttp.hpp` in your C++ source file and use the following example code:

```cpp
#include "shttp.hpp"

int main() {
    shttp::server server;
    
    server.get("/", [](const shttp::request& req, shttp::response& res) {
        res.set_content("Hello, World!", "text/plain");
    });

    server.listen("0.0.0.0", 8080);
    return 0;
}
```

Compile and run your program. The server will be accessible at `http://localhost:8080`.

## API Reference
### shttp::server
- **Methods:**
  - `void listen(const std::string& host, int port);`
  - `void get(const std::string& path, const request_handler& handler);`
  - `void post(const std::string& path, const request_handler& handler);`
  - `void set_content(const std::string& content, const std::string& content_type);`

### shttp::request
- Represents an HTTP request.
- **Properties:**
  - `std::string method;`
  - `std::string path;`
  - `std::unordered_map<std::string, std::string> headers;`
  - `std::string body;`

### shttp::response
- Represents an HTTP response.
- **Methods:**
  - `void set_content(const std::string& content, const std::string& content_type);`

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
