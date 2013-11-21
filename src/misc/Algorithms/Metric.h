#ifndef DISTANCES_H_
#define DISTANCES_H_

#include "../../dataset/Fibers.h"

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

    virtual float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false ) = 0;
    virtual void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false ) = 0;

private:
    Metric( const Metric & );
    Metric &operator=( const Metric & );    
};

/**
 * This class combines several metrics using a weigthted sum.
 */
class WeightedMetric : public Metric
{
public:
    WeightedMetric() {}
    virtual ~WeightedMetric() {}

    void insertMetric( Metric *metric, float weight=1.0 );
    void deleteMetric( int noMetric );
    void updateMetric( int noMetric, float weight );

    float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false );
    void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false );

private:
    vector<float>   m_weights;
    vector<Metric>  m_metrics;
};


/**
 * This class computes the average pointwise distance (aka. MDF) between two streamlines.
 */
class AveragePointwiseMetric : public Metric
{
public:
    AveragePointwiseMetric() {}
    virtual ~AveragePointwiseMetric() {}

    float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false );
    void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false );
};

/**
 * This class computes the geometrical shape distance between two streamlines using
 *  the pointwise angular difference.
 */
class ShapeMetric : public Metric
{
public:
    ShapeMetric() {}
    virtual ~ShapeMetric() {}

    float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false );
    void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false );
};

/**
 * This class computes the orientation distance between two streamlines using
 *  the cosine distance between the two vectors linking streamlines' endpoints.
 */
class OrientationMetric : public Metric
{
public:
    OrientationMetric() {}
    virtual ~OrientationMetric() {}

    float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false );
    void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false );
};

/**
 * This class computes the spatial distance between two streamlines using
 *  the Euclidean distance between streamlines' midpoint.
 */
class SpatialMetric : public Metric
{
public:
    SpatialMetric() {}
    virtual ~SpatialMetric() {}

    float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false );
    void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false );
};

/**
 * This class computes the length distance between two streamlines using
 *  the streamlines' arclength.
 */
class LengthMetric : public Metric
{
public:
    LengthMetric() {}
    virtual ~LengthMetric() {}

    float distance( Cluster *cluster, float *fiber, int N, bool isFlip=false );
    void add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip=false );
};

#endif /* DISTANCES_H_ */
