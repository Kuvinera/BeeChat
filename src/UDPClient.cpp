// UDPClient.cpp
#include "UDPClient.h"

//------------------------------------------------------------------
// Name: UDPClient
// Desc: 
//------------------------------------------------------------------
UDPClient::UDPClient()
{
	hostent* pHost; 
	pHost = (hostent*) gethostbyname( (char*) "localhost" ); 

	// htonl and htons convert ints into 'network representations'
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_addr.s_addr = *((in_addr_t*)pHost->h_addr); 
	serverAddress.sin_port = htons(port); 
}
//------------------------------------------------------------------
// Name: ~UDPClient
// Desc: 
//------------------------------------------------------------------
UDPClient::~UDPClient()
{

}
//------------------------------------------------------------------
// Name: StartClient
// Desc: 
//------------------------------------------------------------------
void UDPClient::StartClient()
{
	// get a socket
	clientSocket = socket( AF_INET, SOCK_DGRAM, 0 ); 

	if ( clientSocket == -1 ) {
		std::cout << "socket() error" << std::endl; 
		return false; 
	}

	return true; 
}
//------------------------------------------------------------------
// Name: StopClient
// Desc: 
//------------------------------------------------------------------
void UDPClient::StopClient()
{
	// close socket
	close( clientSocket ); 
}
//------------------------------------------------------------------
// Name: SendToServer
// Desc: 
//------------------------------------------------------------------
void UDPClient::SendToServer( char* message )
{
	//TODO: are we sure a server exists? 

	if ( sendto( clientSocket, message, MESSAGE_LENGTH, 0, (struct sockaddr*)&serverAddress, sizeof(struct sockaddr) ) == -1 ) {
		//std::cout << "sendto() error" << std::endl; 
		// error
	} 
}
//------------------------------------------------------------------
// Name: IsUnreadMessages
// Desc: 
//------------------------------------------------------------------
bool UDPClient::IsUnreadMessages()
{
	pthread_mutex_lock( & )
}
//------------------------------------------------------------------
// Name: GetLatestMessages
// Desc: messages are always  
//------------------------------------------------------------------
void UDPClient::GetLatestMessages(char* message)
{
	// wait until the listener thread is done with the queue 
	pthread_mutex_lock( &messageQueueMutex ); 

	if ( !messageQueue.empty() ) {

		memcpy( (void*)messageQueue, (void*)messageQueue.front(), MESSAGE_LENGTH ); 
		messageQueue.pop(); 
	}

	pthread_mutex_unlock( &messageQueueMutex ); 
}
//---------------------------------------------------------------------------
// Name: ListenForMessage
// Desc:
//---------------------------------------------------------------------------
void* UDPClient::ListenForMessage( void* threadId )
{
	struct sockaddr_in clientAddress; 
	char* messageBuffer = new char[MESSAGE_LENGTH]; 

	while ( 1 ) {

		// Check if we've been asked to quit
		pthread_mutex_lock( &listenerMutex ); 

		if ( stopListening ) {
			
			break;  
		}

		pthread_mutex_unlock( &listenerMutex );

		memset( (void*)messageBuffer, 0, MESSAGE_LENGTH ); 

		// don't lock until after we've received a message!
		// socket could be a different length when it comes back? 
		int receivedBytes = recvfrom( serverSocket, messageBuffer, MESSAGE_LENGTH, 0, (struct sockaddr*)&clientAddress, &sockLen ); 
		

		if ( receivedBytes == -1 ) {
			// error
			// help help help what do i do 

		} else {

			pthread_mutex_lock( &messageQueueMutex ); 

			// ClientMessage makes it's own copy of the messageBuffer
			messageQueue.push( ClientMessage(clientAddress, messageBuffer) ); 

			pthread_mutex_unlock( &messageQueueMutext );

		}
	}

	delete messageBuffer; 

	pthread_exit(0); 
}


