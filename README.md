# Description
This is a studying project from Codam Core Curriculum and is all about writing one's own HTTP server.
It requires understanding some basic networking stuff (socket management, binding), good understanding of I/O streams and plug-in architecture to add CGI.

## Requirements: 
- HTTP server must be written in C++ 98. :unamused:
- The program has to take a configuration file as argument, or use a default path.
- The server must never block and the client can be bounced properly if necessary.
- It must be non-blocking.
- A request to the server should never hang forever.
- The server must be compatible with the web browser of your choice.
- You server must have default error pages if none are provided.
- You can’t use fork for something else than CGI (like PHP, or Python, and so forth).
- You must be able to serve a fully static website.
- Clients must be able to upload files.
- You need at least GET, POST, and DELETE methods.
- Your server must be able to listen to multiple ports.

# Structure
The general structure of the project is the following:

![](https://github.com/Mollie-S/webserver/blob/main/docs/diagrams/webserver_structure.png)

***Kqueue*** system call is used for I/O Multiplexing. 

# How to run the program
Go to the root of the project and type:

`make && ./webserver`

This will start the webserver with the default configuration.
You can open localhost in your browser (tested with Chrome and Safari) or start sending http requests through Postman.

The project is created in successful collaboration between:
- [Xiaojing](https://github.com/ccxxj)
- [Irem](https://github.com/ergunirem)
- [Olga](https://github.com/olsamar)
