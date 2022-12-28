#include "../include/Client.hpp"

Client::Client(int clientFd) : _clientFd(clientFd), _regist(0) {}

Client::~Client() {}