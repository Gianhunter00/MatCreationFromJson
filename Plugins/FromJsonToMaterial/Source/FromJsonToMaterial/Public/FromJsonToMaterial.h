// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

struct JsonAppearance
{
	FString ConBaseColor = "";
	FString ConMetallic = "";
	FString ConEmissiveColor = "";
	FString ConOpacityMask = "";
};

struct JsonMaterialOption
{
	int32 MaterialDomain;
	int32 MaterialBlendMode;
	int32 MaterialShadingModel;
};

struct JsonTextureSample
{
	FString UVs = "";
	FString Tex = "";
	FString ApplyViewMipBias = "";
	FString TexturePath = "";
};

struct JsonCustomExpression
{
	FString Code = "";
	int32 OutputType;
};

struct JsonPanner
{
	FString Coordinate = "";
	FString Time = "";
	FString Speed = "";
};

struct JsonMask
{
	int32 R;
	int32 G;
	int32 B;
	int32 A;
	FString Value;
};

struct JsonTransformPosition
{
	FString ConTransform = "";
};

struct JsonConstantBias
{
	FString ConScale = "";
};

struct JsonSaturate
{
	FString Value = "";
};

struct JsonOneMinus
{
	FString Value;
};
struct JsonSphereMask
{
	FString ConA = "";
	FString ConB = "";
	float Radius;
	FString ConRadius = "";
	float Hardness;
	FString ConHardness = "";
};

struct JsonLerp
{
	FString ConA = "";
	float A;
	FString ConB = "";
	float B;
	FString ConAlpha = "";
	float Alpha;
};

struct JsonColorStruct
{
	FString Name = "";
	FLinearColor Color;
};

struct Json2OpStruct
{
	float A;
	FString ConA = "";
	float B;
	FString ConB = "";
	FString Operation = "";
};

struct JsonSine
{
	FString ConPeriod = "";
	float Period;
};

struct JsonConstFloatVariable
{
	float Value;
};

struct JsonConst2FloatVariable
{
	float Value1;
	float Value2;
};

struct JsonScalarParameter
{
	FString Name = "";
	float Value;
};

struct JsonMaterialStruct
{
	FString Name;
	TArray<JsonColorStruct> Color;
	int32 Time;
	int32 AbsoluteWorldPosition;
	int32 ObjectPosition;
	TArray<Json2OpStruct> Operation;
	TArray<JsonConstFloatVariable> ConstVariable;
	TArray<JsonConst2FloatVariable> Const2Variable;
	TArray<JsonScalarParameter> ScalarParamVariable;
	TArray<JsonSine> Sines;
	TArray<JsonConstantBias> ConstantBias;
	TArray<JsonLerp> Lerp;
	TArray<JsonSphereMask> SphereMask;
	TArray<JsonOneMinus> OneMinus;
	TArray<JsonSaturate> Saturate;
	TArray<JsonTransformPosition> TransformPosition;
	TArray<JsonMask> Mask;
	TArray<JsonPanner> Panner;
	TArray<JsonTextureSample> TextureSample;
	TArray<JsonCustomExpression> CustomExpression;
	JsonMaterialOption MaterialOption;
	JsonAppearance Appearance;
public:
	static JsonMaterialStruct Default()
	{
		JsonMaterialStruct Obj;
		Obj.Name = TEXT("");
		Obj.Color = TArray<JsonColorStruct>();
		Obj.Time = 0;
		Obj.Operation = TArray<Json2OpStruct>();
		Obj.ConstVariable = TArray<JsonConstFloatVariable>();
		Obj.Const2Variable = TArray<JsonConst2FloatVariable>();
		Obj.ScalarParamVariable = TArray<JsonScalarParameter>();
		Obj.Sines = TArray<JsonSine>();
		Obj.ConstantBias = TArray<JsonConstantBias>();
		Obj.Lerp = TArray<JsonLerp>();
		Obj.SphereMask = TArray<JsonSphereMask>();
		Obj.OneMinus = TArray<JsonOneMinus>();
		Obj.Saturate = TArray<JsonSaturate>();
		Obj.TransformPosition = TArray<JsonTransformPosition>();
		Obj.Mask = TArray<JsonMask>();
		Obj.Panner = TArray<JsonPanner>();
		Obj.TextureSample = TArray<JsonTextureSample>();
		Obj.CustomExpression = TArray<JsonCustomExpression>();
		Obj.MaterialOption = JsonMaterialOption();
		Obj.Appearance = JsonAppearance();
		return Obj;
	}
};

