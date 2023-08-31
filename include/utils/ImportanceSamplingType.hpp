#ifndef IMPORTANCE_SAMPLING_TYPE_HPP
#define IMPORTANCE_SAMPLING_TYPE_HPP

typedef enum ImportanceSamplingType {
    Uniform = 0,
    Cosine = 1,
    Brdf = 2,
    Light = 3

} ImportanceSamplingType;

#endif