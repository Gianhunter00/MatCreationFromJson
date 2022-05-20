// Copyright Epic Games, Inc. All Rights Reserved.

#include "FromJsonToMaterial.h"
#include "AssetRegistryModule.h"
#include "Factories/MaterialFactoryNew.h"
#include "Json.h"

#define LOCTEXT_NAMESPACE "FFromJsonToMaterialModule"

void FFromJsonToMaterialModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	OperationCreator.Add("x", &FFromJsonToMaterialModule::CreateMultiply);
	OperationCreator.Add("^", &FFromJsonToMaterialModule::CreateAppend);
	OperationCreator.Add("+", &FFromJsonToMaterialModule::CreateAdd);
	OperationCreator.Add("-", &FFromJsonToMaterialModule::CreateSubstract);
	OperationCreator.Add("*", &FFromJsonToMaterialModule::CreatePower);
	OperationCreator.Add("/", &FFromJsonToMaterialModule::CreateDivide);
	MaterialEditor = new FromJsonToMaterialEditor();
	FuncToBind = MaterialEditor->OnPathChosenFromDialog->CreateRaw(this, &FFromJsonToMaterialModule::CreateMaterialFromPath);
	MaterialEditor->OnPathChosenFromDialog = &FuncToBind;
}

void FFromJsonToMaterialModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	OperationCreator.Empty();
}

TArray<MaterialStruct> FFromJsonToMaterialModule::ParseJsonMaterialArray(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialStruct> MaterialsArray;
	TArray<TSharedPtr<FJsonValue>> JsonMaterialsArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonMaterialsArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonMaterialsArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialStruct Mat = MaterialStruct::Default();
		Mat.Name = ObjectJson->GetStringField("MaterialName");
		Mat.Color = ParseJsonColorArray(ObjectJson, "Color");
		Mat.Time = ObjectJson->GetIntegerField("Time");
		Mat.AbsoluteWorldPosition = ObjectJson->GetIntegerField("AbsWorldPos");
		Mat.ObjectPosition = ObjectJson->GetIntegerField("ObjPosition");
		Mat.Operation = ParseJson2OpArray(Mat, ObjectJson, "Operation");
		Mat.ConstVariable = ParseJsonConstVariables(ObjectJson, "ConstVar");
		Mat.Const2Variable = ParseJsonConst2Variables(ObjectJson, "Const2Var");
		Mat.ScalarParamVariable = ParseJsonScalarParameterVariables(ObjectJson, "ScalarParVar");
		Mat.Sines = ParseJsonSines(ObjectJson, "Sine");
		Mat.ConstantBias = ParseJsonConstantBias(ObjectJson, "ConstantBias");
		Mat.Lerp = ParseJsonLerp(ObjectJson, "Lerp");
		Mat.SphereMask = ParseJsonSphereMask(ObjectJson, "SphereMask");
		Mat.OneMinus = ParseJsonOneMinus(ObjectJson, "OneMinus");
		Mat.Saturate = ParseJsonSaturate(ObjectJson, "Saturate");
		Mat.TransformPosition = ParseJsonTransformPosition(ObjectJson, "TransformPosition");
		Mat.Mask = ParseJsonMask(ObjectJson, "Mask");
		Mat.Panner = ParseJsonPanner(ObjectJson, "Panner");
		Mat.TextureSample = ParseJsonTextureSample(ObjectJson, "TextureSample");
		Mat.CustomExpression = ParseJsonCustomExpression(ObjectJson, "CustomExpression");
		Mat.MaterialOptions = ParseJsonMaterialOption(ObjectJson, "MaterialOption");
		Mat.Appearance = ParseJsonAppearance(ObjectJson, "Appearance");
		MaterialsArray.Add(Mat);
	}
	return MaterialsArray;
}

TArray<MaterialColorStruct> FFromJsonToMaterialModule::ParseJsonColorArray(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialColorStruct> ColorsArray;
	TArray<TSharedPtr<FJsonValue>> JsonColorsArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonColorsArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonColorsArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialColorStruct Color;
		Color.Name = ObjectJson->GetStringField("ColorName");
		TArray<TSharedPtr<FJsonValue>> ColorVal = ObjectJson->GetArrayField("ColorVal");
		Color.Color = FLinearColor((float)ColorVal[0]->AsNumber(), (float)ColorVal[1]->AsNumber(), (float)ColorVal[2]->AsNumber(), (float)ColorVal[3]->AsNumber());
		ColorsArray.Add(Color);
	}
	return ColorsArray;
}

TArray<Material2OpStruct> FFromJsonToMaterialModule::ParseJson2OpArray(const MaterialStruct Mat, const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<Material2OpStruct> OpArray;
	TArray<TSharedPtr<FJsonValue>> JsonOpArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonOpArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonOpArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		Material2OpStruct Operation;

		double value;
		if (ObjectJson->TryGetNumberField("A", value))
		{
			Operation.A = (float)value;
		}
		else
		{
			FString Param = ObjectJson->GetStringField("A");
			Operation.ConA = Param;
		}
		if (ObjectJson->TryGetNumberField("B", value))
		{
			Operation.B = (float)value;
		}
		else
		{
			FString Param = ObjectJson->GetStringField("B");
			Operation.ConB = Param;
		}

		Operation.Operation = ObjectJson->GetStringField("Op");
		OpArray.Add(Operation);
	}
	return OpArray;
}

