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
  
        ptrUpdateTextureRegion = std::unique_ptr<FUpdateTextureRegion2D>(new FUpdateTextureRegion2D(0, 0, 0, 0, canvasWidth, canvasHeight)); 
  
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


void UAircraftHudDrawingCanvas::setPixelColor(uint8*& pointer, uint8 red, uint8 green, uint8 blue, uint8 alpha) 
{ 
        *pointer = blue; //b 
        *(pointer + 1) = green; //g 
        *(pointer + 2) = red; //r 
        *(pointer + 3) = alpha; //a 
} 

void UAircraftHudDrawingCanvas::setPixel(uint8*& pointer) 
{ 
        *pointer = brushColor.B; //b 
        *(pointer + 1) = brushColor.G; //g 
        *(pointer + 2) = brushColor.R; //r 
        *(pointer + 3) = brushColor.A; //a 
} 

void UAircraftHudDrawingCanvas::UpdateDrawingCanvas() 
{ 
        if (echoUpdateTextureRegion) 
        { 
            dynamicCanvas->UpdateTextureRegions((int32)0, (uint32)1, echoUpdateTextureRegion.get(), (uint32)bufferPitch, (uint32)bytesPerPixel, canvasPixelData.get()); 
        } 
} 



/*---------------------------------*/ 
// DRAWING TOOLS 
/*---------------------------------*/ 


void UAircraftHudDrawingCanvas::SetColor(FColor inBrushColor) 
{ 
        brushColor = inBrushColor; 
} 


void UAircraftHudDrawingCanvas::DrawPoint(int x0, int y0) 
{ 
        uint8* canvasPixelPtr; 
        if (x0 >= 0 && x0 < canvasWidth && y0 >= 0 && y0 < canvasHeight) 
        { 
                canvasPixelPtr = canvasPixelData.get() + (x0 + y0 * canvasWidth) * bytesPerPixel; 
                setPixel(canvasPixelPtr); 
        } 
} 


void UAircraftHudDrawingCanvas::DrawPoint(int x0, int y0, FColor c) 
{ 
        uint8* canvasPixelPtr; 
        if (x0 >= 0 && x0 < canvasWidth && y0 >= 0 && y0 < canvasHeight) 
        { 
                canvasPixelPtr = canvasPixelData.get() + (x0 + y0 * canvasWidth) * bytesPerPixel; 
                setPixelColor(canvasPixelPtr, c.R, c.G, c.B, c.A); 
        } 
} 


void UAircraftHudDrawingCanvas::DrawLine(int x0, int y0, int x1, int y1) 
{ 
        int dx =  abs(x1-x0);
        int sx = x0<x1 ? 1 : -1;
        int dy = -abs(y1-y0);
        int sy = y0<y1 ? 1 : -1; 
        int err = dx+dy;
        int e2; /* error value e_xy */
 
        for(;;)
        {  /* loop */
                DrawPoint(FVector(x0, y0, 0));
                if (x0==x1 && y0==y1) break;
                e2 = 2*err;
                if (e2 >= dy) 
                { 
                        err += dy; 
                        x0 += sx; 
                } /* e_xy+e_x > 0 */
                if (e2 <= dx) 
                { 
                        err += dx; 
                        y0 += sy; 
                } /* e_xy+e_y < 0 */
        }
} 



void UAircraftHudDrawingCanvas::DrawLine((int x0, int y0, int x1, int y1, float wd)
{ 
        int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1; 
        int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1; 
        int err = dx-dy, e2, x2, y2;                          /* error value e_xy */
        float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
   
        for (wd = (wd+1)/2; ; ) 
        {                                   /* pixel loop */
                DrawPoint(x0,y0, FColor(brushColor.R, brushColor.G, brushColor.B, max(0,255*(abs(err-dx+dy)/ed-wd+1))));
                e2 = err; x2 = x0;
                if (2*e2 >= -dx) 
                {                                           /* x step */
                        for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx)
                                DrawPoint(x0, y2 += sy, FColor(brushColor.R, brushColor.G, brushColor.B, max(0,255*(abs(e2)/ed-wd+1))));
                        if (x0 == x1) 
                                break;
                        e2 = err; err -= dy; x0 += sx; 
                } 
                if (2*e2 <= dy) 
                {                                            /* y step */
                        for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy)
                                DrawPoint(x2 += sx, y0, FColor(brushColor.R, brushColor.G, brushColor.B, max(0,255*(abs(e2)/ed-wd+1))));
                        if (y0 == y1) 
                                break;
                        err += dx; y0 += sy; 
                }
        }
}



void UAircraftHudDrawingCanvas::DrawCircle(int xm, int ym, int r) 
{
        int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
        do 
        {
                DrawDot(xm-x, ym+y); /*   I. Quadrant */
                DrawDot(xm-y, ym-x); /*  II. Quadrant */
                DrawDot(xm+x, ym-y); /* III. Quadrant */
                DrawDot(xm+y, ym+x); /*  IV. Quadrant */
                r = err;
                if (r <= y) 
                        err += ++y*2+1;           /* e_xy+e_y < 0 */
                if (r > x || err > y) 
                        err += ++x*2+1; /* e_xy+e_x > 0 or no 2nd y-step */
        } while (x < 0);
}



