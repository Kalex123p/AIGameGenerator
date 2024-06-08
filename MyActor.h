#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "MyActor.generated.h"

UCLASS()
class MYPROJECT3_API AMyActor : public AActor
{
    GENERATED_BODY()

public:    
    // Sets default values for this actor's properties
    AMyActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void StartTCPServer();
    void HandleClient(FSocket* ClientSocket);
    
private:
    FSocket* ServerSocket;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