TArray<MaterialConstFloatVariable> FFromJsonToMaterialModule::ParseJsonConstVariables(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialConstFloatVariable> VariablesArray;
	TArray<TSharedPtr<FJsonValue>> JsonVariablesArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonVariablesArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonVariablesArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialConstFloatVariable Variable;
		Variable.Value = (float)ObjectJson->GetNumberField("Value");
		VariablesArray.Add(Variable);
	}
	return VariablesArray;
}

TArray<MaterialConst2FloatVariable> FFromJsonToMaterialModule::ParseJsonConst2Variables(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialConst2FloatVariable> VariablesArray;
	TArray<TSharedPtr<FJsonValue>> JsonVariablesArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonVariablesArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonVariablesArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialConst2FloatVariable Variable;
		Variable.Value1 = (float)ObjectJson->GetNumberField("Value1");
		Variable.Value2 = (float)ObjectJson->GetNumberField("Value2");
		VariablesArray.Add(Variable);
	}
	return VariablesArray;
}

TArray<MaterialScalarParameter> FFromJsonToMaterialModule::ParseJsonScalarParameterVariables(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialScalarParameter> ScalarParameterVariablesArray;
	TArray<TSharedPtr<FJsonValue>> JsonScalarParameterVariablesArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonScalarParameterVariablesArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonScalarParameterVariablesArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialScalarParameter ScalarParameterVariable;
		ScalarParameterVariable.Name = ObjectJson->GetStringField("ScalarName");
		ScalarParameterVariable.Value = (float)ObjectJson->GetNumberField("Value");
		ScalarParameterVariablesArray.Add(ScalarParameterVariable);
	}
	return ScalarParameterVariablesArray;
}

TArray<MaterialSine> FFromJsonToMaterialModule::ParseJsonSines(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialSine> SineArray;
	TArray<TSharedPtr<FJsonValue>> JsonSineArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonSineArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonSineArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialSine Sine;
		double value;
		if (ObjectJson->TryGetNumberField("Period", value))
		{
			Sine.Period = (float)value;
		}
		else
		{
			FString Param = ObjectJson->GetStringField("Period");
			Sine.ConPeriod = Param;
		}
		SineArray.Add(Sine);
	}
	return SineArray;
}

TArray<MaterialConstantBias> FFromJsonToMaterialModule::ParseJsonConstantBias(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialConstantBias> ConBiasArray;
	TArray<TSharedPtr<FJsonValue>> JsonConBiasArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonConBiasArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonConBiasArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialConstantBias ConBias;
		FString Param = ObjectJson->GetStringField("Scale");
		ConBias.ConScale = Param;
		ConBiasArray.Add(ConBias);
	}
	return ConBiasArray;
}

TArray<MaterialLerp> FFromJsonToMaterialModule::ParseJsonLerp(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialLerp> LerpArray;
	TArray<TSharedPtr<FJsonValue>> JsonLerpArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonLerpArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonLerpArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialLerp Lerp;
		double value;
		if (ObjectJson->TryGetNumberField("A", value))
		{
			Lerp.A = (float)value;
		}
		else
		{
			FString Param = ObjectJson->GetStringField("A");
			Lerp.ConA = Param;
		}

		if (ObjectJson->TryGetNumberField("B", value))
		{
			Lerp.B = (float)value;
		}
		else
		{
			FString Param = ObjectJson->GetStringField("B");
			Lerp.ConB = Param;
		}

		if (ObjectJson->TryGetNumberField("Alpha", value))
		{
			Lerp.Alpha = (float)value;
		}
		else
		{
			FString Param = ObjectJson->GetStringField("Alpha");
			Lerp.ConAlpha = Param;
		}
		LerpArray.Add(Lerp);
	}
	return LerpArray;
}

TArray<MaterialSphereMask> FFromJsonToMaterialModule::ParseJsonSphereMask(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialSphereMask> SphereMaskArray;
	TArray<TSharedPtr<FJsonValue>> JsonSphereMaskArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonSphereMaskArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonSphereMaskArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialSphereMask SphereMask;

		FString Param = ObjectJson->GetStringField("A");
		SphereMask.ConA = Param;

		Param = ObjectJson->GetStringField("B");
		SphereMask.ConB = Param;

		double value;
		if (ObjectJson->TryGetNumberField("Radius", value))
		{
			SphereMask.Radius = (float)value;
		}
		else
		{
			Param = ObjectJson->GetStringField("Radius");
			SphereMask.ConRadius = Param;
		}

		if (ObjectJson->TryGetNumberField("Hardness", value))
		{
			SphereMask.Hardness = (float)value;
		}
		else
		{
			Param = ObjectJson->GetStringField("Hardness");
			SphereMask.ConHardness = Param;
		}
		SphereMaskArray.Add(SphereMask);
	}
	return SphereMaskArray;
}

TArray<MaterialOneMinus> FFromJsonToMaterialModule::ParseJsonOneMinus(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialOneMinus> OneMinusArray;
	TArray<TSharedPtr<FJsonValue>> JsonOneMinusArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonOneMinusArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonOneMinusArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialOneMinus OneMinus;
		FString Param = ObjectJson->GetStringField("Value");
		OneMinus.Value = Param;
		OneMinusArray.Add(OneMinus);
	}
	return OneMinusArray;
}

