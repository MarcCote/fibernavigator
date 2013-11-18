#ifndef DISTANCES_H_
#define DISTANCES_H_

#include "../../dataset/Fibers.h"

//#include "DatasetInfo.h"
//#include "Octree.h"
//#include "../gui/SelectionObject.h"
//#include "../misc/Fantom/FVector.h"

//#include <GL/glew.h>
//#include <wx/wxprec.h>

//#ifndef WX_PRECOMP
//#include <wx/wx.h>
//#endif

#include <fstream>
#include <string>
#include <vector>

struct Cluster
{
    std::vector< int > indices;
    std::vector< float > centroid;
};

/**
 * This class represents an abstract distance measure.
 */
class Metric
{
public:
    Metric() {}
    virtual ~Metric() {}

    virtual float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false) = 0;
    virtual void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false) = 0;

private:
    Metric( const Metric & );
    Metric &operator=( const Metric & );

private:
    

private:
    // Variables
    
};


/**
 * This class computes the average pointwise distance (aka. MDF) between two streamlines.
 */
class AveragePointwiseMetric : public Metric
{
public:
    AveragePointwiseMetric() {}
    virtual ~AveragePointwiseMetric() {}

    float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false);
    void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false);

private:

};

#endif /* DISTANCES_H_ */
