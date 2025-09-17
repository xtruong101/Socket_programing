#include <iostream>
#include <cstring>
#include <string>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <fstream>
#include <string>

using namespace std;
char buffer[1024];


int main() {
    // int argc, char *argv[]
    // if (argc != 3) cout << "Wrong syntax!\n";
    // string state = argv[1];
    // string id = argv[2];
    // string passwd = argv[3];
    //-------------------//
    sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int serverAddrLen = sizeof(serverAddr);
    int iResult;
    //khoi tao
    WORD wVersion = MAKEWORD(2,2);
    WSADATA WSAData;
    int start;
    if ( (start = WSAStartup(wVersion, &WSAData)) != 0) {
        cout << "Startup failed with code: " << WSAGetLastError() << endl;
    }
    else cout << "Startup done!\n";
    
    SOCKET clt_skt = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clt_skt > 0) {
        cout << "Creating socket done!\n";
    }      
    else cout << "Creating socket failed with code " << WSAGetLastError() << endl;
    // specifying address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //connect
    int connect_status = connect(clt_skt, (sockaddr*)&serverAddr, serverAddrLen);
    if (connect_status == SOCKET_ERROR) {
        cout << "Connect failed with code " << WSAGetLastError() << endl;
    }
    else cout << "Connect done!\n";
    
    while (true)
    {
        string statement;
        cout << "Enter statement: ";
        getline(cin, statement);
        iResult = send(clt_skt, statement.c_str(), sizeof(statement), 0);
        if (iResult == SOCKET_ERROR) {
            cout << "Send failed with code " << WSAGetLastError() << endl;
        }
        else cout << "Send done!\n";  
        iResult == recv(clt_skt, buffer, size(buffer), 0);    
        if (iResult == SOCKET_ERROR) {
            cout << "Cannot recive! Error code: " << WSAGetLastError() << endl;
        }
        else cout << "Recive done! From server: " << buffer << endl;
    }
    closesocket(clt_skt);
    return 0;
}