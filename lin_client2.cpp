#include<iostream>
#include<string>
#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<string.h>
#include<sys/uio.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<fstream>
using namespace std;

int main(){
  int port = 3131;

  char msg[1500];

  struct hostent* host = gethostbyname((const char*)"v118-27-22-86.empr.static.cnode.io");
  sockaddr_in sendSockAddr;
  bzero((char*)&sendSockAddr,sizeof(sendSockAddr));
  sendSockAddr.sin_family = AF_INET;
  sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
  sendSockAddr.sin_port = htons(port);
  int clientSd = socket(AF_INET,SOCK_STREAM,0);

  int status = connect(clientSd,(sockaddr*)&sendSockAddr,sizeof(sendSockAddr));
  if(status < 0){
    cout << "Error connectng to socket!" << endl ;
    exit(1);
  }
  cout << "Connected to the server!" << endl;
  int bytesRead,bytesWritten = 0;
  struct timeval start1,end1;
  gettimeofday(&start1,NULL);
  while(1){
    cout << ">";
    string data;
    getline(cin,data);
    memset(&msg,0,sizeof(msg));
    strcpy(msg,data.c_str());
    if(data == "exit"){
      send(clientSd,(char*)&msg,strlen(msg),0);
      break;
    }
    bytesWritten += send(clientSd,(char*)&msg,strlen(msg),0);
    cout << "Awaiting server response..." << endl;
    memset(&msg,0,sizeof(msg));
    bytesRead += recv(clientSd,(char*)&msg,sizeof(msg),0);
    if(!strcmp(msg,"exit")){
      cout << "Server has quit the session" << endl;
      break;
    }
    cout << "Server:" << msg << endl;
  }
  gettimeofday(&end1,NULL);
  close(clientSd);
  cout << "session" << endl;
  cout << "Bytes Written:" << bytesWritten << " Bytes read:" << bytesRead << endl;
  cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << endl;
  cout << "Connection closed" << endl;

  return 0;
}