TArray<MaterialSaturate> FFromJsonToMaterialModule::ParseJsonSaturate(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialSaturate> SaturateArray;
	TArray<TSharedPtr<FJsonValue>> JsonOneMinusArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonOneMinusArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonOneMinusArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialSaturate Saturate;
		FString Param = ObjectJson->GetStringField("Value");
		Saturate.Value = Param;
		SaturateArray.Add(Saturate);
	}
	return SaturateArray;
}

TArray<MaterialTransformPosition> FFromJsonToMaterialModule::ParseJsonTransformPosition(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialTransformPosition> TransformPositionArray;
	TArray<TSharedPtr<FJsonValue>> JsonTransformPositionArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonTransformPositionArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonTransformPositionArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialTransformPosition TransformPosition;
		FString Param = ObjectJson->GetStringField("Value");
		TransformPosition.ConTransform = Param;
		TransformPositionArray.Add(TransformPosition);
	}
	return TransformPositionArray;
}

TArray<MaterialPanner> FFromJsonToMaterialModule::ParseJsonPanner(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialPanner> PannerArray;
	TArray<TSharedPtr<FJsonValue>> JsonPannerArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonPannerArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonPannerArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialPanner Panner;
		FString Param = ObjectJson->GetStringField("Coordinate");
		Panner.Coordinate = Param;
		Param = ObjectJson->GetStringField("Time");
		Panner.Time = Param;
		Param = ObjectJson->GetStringField("Speed");
		Panner.Speed = Param;
		PannerArray.Add(Panner);
	}
	return PannerArray;
}

TArray<MaterialMask> FFromJsonToMaterialModule::ParseJsonMask(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialMask> MaskArray;
	TArray<TSharedPtr<FJsonValue>> JsonMaskArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonMaskArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonMaskArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialMask Mask;
		Mask.R = ObjectJson->GetIntegerField("R");
		Mask.G = ObjectJson->GetIntegerField("G");
		Mask.B = ObjectJson->GetIntegerField("B");
		Mask.A = ObjectJson->GetIntegerField("A");
		Mask.Value = ObjectJson->GetStringField("Value");
		MaskArray.Add(Mask);
	}
	return MaskArray;
}

TArray<MaterialTextureSample> FFromJsonToMaterialModule::ParseJsonTextureSample(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialTextureSample> TextureSampleArray;
	TArray<TSharedPtr<FJsonValue>> JsonTextureSampleArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonTextureSampleArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonTextureSampleArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialTextureSample TextureSample;
		TextureSample.UVs = ObjectJson->GetStringField("UVs");
		TextureSample.Tex = ObjectJson->GetStringField("Tex");
		TextureSample.ApplyViewMipBias = ObjectJson->GetStringField("ApplyViewMipBias");
		TextureSample.TexturePath = ObjectJson->GetStringField("TexturePath");
		TextureSampleArray.Add(TextureSample);
	}
	return TextureSampleArray;
}

TArray<MaterialCustomExpression> FFromJsonToMaterialModule::ParseJsonCustomExpression(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	TArray<MaterialCustomExpression> CustomExpressionArray;
	TArray<TSharedPtr<FJsonValue>> JsonCustomExpressionArray = Json->GetArrayField((TEXT("%s"), Field));
	for (int32 Index = 0; Index < JsonCustomExpressionArray.Num(); Index++)
	{
		TSharedPtr<FJsonValue> ValueJson = JsonCustomExpressionArray[Index];
		TSharedPtr<FJsonObject> ObjectJson = ValueJson->AsObject();
		MaterialCustomExpression CustomExpression;
		CustomExpression.Code = ObjectJson->GetStringField("Code");
		CustomExpression.OutputType = ObjectJson->GetIntegerField("OutputType");
		CustomExpressionArray.Add(CustomExpression);
	}
	return CustomExpressionArray;
}

MaterialOption FFromJsonToMaterialModule::ParseJsonMaterialOption(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	const TSharedPtr<FJsonObject> ObjectJson = Json->GetObjectField((TEXT("%s"), Field));
	MaterialOption Option;
	Option.MaterialDomain = ObjectJson->GetIntegerField("MaterialDomain");
	Option.MaterialBlendMode = ObjectJson->GetIntegerField("MaterialBlendMode");
	Option.MaterialShadingModel = ObjectJson->GetIntegerField("MaterialShadingModel");
	return Option;
}

MaterialAppearance FFromJsonToMaterialModule::ParseJsonAppearance(const TSharedPtr<FJsonObject> Json, const FString Field) const
{
	const TSharedPtr<FJsonObject> ObjectJson = Json->GetObjectField((TEXT("%s"), Field));
	MaterialAppearance Appearance;
	FString Param = ObjectJson->GetStringField("BaseColor");
	Appearance.ConBaseColor = Param;
	Param = ObjectJson->GetStringField("Metallic");
	Appearance.ConMetallic = Param;
	Param = ObjectJson->GetStringField("EmissiveColor");
	Appearance.ConEmissiveColor = Param;
	Param = ObjectJson->GetStringField("OpacityMask");
	Appearance.ConOpacityMask = Param;
	return Appearance;
}

