#include "CreatureAnimStoreEditorViewportClient.h"
#include "Runtime/Engine/Public/Slate/SceneViewport.h"
#include "SEditorViewport.h"
#include "AssetEditorModeManager.h"
#include "CreatureMeshComponent.h"
#include "CreatureAnimationClipsStore.h"
FCreatureAnimStoreEditorViewportClient::FCreatureAnimStoreEditorViewportClient(const TWeakPtr<class SEditorViewport>& InEditorViewportWidget /*= nullptr*/, UCreatureAnimationClipsStore* EditingAnimStore)
	:FEditorViewportClient(nullptr,nullptr,InEditorViewportWidget)
{
	OwnerScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));
	OwnerScene->SetFloorOffset(500);
	PreviewScene = OwnerScene.Get();
	EditingStore=EditingAnimStore;
	PreviewScene = (OwnerScene.Get());
	//((FAssetEditorModeManager*)ModeTools)->SetPreviewScene(PreviewScene);
	DrawHelper.bDrawGrid = true;
	DrawHelper.bDrawPivot = true;
	
	/*SetViewLocation(FVector(0, 30, 0));
	SetLookAtLocation(FVector(0, -30, 0));*/
	

	SetRealtime(true);
	//��PreviewScene����Component
	EditingCreatureMesh = NewObject<UCreatureMeshComponent>();
	//����CreatureMesh���
	SetUpEditingCreatureMesh();
	PreviewScene->AddComponent(EditingCreatureMesh, FTransform::Identity);
	EditingCreatureMesh->SetRelativeRotation(FRotator(0, 90, 0));

	//����Camera
	SetUpCamera();
	
	if (EditingStore->ClipList.Num()!=0)
	{
		//Ĭ�ϲ��ŵ�һ��Clip
		EditingCreatureMesh->SetBluePrintActiveCollectionClip_Name(EditingStore->ClipList[0].ClipName);
	}
	OwnerScene->SetFloorVisibility(false);

}

void FCreatureAnimStoreEditorViewportClient::Tick(float DeltaSeconds)
{
	
	FEditorViewportClient::Tick(DeltaSeconds);
	OwnerScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	//EditingCreatureMesh->UpdateBounds();
	//FBox Box = EditingCreatureMesh->Bounds.GetBox();
	//
	//FocusViewportOnBox(Box);
}

FLinearColor FCreatureAnimStoreEditorViewportClient::GetBackgroundColor() const
{
	return FLinearColor::Black;
}

void FCreatureAnimStoreEditorViewportClient::AddReferencedObjects(FReferenceCollector& Collector)
{
	
}

void FCreatureAnimStoreEditorViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);
}

void FCreatureAnimStoreEditorViewportClient::SetUpEditingCreatureMesh()
{
	if (EditingCreatureMesh==nullptr||EditingStore==nullptr)
	{
		return;
	}
	EditingCreatureMesh->ClipStore = EditingStore;
	//������Clip����
	EditingCreatureMesh->enable_collection_playback = true;
	
}

void FCreatureAnimStoreEditorViewportClient::SetUpCamera()
{
	SetViewLocation(FVector(-100, 0, 0));
	
}

void FCreatureAnimStoreEditorViewportClient::ChangePreviewAnimation(FString AnimationName)
{
	EditingCreatureMesh->SetBluePrintActiveCollectionClip_Name(FName(*AnimationName));
}

void FCreatureAnimStoreEditorViewportClient::ReConstructMesh()
{
	if (EditingCreatureMesh!=nullptr)
	{
		EditingCreatureMesh->DestroyComponent(true);
		EditingCreatureMesh = NewObject<UCreatureMeshComponent>();
		//����CreatureMesh���
		SetUpEditingCreatureMesh();
		PreviewScene->AddComponent(EditingCreatureMesh, FTransform::Identity);
		EditingCreatureMesh->SetRelativeRotation(FRotator(0, 90, 0));

		//����Camera
		SetUpCamera();

		if (EditingStore->ClipList.Num() != 0)
		{
			//Ĭ�ϲ��ŵ�һ��Clip
			EditingCreatureMesh->SetBluePrintActiveCollectionClip_Name(EditingStore->ClipList[0].ClipName);
		}
	}
}

