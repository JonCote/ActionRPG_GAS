// Copyright Jonathan Cote


#include "UI/LayerSystem/LayerWidget.h"

#include "UI/Widget/RpgUserWidget.h"


int32 ULayerWidget::PushToStack(URpgUserWidget* Widget)
{
	return Stack.Add(Widget);
}

URpgUserWidget* ULayerWidget::PopFromStack()
{
	return Stack.Pop();
}

URpgUserWidget* ULayerWidget::PeekStack() const
{
	if (StackIsEmpty()) return nullptr;
	
	return Stack.Top();
}

bool ULayerWidget::StackIsEmpty() const
{
	return Stack.IsEmpty();
}

void ULayerWidget::ClearStack()
{
	for (URpgUserWidget* Widget : Stack)
	{
		if (Widget)
		{
			Widget->RemoveFromParent();
		}
	}
	Stack.Empty();
}

TArray<URpgUserWidget*> ULayerWidget::GetStack() const
{
	return Stack;
}