void FFromJsonToMaterialModule::FromStructToMaterial(const MaterialStruct InMat)
{
	UMaterialFactoryNew* NewMaterial = NewObject<UMaterialFactoryNew>();

	FString AssetName = InMat.Name;
	UPackage* Package = CreatePackage(*FString::Printf(TEXT("/Game/%s"), *AssetName));

	UObject* NewMaterialObject = NewMaterial->FactoryCreateNew(NewMaterial->SupportedClass, Package, *AssetName, EObjectFlags::RF_Standalone | EObjectFlags::RF_Public, nullptr, GWarn);
	FAssetRegistryModule::AssetCreated(NewMaterialObject);

	UMaterial* MaterialCasted = Cast<UMaterial>(NewMaterialObject);

	MaterialCasted->Modify();

	for (Material2OpStruct Operation : InMat.Operation)
	{
		(this->* * OperationCreator.Find(Operation.Operation))(MaterialCasted, Operation);
	}

	FromArrayColorStructToNode(MaterialCasted, InMat.Color);

	//MaterialExpressionConstant_0
	FromArrayConstVarStructToNode(MaterialCasted, InMat.ConstVariable);

	//MaterialExpressionConstant2Vector_0
	FromArrayConst2VarStructToNode(MaterialCasted, InMat.Const2Variable);

	//MaterialExpressionSine_0
	FromArraySineStructToNode(MaterialCasted, InMat.Sines);

	//MaterialExpressionLinearInterpolate_0
	FromArrayLerpStructToNode(MaterialCasted, InMat.Lerp);

	//MaterialExpressionConstantBiasScale_0
	FromArrayConstantBiasStructToNode(MaterialCasted, InMat.ConstantBias);

	FromArrayScalarParameterVarStructToNode(MaterialCasted, InMat.ScalarParamVariable);

	//MaterialExpressionSphereMask_0
	FromArraySphereMaskStructToNode(MaterialCasted, InMat.SphereMask);

	//MaterialExpressionOneMinus_0
	FromArrayOneMinusStructToNode(MaterialCasted, InMat.OneMinus);

	//MaterialExpressionSaturate_0
	FromArraySaturateStructToNode(MaterialCasted, InMat.Saturate);

	//MaterialExpressionTransformPosition_0
	FromArrayTransformPositionStructToNode(MaterialCasted, InMat.TransformPosition);

	//MaterialExpressionComponentMask_0
	FromArrayMaskStructToNode(MaterialCasted, InMat.Mask);

	//MaterialExpressionPanner_0
	FromArrayPannerStructToNode(MaterialCasted, InMat.Panner);

	//MaterialExpressionTextureSample_0
	FromArrayTextureSampleStructToNode(MaterialCasted, InMat.TextureSample);

	//MaterialExpressionCustom_0
	FromArrayCustomExpressionStructToNode(MaterialCasted, InMat.CustomExpression);

	for (int32 Index = 0; Index < InMat.Time; Index++)
	{
		UMaterialExpressionTime* Time = NewObject<UMaterialExpressionTime>(MaterialCasted);
		//MaterialExpressionTime_0
		MaterialCasted->Expressions.Add(Time);
	}

	for (int32 Index = 0; Index < InMat.AbsoluteWorldPosition; Index++)
	{
		UMaterialExpressionWorldPosition* AbsolutePosition = NewObject<UMaterialExpressionWorldPosition>(MaterialCasted);
		//MaterialExpressionWorldPosition_0
		MaterialCasted->Expressions.Add(AbsolutePosition);
	}

	for (int32 Index = 0; Index < InMat.ObjectPosition; Index++)
	{
		UClass* ObjectPositionClass = FindObject<UClass>(ANY_PACKAGE, *FString("/Script/Engine.MaterialExpressionObjectPositionWS"));
		UMaterialExpression* ObjectPosition = NewObject<UMaterialExpression>(MaterialCasted, ObjectPositionClass, FName("UMaterialExpressionObjectPositionWS"));
		//UMaterialExpressionObjectPositionWS* ObjectPos = NewObject<UMaterialExpressionObjectPositionWS>(MaterialCasted);
		//MaterialExpressionObjectPositionWS
		MaterialCasted->Expressions.Add(ObjectPosition);
	}

	ConnectAllBesideAppearance(MaterialCasted, Connection);

	SetMaterialOption(MaterialCasted, InMat.MaterialOptions);

	ConnectAppearance(MaterialCasted, InMat.Appearance);

	MaterialCasted->PostEditChange();
	MaterialCasted->MarkPackageDirty();
	Connection.Empty();
}

TArray<UMaterialExpressionVectorParameter*> FFromJsonToMaterialModule::FromArrayColorStructToNode(UMaterial* OuterMat, const TArray<MaterialColorStruct> InColors)
{
	TArray<UMaterialExpressionVectorParameter*> ColorsNode;
	for (int32 Index = 0; Index < InColors.Num(); Index++)
	{
		UMaterialExpressionVectorParameter* VectorParameter = NewObject<UMaterialExpressionVectorParameter>(OuterMat);
		VectorParameter->DefaultValue = InColors[Index].Color;
		VectorParameter->ParameterName = *InColors[Index].Name;
		OuterMat->Expressions.Add(VectorParameter);
		ColorsNode.Add(VectorParameter);
	}

	return ColorsNode;
}

