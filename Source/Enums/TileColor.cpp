#include "TileColor.h"
#include "CreatorTile.h"

FLinearColor UColorCast::TileColorToLinearColor(const ETileColor color)
{
    const float whiteShade = 1, blackShade = 0.0025f;
    switch(color)
    {
    case ETileColor::None:    return FLinearColor(0, 0, 0, 0);
    case ETileColor::White:   return FLinearColor(whiteShade, whiteShade, whiteShade, 1);
    case ETileColor::Red:     return FLinearColor(0.8, 0.03f, 0.01f, 1);
    case ETileColor::Orange:  return FLinearColor(0.8, 0.15f, 0, 1);
    case ETileColor::Yellow:  return FLinearColor(0.6f, 0.6f, 0, 1);
    case ETileColor::Green:   return FLinearColor(0, 0.4f, 0, 1);
    case ETileColor::Cyan:    return FLinearColor(0.02f, 0.35f, 0.6f, 1);
    case ETileColor::Blue:    return FLinearColor(0, 0.07f, 1.0f, 1);
    case ETileColor::Purple:  return FLinearColor(0.2f, 0, 0.3f, 1);
    case ETileColor::Pink:    return FLinearColor(0.65f, 0, 0.1f, 1);
    case ETileColor::Magenta: return FLinearColor(0.6f, 0, 0.6f, 1);
    case ETileColor::Maroon:  return FLinearColor(0.2f, 0.0f, 0.05f, 1);
    case ETileColor::Black:   return FLinearColor(blackShade, blackShade, blackShade, 1);
    default: throw std::invalid_argument("Invalid tile color enum.");
    }
}
void UColorCast::ColorCreatorTiles(const ETileColor color)
{
    for (const auto& selectedTile : ACreatorTile::SelectedTiles)
        selectedTile->SetColor(color);
}
FKey UColorCast::GetKey(const ETileColor color)
{
    switch(color) 
    {
    case ETileColor::None:    return EKeys::Invalid;
    case ETileColor::White:   return EKeys::W;
    case ETileColor::Red:     return EKeys::R;
    case ETileColor::Orange:  return EKeys::O;
    case ETileColor::Yellow:  return EKeys::Y;
    case ETileColor::Green:   return EKeys::G;
    case ETileColor::Cyan:    return EKeys::C;
    case ETileColor::Blue:    return EKeys::B;
    case ETileColor::Purple:  return EKeys::P;
    case ETileColor::Pink:    return EKeys::N;
    case ETileColor::Magenta: return EKeys::T;
    case ETileColor::Maroon:   return EKeys::M;
    case ETileColor::Black:   return EKeys::K;
    default: throw std::invalid_argument("Invalid color.");
    }
}

bool operator==(ETileColor lhs, ETileColor rhs)
{
    return static_cast<int>(lhs) == static_cast<int>(rhs);
}
bool operator!=(const ETileColor lhs, const ETileColor rhs)
{
    return !(lhs == rhs);
}
