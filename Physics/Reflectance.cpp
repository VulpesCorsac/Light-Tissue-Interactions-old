#include "Reflectance.h"

template float  Physics_NS::FresnelReflectance<float> (float , float , float );
template double Physics_NS::FresnelReflectance<double>(double, double, double);

template float  Physics_NS::BorderReflectance<float> (float , float );
template double Physics_NS::BorderReflectance<double>(double, double);
