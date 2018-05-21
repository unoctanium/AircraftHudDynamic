// Copyright: Odo

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Core/Public/HAL/Platform.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h" 


#include "AircraftHudDrawingCanvas.h" 
#include "AircraftHudComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) 
class UAircraftHudComponent : public UStaticMeshComponent
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
        
        UAircraftHudComponent(); 
        
protected: 

		virtual void InitializeComponent() override;
		virtual void BeginPlay() override;
        virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
        
private: 

        TArray<class UMaterialInstanceDynamic*> mDynamicMaterials; 
        
        // HudImplementation 
        
        float mDeltaTimeDt; 

        // Test

        int mDrawingTest;
	
	
};
