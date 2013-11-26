#ifndef DISTANCES_H_
#define DISTANCES_H_

#include "../../dataset/Fibers.h"

#include <fstream>
#include <string>
#include <vector>

struct ClusterStruct
{
    std::vector< int > indices;
    std::vector< float > centroid;
};

class Metric;

class Cluster
{
public:
    Cluster(Metric *pMetric);

    float distance( const float *pStreamline, uint N, uint D=3 );
    void add( uint idx, const float *pStreamline, uint N, uint D=3 );

    std::vector<uint> m_indices;
    std::vector<float> m_centroid;

private:
    Metric *m_pMetric;
    friend class Metric;

};


/**
 * This class represents an abstract distance measure.
 */
class Metric
{
public:
    Metric() {}
    virtual ~Metric() {}

    virtual vector<float> newCentroid( uint N, uint D ) = 0;
    virtual float distance( const float *pStreamline1, uint N1, const float *pStreamline2, uint N2, uint D ) = 0;
    virtual float distance( float *pCentroid, const float *pStreamline, uint N, uint D ) = 0;
    virtual void add( float *pCentroid, uint clusterSize, const float *pStreamline, uint N, uint D ) = 0;

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

    void addMetric( Metric *pMetric, float weight=1.0 );
    void removeMetric( uint idxMetric );
    void updateMetric( uint idxMetric, float weight );

    vector<float> newCentroid( uint N, uint D );
    float distance( const float *pStreamline1, uint N1, const float *pStreamline2, uint N2, uint D );
    float distance( float *pCentroid, const float *pStreamline, uint N, uint D );
    void add( float *pCentroid, uint clusterSize, const float *pStreamline, uint N, uint D );

private:
    vector<float>       m_weights;
    vector<Metric*>     m_metrics;
    vector<uint>        m_centroidsStart;
};


/**
 * This class computes the average pointwise distance (aka. MDF) between two streamlines.
 */
class MDF : public Metric
{
public:
    MDF() {}
    virtual ~MDF() {}

    vector<float> newCentroid( uint N, uint D );
    float distance( const float *pStreamline1, uint N1, const float *pStreamline2, uint N2, uint D );
    float distance( float *pCentroid, const float *pStreamline, uint N, uint D );
    void add( float *pCentroid, uint clusterSize, const float *pStreamline, uint N, uint D );
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

    vector<float> newCentroid( uint N, uint D );
    float distance( const float *pStreamline1, uint N1, const float *pStreamline2, uint N2, uint D );
    float distance( float *pCentroid, const float *pStreamline, uint N, uint D );
    void add( float *pCentroid, uint clusterSize, const float *pStreamline, uint N, uint D );
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

    vector<float> newCentroid( uint N, uint D );
    float distance( const float *pStreamline1, uint N1, const float *pStreamline2, uint N2, uint D );
    float distance( float *pCentroid, const float *pStreamline, uint N, uint D );
    void add( float *pCentroid, uint clusterSize, const float *pStreamline, uint N, uint D );
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

    vector<float> newCentroid( uint N, uint D );
    float distance( const float *pStreamline1, uint N1, const float *pStreamline2, uint N2, uint D );
    float distance( float *pCentroid, const float *pStreamline, uint N, uint D );
    void add( float *pCentroid, uint clusterSize, const float *pStreamline, uint N, uint D );
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

    vector<float> newCentroid( uint N, uint D );
    float distance( const float *pStreamline1, uint N1, const float *pStreamline2, uint N2, uint D );
    float distance( float *pCentroid, const float *pStreamline, uint N, uint D );
    void add( float *pCentroid, uint clusterSize, const float *pStreamline, uint N, uint D );
};

#endif /* DISTANCES_H_ */