TArray<UMaterialExpressionConstant*> FFromJsonToMaterialModule::FromArrayConstVarStructToNode(UMaterial* OuterMat, const TArray<MaterialConstFloatVariable> InVar)
{
	TArray<UMaterialExpressionConstant*> VariablesNode;

	for (int32 Index = 0; Index < InVar.Num(); Index++)
	{
		UMaterialExpressionConstant* VarParameter = NewObject<UMaterialExpressionConstant>(OuterMat);
		VarParameter->R = InVar[Index].Value;
		OuterMat->Expressions.Add(VarParameter);
		VariablesNode.Add(VarParameter);
	}

	return VariablesNode;
}

TArray<UMaterialExpressionConstant2Vector*> FFromJsonToMaterialModule::FromArrayConst2VarStructToNode(UMaterial* OuterMat, const TArray<MaterialConst2FloatVariable> InVars)
{
	TArray<UMaterialExpressionConstant2Vector*> VariablesNode;

	for (int32 Index = 0; Index < InVars.Num(); Index++)
	{
		UMaterialExpressionConstant2Vector* VarParameter = NewObject<UMaterialExpressionConstant2Vector>(OuterMat);
		VarParameter->R = InVars[Index].Value1;
		VarParameter->G = InVars[Index].Value2;
		OuterMat->Expressions.Add(VarParameter);
		VariablesNode.Add(VarParameter);
	}

	return VariablesNode;
}

TArray<UMaterialExpressionScalarParameter*> FFromJsonToMaterialModule::FromArrayScalarParameterVarStructToNode(UMaterial* OuterMat, const TArray<MaterialScalarParameter> InVar)
{
	TArray<UMaterialExpressionScalarParameter*> ScalarParameterVariablesNode;

	for (int32 Index = 0; Index < InVar.Num(); Index++)
	{
		UMaterialExpressionScalarParameter* ScalarParameterVar = NewObject<UMaterialExpressionScalarParameter>(OuterMat);
		ScalarParameterVar->ParameterName = *InVar[Index].Name;
		ScalarParameterVar->DefaultValue = InVar[Index].Value;
		OuterMat->Expressions.Add(ScalarParameterVar);
		ScalarParameterVariablesNode.Add(ScalarParameterVar);
	}

	return ScalarParameterVariablesNode;
}

TArray<UMaterialExpressionSine*> FFromJsonToMaterialModule::FromArraySineStructToNode(UMaterial* OuterMat, const TArray<MaterialSine> InSines)
{
	TArray<UMaterialExpressionSine*> Sines;
	for (MaterialSine InSine : InSines)
	{
		UMaterialExpressionSine* Sine = NewObject<UMaterialExpressionSine>(OuterMat);
		if (InSine.ConPeriod.IsEmpty())
		{
			Sine->Period = InSine.Period;
		}
		else
		{
			FString Param = InSine.ConPeriod;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Sine, 0, PosParam));
		}

		OuterMat->Expressions.Add(Sine);
		Sines.Add(Sine);
	}

	return Sines;
}

TArray<UMaterialExpressionConstantBiasScale*> FFromJsonToMaterialModule::FromArrayConstantBiasStructToNode(UMaterial* OuterMat, const TArray<MaterialConstantBias> InBiass)
{
	TArray<UMaterialExpressionConstantBiasScale*> ConBiasArr;
	for (MaterialConstantBias InBias : InBiass)
	{
		UMaterialExpressionConstantBiasScale* Bias = NewObject<UMaterialExpressionConstantBiasScale>(OuterMat);
		if (!InBias.ConScale.IsEmpty())
		{
			FString Param = InBias.ConScale;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Bias, 0, PosParam));
		}

		OuterMat->Expressions.Add(Bias);
		ConBiasArr.Add(Bias);
	}

	return ConBiasArr;
}

TArray<UMaterialExpressionLinearInterpolate*> FFromJsonToMaterialModule::FromArrayLerpStructToNode(UMaterial* OuterMat, const TArray<MaterialLerp> InLerps)
{
	TArray<UMaterialExpressionLinearInterpolate*> LerpArr;
	for (MaterialLerp InLerp : InLerps)
	{
		UMaterialExpressionLinearInterpolate* Lerp = NewObject<UMaterialExpressionLinearInterpolate>(OuterMat);
		if (InLerp.ConA.IsEmpty())
		{
			Lerp->ConstA = InLerp.A;
		}
		else
		{
			FString Param = InLerp.ConA;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Lerp, 0, PosParam));
		}

		if (InLerp.ConB.IsEmpty())
		{
			Lerp->ConstB = InLerp.B;
		}
		else
		{
			FString Param = InLerp.ConB;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Lerp, 1, PosParam));
		}

		if (InLerp.ConAlpha.IsEmpty())
		{
			Lerp->ConstAlpha = InLerp.Alpha;
		}
		else
		{
			FString Param = InLerp.ConAlpha;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Lerp, 2, PosParam));
		}
		OuterMat->Expressions.Add(Lerp);
		LerpArr.Add(Lerp);
	}

	return LerpArr;
}

