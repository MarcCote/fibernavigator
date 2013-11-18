#ifndef INTERPOLATION_H_
#define INTERPOLATION_H_

typedef unsigned int uint;

namespace Interpolation
{
    bool linear3D(float *curve, uint N, float *newCurve, uint newN);
}

#endif /* INTERPOLATION_H_ */
