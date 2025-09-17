#include <iostream>
#include <cstring>
#include <string>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <fstream>
#include <string>
#include <cstring>
#include "json.hpp"
using json = nlohmann::json;

using namespace std;
char buffer[1024];
#define bufferSize 1024

//Ham check thong tin
bool verifyUser(string &inputID, string &inputPwd) {
    //string &id, string &passwd
    ifstream file("D:\\FILE_HOC_TREN_TRUONG\\LapTrinhMang_Code_n_Lecture\\network_programming_code\\user.json");
    if ( file.is_open()) cout << "Open data done!\nChecking...";
    else cout << "Cannot access data!\n";
    json data;
    file >> data;
    for (auto&user : data) {
        if (user["id"] == inputID && user["password"] ==  inputPwd) {
            cout << "Login done!\n";
            return 1;
        }
    }
    cout << "Login failed!\n";   
    file.close();
    return 0;
}

//ham tach tung thanh phan nhan tu client va kiem tra thong tin
//flag la trang thai dang nhap y/n
void messageProcess(SOCKET s, string &message, bool &flag) {
    string command, id, passwd;
    istringstream dataString(message);
    dataString >> command >> id >> passwd;
    cout << "Command: " << command << endl;
    cout << "ID: " << id << endl;
    cout << "Password: " << passwd << endl;
    string mess;
    if (command == "LOGIN") {
        if (verifyUser(id, passwd)) flag = 1;
        
        mess = "Login done!\n";
        //send
        if(send(s, mess.c_str(), sizeof(mess), 0) == SOCKET_ERROR) cout << "Send failed, error code: " << WSAGetLastError() << endl;
        else cout << "Send done!\n";      
    }
    else if (command == "LOGOUT") {
        if (flag == 0) cout << "No login yet!\n";
        else {
            cout << "Logout done!\n"; flag = 0;
            mess = "Logout done!\n";
            //send
            if(send(s, mess.c_str(), sizeof(mess), 0) == SOCKET_ERROR) cout << "Send failed, error code: " << WSAGetLastError() << endl;
            else cout << "Send done!\n";      
        }
    }
    else {
        cout << "Undefined commnad!\n";
        mess = "Unknow command! Command must be LOGIN or LOGOUT\n";
        //send
        if(send(s, mess.c_str(), sizeof(mess), 0) == SOCKET_ERROR) cout << "Send failed, error code: " << WSAGetLastError() << endl;
        else cout << "Send done!\n"; 
    }
    cout << "Current state (1 is login done, 0 is not login!):  " << flag << endl;
}

int main(){
    SOCKET client_skt;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int serverAddrLen = sizeof(serverAddr);
    int iResult;
    //khoi tao 
    WORD wVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    int start;
    if((start = WSAStartup(wVersion, &wsaData)) != 0) {
        cout << "Startup failed with error code" << WSAGetLastError() << endl;
    }
    else {
        cout << "Startup completed!" << endl;
    }
    SOCKET sv_skt = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( sv_skt > 0) {
        cout << "Creating socket completed!" << endl;
    }
    else {
        cout << "Creating socket failed with code: " << WSAGetLastError() << endl;
    }
    //specifying address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    
    //bind
    int bind_status = bind(sv_skt, (sockaddr*)&serverAddr, serverAddrLen);
    if (bind_status == SOCKET_ERROR) {
        cout << "Bind failed with code: "<< WSAGetLastError() << endl;
    }
    else {
        cout << "Bind completed\n";
    }
    //listen
    int listen_status = listen(sv_skt, SOMAXCONN);
    if (listen_status == SOCKET_ERROR) {
        cout << "Listen failed with code: " << WSAGetLastError() << endl;
    }
    else {
        cout << "Server is listening request...wait\n"; 
    }
    //accept
    client_skt = accept(sv_skt, (sockaddr*)&clientAddr, &clientAddrLen);
    if (client_skt == SOCKET_ERROR) {
        cout << "Accept failed with code " << WSAGetLastError() << endl;
    }
    else {
        cout << "Accept done!\n";
    }
    // trang thai ban dau chua dang nhap 
    bool flag = 0;

    while (true)
    {   
        // recv
        iResult = recv(client_skt, buffer, bufferSize, 0);
        if (iResult > 0 ) {
            cout << "Client message: " << buffer << endl;
        }
        else cout << "Recive failed!\n";
        //verifyUser();
        string msgString(buffer);
        messageProcess(client_skt, msgString, flag);
    }
    closesocket(sv_skt);
    return 0;
}
