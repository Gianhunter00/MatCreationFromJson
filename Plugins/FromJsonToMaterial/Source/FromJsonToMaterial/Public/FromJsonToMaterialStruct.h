// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct MaterialAppearance
{
	FString ConBaseColor = "";
	FString ConMetallic = "";
	FString ConEmissiveColor = "";
	FString ConOpacityMask = "";
};

struct MaterialOption
{
	int32 MaterialDomain;
	int32 MaterialBlendMode;
	int32 MaterialShadingModel;
};

struct MaterialTextureSample
{
	FString UVs = "";
	FString Tex = "";
	FString ApplyViewMipBias = "";
	FString TexturePath = "";
};

struct MaterialCustomExpression
{
	FString Code = "";
	int32 OutputType;
};

struct MaterialPanner
{
	FString Coordinate = "";
	FString Time = "";
	FString Speed = "";
};

struct MaterialMask
{
	int32 R;
	int32 G;
	int32 B;
	int32 A;
	FString Value;
};

struct MaterialTransformPosition
{
	FString ConTransform = "";
};

struct MaterialConstantBias
{
	FString ConScale = "";
};

struct MaterialSaturate
{
	FString Value = "";
};

struct MaterialOneMinus
{
	FString Value;
};
struct MaterialSphereMask
{
	FString ConA = "";
	FString ConB = "";
	float Radius;
	FString ConRadius = "";
	float Hardness;
	FString ConHardness = "";
};

struct MaterialLerp
{
	FString ConA = "";
	float A;
	FString ConB = "";
	float B;
	FString ConAlpha = "";
	float Alpha;
};

struct MaterialColorStruct
{
	FString Name = "";
	FLinearColor Color;
};

struct Material2OpStruct
{
	float A;
	FString ConA = "";
	float B;
	FString ConB = "";
	FString Operation = "";
};

struct MaterialSine
{
	FString ConPeriod = "";
	float Period;
};

struct MaterialConstFloatVariable
{
	float Value;
};

struct MaterialConst2FloatVariable
{
	float Value1;
	float Value2;
};

struct MaterialScalarParameter
{
	FString Name = "";
	float Value;
};

struct MaterialStruct
{
	FString Name;
	TArray<MaterialColorStruct> Color;
	int32 Time;
	int32 AbsoluteWorldPosition;
	int32 ObjectPosition;
	TArray<Material2OpStruct> Operation;
	TArray<MaterialConstFloatVariable> ConstVariable;
	TArray<MaterialConst2FloatVariable> Const2Variable;
	TArray<MaterialScalarParameter> ScalarParamVariable;
	TArray<MaterialSine> Sines;
	TArray<MaterialConstantBias> ConstantBias;
	TArray<MaterialLerp> Lerp;
	TArray<MaterialSphereMask> SphereMask;
	TArray<MaterialOneMinus> OneMinus;
	TArray<MaterialSaturate> Saturate;
	TArray<MaterialTransformPosition> TransformPosition;
	TArray<MaterialMask> Mask;
	TArray<MaterialPanner> Panner;
	TArray<MaterialTextureSample> TextureSample;
	TArray<MaterialCustomExpression> CustomExpression;
	MaterialOption MaterialOptions;
	MaterialAppearance Appearance;
public:
	static MaterialStruct Default()
	{
		MaterialStruct Obj;
		Obj.Name = TEXT("");
		Obj.Color = TArray<MaterialColorStruct>();
		Obj.Time = 0;
		Obj.Operation = TArray<Material2OpStruct>();
		Obj.ConstVariable = TArray<MaterialConstFloatVariable>();
		Obj.Const2Variable = TArray<MaterialConst2FloatVariable>();
		Obj.ScalarParamVariable = TArray<MaterialScalarParameter>();
		Obj.Sines = TArray<MaterialSine>();
		Obj.ConstantBias = TArray<MaterialConstantBias>();
		Obj.Lerp = TArray<MaterialLerp>();
		Obj.SphereMask = TArray<MaterialSphereMask>();
		Obj.OneMinus = TArray<MaterialOneMinus>();
		Obj.Saturate = TArray<MaterialSaturate>();
		Obj.TransformPosition = TArray<MaterialTransformPosition>();
		Obj.Mask = TArray<MaterialMask>();
		Obj.Panner = TArray<MaterialPanner>();
		Obj.TextureSample = TArray<MaterialTextureSample>();
		Obj.CustomExpression = TArray<MaterialCustomExpression>();
		Obj.MaterialOptions = MaterialOption();
		Obj.Appearance = MaterialAppearance();
		return Obj;
	}
};

struct MaterialConnection
{
	FString ToConnectInput; //>
	UMaterialExpression* Input; //->
	int32 InputPin; //->
	int32 ToConnectInputPin; //>
};
