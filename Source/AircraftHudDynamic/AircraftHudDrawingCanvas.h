// Copyright Odo

#pragma once 
  
#include <memory> 
//#include "Runtime/Core/Public/HAL/Platform.h"
//#include "Runtime/RHI/Public/RHI.h"
#include "HAL/Platform.h"
#include "Public/RHI.h"
#include "Engine/Texture2D.h" 
#include "Object.h" 
#include "AircraftHudDrawingCanvas.generated.h" 
  
UCLASS(Blueprintable, BlueprintType) 
class UAircraftHudDrawingCanvas : public UObject 
{ 
        GENERATED_BODY() 
  
public: 
  

  
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void InitializeDrawingCanvas(const int32 pixelsH, const int32 pixelsV); 

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                UTexture2D* GetDrawingCanvas(); 
                
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void UpdateDrawingCanvas(); 
                
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void ClearDrawingCanvas(); 

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void FillDrawingCanvas(FColor FillColor); 
                
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void InitializeDrawingTools(const int32 inBrushRadius, FColor inBrushColor); 

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawDot(const int32 pixelCoordX, const int32 pixelCoordY, bool bAutoUpdate = true); 

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawPixel(const int32 pixelCoordX, const int32 pixelCoordY, bool bAutoUpdate = true); 
  
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void SetColor(FColor inBrushColor);

        UAircraftHudDrawingCanvas(); 
        //~UAircraftHudDrawingCanvas(); 
  
private: 
  
        // canvas 
        UPROPERTY() 
                UTexture2D* dynamicCanvas; 
        std::unique_ptr<FUpdateTextureRegion2D> echoUpdateTextureRegion; 

        std::unique_ptr<uint8[]> canvasPixelData; 
        int canvasWidth; 
        int canvasHeight; 
        int bytesPerPixel; 
        int bufferPitch; 
        int bufferSize; 
  
        
        void setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha); 
        
        // draw brush tool 
        std::unique_ptr<uint8[]> canvasBrushMask; 
        int brushRadius; 
        int brushBufferSize; 
        FColor brushColor;
        
}; 



