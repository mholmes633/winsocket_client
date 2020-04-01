// winsocket_client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    string ipAddress = "192.168.1.51";     // IP address of the server
    int port = 8080;                   // listening port # on the server

    // initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "can't start Winsock, Err #" << wsResult << endl;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // to use IP version 6 AF_INET6 otherwise using AF_INET which is IP version 4
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
        WSACleanup();
    }

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // Do-while loop to send and receive data
    char buf[4096];
    string userInput;
    bool loop = 1;

    while(loop)
    {
        // prompt the user for some text
        cout << "> ";
        getline(cin, userInput);

        if (strcmp(userInput.c_str(), "quit") == 0)
            loop = false;

        if (userInput.size() > 0) // make sure user has typed in something
        {
            // Send the text
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR)
            {
                // wait for response
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0)
                {
                    // echo response to consol
                    cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }
    }

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
