#include "ShortcutDetector.h"

#include "CreatorBoard.h"
#include "CreatorMenu.h"
#include "Logger.h"
#include "TileColor.h"
#include "CreatorRotator.h"
#include "CreatorTile.h"
#include "DragSelect.h"
#include "MeshGenerator.h"
#include "ModuleFactory.h"
#include "SelectionAngle.h"
#include "ToggleButton.h"
#include "Kismet/GameplayStatics.h"

UShortcutDetector::UShortcutDetector() { }

void UShortcutDetector::SelectAdjacentTiles(ACreatorTile* creatorTile, TArray<ACreatorTile*>& creatorTiles)
{
    if (creatorTile->GetColor() != ACreatorTile::FirstSelectedTile()->GetColor()
        || creatorTiles.Contains(creatorTile))
        return;
    creatorTiles.Add(creatorTile);
    creatorTile->Select(true, true);

    for (auto adjacent : creatorTile->GetNeighbors())
        SelectAdjacentTiles(Cast<ACreatorTile>(adjacent), creatorTiles);
}
// ReSharper disable once CppMemberFunctionMayBeConst
void UShortcutDetector::AnyKey(const FKey key)
{
    CallShortcut(key);

    EModifier modifier = key == EKeys::LeftShift   || key == EKeys::RightShift   ? EModifier::Shift
                        : key == EKeys::LeftControl || key == EKeys::RightControl ? EModifier::Control
                        : key == EKeys::LeftAlt     || key == EKeys::RightAlt     ? EModifier::Alt
                        : EModifier::None;
    if (modifierTimes.Contains(modifier))
        modifierTimes[modifier] = GetWorld()->TimeSeconds;
}
void UShortcutDetector::BeginPlay()
{
    Super::BeginPlay();
    controller = GetWorld()->GetFirstPlayerController();
    controller->InputComponent->BindAction("AnyKey", IE_Released, this, &UShortcutDetector::AnyKey);
    
    modifierTimes = TMap<EModifier, float> { { EModifier::Shift, 0 },
                                             { EModifier::Control, 0 },
                                             { EModifier::Alt, 0 } };
    
    const auto SetSelectionShape = [this](const ESelectionShape shape)
    {
        const auto button = creatorMenu->GetShapeButton(shape);
        button->OnToggle();
        dragSelect->ChangeSelectionShape(shape);
    };
    const auto SetSelectionAngle = [SetSelectionShape, this](const ESelectionAngle angle)
    {
        if (dragSelect->Shape() != ESelectionShape::Square)
            SetSelectionShape(ESelectionShape::Square);
        const auto button = creatorMenu->GetAngleButton(angle);
        button->OnToggle();
        rotator->SetSelectionType(angle);
    };
    const auto SetColor          = [this](const ETileColor color)
    {
        const auto button = creatorMenu->GetColorButton(color);
        button->OnToggle();
        UColorCast::ColorCreatorTiles(color);
    };                   
    const auto AddModule         = [this](const EModule module)
    {
        for (const auto c : ACreatorTile::SelectedTiles())
            if (!c->HasModule(module))
                c->AddModule(ModuleFactory::Produce(module, c));
    };

    shortcuts =
    {
#pragma region NoModifier
        new ModifiedShortcuts({
            { EKeys::L,         [] { Log("Level Select"); } },
            { EKeys::Enter,     [] { Log("Return"); } },
            { EKeys::Escape,    [] { ACreatorTile::DeselectAll(); } },
            { EKeys::Delete,    [] { Log("Delete"); } },
            { EKeys::BackSpace, [] { Log("Backspace"); } },
            { EKeys::Q, [this, SetSelectionShape] { SetSelectionShape(ESelectionShape::Triangle); } },
            { EKeys::W, [this, SetSelectionShape] { SetSelectionShape(ESelectionShape::Square); } },
            { EKeys::E, [this, SetSelectionShape] { SetSelectionShape(ESelectionShape::Circle); } },
            { EKeys::A, [this, SetSelectionAngle] { SetSelectionAngle(ESelectionAngle::Downhill); } },
            { EKeys::S, [this, SetSelectionAngle] { SetSelectionAngle(ESelectionAngle::Flat); } },
            { EKeys::D, [this, SetSelectionAngle] { SetSelectionAngle(ESelectionAngle::Uphill); } },
            { EKeys::M, [this] { UMeshGenerator::Merge(false); } },
        }),
#pragma endregion
#pragma region Shift
        new ModifiedShortcuts({
            { EKeys::W, [this, SetColor] { SetColor(ETileColor::White); } },
            { EKeys::R, [this, SetColor] { SetColor(ETileColor::Red); } },
            { EKeys::O, [this, SetColor] { SetColor(ETileColor::Orange); } },
            { EKeys::Y, [this, SetColor] { SetColor(ETileColor::Yellow); } },
            { EKeys::G, [this, SetColor] { SetColor(ETileColor::Green); } },
            { EKeys::C, [this, SetColor] { SetColor(ETileColor::Cyan); } },
            { EKeys::B, [this, SetColor] { SetColor(ETileColor::Blue); } },
            { EKeys::P, [this, SetColor] { SetColor(ETileColor::Purple); } },
            { EKeys::M, [this, SetColor] { SetColor(ETileColor::Maroon);  } }, 
            { EKeys::T, [this, SetColor] { SetColor(ETileColor::Magenta); } } ,
            { EKeys::N, [this, SetColor] { SetColor(ETileColor::Pink); } },
            { EKeys::K, [this, SetColor] { SetColor(ETileColor::Black); } },
        }),
#pragma endregion
#pragma region Ctrl
        new ModifiedShortcuts({
            { EKeys::W, [this]
            {
                if (ACreatorTile::SelectedTiles().Num() == 0)
                    return;
                TArray<ACreatorTile*> tiles;
                SelectAdjacentTiles(ACreatorTile::FirstSelectedTile(), tiles);
            } },
            { EKeys::Z, [] { Log("Undo"); } },
            { EKeys::C, [this] { creatorBoard->GetClipboard().Copy(); } },
            { EKeys::V, [this] { creatorBoard->GetClipboard().Paste(); } },
            { EKeys::X, [this] { creatorBoard->GetClipboard().Cut(); } },
            { EKeys::A, [this]
            {
                for (auto [_, tile] : creatorBoard->GetTiles())
                {
                    auto creatorTile = Cast<ACreatorTile>(tile);
                    creatorTile->Select(true, true);
                }
            } },
            { EKeys::R, [] { Log("Refresh"); } },
            { EKeys::D, [] { ACreatorTile::DeselectAll(); } },
            { EKeys::N, [] { Log("Size Board"); } },
            { EKeys::S, [] { Log("Save"); } },
            { EKeys::M, [] { UMeshGenerator::Unmerge(true); } },
        }),
#pragma endregion 
#pragma region Shift_Ctrl
        new ModifiedShortcuts({
            { EKeys::Z, [] { Log("Redo"); } },
            { EKeys::W, [this]
            {
                auto targetColor = ACreatorTile::SelectedTiles()[0]->GetColor();
                auto matchingTiles = creatorBoard->GetTiles().Values()
                    .FilterByPredicate([&](const ATile* t) { return ((ACreatorTile*)t)->GetColor() == targetColor; });
                for (auto matchingTile : matchingTiles)
                {
                    ((ACreatorTile*)matchingTile)->Select(true);
                }
            } },
            { EKeys::M, [] { UMeshGenerator::Merge(true); } },
        }),
#pragma endregion
#pragma region Alt
        new ModifiedShortcuts({
            { EKeys::C, [&] { AddModule(EModule::Camo); } },
            { EKeys::R, [&] { AddModule(EModule::Rotator); } },
            { EKeys::T, [&] { AddModule(EModule::TeleportWrap); } },
            { EKeys::L, [&] { AddModule(EModule::Link); } },
            { EKeys::X, [&] { Log("Gap"); } },
            { EKeys::W, [&] { Log("Wrap"); } },
            { EKeys::I, [&] { Log("Iron"); } },
            { EKeys::U, [&] { Log("Cloud"); } },
            { EKeys::F, [&] { Log("Rift"); } },
            { EKeys::S, [&] { Log("Swap"); } },
        }),
#pragma endregion
#pragma region Shift_Alt
        new ModifiedShortcuts({
            { EKeys::Z, [] { Log("Shift Alt Z"); } },
        }),
#pragma endregion
#pragma region Ctrl_Alt
        new ModifiedShortcuts({ {
            EKeys::Z, [] { Log("Ctrl Alt Z"); } },
        }),
#pragma endregion
#pragma region Shift_Ctrl_Alt
        new ModifiedShortcuts({ {
            EKeys::Z, [] { Log("Shift Ctrl Alt Z"); } },
        }),
#pragma endregion
    };
}
void UShortcutDetector::CallShortcut(const FKey key) const
{
    const int modifier = Alt() << 2 | Ctrl() << 1 | Shift() << 0;

    if (shortcuts[modifier]->Contains(key))
        (*shortcuts[modifier])[key]();
}

bool UShortcutDetector::Key(const FKey key)     const { return controller->IsInputKeyDown(key); }
bool UShortcutDetector::KeyDown(const FKey key) const { return controller->WasInputKeyJustPressed(key); }

bool UShortcutDetector::IsModifierDown(FKey left, FKey right, EModifier modifier) const {  return Key(left) || Key(right) || GetWorld()->TimeSeconds - modifierTimes[modifier] < 0.15f; }
bool UShortcutDetector::Shift() const { return IsModifierDown(EKeys::LeftShift,   EKeys::RightShift,   EModifier::Shift);   }
bool UShortcutDetector::Ctrl()  const { return IsModifierDown(EKeys::LeftControl, EKeys::RightControl, EModifier::Control); }
bool UShortcutDetector::Alt()   const { return IsModifierDown(EKeys::LeftAlt,     EKeys::RightAlt,     EModifier::Alt);     }
//458