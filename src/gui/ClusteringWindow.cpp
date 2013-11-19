#include "ClusteringWindow.h"

#include "../Logger.h"

#include "../dataset/DatasetManager.h"
#include "../dataset/Fibers.h"

#include "../misc/Algorithms/QuickBundles.h"
#include "../misc/Algorithms/Metric.h"

#include <sstream>
using std::stringstream;

//#include "MainFrame.h"
//#include "SceneManager.h"
//#include "SelectionBox.h"
//#include "SelectionEllipsoid.h"
//#include "../main.h"
//#include "../dataset/Fibers.h"

/*#include "../dataset/Anatomy.h"
#include "../dataset/ODFs.h"
#include "../dataset/Tensors.h"
#include "../dataset/Maximas.h"
#include "../misc/IsoSurface/CIsoSurface.h"
#include "../misc/IsoSurface/TriangleMesh.h"*/

/*#include <wx/checkbox.h>
#include <wx/grid.h>
#include <wx/tglbtn.h>
#include <wx/treectrl.h>*/

#include <wx/statline.h>


IMPLEMENT_DYNAMIC_CLASS( ClusteringWindow, wxScrolledWindow )

BEGIN_EVENT_TABLE( ClusteringWindow, wxScrolledWindow )
END_EVENT_TABLE()


