// Copyright Odo

#include "AircraftHudDrawingCanvas.h" 

// REWORK FOR MODULE
#include "AircraftHudDynamic.h"
#include "Modules/ModuleManager.h"


UAircraftHudDrawingCanvas::UAircraftHudDrawingCanvas()
{

}


void UAircraftHudDrawingCanvas::InitializeDrawingCanvas(const int32 pixelsH, const int32 pixelsV) 
{ 
        //dynamic texture initialization 
        canvasWidth = pixelsH; 
        canvasHeight = pixelsV; 
  
        dynamicCanvas = UTexture2D::CreateTransient(canvasWidth, canvasHeight); 
#if WITH_EDITORONLY_DATA 
        dynamicCanvas->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps; 
#endif 
        dynamicCanvas->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap; 
        dynamicCanvas->SRGB = 0; 
        dynamicCanvas->AddToRoot(); 
        dynamicCanvas->Filter = TextureFilter::TF_Nearest; 
        dynamicCanvas->UpdateResource(); 
  
        echoUpdateTextureRegion = std::unique_ptr<FUpdateTextureRegion2D>(new FUpdateTextureRegion2D(0, 0, 0, 0, canvasWidth, canvasHeight)); 
  
         // buffers initialization 
        bytesPerPixel = 4; // r g b a 
        bufferPitch = canvasWidth * bytesPerPixel; 
        bufferSize = canvasWidth * canvasHeight * bytesPerPixel; 
        canvasPixelData = std::unique_ptr<uint8[]>(new uint8[bufferSize]); 
  
        ClearDrawingCanvas(); 
} 

UTexture2D * UAircraftHudDrawingCanvas::GetDrawingCanvas()
{
        return dynamicCanvas;
}


void UAircraftHudDrawingCanvas::setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha) 
{ 
        *pointer = blue; //b 
        *(pointer + 1) = green; //g 
        *(pointer + 2) = red; //r 
        *(pointer + 3) = alpha; //a 
} 


void UAircraftHudDrawingCanvas::ClearDrawingCanvas() 
{ 
        FillDrawingCanvas(FColor::White);
} 


void UAircraftHudDrawingCanvas::FillDrawingCanvas(FColor FillColor) 
{ 
        uint8* canvasPixelPtr = canvasPixelData.get(); 
        for (int i = 0; i < canvasWidth * canvasHeight; ++i) 
        { 
                setPixelColor(canvasPixelPtr, FillColor.R, FillColor.G, FillColor.B, FillColor.A);  
                canvasPixelPtr += bytesPerPixel; 
        } 
        UpdateDrawingCanvas(); 
} 


void UAircraftHudDrawingCanvas::UpdateDrawingCanvas() 
{ 
        if (echoUpdateTextureRegion) 
        { 
                dynamicCanvas->UpdateTextureRegions((int32)0, (uint32)1, echoUpdateTextureRegion.get(), (uint32)bufferPitch, (uint32)bytesPerPixel, canvasPixelData.get()); 
        } 
} 

void UAircraftHudDrawingCanvas::InitializeDrawingTools(const int32 inBrushRadius, FColor inBrushColor) 
{ 

        brushRadius = inBrushRadius; 
        brushColor = inBrushColor;
        brushBufferSize = brushRadius * brushRadius * 4 * bytesPerPixel; //2r*2r * bpp 
        canvasBrushMask = std::unique_ptr<uint8[]>(new uint8[brushBufferSize]); 
        uint8* canvasBrushPixelPtr = canvasBrushMask.get(); 
        for (int px = -brushRadius; px < brushRadius; ++px) 
        { 
                for (int py = -brushRadius; py < brushRadius; ++py) 
                { 
                        int32 tx = px + brushRadius; 
                        int32 ty = py + brushRadius; 
                        canvasBrushPixelPtr = canvasBrushMask.get() + (tx +  + ty * 2 * brushRadius) * bytesPerPixel; 
                        if (px*px + py*py < brushRadius*brushRadius) 
                        { 
                                setPixelColor(canvasBrushPixelPtr, brushColor.R, brushColor.G, brushColor.B, 255); //black alpha 255 - bgra 
                        } 
                        else 
                        { 
                                setPixelColor(canvasBrushPixelPtr, 0, 0, 0, 0); // alpha 0 
                        } 
                } 
        } 
} 


void UAircraftHudDrawingCanvas::DrawDot(const int32 pixelCoordX, const int32 pixelCoordY, bool bAutoUpdate) 
{ 
        uint8* canvasPixelPtr = canvasPixelData.get(); 
        const uint8* canvasBrushPixelPtr = canvasBrushMask.get(); 
        for (int px = -brushRadius; px < brushRadius; ++px) 
        { 
                for (int py = -brushRadius; py < brushRadius; ++py) 
                { 
                        int32 tbx = px + brushRadius; 
                        int32 tby = py + brushRadius; 
                        canvasBrushPixelPtr = canvasBrushMask.get() + (tbx + tby * 2* brushRadius) * bytesPerPixel; 
                        if (*(canvasBrushPixelPtr + 3) == 255) // check the alpha value of the pixel of the brush mask 
                        { 
                                int32 tx = pixelCoordX + px; 
                                int32 ty = pixelCoordY + py; 
                                if (tx >= 0 && tx < canvasWidth && ty >= 0 && ty < canvasHeight) 
                                { 
                                        canvasPixelPtr = canvasPixelData.get() + (tx + ty * canvasWidth) * bytesPerPixel; 
                                        setPixelColor(canvasPixelPtr, *(canvasBrushPixelPtr + 2), *(canvasBrushPixelPtr + 1), *(canvasBrushPixelPtr), *(canvasBrushPixelPtr + 3)); 
                                } 
                        } 
                } 
        } 
        if (bAutoUpdate)
                UpdateDrawingCanvas(); 
}

void UAircraftHudDrawingCanvas::DrawPixel(const int32 pixelCoordX, const int32 pixelCoordY, bool bAutoUpdate) 
{
        uint8* canvasPixelPtr; 
        if (pixelCoordX >= 0 && pixelCoordX < canvasWidth && pixelCoordY >= 0 && pixelCoordY < canvasHeight) 
        { 
                canvasPixelPtr = canvasPixelData.get() + (pixelCoordX + pixelCoordY * canvasWidth) * bytesPerPixel; 
                setPixelColor(canvasPixelPtr, brushColor.R, brushColor.G, brushColor.B, brushColor.A); 
        }
        if (bAutoUpdate)
                UpdateDrawingCanvas();  

}