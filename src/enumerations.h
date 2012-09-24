#ifndef ENUM_H_
#define ENUM_H_

enum ColourVariation
  {
    AsPalette,
    LittleVariation,
    SomeVariation,
    LotsOfVariation,
    WildVariation
   };

enum LayoutBound
  {
    SceneOnly,
    ImageOnly,
    Combined
  };

enum LayoutPath
  {
    CircularPath,
    RectangularPath
  };

enum WordOrientation
  {
    AnyWordOrientation,
    HorizontalWordOrientation,
    MostlyHorizontalWordOrientation,
    HalfAndHalfWordOrientation,
    MostlyVerticalWordOrientation,
    VerticalWordOrientation
  };

#endif