TArray<UMaterialExpressionSphereMask*> FFromJsonToMaterialModule::FromArraySphereMaskStructToNode(UMaterial* OuterMat, const TArray<MaterialSphereMask> InSpheres)
{
	TArray<UMaterialExpressionSphereMask*> SphereMaskArr;
	for (MaterialSphereMask InSphere : InSpheres)
	{
		UMaterialExpressionSphereMask* SphereMask = NewObject<UMaterialExpressionSphereMask>(OuterMat);
		if (!InSphere.ConA.IsEmpty())
		{
			FString Param = InSphere.ConA;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], SphereMask, 0, PosParam));
		}

		if (!InSphere.ConB.IsEmpty())
		{
			FString Param = InSphere.ConB;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], SphereMask, 1, PosParam));
		}

		if (InSphere.ConRadius.IsEmpty())
		{
			SphereMask->AttenuationRadius = InSphere.Radius;
		}
		else
		{
			FString Param = InSphere.ConRadius;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], SphereMask, 2, PosParam));
		}

		if (InSphere.ConHardness.IsEmpty())
		{
			SphereMask->HardnessPercent = InSphere.Hardness;
		}
		else
		{
			FString Param = InSphere.ConHardness;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], SphereMask, 3, PosParam));
		}

		OuterMat->Expressions.Add(SphereMask);
		SphereMaskArr.Add(SphereMask);
	}

	return SphereMaskArr;
}

TArray<UMaterialExpressionOneMinus*> FFromJsonToMaterialModule::FromArrayOneMinusStructToNode(UMaterial* OuterMat, const TArray<MaterialOneMinus> InOneMinuses)
{
	TArray<UMaterialExpressionOneMinus*> OneMinusArr;
	for (MaterialOneMinus InOneMinus : InOneMinuses)
	{
		UMaterialExpressionOneMinus* OneMinus = NewObject<UMaterialExpressionOneMinus>(OuterMat);
		if (!InOneMinus.Value.IsEmpty())
		{
			FString Param = InOneMinus.Value;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], OneMinus, 0, PosParam));
		}

		OuterMat->Expressions.Add(OneMinus);
		OneMinusArr.Add(OneMinus);
	}

	return OneMinusArr;
}

TArray<UMaterialExpressionSaturate*> FFromJsonToMaterialModule::FromArraySaturateStructToNode(UMaterial* OuterMat, const TArray<MaterialSaturate> InSaturates)
{
	TArray<UMaterialExpressionSaturate*> SaturatesArr;
	for (MaterialSaturate InSaturate : InSaturates)
	{
		UMaterialExpressionSaturate* Saturate = NewObject<UMaterialExpressionSaturate>(OuterMat);
		if (!InSaturate.Value.IsEmpty())
		{
			FString Param = InSaturate.Value;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Saturate, 0, PosParam));
		}

		OuterMat->Expressions.Add(Saturate);
		SaturatesArr.Add(Saturate);
	}

	return SaturatesArr;
}

TArray<UMaterialExpressionTransformPosition*> FFromJsonToMaterialModule::FromArrayTransformPositionStructToNode(UMaterial* OuterMat, const TArray<MaterialTransformPosition> InTransforms)
{
	TArray<UMaterialExpressionTransformPosition*> TransformPositionArr;
	for (MaterialTransformPosition InTransform : InTransforms)
	{
		UMaterialExpressionTransformPosition* TransformPosition = NewObject<UMaterialExpressionTransformPosition>(OuterMat);
		TransformPosition->TransformSourceType = EMaterialPositionTransformSource::TRANSFORMPOSSOURCE_World;
		if (!InTransform.ConTransform.IsEmpty())
		{
			FString Param = InTransform.ConTransform;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], TransformPosition, 0, PosParam));
		}

		OuterMat->Expressions.Add(TransformPosition);
		TransformPositionArr.Add(TransformPosition);
	}

	return TransformPositionArr;
}

TArray<UMaterialExpressionComponentMask*> FFromJsonToMaterialModule::FromArrayMaskStructToNode(UMaterial* OuterMat, const TArray<MaterialMask> InMasks)
{
	TArray<UMaterialExpressionComponentMask*> MaskArr;
	for (MaterialMask InMask : InMasks)
	{
		UMaterialExpressionComponentMask* Mask = NewObject<UMaterialExpressionComponentMask>(OuterMat);
		if (!InMask.Value.IsEmpty())
		{
			FString Param = InMask.Value;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Mask, 0, PosParam));
		}
		Mask->R = InMask.R;
		Mask->G = InMask.G;
		Mask->B = InMask.B;
		Mask->A = InMask.A;

		OuterMat->Expressions.Add(Mask);
		MaskArr.Add(Mask);
	}

	return MaskArr;
}

TArray<UMaterialExpressionPanner*> FFromJsonToMaterialModule::FromArrayPannerStructToNode(UMaterial* OuterMat, const TArray<MaterialPanner> InPanners)
{
	TArray<UMaterialExpressionPanner*> PannerArr;
	for (MaterialPanner InPanner : InPanners)
	{
		UMaterialExpressionPanner* Panner = NewObject<UMaterialExpressionPanner>(OuterMat);
		if (!InPanner.Coordinate.IsEmpty())
		{
			FString Param = InPanner.Coordinate;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Panner, 0, PosParam));
		}
		if (!InPanner.Time.IsEmpty())
		{
			FString Param = InPanner.Time;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Panner, 1, PosParam));
		}
		if (!InPanner.Speed.IsEmpty())
		{
			FString Param = InPanner.Speed;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Panner, 2, PosParam));
		}

		OuterMat->Expressions.Add(Panner);
		PannerArr.Add(Panner);
	}

	return PannerArr;
}

