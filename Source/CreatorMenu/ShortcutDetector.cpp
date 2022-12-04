#pragma once
#include "ShortcutDetector.h"

#include "CreatorBoard.h"
#include "CreatorMenu.h"
#include "Logger.h"
#include "TileColor.h"
#include "CreatorRotator.h"
#include "CreatorTile.h"
#include "DragSelect.h"
#include "MeshGenerator.h"
#include "SelectionAngle.h"
#include "ToggleButton.h"

// TODO:: pressing A or D should automatically switch to Rect mode then applies the angle
UShortcutDetector::UShortcutDetector() { }

// ReSharper disable once CppMemberFunctionMayBeConst
void UShortcutDetector::AnyKey(const FKey key) { CallShortcut(key); }
void UShortcutDetector::BeginPlay()
{
    Super::BeginPlay();
    controller = GetWorld()->GetFirstPlayerController();
    controller->InputComponent->BindAction("AnyKey", IE_Released, this, &UShortcutDetector::AnyKey);

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
    const auto SetColor = [this](const ETileColor color)
    {
        const auto button = creatorMenu->GetColorButton(color);
        button->OnToggle();
        UColorCast::ColorCreatorTiles(color);
    };

    shortcuts =
    {
        // No Modifiers
        new ModifiedShortcuts({
            { EKeys::L,         [] { Log("Level Select"); } },
            { EKeys::Enter,     [] { Log("Return"); } },
            { EKeys::Escape,    [] { Log("Escape"); } },
            { EKeys::Delete,    [] { Log("Delete"); } },
            { EKeys::BackSpace, [] { Log("Backspace"); } },
            { EKeys::Q, [this, SetSelectionShape] { SetSelectionShape(ESelectionShape::Triangle); } },
            { EKeys::W, [this, SetSelectionShape] { SetSelectionShape(ESelectionShape::Square); } },
            { EKeys::E, [this, SetSelectionShape] { SetSelectionShape(ESelectionShape::Circle); } },
            { EKeys::A, [this, SetSelectionAngle] { SetSelectionAngle(ESelectionAngle::Downhill); } },
            { EKeys::S, [this, SetSelectionAngle] { SetSelectionAngle(ESelectionAngle::Flat); } },
            { EKeys::D, [this, SetSelectionAngle] { SetSelectionAngle(ESelectionAngle::Uphill); } },
            { EKeys::M, [this]
                {
                    UMeshGenerator::TilesToMerge = ACreatorTile::SelectedTiles;
                    UMeshGenerator::Merge();
                }
            },
        }),
        // Shift
        new ModifiedShortcuts({
            { EKeys::W, [this, SetColor] { SetColor(ETileColor::White); } },
            { EKeys::R, [this, SetColor] { SetColor(ETileColor::Red); } },
            { EKeys::O, [this, SetColor] { SetColor(ETileColor::Orange); } },
            { EKeys::Y, [this, SetColor] { SetColor(ETileColor::Yellow); } },
            { EKeys::G, [this, SetColor] { SetColor(ETileColor::Green); } },
            { EKeys::C, [this, SetColor] { SetColor(ETileColor::Cyan); } },
            { EKeys::B, [this, SetColor] { SetColor(ETileColor::Blue); } },
            { EKeys::P, [this, SetColor] { SetColor(ETileColor::Purple); } },
         // { EKeys::M, [this, SetColor]{ SetColor(ETileColor::Maroon);  } }, 
            { EKeys::T, [this, SetColor] { SetColor(ETileColor::Magenta); } } ,
            { EKeys::N, [this, SetColor] { SetColor(ETileColor::Pink); } },
            { EKeys::K, [this, SetColor] { SetColor(ETileColor::Black); } },
        }),
        // Ctrl
        new ModifiedShortcuts({
            { EKeys::W, [] { Log("Wand"); } },
            { EKeys::Z, [] { Log("Undo"); } },
            { EKeys::C, [] { Log("Copy"); } },
            { EKeys::V, [] { Log("Paste"); } },
            { EKeys::X, [] { Log("Cut"); } },
            { EKeys::A, [] { Log("All"); } },
            { EKeys::R, [] { Log("Refresh"); } },
            { EKeys::D, [] { Log("Deselect"); } },
            { EKeys::N, [] { Log("Size Board"); } },
            { EKeys::S, [] { Log("Save"); } },
        }),
        // Shift Ctrl
        new ModifiedShortcuts({
            { EKeys::Z, [] { Log("Redo"); } },
            { EKeys::A, [] { Log("Select All"); } },
        }),
        // Alt
        new ModifiedShortcuts({
            { EKeys::X, [] { Log("Gap"); } },
            { EKeys::W, [] { Log("Wrap"); } },
            { EKeys::C, [] { Log("Camo"); } },
            { EKeys::B, [] { Log("Bandaged"); } },
            { EKeys::R, [] { Log("Rotator"); } },
            { EKeys::I, [] { Log("Iron"); } },
            { EKeys::U, [] { Log("Cloud"); } },
            { EKeys::F, [] { Log("Rift"); } },
            { EKeys::L, [] { Log("Link"); } },
            { EKeys::S, [] { Log("Swap"); } },
        }),
        // Shift Alt
        new ModifiedShortcuts({
            { EKeys::Z, [] { Log("Shift Alt Z"); } },
        }),
        // Ctrl Alt
        new ModifiedShortcuts({ {
            EKeys::Z, [] { Log("Ctrl Alt Z"); } },
        }),
        // Shift Ctrl Alt
        new ModifiedShortcuts({ {
            EKeys::Z, [] { Log("Shift Ctrl Alt Z"); } },
        }),
    };
}

bool UShortcutDetector::Key(const FKey key)     const { return controller->IsInputKeyDown(key); }
bool UShortcutDetector::KeyDown(const FKey key) const { return controller->WasInputKeyJustPressed(key); }

bool UShortcutDetector::Shift() const { return Key(EKeys::LeftShift)   || Key(EKeys::RightShift); }
bool UShortcutDetector::Ctrl()  const { return Key(EKeys::LeftControl) || Key(EKeys::RightControl); }
bool UShortcutDetector::Alt()   const { return Key(EKeys::LeftAlt)     || Key(EKeys::RightAlt); }
//458