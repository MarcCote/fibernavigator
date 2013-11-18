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

        for( int i=0; i < (*itBundle)->m_countLines; ++i )
        {
            int idx = ((*pLinePointers)[i]);

            //ss.str( "" );
            //ss << "First point of streamlines: " << (*pPointsColor)[idx*3];
            //Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );

            m_fibersLines.push_back(  &((*pPoints)[idx*3]) );
            m_fibersLength.push_back( (*pLinePointers)[i+1] - idx );
            m_fibersColors.push_back( &((*pPointsColor)[idx*3]) );
        }
    }

    // Downsample all streamlines so they have the same number of points.
    m_points = new float[m_fibersLength.size() * m_nbDownsamplingPts * 3];
    
    //ss.str("");
    //ss <<"Memory Allocated of " << m_fibersLength.size() * m_nbDownsamplingPts * 3 << " bytes";
    //Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );
    for( uint i=0; i < m_fibersLength.size(); ++i )
    {
        //Logger::getInstance()->print( wxT( "Interpolating" ), LOGLEVEL_MESSAGE );
        m_line2point.push_back(i * m_nbDownsamplingPts * 3);
        //ss.str("");
        //ss <<"Interpolated fiber # " << i << " located at " << m_line2point.back();
        //Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );

        Interpolation::linear3D(m_fibersLines[i], m_fibersLength[i], &m_points[m_line2point.back()], m_nbDownsamplingPts);

        /*Logger::getInstance()->print( wxT( "Original" ), LOGLEVEL_MESSAGE );
        for ( uint j=0; j != m_fibersLength[0]*3; j+=3)
        {
            ss.str( "" );
            ss << "( " << m_fibersLines[0][j] << ", " << m_fibersLines[0][j+1] << ", " << m_fibersLines[0][j+2] << ")";        
            Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );
        }

        Logger::getInstance()->print( wxT( "Interpolated" ), LOGLEVEL_MESSAGE );
        for ( uint j=0; j != m_nbDownsamplingPts*3; j+=3)
        {
            ss.str( "" );
            ss << "( " << m_points[j] << ", " << m_points[j+1] << ", " << m_points[j+2] << ")";        
            Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );
        }*/

    }

    /*ss.str( "" );
    ss << "Nb. streamlines: " << m_fibersLines.size();
    ss << "\nNb. lengths: " << m_fibersLength.size();
    ss << "\nNb. colors: " << m_fibersColors.size();
    Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );

    for( uint i=0; i < m_fibersLines.size()-1; ++i )
    {
        ss.str( "" );
        ss << "\tx0: " << m_fibersLines[i][0];
        ss << "\taddr: " << m_fibersLines[i];
        ss << "\tDiff: " << (m_fibersLines[i+1] - m_fibersLines[i]) / 3.0;
        ss << "\tLength: " << m_fibersLength[i];
        ss << "\tColor: (" << &m_fibersColors[i][0] << "," << &m_fibersColors[i][1] << "," << &m_fibersColors[i][2] << ")";
        Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );
    }*/

    cluster();
}

QuickBundles::~QuickBundles()
{
    delete[] m_points;
}

void QuickBundles::cluster()
{
    stringstream ss;
    Logger::getInstance()->print( wxT( "Clustering..." ), LOGLEVEL_MESSAGE );
    ss.str( "" );
    ss << "Using threshold of " << m_threshold;
    Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );

    uint i_k, s_i_length;
    float dist_min, dist_ck_si, dist_ck_si_flip;
    float* s_i;
    bool is_flip;

    // Assign each streamlines to a cluster
    for( uint idx=0; idx < m_line2point.size(); ++idx )
    {
        s_i = &m_points[m_line2point[idx]];
        //s_i_length = m_fibersLength[idx];
        
        //ss.str( "" );
        //ss << "length: " << s_i_length << " x0: " << s_i[0];
        //Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );

        // Find closest cluster
        dist_min = BIGGEST_FLOAT;
        for( uint k=0; k < m_clusters.size(); ++k )
        {
            dist_ck_si = m_pMetric->distance(&(m_clusters[k]), s_i, m_nbDownsamplingPts, false);
            dist_ck_si_flip = m_pMetric->distance(&(m_clusters[k]), s_i, m_nbDownsamplingPts, true);

            // Keep track of the closest cluster
            if( dist_ck_si < dist_min )
            {
                dist_min = dist_ck_si;
                i_k = k;
                is_flip = false;
            }

            if( dist_ck_si_flip < dist_min )
            {
                dist_min = dist_ck_si_flip;
                i_k = k;
                is_flip = true;
            }
        }

        // Check if distance with the closest cluster is below some threshold
        if( dist_min < m_threshold )
        {
            if( is_flip )
                m_pMetric->add(s_i, m_nbDownsamplingPts, idx, &(m_clusters[i_k]), true);
            else
                m_pMetric->add(s_i, m_nbDownsamplingPts, idx, &(m_clusters[i_k]), false);
        }
        else  // If not, add new cluster
        {
            m_clusters.push_back(Cluster());
            m_pMetric->add(s_i, m_nbDownsamplingPts, idx, &(m_clusters[m_clusters.size()-1]), false);
        }
    }

    ss.str( "" );
    ss << "Clustering done... " << m_clusters.size() << " clusters found";
    Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );

    /*//Freeing memory
    for k in range(clusters.size()):
        indices.push_back(clusters[k].indices)
        free(clusters[k].centroid)*/
}

