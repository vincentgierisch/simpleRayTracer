#ifndef BRDFTYPE_HPP
#define BRDFTYPE_HPP

typedef enum BrdfType {
    Lambertian = 0,
    Phong = 1,
    Layered = 2,
    CookTorrance = 3,
    LayeredCT = 4

} BrdfType;

#endif