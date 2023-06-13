#include "Client.h"

int main()
{
    std::cout << "CLIENT IS ESTABLISHING A CONNECTION WITH SERVER THROUGH PORT: " << PORT << std::endl;
    sendRequest();
    return 0;
}