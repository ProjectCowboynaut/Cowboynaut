// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/OvodusButton.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOvodusButton() {}
// Cross Module References
	CWBUTTON_API UClass* Z_Construct_UClass_UOvodusButton_NoRegister();
	CWBUTTON_API UClass* Z_Construct_UClass_UOvodusButton();
	UMG_API UClass* Z_Construct_UClass_UButton();
	UPackage* Z_Construct_UPackage__Script_CWButton();
	CWBUTTON_API UFunction* Z_Construct_UFunction_UOvodusButton_SetAdvancedHitAlpha();
	CWBUTTON_API UFunction* Z_Construct_UFunction_UOvodusButton_SetAdvancedHitTexture();
	ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
// End Cross Module References
	void UOvodusButton::StaticRegisterNativesUOvodusButton()
	{
		UClass* Class = UOvodusButton::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "SetAdvancedHitAlpha", (Native)&UOvodusButton::execSetAdvancedHitAlpha },
			{ "SetAdvancedHitTexture", (Native)&UOvodusButton::execSetAdvancedHitTexture },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	UFunction* Z_Construct_UFunction_UOvodusButton_SetAdvancedHitAlpha()
	{
		struct OvodusButton_eventSetAdvancedHitAlpha_Parms
		{
			int32 InAlpha;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_InAlpha = { UE4CodeGen_Private::EPropertyClass::Int, "InAlpha", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(OvodusButton_eventSetAdvancedHitAlpha_Parms, InAlpha), METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InAlpha,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "AdvancedHitTest" },
				{ "ModuleRelativePath", "Public/OvodusButton.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UOvodusButton, "SetAdvancedHitAlpha", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(OvodusButton_eventSetAdvancedHitAlpha_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UOvodusButton_SetAdvancedHitTexture()
	{
		struct OvodusButton_eventSetAdvancedHitTexture_Parms
		{
			UTexture2D* InTexture;
		};
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_InTexture = { UE4CodeGen_Private::EPropertyClass::Object, "InTexture", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000080, 1, nullptr, STRUCT_OFFSET(OvodusButton_eventSetAdvancedHitTexture_Parms, InTexture), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(nullptr, 0) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_InTexture,
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
				{ "Category", "AdvancedHitTest" },
				{ "ModuleRelativePath", "Public/OvodusButton.h" },
			};
#endif
			static const UE4CodeGen_Private::FFunctionParams FuncParams = { (UObject*(*)())Z_Construct_UClass_UOvodusButton, "SetAdvancedHitTexture", RF_Public|RF_Transient|RF_MarkAsNative, nullptr, (EFunctionFlags)0x04020401, sizeof(OvodusButton_eventSetAdvancedHitTexture_Parms), PropPointers, ARRAY_COUNT(PropPointers), 0, 0, METADATA_PARAMS(Function_MetaDataParams, ARRAY_COUNT(Function_MetaDataParams)) };
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UOvodusButton_NoRegister()
	{
		return UOvodusButton::StaticClass();
	}
	UClass* Z_Construct_UClass_UOvodusButton()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			static UObject* (*const DependentSingletons[])() = {
				(UObject* (*)())Z_Construct_UClass_UButton,
				(UObject* (*)())Z_Construct_UPackage__Script_CWButton,
			};
			static const FClassFunctionLinkInfo FuncInfo[] = {
				{ &Z_Construct_UFunction_UOvodusButton_SetAdvancedHitAlpha, "SetAdvancedHitAlpha" }, // 2134695044
				{ &Z_Construct_UFunction_UOvodusButton_SetAdvancedHitTexture, "SetAdvancedHitTexture" }, // 3492827066
			};
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
				{ "IncludePath", "OvodusButton.h" },
				{ "ModuleRelativePath", "Public/OvodusButton.h" },
				{ "ObjectInitializerConstructorDeclared", "" },
				{ "ToolTip", "Classic UMG Button with Advanced Hit implementation.\nUse SetAdvancedHitTexture to set unique button's geometry, determined by it's alpha channel.\nUse SetAdvancedHitAlpha to set an integer in range 0-255. If pixel's alpha is lower than this value, it will be treated as an empty space." },
			};
#endif
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AdvancedHitAlpha_MetaData[] = {
				{ "Category", "AdvancedHitTest" },
				{ "ClampMax", "255.0" },
				{ "ClampMin", "0.0" },
				{ "ModuleRelativePath", "Public/OvodusButton.h" },
				{ "UIMax", "255.0" },
				{ "UIMin", "0.0" },
			};
#endif
			static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_AdvancedHitAlpha = { UE4CodeGen_Private::EPropertyClass::Int, "AdvancedHitAlpha", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UOvodusButton, AdvancedHitAlpha), METADATA_PARAMS(NewProp_AdvancedHitAlpha_MetaData, ARRAY_COUNT(NewProp_AdvancedHitAlpha_MetaData)) };
#if WITH_METADATA
			static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AdvancedHitTexture_MetaData[] = {
				{ "Category", "AdvancedHitTest" },
				{ "ModuleRelativePath", "Public/OvodusButton.h" },
			};
#endif
			static const UE4CodeGen_Private::FObjectPropertyParams NewProp_AdvancedHitTexture = { UE4CodeGen_Private::EPropertyClass::Object, "AdvancedHitTexture", RF_Public|RF_Transient|RF_MarkAsNative, 0x0010000000000015, 1, nullptr, STRUCT_OFFSET(UOvodusButton, AdvancedHitTexture), Z_Construct_UClass_UTexture2D_NoRegister, METADATA_PARAMS(NewProp_AdvancedHitTexture_MetaData, ARRAY_COUNT(NewProp_AdvancedHitTexture_MetaData)) };
			static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[] = {
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AdvancedHitAlpha,
				(const UE4CodeGen_Private::FPropertyParamsBase*)&NewProp_AdvancedHitTexture,
			};
			static const FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
				TCppClassTypeTraits<UOvodusButton>::IsAbstract,
			};
			static const UE4CodeGen_Private::FClassParams ClassParams = {
				&UOvodusButton::StaticClass,
				DependentSingletons, ARRAY_COUNT(DependentSingletons),
				0x00A00080u,
				FuncInfo, ARRAY_COUNT(FuncInfo),
				PropPointers, ARRAY_COUNT(PropPointers),
				nullptr,
				&StaticCppClassTypeInfo,
				nullptr, 0,
				METADATA_PARAMS(Class_MetaDataParams, ARRAY_COUNT(Class_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUClass(OuterClass, ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UOvodusButton, 1097004954);
	static FCompiledInDefer Z_CompiledInDefer_UClass_UOvodusButton(Z_Construct_UClass_UOvodusButton, &UOvodusButton::StaticClass, TEXT("/Script/CWButton"), TEXT("UOvodusButton"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UOvodusButton);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
