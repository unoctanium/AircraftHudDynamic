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


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDrawingType : uint8
{
        DT_None 	UMETA(DisplayName="None"),
        DT_Point 	UMETA(DisplayName="Point"),
        DT_ThickPoint 	UMETA(DisplayName="ThickPoint"),
        DT_Line 	UMETA(DisplayName="Line"),        
        DT_ThickLine 	UMETA(DisplayName="ThickLine"),
	DT_Circle	UMETA(DisplayName="Circle"),
        DT_Brush	UMETA(DisplayName="Brush")
};



USTRUCT()
struct FDrawingPrimitive
{
	GENERATED_BODY()
 
        UPROPERTY()
                EDrawingType dtype;

	UPROPERTY()
	        int32 p0x;

	UPROPERTY()
	        int32 p0y;

	UPROPERTY()
	        int32 p1x;

	UPROPERTY()
	        int32 p1y;

	UPROPERTY()
	        float wd; // or r

        //Constructors
        FDrawingPrimitive(EdrawingType t_, int32 p0x_, int32 p0y_, int32 p1x_, int32 p1y_, float wd_) : dtype(t_), p0x(p0x_), p0y(p0y_), p1x(p1x_), p1y(p1y_), wd(wd_) {}
        FDrawingPrimitive(EdrawingType t_, int32 p0x_, int32 p0y_, int32 p1x_, int32 p1y_) : dtype(t_), p0x(p0x_), p0y(p0y_), p1x(p1x_), p1y(p1y_), wd(0) {}
        FDrawingPrimitive(EdrawingType t_, int32 p0x_, int32 p0y_, float wd_) : dtype(t_), p0x(p0x_), p0y(p0y_), p1x(0), p1y(0), wd(wd_) {}
        FDrawingPrimitive(EdrawingType t_, int32 p0x_, int32 p0y_) : dtype(t_), p0x(p0x_), p0y(p0y_), p1x(0), p1y(0), wd(0) {}
	FDrawingPrimitive() : dtype(EDrawingType::DT_None), p0x(0), p0y(0), p1x(0), p1y(0), wd(0) {}
};



USTRUCT()
 struct FDrawingLayer
 {
        GENERATED_BODY()
 
        UPROPERTY()
             TArray<FDrawingPrimitive> DrawingPrimitives;

        UPROPERTY()
                int32 tx;

        UPROPERTY()
                int32 ty;
        
        UPROPERTY()
                int32 rx;

        UPROPERTY()
                int32 ry;

        UPROPERTY()
                float rad;

        UPROPERTY()
                bool vis;

        //Constructor
	FDrawingLayer() : DrawingPrimitives(FDrawingPrimitive.Empty()) {}
 };



UCLASS(Blueprintable, BlueprintType) 
class UAircraftHudDrawingCanvas : public UObject 
{ 
        GENERATED_BODY() 
  
public: 
  
        // CANVAS
  
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

        // DRAWING

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void SetColor(FColor inBrushColor); 
        
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawPoint(int x0, int y0); 

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawPoint(int x0, int y0, FColor c);
        
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawLine(int x0, int y0, int x1, int y1); 

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawLine(int x0, int y0, int x1, int y1, float wd); 

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawCircle(int xm, int ym, int r);

        // PRIMITIVES

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void AddPoint(const int layer, const int32 px, const int32 py);

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void AddPoint(const int layer, const int32 px, const int32 py, float wd);
        
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void AddLine(const int32 p0x, const int32 p0y, const int32 p1x, const int p1y);

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void AddLine(const int32 p0x, const int32 p0y, const int32 p1x, const int p1y, float wd);

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void AddCircle(const int layer, const int32 mx, const int32 my, float r);

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
               void AddBrush(const int layer, const int32 x, const int32 y, float n);


        // LAYERS

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void AddLayer();

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                int GetNumLayers();

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void TransformLayer(int32 l, int32 tx, int32 ty, int32 mx, int 32 my, float rad, bool vis);

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawLayers();

        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void DrawLayer(int32 l);









        UFUNCTION(BlueprintCallable, Category = DrawingTools)
                void RotatePoint(const int32 x0, const int32 y0, const float rad);







        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void SetBrush(const int32 inBrushRadius, FColor inBrushColor); 
                                
        UFUNCTION(BlueprintCallable, Category = DrawingTools) 
                void PaintBrush(const int32 x, const int32 y); 
                        
        //UFUNCTION(BlueprintCallable, Category = DrawingTools) 
        //        void PaintLine(const int32 x0, const int32 y0, const int32 x1, const int32 y1); 
                

  
        UAircraftHudDrawingCanvas(); 
        //~UAircraftHudDrawingCanvas(); 
  
private: 
  
        // canvas 
        UPROPERTY() 
                UTexture2D* dynamicCanvas; 
        
        std::unique_ptr<FUpdateTextureRegion2D> ptrUpdateTextureRegion; 

        std::unique_ptr<uint8[]> canvasPixelData; 
        int canvasWidth; 
        int canvasHeight; 
        int bytesPerPixel; 
        int bufferPitch; 
        int bufferSize; 
  
        void setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha); 
        void setPixel(uint8*& pointer); 
        
        // draw brush tool 
        FColor brusholor; 

        std::unique_ptr<uint8[]> canvasBrushMask; 
        int brushRadius; 
        int brushBufferSize; 

        // Polypoint Container. Z =1 : Endpoint, Z=0: Start/Mid-Point
        TArray<FDrawingLayer> DrawingLayers;
        
}; 
