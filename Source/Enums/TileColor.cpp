#include "TileColor.h"

FLinearColor UColorCast::TileColorToLinearColor(const ETileColor color)
{
    switch(color)
    {
    case ETileColor::None:    return FLinearColor(0, 0, 0, 0);
    case ETileColor::White:   return FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
    case ETileColor::Red:     return FLinearColor(0.8, 0, 0, 1);
    case ETileColor::Orange:  return FLinearColor(1, 0.15f, 0, 1);
    case ETileColor::Yellow:  return FLinearColor(0.6f, 0.6f, 0, 1);
    case ETileColor::Green:   return FLinearColor(0, 0.4f, 0, 1);
    case ETileColor::Cyan:    return FLinearColor(0.02f, 0.35f, 0.6f, 1);
    case ETileColor::Blue:    return FLinearColor(0, 0.07f, 1.0f, 1);
    case ETileColor::Purple:  return FLinearColor(0.2f, 0, 0.3f, 1);
    case ETileColor::Pink:    return FLinearColor(0.65f, 0, 0.1f, 1);
    case ETileColor::Magenta: return FLinearColor(0.6f, 0, 0.6f, 1);
    case ETileColor::Black:   return FLinearColor(0.025f, 0.025f, 0.025f, 1);
    default: throw std::invalid_argument("Invalid tile color enum.");
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
