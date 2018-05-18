// Copyright Odo
#include "AircraftHudCanvasActor.h" 

// REWORK FOR MODULE
#include "AircraftHudDynamic.h"
#include "Modules/ModuleManager.h"


AAircraftHudCanvasActor::AAircraftHudCanvasActor() 
{ 
        PrimaryActorTick.bCanEverTick = true; 
        //TextureSize = 256; 

        // Create the drawing canvas (the dynamic texture) to paint on in
        HudDrawingCanvas = NewObject<UAircraftHudDrawingCanvas>();

        // Create the static mesh to apply the dynamic texture to it
        HudDrawingCanvasMesh = GetStaticMeshComponent();


        //HudDrawingCanvasMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HudDrawingCanvasMesh")); 
        //HudDrawingCanvasMesh->AttachParent = (USceneComponent*)this;
        ConstructorHelpers::FObjectFinder<UStaticMesh>MeshRef(TEXT("/Game/AircraftHudDynamic/Mesh_AircraftHudCanvas")); 
        //if (MeshRef.Obj != nullptr)
        if (MeshRef.Succeeded())
        {
            HudDrawingCanvasMesh->SetStaticMesh(MeshRef.Object); 
            //HudDrawingCanvasMesh->AddToRoot();
        }

} 



void AAircraftHudCanvasActor::BeginPlay() 
{ 
        Super::BeginPlay(); 
} 

void AAircraftHudCanvasActor::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{ 
        Super::EndPlay(EndPlayReason); 
} 

void AAircraftHudCanvasActor::PostInitializeComponents() 
{ 
        Super::PostInitializeComponents(); 
        
        SetupCanvas(); 
} 


void AAircraftHudCanvasActor::SetupCanvas() 
{ 

        HudDrawingCanvasMesh = GetStaticMeshComponent();

        //HudDrawingCanvas->InitializeDrawingCanvas(TextureSize, TextureSize); 
        //HudDrawingCanvas->InitializeDrawingTools(10, FColor::Blue); 

        //mDynamicMaterials.Empty(); 
        //mDynamicMaterials.Add(GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0)); 
        //mDynamicMaterials[0]->SetTextureParameterValue(TEXT("DynamicTexture"), HudDrawingCanvas->GetDrawingCanvas()); 
       
        //SetActorRelativeScale3D(FVector(TextureSize / 100, TextureSize / 100, 1)); 
} 


void AAircraftHudCanvasActor::Tick(float DeltaTime) 
{ 
        Super::Tick(DeltaTime); 

        mDeltaTimeDt += DeltaTime; 
        
        if (mDeltaTimeDt > 0.05) 
        { 
                mDeltaTimeDt = 0.0f; 
                HudDrawingCanvas->DrawDot(100,100); 
                UpdateCanvas(); 
        } 
} 


void AAircraftHudCanvasActor::UpdateCanvas() 
{ 
        HudDrawingCanvas->UpdateDrawingCanvas(); 
} 


