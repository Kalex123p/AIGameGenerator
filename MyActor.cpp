#include "MyActor.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Engine/Engine.h"

AMyActor::AMyActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    StartTCPServer();
}

void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMyActor::StartTCPServer()
{
    FIPv4Address::Any;
    FIPv4Endpoint Endpoint(FIPv4Address::Any, 7777);
    ServerSocket = FTcpSocketBuilder(TEXT("MyTCPSocket"))
        .AsReusable()
        .BoundToEndpoint(Endpoint)
        .Listening(8);

    if (ServerSocket)
    {
        UE_LOG(LogTemp, Warning, TEXT("TCP Server started successfully."));
        // Add your handling code here
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to start TCP Server."));
    }
}

void AMyActor::HandleClient(FSocket* ClientSocket)
{
    if (!ClientSocket) return;

    uint32 DataSize;
    while (ClientSocket->Recv(reinterpret_cast<uint8*>(&DataSize), sizeof(DataSize), ESocketReceiveFlags::WaitAll))
    {
        TArray<uint8> ReceivedData;
        ReceivedData.SetNumUninitialized(DataSize);

        int32 BytesRead;
        ClientSocket->Recv(ReceivedData.GetData(), DataSize, BytesRead);

        // Handle received data
        FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
        UE_LOG(LogTemp, Warning, TEXT("Received Data: %s"), *ReceivedString);

        // Send response
        FString Response = TEXT("Message received");
        TArray<uint8> ResponseData;
        ResponseData.SetNumUninitialized(Response.Len());
        memcpy(ResponseData.GetData(), TCHAR_TO_ANSI(*Response), Response.Len());

        ClientSocket->Send(ResponseData.GetData(), ResponseData.Num(), BytesRead);
    }

    ClientSocket->Close();
}

void AMyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (ServerSocket)
    {
        ServerSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ServerSocket);
    }
}
