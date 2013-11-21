/*
 *  The Metric class implementation.
 *
 */

#include "Metric.h"


/***
 * Utils
 */
float arcLength( float *fiber, int N, int D=3 )
{
    float arcLength = 0.0;
    float dx, dy, dz;

    for( int i=1; i < N*D; i+=D )
    {
        dx = fiber[i  ] - fiber[i  -D];
        dy = fiber[i+1] - fiber[i+1-D];
        dz = fiber[i+2] - fiber[i+2-D];
        arcLength += sqrt(dx * dx + dy * dy + dz * dz);
    }

    return arcLength;
}


/***
 * WeightedMetric
 */
void WeightedMetric::insertMetric( Metric *metric, float weight )
{
    
}

void WeightedMetric::deleteMetric( int noMetric )
{

}

void WeightedMetric::updateMetric( int noMetric, float weight )
{

}

/***
 * AveragePointwiseMetric
 */
float AveragePointwiseMetric::distance( Cluster *cluster, float *fiber, int N, bool isFlip )
{
    int D = 3;
    float d = 0.0;
    float dx, dy, dz;
    int i_flip;

    for( int i=0; i < N*D; i+=D )
    {
        i_flip = i;
        if (isFlip )
            i_flip = (N-1)*D - i;

        dx = fiber[i_flip + 0] - cluster->centroid[i + 0];
        dy = fiber[i_flip + 1] - cluster->centroid[i + 1];
        dz = fiber[i_flip + 2] - cluster->centroid[i + 2];
        d += sqrt(dx * dx + dy * dy + dz * dz);
    }

    return d / N;
}

void AveragePointwiseMetric::add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip )
{
    int D = 3;
    float C = cluster->indices.size();
    int i_flip;

    if( C == 0 )
    {
        //cluster->centroid = <float*>calloc(N * D, sizeof(float))
        //cluster->centroid = vector<float>(N * D, 0.0);
        for( int i=0; i < N*D; ++i )
            cluster->centroid.push_back(0.0);
    }

    for( int i=0; i < N*D; ++i )
    {
        i_flip = i;
        if (isFlip )
            i_flip = (N-1)*D - i;

        cluster->centroid[i] = ((cluster->centroid[i] * C) + fiber[i_flip]) / (C+1);
    }

    cluster->indices.push_back(idx);
}

/***
 * ShapeMetric
 */
float ShapeMetric::distance( Cluster *cluster, float *fiber, int N, bool isFlip )
{
    int D = 3;
    float d = 0.0;
    float dx, dy, dz;
    int i_flip;

    for( int i=0; i < N*D; i+=D )
    {
        i_flip = i;
        if (isFlip )
            i_flip = (N-1)*D - i;

        dx = fiber[i_flip + 0] - cluster->centroid[i + 0];
        dy = fiber[i_flip + 1] - cluster->centroid[i + 1];
        dz = fiber[i_flip + 2] - cluster->centroid[i + 2];
        d += sqrt(dx * dx + dy * dy + dz * dz);
    }

    return d / N;
}

void ShapeMetric::add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip )
{
    int D = 3;
    float C = cluster->indices.size();
    int i_flip;

    if( C == 0 )
    {
        //cluster->centroid = <float*>calloc(N * D, sizeof(float))
        //cluster->centroid = vector<float>(N * D, 0.0);
        for( int i=0; i < N*D; ++i )
            cluster->centroid.push_back(0.0);
    }

    for( int i=0; i < N*D; ++i )
    {
        i_flip = i;
        if (isFlip )
            i_flip = (N-1)*D - i;

        cluster->centroid[i] = ((cluster->centroid[i] * C) + fiber[i_flip]) / (C+1);
    }

    cluster->indices.push_back(idx);
}


/***
 * OrientationMetric
 */
float OrientationMetric::distance( Cluster *cluster, float *fiber, int N, bool isFlip )
{
    int D = 3;
    float d = 0.0;
    float dx, dy, dz;
    int i_flip;

    for( int i=0; i < N*D; i+=D )
    {
        i_flip = i;
        if (isFlip )
            i_flip = (N-1)*D - i;

        dx = fiber[i_flip + 0] - cluster->centroid[i + 0];
        dy = fiber[i_flip + 1] - cluster->centroid[i + 1];
        dz = fiber[i_flip + 2] - cluster->centroid[i + 2];
        d += sqrt(dx * dx + dy * dy + dz * dz);
    }

    return d / N;
}

void OrientationMetric::add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip )
{
    int D = 3;
    float C = cluster->indices.size();
    int i_flip;

    if( C == 0 )
    {
        //cluster->centroid = <float*>calloc(N * D, sizeof(float))
        //cluster->centroid = vector<float>(N * D, 0.0);
        for( int i=0; i < N*D; ++i )
            cluster->centroid.push_back(0.0);
    }

    for( int i=0; i < N*D; ++i )
    {
        i_flip = i;
        if (isFlip )
            i_flip = (N-1)*D - i;

        cluster->centroid[i] = ((cluster->centroid[i] * C) + fiber[i_flip]) / (C+1);
    }

    cluster->indices.push_back(idx);
}


/***
 * SpatialMetric
 */
float SpatialMetric::distance( Cluster *cluster, float *fiber, int N, bool isFlip )
{
    int D = 3;
    float d = 0.0;
    float dx, dy, dz;
    int i_flip;

    for( int i=0; i < N*D; i+=D )
    {
        i_flip = i;
        if (isFlip )
            i_flip = (N-1)*D - i;

        dx = fiber[i_flip + 0] - cluster->centroid[i + 0];
        dy = fiber[i_flip + 1] - cluster->centroid[i + 1];
        dz = fiber[i_flip + 2] - cluster->centroid[i + 2];
        d += sqrt(dx * dx + dy * dy + dz * dz);
    }

    return d / N;
}

void SpatialMetric::add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip )
{
    int D = 3;
    float C = cluster->indices.size();
    int i_flip;

    if( C == 0 )
    {
        //cluster->centroid = <float*>calloc(N * D, sizeof(float))
        //cluster->centroid = vector<float>(N * D, 0.0);
        for( int i=0; i < N*D; ++i )
            cluster->centroid.push_back(0.0);
    }

    for( int i=0; i < N*D; ++i )
    {
        i_flip = i;
        if (isFlip )
            i_flip = (N-1)*D - i;

        cluster->centroid[i] = ((cluster->centroid[i] * C) + fiber[i_flip]) / (C+1);
    }

    cluster->indices.push_back(idx);
}

/***
 * LengthMetric
 */
float LengthMetric::distance( Cluster *cluster, float *fiber, int N, bool isFlip )
{
    return fabs(arcLength(fiber, N) - cluster->centroid[0]);
}

void LengthMetric::add( float *fiber, int N, int idx, Cluster *cluster, bool isFlip )
{
    float C = cluster->indices.size();

    if( C == 0 )
        cluster->centroid.push_back(0.0);

    cluster->centroid[0] = ((cluster->centroid[0] * C) + arcLength(fiber, N)) / (C+1);

    cluster->indices.push_back(idx);
}