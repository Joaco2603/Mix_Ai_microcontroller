#ifndef SERVER_H
#define SERVER_H

class MyWebServer {
public:
  void start();

private:
  void initializeWebSockets();
};

#endif
