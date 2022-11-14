#pragma once
#include "ShortcutDetector.h"
#include "ShortcutDetector.h"
#include "Logger.h"
#include "TileColor.h"
#include "CreatorRotator.h"
#include "DragSelect.h"
#include "SelectionAngle.h"

UShortcutDetector::UShortcutDetector()
{

}
// ReSharper disable once CppMemberFunctionMayBeConst
void UShortcutDetector::AnyKey(const FKey key)
{
    CallShortcut(key);
}

void UShortcutDetector::BeginPlay()
{
   Super::BeginPlay();
   controller = GetWorld()->GetFirstPlayerController();
   controller->InputComponent->BindAction("AnyKey", IE_Released, this, &UShortcutDetector::AnyKey);

   const auto SetSelectionType = [this] { rotator ->SetSelectionType(ESelectionAngle::Downhill); };

   shortcuts = 
   {
       // No Modifiers
       new ModifiedShortcuts
       ({
           { EKeys::L, [] { Log("Level Select"); } },
           { EKeys::BackSpace, [] { Log("Backspace"); } },
           { EKeys::Enter, [] { Log("Return"); } },
           { EKeys::Escape, [] { Log("Escape"); } },
           { EKeys::Delete, [] { Log("Delete"); } },
           { EKeys::Q, [this] { dragSelect->ChangeSelectionShape(ESelectionShape::Triangle); } },
           { EKeys::W, [this] { dragSelect->ChangeSelectionShape(ESelectionShape::Square); } },
           { EKeys::E, [this] { dragSelect->ChangeSelectionShape(ESelectionShape::Circle); } },
           { EKeys::One, [this]{ rotator->SetSelectionType(ESelectionAngle::Downhill); } },
           { EKeys::NumPadOne, [this]{ rotator->SetSelectionType(ESelectionAngle::Downhill); } },
           { EKeys::Two, [this]{ rotator->SetSelectionType(ESelectionAngle::Flat); } },
           { EKeys::NumPadTwo, [this]{ rotator->SetSelectionType(ESelectionAngle::Flat); } },
           { EKeys::Three, [this]{ rotator->SetSelectionType(ESelectionAngle::Uphill); } },
           { EKeys::NumPadThree, [this]{ rotator->SetSelectionType(ESelectionAngle::Uphill); } },
       }),
       // Shift
       new ModifiedShortcuts
       ({
          { EKeys::W, []{ UColorCast::ColorCreatorTiles(ETileColor::White);   } }, 
          { EKeys::R, []{ UColorCast::ColorCreatorTiles(ETileColor::Red);     } }, 
          { EKeys::O, []{ UColorCast::ColorCreatorTiles(ETileColor::Orange);  } }, 
          { EKeys::Y, []{ UColorCast::ColorCreatorTiles(ETileColor::Yellow);  } }, 
          { EKeys::G, []{ UColorCast::ColorCreatorTiles(ETileColor::Green);   } }, 
          { EKeys::C, []{ UColorCast::ColorCreatorTiles(ETileColor::Cyan);    } }, 
          { EKeys::B, []{ UColorCast::ColorCreatorTiles(ETileColor::Blue);    } }, 
          { EKeys::P, []{ UColorCast::ColorCreatorTiles(ETileColor::Purple);  } }, 
          // { EKeys::M, []{ UColorCast::ColorCreatorTiles(ETileColor::Maroon);  } }, 
          { EKeys::T, []{ UColorCast::ColorCreatorTiles(ETileColor::Magenta); } }, 
          { EKeys::N, []{ UColorCast::ColorCreatorTiles(ETileColor::Pink);    } }, 
          { EKeys::K, []{ UColorCast::ColorCreatorTiles(ETileColor::Black);   } }, 
       }),
       // Ctrl
       new ModifiedShortcuts
       ({
          { EKeys::W, [] { Log("Wand"); } },
          { EKeys::Z, []{ Log("Undo"); } }, 
          { EKeys::C, []{ Log("Copy"); } }, 
          { EKeys::V, []{ Log("Paste"); } }, 
          { EKeys::X, []{ Log("Cut"); } }, 
          { EKeys::A, []{ Log("All"); } }, 
          { EKeys::R, []{ Log("Refresh"); } }, 
          { EKeys::D, []{ Log("Deselect"); } }, 
          { EKeys::N, []{ Log("Size Board"); } }, 
          { EKeys::S, []{ Log("Save"); } }, 
       }),
       // Shift Ctrl
       new ModifiedShortcuts
       ({
          { EKeys::Z, []{ Log("Redo"); } }, 
          { EKeys::A, []{ Log("Select All"); } }, 
       }),
       // Alt
       new ModifiedShortcuts
       ({
          { EKeys::X, []{ Log("Gap"); } },  
          { EKeys::W, []{ Log("Wrap"); } }, 
          { EKeys::C, []{ Log("Camo"); } }, 
          { EKeys::R, []{ Log("Rotator"); } }, 
          { EKeys::I, []{ Log("Iron"); } }, 
          { EKeys::F, []{ Log("Rift"); } }, 
          { EKeys::L, []{ Log("Link"); } }, 
          { EKeys::S, []{ Log("Swap"); } }, 
       }),
       // Shift Alt
       new ModifiedShortcuts
       ({
          { EKeys::Z, []{ Log("Shift Alt Z"); } }, 
       }),
       // Ctrl Alt
       new ModifiedShortcuts
       ({
          { EKeys::Z, []{ Log("Ctrl Alt Z"); } }, 
       }),
       // Shift Ctrl Alt
       new ModifiedShortcuts
       ({
          { EKeys::Z, []{ Log("Shift Ctrl Alt Z"); } }, 
       }),
   };
}

bool UShortcutDetector::KeyDown(const FKey key) const { return controller->WasInputKeyJustPressed(key); }
bool UShortcutDetector::Key(const FKey key)     const { return controller->IsInputKeyDown(key); }
bool UShortcutDetector::Shift()                 const { return Key(EKeys::LeftShift)   || Key(EKeys::RightShift); }
bool UShortcutDetector::Ctrl()                  const { return Key(EKeys::LeftControl) || Key(EKeys::RightControl); }
bool UShortcutDetector::Alt()                   const { return Key(EKeys::LeftAlt)     || Key(EKeys::RightAlt); }
