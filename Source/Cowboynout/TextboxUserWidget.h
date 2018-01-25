// you no take candle!

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextboxUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class COWBOYNOUT_API UTextboxUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(Category = "UI", BlueprintImplementableEvent, BlueprintCallable)
	void SetTextbox(const FString& message, bool newVisibility);

	
	
};
