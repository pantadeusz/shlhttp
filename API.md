# shlhttp Detailed Method Documentation

## shttp::server

### Methods

#### `void listen(const std::string& host, int port);`
Starts the HTTP server to listen on the specified host and port.

- **Parameters:**
  - `host`: A string specifying the host address (e.g., "0.0.0.0" for all interfaces).
  - `port`: An integer specifying the port number (e.g., 8080).

- **Usage Example:**
  ```cpp
  server.listen("0.0.0.0", 8080);
  ```

#### `void get(const std::string& path, const request_handler& handler);`
Registers a handler function for HTTP GET requests on the specified path.

- **Parameters:**
  - `path`: A string specifying the URL path (e.g., "/").
  - `handler`: A function that takes a `const shttp::request&` and a `shttp::response&` and processes the request.

- **Usage Example:**
  ```cpp
  server.get("/", [](const shttp::request& req, shttp::response& res) {
      res.set_content("Hello, World!", "text/plain");
  });
  ```

#### `void post(const std::string& path, const request_handler& handler);`
Registers a handler function for HTTP POST requests on the specified path.

- **Parameters:**
  - `path`: A string specifying the URL path (e.g., "/submit").
  - `handler`: A function that takes a `const shttp::request&` and a `shttp::response&` and processes the request.

- **Usage Example:**
  ```cpp
  server.post("/submit", [](const shttp::request& req, shttp::response& res) {
      res.set_content("Data received", "text/plain");
  });
  ```

## shttp::request

### Properties

#### `std::string method;`
The HTTP method of the request (e.g., "GET", "POST").

#### `std::string path;`
The URL path of the request.

#### `std::unordered_map<std::string, std::string> headers;`
A map of the request headers.

#### `std::string body;`
The body content of the request.

## shttp::response

### Methods

#### `void set_content(const std::string& content, const std::string& content_type);`
Sets the content and content type for the response.

- **Parameters:**
  - `content`: A string specifying the response body content.
  - `content_type`: A string specifying the MIME type of the response (e.g., "text/plain", "application/json").

- **Usage Example:**
  ```cpp
  res.set_content("Hello, World!", "text/plain");
  ```