void QuickBundles::colorCluster(const Cluster& cluster, const wxColor& color)
{
    stringstream ss;
    ss.str( "" );
    ss << "Coloring one cluster of " << cluster.indices.size() << " streamlines...";
    Logger::getInstance()->print( wxString( ss.str().c_str(), wxConvUTF8 ), LOGLEVEL_MESSAGE );
    vector<int>::const_iterator idx;
    for( idx = cluster.indices.begin(); idx != cluster.indices.end(); ++idx )
    {
        int N = m_fibersLength[*idx];
        float* pColorData = m_fibersColors[*idx];

        float r = color.Red() / 255.f;
        float g = color.Green() / 255.f;
        float b = color.Blue() / 255.f;
        
        for( int ptColorIdx = 0; ptColorIdx < N * 3; ptColorIdx += 3 )
        {
            pColorData[ptColorIdx] = r;
            pColorData[ptColorIdx + 1] = g;
            pColorData[ptColorIdx + 2] = b;
        }

    }

}

void QuickBundles::applyDifferentColors()
{
    Logger::getInstance()->print( wxT( "Coloring clustering..." ), LOGLEVEL_MESSAGE );
    int totalColorsNb( m_clusters.size() );
    int nbColorIntervals( totalColorsNb / 6 + 1 );
    
    int curColorInterval( 0 );
    int curColorIncrement( 255 );
    
    for( int colorIdx( 0 ); colorIdx < totalColorsNb; ++colorIdx )
    {
        int remain( colorIdx % 6 );
        int r, g, b;
        
        if( remain == 0 )     { r = curColorIncrement; g = 0; b = 0; }
        else if( remain == 1 ){ r = 0; g = curColorIncrement; b = 0; }
        else if( remain == 2 ){ r = 0; g = 0; b = curColorIncrement; }
        else if( remain == 3 ){ r = curColorIncrement; g = curColorIncrement; b = 0; }
        else if( remain == 4 ){ r = curColorIncrement; g = 0; b = curColorIncrement; }
        else                  { r = 0; g = curColorIncrement; b = curColorIncrement; }
        
        colorCluster( m_clusters[colorIdx], wxColor(r,g,b) );
        
        if( colorIdx % 6 == 5 )
        {
            ++curColorInterval;
            curColorIncrement = ( 255 / nbColorIntervals) * (nbColorIntervals - curColorInterval );
        }
    }
}

vector<float*> QuickBundles::getPoints( uint idx_cluster )
{
    vector<float*> points;
    if( idx_cluster >= m_clusters.size() )
        return points;

    for (vector<int>::iterator it = m_clusters[idx_cluster].indices.begin(); it != m_clusters[idx_cluster].indices.end(); ++it)
        points.push_back(m_fibersLines[*it]);

    return points;
}

vector<int> QuickBundles::getLengths( uint idx_cluster )
{
    vector<int> lengths;
    if( idx_cluster >= m_clusters.size() )
        return lengths;

    for (vector<int>::iterator it = m_clusters[idx_cluster].indices.begin(); it != m_clusters[idx_cluster].indices.end(); ++it)
        lengths.push_back(m_fibersLength[*it]);

    return lengths;
}

vector<float*> QuickBundles::getColors( uint idx_cluster )
{
    vector<float*> colors;
    if( idx_cluster >= m_clusters.size() )
        return colors;

    for (vector<int>::iterator it = m_clusters[idx_cluster].indices.begin(); it != m_clusters[idx_cluster].indices.end(); ++it)
        colors.push_back(m_fibersColors[*it]);

    return colors;
}
