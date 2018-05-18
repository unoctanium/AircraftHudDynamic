// Copyright Odo

#pragma once 

#include "Runtime/Core/Public/HAL/Platform.h"
#include "Engine/StaticMeshActor.h" 
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h" 
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" 
//#include "ProceduralMesh/ProceduralMeshComponent.h"
#include "AircraftHudDrawingCanvas.h" 
#include "AircraftHudCanvasActor.generated.h" 

UCLASS(Blueprintable, BlueprintType) 
class AAircraftHudCanvasActor : public AStaticMeshActor 
{ 
        GENERATED_BODY() 

public: 
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AircraftHud) 
                UAircraftHudDrawingCanvas *HudDrawingCanvas; 

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes) 
                UStaticMeshComponent *HudDrawingCanvasMesh; 

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AircraftHud) 
                FName MaterialTextureParameter = TEXT("DynamicTexture");
                
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AircraftHud) 
                int32 TextureSize = (int32)256; 
        
        UFUNCTION(BlueprintCallable, Category = AircraftHud) 
                void SetupCanvas(); 
        
        UFUNCTION(BlueprintCallable, Category = AircraftHud) 
                void UpdateCanvas(); 
        
        AAircraftHudCanvasActor(); 
        
protected: 
        virtual void BeginPlay() override; 
        virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
        virtual void PostInitializeComponents() override; 
        virtual void Tick(float DeltaTime) override; 
        
private: 

        TArray<class UMaterialInstanceDynamic*> mDynamicMaterials; 
        
        // HudImplementation 
        
        float mDeltaTimeDt; 
}; 