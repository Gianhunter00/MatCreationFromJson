// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Brushes/SlateBoxBrush.h"
#include "CoreMinimal.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Framework/Docking/TabManager.h"
#include "LevelEditor.h"
#include "Misc/MessageDialog.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"

DECLARE_DELEGATE_OneParam(OnPathChosen, FString)

class FromJsonToMaterialEditor
{
public:
	FromJsonToMaterialEditor();
	~FromJsonToMaterialEditor();
	TSharedRef<SDockTab> OnSpawnNomad(const FSpawnTabArgs& InTabSpawnArgs);

	FReply OpenFileDialog();
	FReply CreateMaterial();
	OnPathChosen* OnPathChosenFromDialog;
	FString Path;

private:
	const FString DialogTitle = FString("Choose the Json file to import");
	const FString DefaultPath = FPaths::GameSourceDir();
	const FString FileTypes = FString("Json Material|*.json;");
	TArray<FString> OutFileNamesChoice;
	TSharedPtr<SEditableTextBox> PathText;
};