TArray<UMaterialExpressionTextureSample*> FFromJsonToMaterialModule::FromArrayTextureSampleStructToNode(UMaterial* OuterMat, const TArray<MaterialTextureSample> InTextures)
{
	TArray<UMaterialExpressionTextureSample*> TextureSampleArr;
	for (MaterialTextureSample InTexture : InTextures)
	{
		UMaterialExpressionTextureSample* TextureSample = NewObject<UMaterialExpressionTextureSample>(OuterMat);
		if (!InTexture.UVs.IsEmpty())
		{
			FString Param = InTexture.UVs;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], TextureSample, 0, PosParam));
		}
		if (!InTexture.Tex.IsEmpty())
		{
			FString Param = InTexture.Tex;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], TextureSample, 1, PosParam));
		}
		if (!InTexture.ApplyViewMipBias.IsEmpty())
		{
			FString Param = InTexture.ApplyViewMipBias;
			TArray<FString> ParamArr;
			Param.ParseIntoArray(ParamArr, TEXT("."));
			int32 PosParam = FCString::Atoi(*ParamArr[1]);
			Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], TextureSample, 2, PosParam));
		}

		if (!InTexture.TexturePath.IsEmpty())
		{
			FString Param = InTexture.TexturePath;
			UTexture* Texture = LoadObject<UTexture>(nullptr, *InTexture.TexturePath);
			TextureSample->Texture = Texture;
		}

		OuterMat->Expressions.Add(TextureSample);
		TextureSampleArr.Add(TextureSample);
	}

	return TextureSampleArr;
}

TArray<UMaterialExpressionCustom*> FFromJsonToMaterialModule::FromArrayCustomExpressionStructToNode(UMaterial* OuterMat, const TArray<MaterialCustomExpression> InCustoms)
{
	TArray<UMaterialExpressionCustom*> CustomExpressionArr;
	for (MaterialCustomExpression InCustom : InCustoms)
	{
		UMaterialExpressionCustom* CustomExpression = NewObject<UMaterialExpressionCustom>(OuterMat);
		if (!InCustom.Code.IsEmpty())
		{
			CustomExpression->Code = InCustom.Code;
		}
		CustomExpression->OutputType = static_cast<ECustomMaterialOutputType>(InCustom.OutputType);
		OuterMat->Expressions.Add(CustomExpression);
		CustomExpressionArr.Add(CustomExpression);
	}
	return CustomExpressionArr;
}

void FFromJsonToMaterialModule::ConnectAllBesideAppearance(UMaterial* InMat, TArray<TTuple<FString, UMaterialExpression*, int32, int32>> InConnections)
{
	TArray<FString> ParamArr;
	for (TTuple<FString, UMaterialExpression*, int32, int32> InConnection : InConnections)
	{
		for (UMaterialExpression* Exp : InMat->Expressions)
		{
			if (Exp->GetParameterName().ToString().Equals(InConnection.Get<0>(), ESearchCase::IgnoreCase) ||
				Exp->GetName().Equals(InConnection.Get<0>(), ESearchCase::IgnoreCase))
			{
				Exp->ConnectExpression(InConnection.Get<1>()->GetInput(InConnection.Get<2>()), InConnection.Get<3>());
			}
		}
	}
}

void FFromJsonToMaterialModule::SetMaterialOption(UMaterial* OuterMat, const MaterialOption InOption)
{
	OuterMat->MaterialDomain = static_cast<EMaterialDomain>(InOption.MaterialDomain);
	OuterMat->BlendMode = static_cast<EBlendMode>(InOption.MaterialBlendMode);
	OuterMat->SetShadingModel(static_cast<EMaterialShadingModel>(InOption.MaterialShadingModel));
}

void FFromJsonToMaterialModule::ConnectAppearance(UMaterial* OuterMat, const MaterialAppearance InApp)
{
	if (!InApp.ConBaseColor.IsEmpty())
	{
		FString Param = InApp.ConBaseColor;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		for (UMaterialExpression* Exp : OuterMat->Expressions)
		{
			if (Exp->GetParameterName().ToString().Equals(ParamArr[0], ESearchCase::IgnoreCase) ||
				Exp->GetName().Equals(ParamArr[0], ESearchCase::IgnoreCase))
			{
				int32 PosParam = FCString::Atoi(*ParamArr[1]);
				Exp->ConnectExpression(&OuterMat->BaseColor, PosParam);
			}
		}
	}
	if (!InApp.ConMetallic.IsEmpty())
	{
		FString Param = InApp.ConMetallic;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		for (UMaterialExpression* Exp : OuterMat->Expressions)
		{
			if (Exp->GetParameterName().ToString().Equals(ParamArr[0], ESearchCase::IgnoreCase) ||
				Exp->GetName().Equals(ParamArr[0], ESearchCase::IgnoreCase))
			{
				int32 PosParam = FCString::Atoi(*ParamArr[1]);
				Exp->ConnectExpression(&OuterMat->Metallic, PosParam);
			}
		}
	}
	if (!InApp.ConEmissiveColor.IsEmpty())
	{
		FString Param = InApp.ConEmissiveColor;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		for (UMaterialExpression* Exp : OuterMat->Expressions)
		{
			if (Exp->GetParameterName().ToString().Equals(ParamArr[0], ESearchCase::IgnoreCase) ||
				Exp->GetName().Equals(ParamArr[0], ESearchCase::IgnoreCase))
			{
				int32 PosParam = FCString::Atoi(*ParamArr[1]);
				Exp->ConnectExpression(&OuterMat->EmissiveColor, PosParam);
			}
		}
	}

	if (!InApp.ConOpacityMask.IsEmpty())
	{
		FString Param = InApp.ConOpacityMask;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		for (UMaterialExpression* Exp : OuterMat->Expressions)
		{
			if (Exp->GetParameterName().ToString().Equals(ParamArr[0], ESearchCase::IgnoreCase) ||
				Exp->GetName().Equals(ParamArr[0], ESearchCase::IgnoreCase))
			{
				int32 PosParam = FCString::Atoi(*ParamArr[1]);
				Exp->ConnectExpression(&OuterMat->OpacityMask, PosParam);
			}
		}
	}
}

