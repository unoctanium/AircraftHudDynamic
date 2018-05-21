// Copyright Odo

#include "AircraftHudComponent.h"


// REWORK FOR MODULE
#include "AircraftHudDynamic.h"
#include "Modules/ModuleManager.h"

UAircraftHudComponent::UAircraftHudComponent() 
{ 

        //TextureSize = 256; 

        bWantsInitializeComponent = true;
        bWantsBeginPlay = true;
        bAutoActivate = true;

        // Create the static mesh to apply the dynamic texture to it
        //HudDrawingCanvasMesh = GetStaticMeshComponent();
        HudDrawingCanvasMesh = this;


        ConstructorHelpers::FObjectFinder<UStaticMesh>MeshRef(TEXT("/Game/AircraftHudDynamic/Mesh_AircraftHudCanvas")); 
        if (MeshRef.Succeeded())
        {
            HudDrawingCanvasMesh->SetStaticMesh(MeshRef.Object); 
        }

} 


void UAircraftHudComponent::InitializeComponent()
{
	Super::InitializeComponent();

    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    SetupCanvas();
    SetComponentTickEnabled(true);
}

void UAircraftHudComponent::BeginPlay() 
{ 
        Super::BeginPlay(); 
} 


void UAircraftHudComponent::SetupCanvas() 
{ 

        //HudDrawingCanvasMesh = GetStaticMeshComponent();
        HudDrawingCanvasMesh = this; 

        // Create the drawing canvas (the dynamic texture) to paint on in
        HudDrawingCanvas = NewObject<UAircraftHudDrawingCanvas>();

        HudDrawingCanvas->InitializeDrawingCanvas(TextureSize, TextureSize); 
        HudDrawingCanvas->InitializeDrawingTools(4, FColor::Blue); 

        mDynamicMaterials.Empty(); 
        mDynamicMaterials.Add(HudDrawingCanvasMesh->CreateAndSetMaterialInstanceDynamic(0)); 
        mDynamicMaterials[0]->SetTextureParameterValue(TEXT("DynamicTexture"), HudDrawingCanvas->GetDrawingCanvas()); 
       
        SetRelativeScale3D(FVector(TextureSize / 100.0f, TextureSize / 100.0f, 1)); 
} 


void UAircraftHudComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{ 
        Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 

        mDeltaTimeDt += DeltaTime; 
        
        if (mDeltaTimeDt > 0.02) 
        { 
                mDeltaTimeDt = 0.0f; 

                for (int u=0; u<256;u++)
                        HudDrawingCanvas->DrawDot(u, mDrawingTest);         
                mDrawingTest += 1;
                mDrawingTest %= 256;
               
                UpdateCanvas(); 
        } 


        //UE_LOG(LogTemp, Warning, TEXT("Component Tick"));
} 


void UAircraftHudComponent::UpdateCanvas() 
{ 
        HudDrawingCanvas->UpdateDrawingCanvas(); 
} 





