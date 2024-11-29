// Copyright Jonathan Cote


#include "UI/Widget/RpgUserWidget.h"


void URpgUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}