class FFromJsonToMaterialModule : public IModuleInterface, public FSelfRegisteringExec
{
public:
	typedef UMaterialExpression* (FFromJsonToMaterialModule::* Operation)(UMaterial*, const Json2OpStruct);
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	TArray<JsonMaterialStruct> ParseJsonMaterialArray(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonColorStruct> ParseJsonColorArray(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<Json2OpStruct> ParseJson2OpArray(const JsonMaterialStruct Mat, const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonConstFloatVariable> ParseJsonConstVariables(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonConst2FloatVariable> ParseJsonConst2Variables(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonScalarParameter> ParseJsonScalarParameterVariables(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonSine> ParseJsonSines(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonConstantBias> ParseJsonConstantBias(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonLerp> ParseJsonLerp(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonSphereMask> ParseJsonSphereMask(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonOneMinus> ParseJsonOneMinus(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonSaturate> ParseJsonSaturate(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonTransformPosition> ParseJsonTransformPosition(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonPanner> ParseJsonPanner(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonMask> ParseJsonMask(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonTextureSample> ParseJsonTextureSample(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	TArray<JsonCustomExpression> ParseJsonCustomExpression(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	JsonMaterialOption ParseJsonMaterialOption(const TSharedPtr<FJsonObject> Json, const FString Field) const;
	JsonAppearance ParseJsonAppearance(const TSharedPtr<FJsonObject> Json, const FString Field) const;



	void FromStructToMaterial(const JsonMaterialStruct InMat);
	TArray<UMaterialExpressionVectorParameter*> FromArrayColorStructToNode(UMaterial* OuterMat, const TArray<JsonColorStruct> InColors);
	TArray<UMaterialExpressionConstant*> FromArrayConstVarStructToNode(UMaterial* OuterMat, const TArray<JsonConstFloatVariable> InVar);
	TArray<UMaterialExpressionConstant2Vector*> FromArrayConst2VarStructToNode(UMaterial* OuterMat, const TArray<JsonConst2FloatVariable> InVars);
	TArray<UMaterialExpressionScalarParameter*> FromArrayScalarParameterVarStructToNode(UMaterial* OuterMat, const TArray<JsonScalarParameter> InVar);
	TArray<UMaterialExpressionSine*> FromArraySineStructToNode(UMaterial* OuterMat, const TArray<JsonSine> InSines);
	TArray<UMaterialExpressionConstantBiasScale*> FromArrayConstantBiasStructToNode(UMaterial* OuterMat, const TArray<JsonConstantBias> InBias);
	TArray<UMaterialExpressionLinearInterpolate*> FromArrayLerpStructToNode(UMaterial* OuterMat, const TArray<JsonLerp> InLerps);
	TArray<UMaterialExpressionSphereMask*> FromArraySphereMaskStructToNode(UMaterial* OuterMat, const TArray<JsonSphereMask> InSpheres);
	TArray<UMaterialExpressionOneMinus*> FromArrayOneMinusStructToNode(UMaterial* OuterMat, const TArray<JsonOneMinus> InOneMinuses);
	TArray<UMaterialExpressionSaturate*> FromArraySaturateStructToNode(UMaterial* OuterMat, const TArray<JsonSaturate> InSaturates);
	TArray<UMaterialExpressionTransformPosition*> FromArrayTransformPositionStructToNode(UMaterial* OuterMat, const TArray<JsonTransformPosition> InTransforms);
	TArray<UMaterialExpressionComponentMask*> FromArrayMaskStructToNode(UMaterial* OuterMat, const TArray<JsonMask> InMasks);
	TArray<UMaterialExpressionPanner*> FromArrayPannerStructToNode(UMaterial* OuterMat, const TArray<JsonPanner> InPanners);
	TArray<UMaterialExpressionTextureSample*> FromArrayTextureSampleStructToNode(UMaterial* OuterMat, const TArray<JsonTextureSample> InTextures);
	TArray<UMaterialExpressionCustom*> FromArrayCustomExpressionStructToNode(UMaterial* OuterMat, const TArray<JsonCustomExpression> InCustoms);

	void ConnectAllBesideAppearance(UMaterial* InMat, TArray<TTuple<FString, UMaterialExpression*, int32, int32>> InConnections);
	void ConnectAppearance(UMaterial* OuterMat, const JsonAppearance InApp);
	void SetMaterialOption(UMaterial* OuterMat, const JsonMaterialOption InOption);

	UMaterialExpression* CreateMultiply(UMaterial* OuterMat, const Json2OpStruct Op);
	UMaterialExpression* CreateAppend(UMaterial* OuterMat, const Json2OpStruct Op);
	UMaterialExpression* CreateAdd(UMaterial* OuterMat, const Json2OpStruct Op);
	UMaterialExpression* CreateSubstract(UMaterial* OuterMat, const Json2OpStruct Op);
	UMaterialExpression* CreatePower(UMaterial* OuterMat, const Json2OpStruct Op);
	UMaterialExpression* CreateDivide(UMaterial* OuterMat, const Json2OpStruct Op);

	TArray<TTuple<FString, UMaterialExpression*, int32, int32>> Connection;
	TMap<FString, Operation> OperationCreator;
};
