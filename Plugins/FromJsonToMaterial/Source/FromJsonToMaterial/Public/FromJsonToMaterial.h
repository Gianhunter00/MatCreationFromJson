// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FromJsonToMaterialStruct.h"
#include "FromJsonToMaterialEditor.h"
#include "Materials/MaterialExpression.h"
#include "Materials/MaterialExpressionAdd.h"
#include "Materials/MaterialExpressionAppendVector.h"
#include "Materials/MaterialExpressionComponentMask.h"
#include "Materials/MaterialExpressionConstant.h"
#include "Materials/MaterialExpressionConstant2Vector.h"
#include "Materials/MaterialExpressionConstantBiasScale.h"
#include "Materials/MaterialExpressionCustom.h"
#include "Materials/MaterialExpressionDivide.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionObjectPositionWS.h"
#include "Materials/MaterialExpressionOneMinus.h"
#include "Materials/MaterialExpressionPanner.h"
#include "Materials/MaterialExpressionPower.h"
#include "Materials/MaterialExpressionRuntimeVirtualTextureSample.h"
#include "Materials/MaterialExpressionSaturate.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionSine.h"
#include "Materials/MaterialExpressionSphereMask.h"
#include "Materials/MaterialExpressionSubtract.h"
#include "Materials/MaterialExpressionTime.h"
#include "Materials/MaterialExpressionTransformPosition.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionWorldPosition.h"
#include "Modules/ModuleManager.h"



class FFromJsonToMaterialModule : public IModuleInterface, public FSelfRegisteringExec
{
public:
	typedef UMaterialExpression* (FFromJsonToMaterialModule::* Operation)(UMaterial*, const Material2OpStruct);

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	TArray<MaterialStruct> ParseJsonMaterialArray(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialColorStruct> ParseJsonColorArray(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<Material2OpStruct> ParseJson2OpArray(const MaterialStruct Mat, const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialConstFloatVariable> ParseJsonConstVariables(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialConst2FloatVariable> ParseJsonConst2Variables(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialScalarParameter> ParseJsonScalarParameterVariables(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialSine> ParseJsonSines(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialConstantBias> ParseJsonConstantBias(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialLerp> ParseJsonLerp(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialSphereMask> ParseJsonSphereMask(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialOneMinus> ParseJsonOneMinus(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialSaturate> ParseJsonSaturate(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialTransformPosition> ParseJsonTransformPosition(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialPanner> ParseJsonPanner(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialMask> ParseJsonMask(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialTextureSample> ParseJsonTextureSample(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<MaterialCustomExpression> ParseJsonCustomExpression(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	MaterialOption ParseJsonMaterialOption(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	MaterialAppearance ParseJsonAppearance(const TSharedPtr<FJsonObject> Json, const FString Field) const;



	void FromStructToMaterial(const MaterialStruct InMat);
	TArray<UMaterialExpressionVectorParameter*> FromArrayColorStructToNode(UMaterial* OuterMat, const TArray<MaterialColorStruct> InColors);
	TArray<UMaterialExpressionConstant*> FromArrayConstVarStructToNode(UMaterial* OuterMat, const TArray<MaterialConstFloatVariable> InVar);
	TArray<UMaterialExpressionConstant2Vector*> FromArrayConst2VarStructToNode(UMaterial* OuterMat, const TArray<MaterialConst2FloatVariable> InVars);
	TArray<UMaterialExpressionScalarParameter*> FromArrayScalarParameterVarStructToNode(UMaterial* OuterMat, const TArray<MaterialScalarParameter> InVar);
	TArray<UMaterialExpressionSine*> FromArraySineStructToNode(UMaterial* OuterMat, const TArray<MaterialSine> InSines);
	TArray<UMaterialExpressionConstantBiasScale*> FromArrayConstantBiasStructToNode(UMaterial* OuterMat, const TArray<MaterialConstantBias> InBias);
	TArray<UMaterialExpressionLinearInterpolate*> FromArrayLerpStructToNode(UMaterial* OuterMat, const TArray<MaterialLerp> InLerps);
	TArray<UMaterialExpressionSphereMask*> FromArraySphereMaskStructToNode(UMaterial* OuterMat, const TArray<MaterialSphereMask> InSpheres);
	TArray<UMaterialExpressionOneMinus*> FromArrayOneMinusStructToNode(UMaterial* OuterMat, const TArray<MaterialOneMinus> InOneMinuses);
	TArray<UMaterialExpressionSaturate*> FromArraySaturateStructToNode(UMaterial* OuterMat, const TArray<MaterialSaturate> InSaturates);
	TArray<UMaterialExpressionTransformPosition*> FromArrayTransformPositionStructToNode(UMaterial* OuterMat, const TArray<MaterialTransformPosition> InTransforms);
	TArray<UMaterialExpressionComponentMask*> FromArrayMaskStructToNode(UMaterial* OuterMat, const TArray<MaterialMask> InMasks);
	TArray<UMaterialExpressionPanner*> FromArrayPannerStructToNode(UMaterial* OuterMat, const TArray<MaterialPanner> InPanners);
	TArray<UMaterialExpressionTextureSample*> FromArrayTextureSampleStructToNode(UMaterial* OuterMat, const TArray<MaterialTextureSample> InTextures);
	TArray<UMaterialExpressionCustom*> FromArrayCustomExpressionStructToNode(UMaterial* OuterMat, const TArray<MaterialCustomExpression> InCustoms);

	void ConnectAllBesideAppearance(UMaterial* InMat, TArray<TTuple<FString, UMaterialExpression*, int32, int32>> InConnections);
	void ConnectAppearance(UMaterial* OuterMat, const MaterialAppearance InApp);
	void SetMaterialOption(UMaterial* OuterMat, const MaterialOption InOption);

	UMaterialExpression* CreateMultiply(UMaterial* OuterMat, const Material2OpStruct Op);
	UMaterialExpression* CreateAppend(UMaterial* OuterMat, const Material2OpStruct Op);
	UMaterialExpression* CreateAdd(UMaterial* OuterMat, const Material2OpStruct Op);
	UMaterialExpression* CreateSubstract(UMaterial* OuterMat, const Material2OpStruct Op);
	UMaterialExpression* CreatePower(UMaterial* OuterMat, const Material2OpStruct Op);
	UMaterialExpression* CreateDivide(UMaterial* OuterMat, const Material2OpStruct Op);


	void CreateMaterialFromPath(FString Path);
	TArray<TTuple<FString, UMaterialExpression*, int32, int32>> Connection;
	TMap<FString, Operation> OperationCreator;
	FromJsonToMaterialEditor* MaterialEditor;
	OnPathChosen FuncToBind;
};
