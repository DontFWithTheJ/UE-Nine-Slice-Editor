#include "NineSliceEditor.h"

#include "Details/MarginCustomization.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "PropertyHandle.h"
#include "Styling/SlateBrush.h"
#include "UObject/UnrealType.h"

namespace
{
    class FBrushMarginIdentifier final : public IPropertyTypeIdentifier
    {
    public:
        bool IsPropertyTypeCustomized(const IPropertyHandle& Handle) const override
        {
            const TSharedPtr<IPropertyHandle> Parent = Handle.GetParentHandle();
            const FStructProperty* Property = Parent.IsValid() ? CastField<FStructProperty>(Parent->GetProperty()) : nullptr;
            return Property && Property->Struct == FSlateBrush::StaticStruct();
        }
    };
}

void FNineSliceEditor::StartupModule()
{
    FPropertyEditorModule& PropertyEditorModule =
        FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
            "PropertyEditor"
        );

    BrushMarginIdentifier = MakeShared<FBrushMarginIdentifier>();
    PropertyEditorModule.RegisterCustomPropertyTypeLayout(
        TEXT("Margin"),
        FOnGetPropertyTypeCustomizationInstance::CreateStatic(
            &FMarginCustomization::MakeInstance
        ),
        BrushMarginIdentifier
    );

    PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FNineSliceEditor::ShutdownModule()
{
    if (!FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
    {
        return;
    }

    FPropertyEditorModule& PropertyEditorModule =
        FModuleManager::GetModuleChecked<FPropertyEditorModule>(
            "PropertyEditor"
        );

    PropertyEditorModule.UnregisterCustomPropertyTypeLayout(
        TEXT("Margin"), BrushMarginIdentifier
    );

    PropertyEditorModule.NotifyCustomizationModuleChanged();
}

IMPLEMENT_MODULE(FNineSliceEditor, NineSliceEditor)