/*---------------------------------*/ 
// DRAWING PRIMITIVES OPERATIONS 
/*---------------------------------*/ 


void UAircraftHudDrawingCanvas::AddPoint(const int layer, const int32 px, const int32 py)
{
        DrawingLayers[layer].Add(FDrawingPrimitive(EDrawingType::DT_Point, px, py));
}

void UAircraftHudDrawingCanvas::AddPoint(const int layer, const int32 px, const int32 py, float wd)
{
        DrawingLayers[layer].Add(FDrawingPrimitive(EDrawingType::DT_ThickPoint, px, py, wd));
}

void UAircraftHudDrawingCanvas::AddLine(const int32 p0x, const int32 p0y, const int32 p1x, const int p1y)
{
        DrawingLayers[layer].Add(FDrawingPrimitive(EDrawingType::DT_Line, p0x, p0y, p1x, p1y));
}

void UAircraftHudDrawingCanvas::AddLine(const int32 p0x, const int32 p0y, const int32 p1x, const int p1y, float wd)
{
        DrawingLayers[layer].Add(FDrawingPrimitive(EDrawingType::DT_ThickLine, p0x, p0y, p1x, p1y, wd));
}

void UAircraftHudDrawingCanvas::AddCircle(const int layer, const int32 mx, const int32 my, float r)
{
        DrawingLayers[layer].Add(FDrawingPrimitive(EDrawingType::DT_Circle, mx, my, r));
}

void UAircraftHudDrawingCanvas::AddBrush(const int layer, const int32 mx, const int32 my, float r)
{
        DrawingLayers[layer].Add(FDrawingPrimitive(EDrawingType::DT_Brush, mx, my, r));
}


/*---------------------------------*/ 
// DRAWING LAYERS OPERATIONS 
/*---------------------------------*/ 


void UAircraftHudDrawingCanvas::AddLayer()
{
        DrawingLayers.Add(FDrawingLayer());
}


int UAircraftHudDrawingCanvas::GetNumLayers()
{
        return DrawingLayers.Num();
}


void UAircraftHudDrawingCanvas::TransformLayer(int32 l, int32 tx, int32 ty, int32 mx, int 32 my, float rad, bool vis)
{
}


void UAircraftHudDrawingCanvas::DrawLayers()
{
        for(int32 i = 0; i < DrawingLayers.Num(); i++)
        {
                DrawLayer(i);
        }
}

void UAircraftHudDrawingCanvas::DrawLayer(int32 l)
{
        for(int32 k = 0; k < DrawingLayers[l].DrawingPrimitives.Num(); k++)
        {
                FDrawingPrimitive dp = DrawingLayers[l].DrawingPrimitives[k];
                switch (dp.dtype)
                {
                        case DT_Point:
                        break;
                        case DT_ThickPoint:
                        break;
                        case DT_Line:
                        break;
                        case DT_ThickLine:
                        break;
                        case DT_Circle:
                        break;
                        case DT_Brush:
                        break;
                        default:
                        break;
                }
        }
}













void UAircraftHudDrawingCanvas::RotatePoint(const int32 px, const int32 py, const float rad, const int ox, const int oy)
{
        float s = sin(-rad); // angle is in radians 
        float c = cos(-rad); // angle is in radians 

        int rx = c * (px-ox) + d * s * (py-oy) + ox; 
        int ry = s * (px-ox) + c * (py-oy) + oy;


        DrawPixel(rx, ry);
} 






void UAircraftHudDrawingCanvas::SetBrush(const int32 inBrushRadius, FColor inBrushColor) 
{ 
        brushRadius = inBrushRadius; 
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
                                setPixelColor(canvasBrushPixelPtr, inBrushColor.R, inBrushColor.G, inBrushColor.B, inBrushColor.A);   
                        } 
                        else 
                        { 
                                setPixelColor(canvasBrushPixelPtr, 0, 0, 0, 0); // alpha 0 
                        } 
                } 
        } 
} 


void UAircraftHudDrawingCanvas::PaintBrush(const int32 x, const int32 y) 
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
                        if (*(canvasBrushPixelPtr + 3) > 0) // check the alpha value of the pixel of the brush mask 
                        { 
                                int32 tx = x + px; 
                                int32 ty = y + py; 
                                if (tx >= 0 && tx < canvasWidth && ty >= 0 && ty < canvasHeight) 
                                { 
                                        canvasPixelPtr = canvasPixelData.get() + (tx + ty * canvasWidth) * bytesPerPixel; 
                                        setPixelColor(canvasPixelPtr, *(canvasBrushPixelPtr + 2), *(canvasBrushPixelPtr + 1), *(canvasBrushPixelPtr), *(canvasBrushPixelPtr + 3)); 
                                } 
                        } 
                } 
        } 
} 