UMaterialExpression* FFromJsonToMaterialModule::CreateMultiply(UMaterial* OuterMat, const Material2OpStruct Op)
{
	UMaterialExpressionMultiply* Multiply = NewObject<UMaterialExpressionMultiply>(OuterMat);
	if (Op.ConA.IsEmpty())
	{
		Multiply->ConstA = Op.A;
	}
	else
	{
		FString Param = Op.ConA;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Multiply, 0, PosParam));
	}

	if (Op.ConB.IsEmpty())
	{
		Multiply->ConstB = Op.B;
	}
	else
	{
		FString Param = Op.ConB;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Multiply, 1, PosParam));

	}

	OuterMat->Expressions.Add(Multiply);
	return Multiply;
}

UMaterialExpression* FFromJsonToMaterialModule::CreateAppend(UMaterial* OuterMat, const Material2OpStruct Op)
{
	UMaterialExpressionAppendVector* Append = NewObject<UMaterialExpressionAppendVector>(OuterMat);
	if (!Op.ConA.IsEmpty())
	{
		FString Param = Op.ConA;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Append, 0, PosParam));
	}
	if (!Op.ConB.IsEmpty())
	{
		FString Param = Op.ConB;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Append, 1, PosParam));
	}

	OuterMat->Expressions.Add(Append);
	return Append;
}

UMaterialExpression* FFromJsonToMaterialModule::CreateAdd(UMaterial* OuterMat, const Material2OpStruct Op)
{
	UMaterialExpressionAdd* Add = NewObject<UMaterialExpressionAdd>(OuterMat);
	if (!Op.ConA.IsEmpty())
	{
		FString Param = Op.ConA;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Add, 0, PosParam));
	}
	if (!Op.ConB.IsEmpty())
	{
		FString Param = Op.ConB;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Add, 1, PosParam));
	}

	OuterMat->Expressions.Add(Add);
	return Add;
}

UMaterialExpression* FFromJsonToMaterialModule::CreateSubstract(UMaterial* OuterMat, const Material2OpStruct Op)
{
	UMaterialExpressionSubtract* Sub = NewObject<UMaterialExpressionSubtract>(OuterMat);
	if (!Op.ConA.IsEmpty())
	{
		FString Param = Op.ConA;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Sub, 0, PosParam));
	}
	if (!Op.ConB.IsEmpty())
	{
		FString Param = Op.ConB;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Sub, 1, PosParam));
	}

	OuterMat->Expressions.Add(Sub);
	return Sub;
}

UMaterialExpression* FFromJsonToMaterialModule::CreatePower(UMaterial* OuterMat, const Material2OpStruct Op)
{
	UMaterialExpressionPower* Power = NewObject<UMaterialExpressionPower>(OuterMat);
	if (!Op.ConA.IsEmpty())
	{
		FString Param = Op.ConA;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Power, 0, PosParam));
	}
	if (!Op.ConB.IsEmpty())
	{
		FString Param = Op.ConB;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Power, 1, PosParam));
	}

	OuterMat->Expressions.Add(Power);
	return Power;
}

UMaterialExpression* FFromJsonToMaterialModule::CreateDivide(UMaterial* OuterMat, const Material2OpStruct Op)
{
	UMaterialExpressionDivide* Divide = NewObject<UMaterialExpressionDivide>(OuterMat);
	if (!Op.ConA.IsEmpty())
	{
		FString Param = Op.ConA;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Divide, 0, PosParam));
	}
	if (!Op.ConB.IsEmpty())
	{
		FString Param = Op.ConB;
		TArray<FString> ParamArr;
		Param.ParseIntoArray(ParamArr, TEXT("."));
		int32 PosParam = FCString::Atoi(*ParamArr[1]);
		Connection.Add(TTuple<FString, UMaterialExpression*, int32, int32>(ParamArr[0], Divide, 1, PosParam));
	}

	OuterMat->Expressions.Add(Divide);
	return Divide;
}

void FFromJsonToMaterialModule::CreateMaterialFromPath(FString Path)
{
	FString JsonStr;
	FFileHelper::LoadFileToString(JsonStr, *Path);
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonStr);
	TSharedPtr<FJsonObject> JsonParsed = MakeShareable(new FJsonObject);
	TArray<MaterialStruct> Materials;
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed) && JsonParsed.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Converting"));
		Materials = ParseJsonMaterialArray(JsonParsed, "Materials");
		for (MaterialStruct Material : Materials)
		{
			FromStructToMaterial(Material);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Json not correct"));
	}
}



bool FFromJsonToMaterialModule::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FParse::Command(&Cmd, TEXT("newmaterial")))
	{
		FString Path = (TEXT("%s"), Cmd);
		CreateMaterialFromPath(Path);
		return true;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFromJsonToMaterialModule, FromJsonToMaterial)