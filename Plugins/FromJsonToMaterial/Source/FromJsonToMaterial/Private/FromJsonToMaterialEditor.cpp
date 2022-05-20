// Fill out your copyright notice in the Description page of Project Settings.


#include "FromJsonToMaterialEditor.h"

#define LOCTEXT_NAMESPACE "FFromJsonToMaterialEditor"

static const FName JsonEditorPluginName = FName("JsonEditorPlugin");

FromJsonToMaterialEditor::FromJsonToMaterialEditor()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(JsonEditorPluginName, FOnSpawnTab::CreateRaw(this, &FromJsonToMaterialEditor::OnSpawnNomad))
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetDeveloperToolsMiscCategory());
}

FromJsonToMaterialEditor::~FromJsonToMaterialEditor()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(JsonEditorPluginName);
}

TSharedRef<SDockTab> FromJsonToMaterialEditor::OnSpawnNomad(const FSpawnTabArgs& InTabSpawnArgs)
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SBorder)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().AutoHeight().Padding(0, 0, 0, 8).HAlign(EHorizontalAlignment::HAlign_Center)
				[
					SNew(SButton)
					.Text(LOCTEXT("OpenFileDialog", "Open File Dialog"))
					.OnClicked_Raw(this, &FromJsonToMaterialEditor::OpenFileDialog)
				]
			]
		];
}

FReply FromJsonToMaterialEditor::OpenFileDialog()
{
	if (GEngine)
	{
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (DesktopPlatform)
		{
			//Opening the file picker!
			uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
			DesktopPlatform->OpenFileDialog(nullptr, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNamesChoice);
			Path = OutFileNamesChoice[0];
			if (OnPathChosenFromDialog->IsBound())
			{
				OnPathChosenFromDialog->Execute(Path);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Not Bound"));
			}
		}
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