ClusteringWindow::ClusteringWindow( wxWindow *pParent, MainFrame *pMf, wxWindowID id, const wxPoint &pos, const wxSize &size )
:   wxScrolledWindow( pParent, id, pos, size, wxBORDER_NONE, _T("Clustering Canvas") ),
    m_pMainFrame( pMf )
{
    SetBackgroundColour( *wxLIGHT_GREY );
    m_pClusteringSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( m_pClusteringSizer );
    SetAutoLayout( true );

    //Content of the clustering panel
    /********************************/

    //m_pBtnSelectFile = new wxButton( this, wxID_ANY,wxT("DTI not selected"), wxPoint(30,0), wxSize(100, -1) );
    //Connect( m_pBtnSelectFile->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClusteringWindow::OnSelectFileDTI) );
    //m_pBtnSelectFile->SetBackgroundColour(wxColour( 255, 147, 147 ));

	//m_pBtnSelectShell = new wxButton( this, wxID_ANY,wxT("Shell not selected"), wxPoint(30,30), wxSize(100, -1) );
    //Connect( m_pBtnSelectShell->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClusteringWindow::OnSelectShell) );

    //m_pToggleShell = new wxToggleButton( this, wxID_ANY,wxT("Shell seed OFF"), wxPoint(130,30), wxSize(100, -1) );
    //Connect( m_pToggleShell->GetId(), wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(ClusteringWindow::OnShellSeeding) );
	//m_pToggleShell->Enable(false);

    m_pTextWeigthMDF = new wxStaticText( this, wxID_ANY, wxT("MDF"), wxPoint(0,30), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeigthMDF = new MySlider( this, wxID_ANY, 0, 0.1, 100, wxPoint(60,30), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    m_pSliderWeigthMDF->SetValue( 1 );
    Connect( m_pSliderWeigthMDF->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeigthMDFMoved) );
    m_pTxtWeigthMDFBox = new wxTextCtrl( this, wxID_ANY, wxT("20"), wxPoint(190,30), wxSize(55, -1), wxTE_CENTRE | wxTE_READONLY );

    m_pTextWeigthGeo = new wxStaticText( this, wxID_ANY, wxT("Geo"), wxPoint(0,60), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeigthGeo = new MySlider( this, wxID_ANY, 0, 0.1, 100, wxPoint(60,60), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    m_pSliderWeigthGeo->SetValue( 0 );
    Connect( m_pSliderWeigthGeo->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeigthGeoMoved) );
    m_pTxtWeigthGeoBox = new wxTextCtrl( this, wxID_ANY, wxT("20"), wxPoint(190,60), wxSize(55, -1), wxTE_CENTRE | wxTE_READONLY );

    m_pTextWeigthOrientation = new wxStaticText( this, wxID_ANY, wxT("Orientation"), wxPoint(0,90), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeigthOrientation = new MySlider( this, wxID_ANY, 0, 0.1, 100, wxPoint(60,90), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    m_pSliderWeigthOrientation->SetValue( 0 );
    Connect( m_pSliderWeigthOrientation->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeigthOrientationMoved) );
    m_pTxtWeigthOrientationBox = new wxTextCtrl( this, wxID_ANY, wxT("20"), wxPoint(190,90), wxSize(55, -1), wxTE_CENTRE | wxTE_READONLY );

    m_pTextWeigthSpatial = new wxStaticText( this, wxID_ANY, wxT("Spatial"), wxPoint(0,120), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeigthSpatial = new MySlider( this, wxID_ANY, 0, 0.1, 100, wxPoint(60,120), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    m_pSliderWeigthSpatial->SetValue( 0 );
    Connect( m_pSliderWeigthSpatial->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeigthSpatialMoved) );
    m_pTxtWeigthSpatialBox = new wxTextCtrl( this, wxID_ANY, wxT("20"), wxPoint(190,120), wxSize(55, -1), wxTE_CENTRE | wxTE_READONLY );

    m_pTextWeigthLength = new wxStaticText( this, wxID_ANY, wxT("Length"), wxPoint(0,150), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeigthLength = new MySlider( this, wxID_ANY, 0, 0.1, 100, wxPoint(60,150), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    m_pSliderWeigthLength->SetValue( 0 );
    Connect( m_pSliderWeigthLength->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeigthLengthMoved) );
    m_pTxtWeigthLengthBox = new wxTextCtrl( this, wxID_ANY, wxT("20"), wxPoint(190,150), wxSize(55, -1), wxTE_CENTRE | wxTE_READONLY );

    new wxStaticLine( this, wxID_ANY, wxPoint(0,180), wxSize(230, -1) );

    m_pTextThreshold = new wxStaticText( this, wxID_ANY, wxT("Threshold"), wxPoint(0,190), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderThreshold = new MySlider( this, wxID_ANY, 0, 0.1, 100, wxPoint(60,190), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    m_pSliderThreshold->SetValue( 20 );
    Connect( m_pSliderThreshold->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderThresholdMoved) );
    m_pTxtThresholdBox = new wxTextCtrl( this, wxID_ANY, wxT("20"), wxPoint(190,190), wxSize(55, -1), wxTE_CENTRE | wxTE_READONLY );

    m_pBtnStart = new wxButton( this, wxID_ANY,wxT("Do clustering"), wxPoint(10,220), wxSize(230, -1) );
    Connect( m_pBtnStart->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClusteringWindow::OnDoClustering) );
    m_pBtnStart->Enable(true);
    
    //m_pTextTotalSeedNb = new wxStaticText( this, wxID_ANY, wxT("Number of current seeds"), wxPoint(7,270), wxSize(150, -1), wxALIGN_LEFT );
    //m_pTxtTotalSeedNbBox = new wxTextCtrl( this, wxID_ANY, wxT("1000"), wxPoint(190,270), wxSize(55, -1), wxTE_CENTRE | wxTE_READONLY );

	//m_pBtnConvert = new wxButton( this, wxID_ANY,wxT("Convert Fibers"), wxPoint(50,300), wxSize(140, 30) );
	//Connect( m_pBtnConvert->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClusteringWindow::OnConvertToFibers) );

}

void ClusteringWindow::OnSize( wxSizeEvent &WXUNUSED(event) )
{}

void ClusteringWindow::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );
}

wxSizer* ClusteringWindow::getWindowSizer()
{
    return m_pClusteringSizer;
}

void ClusteringWindow::OnDoClustering( wxCommandEvent& WXUNUSED(event) )
{
    wxString msg;
    Logger::getInstance()->print( wxT( "Clustering..." ), LOGLEVEL_MESSAGE );
    // TODO: Deactive clustering button instead
    long index = m_pMainFrame->getCurrentListIndex();
    DatasetInfo *pDatasetInfo = DatasetManager::getInstance()->getDataset(m_pMainFrame->m_pListCtrl->GetItem( index ));

    if( pDatasetInfo->getType() != FIBERS && pDatasetInfo->getType() != FIBERSGROUP )
    //if( pDatasetInfo->getType() != FIBERS && pDatasetInfo->getType() != FIBERSGROUP )
        return;
    
    wxString basename;
    vector<Fibers*> selectedFibers;
    if( pDatasetInfo->getType() == FIBERSGROUP )
    {
        selectedFibers = DatasetManager::getInstance()->getFibers();
        basename = wxString( "Bundle", wxConvUTF8 );
    }
    else
    {
        selectedFibers.push_back(DatasetManager::getInstance()->getSelectedFibers( m_pMainFrame->m_pListCtrl->GetItem( index ) ));
        basename = selectedFibers.back()->getName();
        basename = basename(0, basename.length()-4);
    }

    AveragePointwiseMetric mdf;
    float threshold = m_pSliderThreshold->GetValue();

    Logger::getInstance()->print( wxT( "Button Clicked..." ), LOGLEVEL_MESSAGE );
    QuickBundles clustering(selectedFibers, &mdf, threshold, 18);

    // Create new Fibers in the Fibergroup
    vector<Fibers*> bundles;
    for (int i = 0; i < clustering.getNbClusters(); ++i)
    {
        vector<float*> points = clustering.getPoints(i);
        vector<int> lengths = clustering.getLengths(i);
        vector<float*> colors = clustering.getColors(i);

        wxString name = wxString(basename);
        name << wxT(" #") << i+1;

        Fibers* bundle = new Fibers();
        bundle->createFrom(points, lengths, colors, name);
        bundles.push_back(bundle);
    }

    // Remove old Fibers objects
    vector<DatasetIndex> indices;
    for( uint index= m_pMainFrame->m_pListCtrl->GetItemCount()-1; index > 0 ; --index)
    {
        DatasetInfo* info = DatasetManager::getInstance()->getDataset( m_pMainFrame->m_pListCtrl->GetItem( index ) );

        if ( info->getType() != FIBERS )
            continue;

        for (uint i = 0; i < selectedFibers.size(); ++i)
        {
            if ( selectedFibers[i] != NULL && ((Fibers*)info) == selectedFibers[i] )
            {
                m_pMainFrame->m_pListCtrl->SelectItem( index );
                m_pMainFrame->deleteListItem();
                break;
            }
        }
    }

    // Insert new bundles (Fibers object)
    for (int i = 0; i < clustering.getNbClusters(); ++i)
    {
        DatasetIndex index = DatasetManager::getInstance()->addFibers( bundles[i] );
        m_pMainFrame->m_pListCtrl->InsertItem( index );
    }

    // Apply different colors
    DatasetManager::getInstance()->getFibersGroup()->OnApplyDifferentColors();
    for( vector<Fibers *>::const_iterator it = bundles.begin(); it != bundles.end(); ++it )
    {
        (*it)->setColorationMode( CONSTANT_COLOR );
        (*it)->updateColorationMode();
        (*it)->updatePropertiesSizer();
        (*it)->setThreshold( 1 );  // Ugly patch in order to force an updateFibersColors by the Fibers::draw function.
        //(*it)->updateFibersColors();
    }

    msg.clear();
    msg << wxT( "Found " ) << bundles.size() <<  wxT( " bundles." );
    Logger::getInstance()->print(msg, LOGLEVEL_MESSAGE );
}

void ClusteringWindow::OnSliderThresholdMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderThreshold->GetValue();
    m_pTxtThresholdBox->SetValue( wxString::Format( wxT( "%.1f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeigthMDFMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeigthMDF->GetValue();
    m_pTxtWeigthMDFBox->SetValue( wxString::Format( wxT( "%.1f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeigthGeoMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeigthGeo->GetValue();
    m_pTxtWeigthGeoBox->SetValue( wxString::Format( wxT( "%.1f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeigthOrientationMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeigthOrientation->GetValue();
    m_pTxtWeigthOrientationBox->SetValue( wxString::Format( wxT( "%.1f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeigthSpatialMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeigthSpatial->GetValue();
    m_pTxtWeigthSpatialBox->SetValue( wxString::Format( wxT( "%.1f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeigthLengthMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeigthLength->GetValue();
    m_pTxtWeigthLengthBox->SetValue( wxString::Format( wxT( "%.1f"), sliderValue ) );
}



/*void ClusteringWindow::OnPlay( wxCommandEvent& WXUNUSED(event) )
{
    ClusteringHelper::getInstance()->setTrackAction(true);
    ClusteringHelper::getInstance()->togglePlayStop();
    if(!ClusteringHelper::getInstance()->isTrackActionPaused())
    {
        m_pPlayPause->SetBitmapLabel(m_bmpPause);
        m_pMainFrame->setTimerSpeed();
    }
    else
    {
        m_pPlayPause->SetBitmapLabel(m_bmpPlay);
        m_pMainFrame->setTimerSpeed();
    }
}

void ClusteringWindow::OnStop( wxCommandEvent& WXUNUSED(event) )
{
    m_pMainFrame->m_pMainGL->m_pClusteredFibers->m_trackActionStep = 0;
    ClusteringHelper::getInstance()->setTrackAction(false);
    ClusteringHelper::getInstance()->setTrackActionPause(true);
    m_pPlayPause->SetBitmapLabel(m_bmpPlay);
    m_pMainFrame->setTimerSpeed();
}*/