#include "QuickBundles.h"

#include "../../Logger.h"
#include "Interpolation.h"
#include <sstream>
using std::stringstream;

QuickBundles::QuickBundles(const vector<Fibers*>& bundles, Metric *pMetric, float threshold, uint nbDownsamplingPts)
:m_pMetric(pMetric), m_threshold(threshold), m_nbDownsamplingPts(nbDownsamplingPts)
{
    stringstream ss;
    for( vector<Fibers *>::const_iterator itBundle = bundles.begin(); itBundle != bundles.end(); ++itBundle )
    {
        vector<float>* pPoints       = &((*itBundle)->m_pointArray);
        vector<int>*   pLinePointers = &((*itBundle)->m_linePointers);
        vector<float>* pPointsColor  = &((*itBundle)->m_colorArray);

        for( int i=0; i < (*itBundle)->getFibersCount(); ++i )
        {
            int idx = ((*pLinePointers)[i]);
            m_fibersLines.push_back(  &((*pPoints)[idx*3]) );
            m_fibersLength.push_back( (*pLinePointers)[i+1] - idx );
            m_fibersColors.push_back( &((*pPointsColor)[idx*3]) );
        }
    }

    // Downsample all streamlines so they have the same number of points.
    m_pPoints = new float[m_fibersLength.size() * m_nbDownsamplingPts * 3];

    for( uint i=0; i < m_fibersLength.size(); ++i )
    {
        m_line2point.push_back(i * m_nbDownsamplingPts * 3);
        Interpolation::linear3D(m_fibersLines[i], m_fibersLength[i], &m_pPoints[m_line2point.back()], m_nbDownsamplingPts);
    }

    cluster();
}

QuickBundles::~QuickBundles()
{
    delete[] m_pPoints;
}

void QuickBundles::cluster()
{
    stringstream ss;
    ss << "Computing QuickBundles using a threshold of " << m_threshold;
    Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_DEBUG );

    int i_k = -1;
    //float dist_min, dist_ck_si, dist_ck_si_flip;
    float dist_min, dist_ck_si;
    float* pStreamline;
    //bool is_flip = false;

    // Assign each streamlines to a cluster
    for( uint idx=0; idx < m_line2point.size(); ++idx )
    {
        // ss.str( "" );
        // ss << "Processing streamline #" << idx;
        // Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_DEBUG );

        pStreamline = &m_pPoints[m_line2point[idx]];

        // Find closest cluster
        dist_min = BIGGEST_FLOAT;
        for( uint k=0; k < m_clusters.size(); ++k )
        {
            dist_ck_si = m_clusters[k].distance( pStreamline, m_nbDownsamplingPts );

            // Keep track of the closest cluster
            if( dist_ck_si < dist_min )
            {
                dist_min = dist_ck_si;
                i_k = k;
            }

            //dist_ck_si = m_pMetric->distance(&(m_clusters[k]), pStreamline, m_nbDownsamplingPts, false);
            //dist_ck_si_flip = m_pMetric->distance(&(m_clusters[k]), pStreamline, m_nbDownsamplingPts, true);

            // Keep track of the closest cluster
            // if( dist_ck_si < dist_min )
            // {
            //     dist_min = dist_ck_si;
            //     i_k = k;
            //     is_flip = false;
            // }

            // if( dist_ck_si_flip < dist_min )
            // {
            //     dist_min = dist_ck_si_flip;
            //     i_k = k;
            //     is_flip = true;
            // }
        }

        // Check if distance with the closest cluster is below some threshold
        if( dist_min < m_threshold )
        {
            // ss.str( "" );
            // ss << "QuickBundles merge " << idx << " in cluster # " << i_k;
            // Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_DEBUG );
            m_clusters[i_k].add( idx, pStreamline, m_nbDownsamplingPts );

            // if( is_flip )
            // {
            //     m_pMetric->add(pStreamline, m_nbDownsamplingPts, idx, &(m_clusters[i_k]), true);
            // }
            // else
            // {
            //     m_pMetric->add(pStreamline, m_nbDownsamplingPts, idx, &(m_clusters[i_k]), false);
            // }
        }
        else  // If not, add new cluster
        {
            // ss.str( "" );
            // ss << "QuickBundles add new cluster for #" << idx;
            // Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_DEBUG );
            m_clusters.push_back(Cluster( m_pMetric ));
            m_clusters.back().add( idx, pStreamline, m_nbDownsamplingPts );

            // ss.str( "" );
            // ss << "Size of centroid " << m_clusters.back().m_centroid.size();
            // Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_DEBUG );

            //m_clusters.push_back(Cluster());
            //m_pMetric->add(pStreamline, m_nbDownsamplingPts, idx, &(m_clusters[m_clusters.size()-1]), false);
        }
    }

    ss.str( "" );
    ss << "QuickBundles done... " << m_clusters.size() << " clusters found";
    Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_DEBUG );
}

vector<float*> QuickBundles::getPoints( uint idx_cluster )
{
    vector<float*> points;
    if( idx_cluster >= m_clusters.size() )
    {
        return points;
    }

    for (vector<uint>::iterator it = m_clusters[idx_cluster].m_indices.begin(); it != m_clusters[idx_cluster].m_indices.end(); ++it)
    {
        points.push_back(m_fibersLines[*it]);
    }

    return points;
}

vector<int> QuickBundles::getLengths( uint idx_cluster )
{
    vector<int> lengths;
    if( idx_cluster >= m_clusters.size() )
    {
        return lengths;
    }

    for (vector<uint>::iterator it = m_clusters[idx_cluster].m_indices.begin(); it != m_clusters[idx_cluster].m_indices.end(); ++it)
    {
        lengths.push_back(m_fibersLength[*it]);
    }

    return lengths;
}

vector<float*> QuickBundles::getColors( uint idx_cluster )
{
    vector<float*> colors;
    if( idx_cluster >= m_clusters.size() )
    {
        return colors;
    }


    for (vector<uint>::iterator it = m_clusters[idx_cluster].m_indices.begin(); it != m_clusters[idx_cluster].m_indices.end(); ++it)
    {
        colors.push_back(m_fibersColors[*it]);
    }

    return colors;
}
