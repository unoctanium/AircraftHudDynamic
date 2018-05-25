// Copyright Odo
#include "AircraftHudActor.h" 

// REWORK FOR MODULE
#include "AircraftHudDynamic.h"
#include "Modules/ModuleManager.h"


AAircraftHudActor::AAircraftHudActor() 
{ 
        PrimaryActorTick.bCanEverTick = true; 
        //TextureSize = 256; 



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



void AAircraftHudActor::BeginPlay() 
{ 
        Super::BeginPlay(); 
} 

void AAircraftHudActor::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{ 
        Super::EndPlay(EndPlayReason); 
} 

void AAircraftHudActor::PostInitializeComponents() 
{ 
        Super::PostInitializeComponents(); 
        
        SetupCanvas(); 
} 


void AAircraftHudActor::SetupCanvas() 
{ 

        HudDrawingCanvasMesh = GetStaticMeshComponent();


        // Create the drawing canvas (the dynamic texture) to paint on in
        HudDrawingCanvas = NewObject<UAircraftHudDrawingCanvas>();

        HudDrawingCanvas->InitializeDrawingCanvas(TextureSize, TextureSize); 
        HudDrawingCanvas->SetBrush(4, FColor::Blue); 

        mDynamicMaterials.Empty(); 
        mDynamicMaterials.Add(HudDrawingCanvasMesh->CreateAndSetMaterialInstanceDynamic(0)); 
        mDynamicMaterials[0]->SetTextureParameterValue(TEXT("DynamicTexture"), HudDrawingCanvas->GetDrawingCanvas()); 
       
        SetActorRelativeScale3D(FVector(TextureSize / 100.0f, TextureSize / 100.0f, 1)); 
} 


void AAircraftHudActor::Tick(float DeltaTime) 
{ 
        Super::Tick(DeltaTime); 

        mDeltaTimeDt += DeltaTime; 
        
        if (mDeltaTimeDt > 0.02) 
        { 
                mDeltaTimeDt = 0.0f; 

                for (int u=0; u<256;u++)
                        HudDrawingCanvas->DrawPoint(u, mDrawingTest);         
                mDrawingTest += 1;
                mDrawingTest %= 256;
               
                UpdateCanvas(); 
        } 

        //UE_LOG(LogTemp, Warning, TEXT("ActorTick"));
} 


void AAircraftHudActor::UpdateCanvas() 
{ 
        HudDrawingCanvas->UpdateDrawingCanvas(); 
} 


