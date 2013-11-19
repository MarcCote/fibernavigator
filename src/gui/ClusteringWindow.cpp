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

const double MAX_THRESHOLD = 100;
const double MAX_WEIGHT_MDF = 1;
const double MAX_WEIGHT_GEO = 1;
const double MAX_WEIGHT_ORIENTATION = 1;
const double MAX_WEIGHT_SPATIAL = 1;
const double MAX_WEIGHT_LENGTH = 1;


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

    // ---MDF---
    m_pTextWeightMDF = new wxStaticText( this, wxID_ANY, wxT("MDF"), wxPoint(0,30), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeightMDF = new MySlider( this, wxID_ANY, 100, 0, 100, wxPoint(60,30), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    Connect( m_pSliderWeightMDF->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeightMDFMoved) );
    m_pTxtWeightMDFBox = new wxTextCtrl( this, wxID_ANY, wxT("1.00"), wxPoint(190,30), wxSize(55, -1), wxTE_CENTRE );
    m_pTxtWeightMDFBox->Connect(wxEVT_KILL_FOCUS, (wxObjectEventFunction)&ClusteringWindow::OnTextBoxWeightMDFFocusLost, this );

    // ---Geo---
    m_pTextWeightGeo = new wxStaticText( this, wxID_ANY, wxT("Geo"), wxPoint(0,60), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeightGeo = new MySlider( this, wxID_ANY, 0, 0, 100, wxPoint(60,60), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    Connect( m_pSliderWeightGeo->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeightGeoMoved) );
    m_pTxtWeightGeoBox = new wxTextCtrl( this, wxID_ANY, wxT("0.00"), wxPoint(190,60), wxSize(55, -1), wxTE_CENTRE );
    m_pTxtWeightGeoBox->Connect(wxEVT_KILL_FOCUS, (wxObjectEventFunction)&ClusteringWindow::OnTextBoxWeightGeoFocusLost, this );

    // ---Orientation---
    m_pTextWeightOrientation = new wxStaticText( this, wxID_ANY, wxT("Orientation"), wxPoint(0,90), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeightOrientation = new MySlider( this, wxID_ANY, 0, 0, 100, wxPoint(60,90), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    Connect( m_pSliderWeightOrientation->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeightOrientationMoved) );
    m_pTxtWeightOrientationBox = new wxTextCtrl( this, wxID_ANY, wxT("0.00"), wxPoint(190,90), wxSize(55, -1), wxTE_CENTRE );
    m_pTxtWeightOrientationBox->Connect(wxEVT_KILL_FOCUS, (wxObjectEventFunction)&ClusteringWindow::OnTextBoxWeightOrientationFocusLost, this );

    // ---Spatial---
    m_pTextWeightSpatial = new wxStaticText( this, wxID_ANY, wxT("Spatial"), wxPoint(0,120), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeightSpatial = new MySlider( this, wxID_ANY, 0, 0, 100, wxPoint(60,120), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    Connect( m_pSliderWeightSpatial->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeightSpatialMoved) );
    m_pTxtWeightSpatialBox = new wxTextCtrl( this, wxID_ANY, wxT("0.00"), wxPoint(190,120), wxSize(55, -1), wxTE_CENTRE );
    m_pTxtWeightSpatialBox->Connect(wxEVT_KILL_FOCUS, (wxObjectEventFunction)&ClusteringWindow::OnTextBoxWeightSpatialFocusLost, this );

    // ---Lengthj---
    m_pTextWeightLength = new wxStaticText( this, wxID_ANY, wxT("Length"), wxPoint(0,150), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderWeightLength = new MySlider( this, wxID_ANY, 0, 0, 100, wxPoint(60,150), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    Connect( m_pSliderWeightLength->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderWeightLengthMoved) );
    m_pTxtWeightLengthBox = new wxTextCtrl( this, wxID_ANY, wxT("0.00"), wxPoint(190,150), wxSize(55, -1), wxTE_CENTRE );
    m_pTxtWeightLengthBox->Connect(wxEVT_KILL_FOCUS, (wxObjectEventFunction)&ClusteringWindow::OnTextBoxWeightLengthFocusLost, this );

    new wxStaticLine( this, wxID_ANY, wxPoint(0,180), wxSize(230, -1) );

    // ---Threshold---
    m_pTextThreshold = new wxStaticText( this, wxID_ANY, wxT("Threshold"), wxPoint(0,190), wxSize(60, -1), wxALIGN_CENTER );
    m_pSliderThreshold = new MySlider( this, wxID_ANY, 200, 0, 1000, wxPoint(60,190), wxSize(130, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS );
    Connect( m_pSliderThreshold->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEventHandler(ClusteringWindow::OnSliderThresholdMoved) );
    m_pTxtThresholdBox = new wxTextCtrl( this, wxID_ANY, wxT("20.0"), wxPoint(190,190), wxSize(55, -1), wxTE_CENTRE );
    m_pTxtThresholdBox->Connect(wxEVT_KILL_FOCUS, (wxObjectEventFunction)&ClusteringWindow::OnTextBoxThresholdFocusLost, this );

    // ---Clustering Button---
    m_pBtnStart = new wxButton( this, wxID_ANY,wxT("Do clustering"), wxPoint(10,220), wxSize(230, -1) );
    Connect( m_pBtnStart->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClusteringWindow::OnDoClustering) );
    m_pBtnStart->Enable(true);

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

void ClusteringWindow::OnTextBoxWeightMDFFocusLost(wxFocusEvent& event)
{
    ClusteringWindow *self = (ClusteringWindow*)event.m_callbackUserData;
    wxString txt = self->m_pTxtWeightMDFBox->GetValue();

    double value;

    if( !txt.ToDouble(&value) )
        self->m_pTxtWeightMDFBox->ChangeValue( wxString::Format( wxT( "%.2f"), self->m_pSliderWeightMDF->GetValue() / 100.0 ) );

    value = std::min(value, MAX_WEIGHT_MDF);
    value = std::max(value, 0.0);

    self->m_pSliderWeightMDF->SetValue(value * 100);
    self->m_pTxtWeightMDFBox->ChangeValue( wxString::Format( wxT( "%.2f"), value ) );
}

void ClusteringWindow::OnTextBoxWeightGeoFocusLost(wxFocusEvent& event)
{
    ClusteringWindow *self = (ClusteringWindow*)event.m_callbackUserData;
    wxString txt = self->m_pTxtWeightGeoBox->GetValue();

    double value;

    if( !txt.ToDouble(&value) )
        self->m_pTxtWeightGeoBox->ChangeValue( wxString::Format( wxT( "%.2f"), self->m_pSliderWeightGeo->GetValue() / 100.0 ) );

    value = std::min(value, MAX_WEIGHT_GEO);
    value = std::max(value, 0.0);

    self->m_pSliderWeightGeo->SetValue(value * 100);
    self->m_pTxtWeightGeoBox->ChangeValue( wxString::Format( wxT( "%.2f"), value ) );
}

void ClusteringWindow::OnTextBoxWeightOrientationFocusLost(wxFocusEvent& event)
{
    ClusteringWindow *self = (ClusteringWindow*)event.m_callbackUserData;
    wxString txt = self->m_pTxtWeightOrientationBox->GetValue();

    double value;

    if( !txt.ToDouble(&value) )
        self->m_pTxtWeightOrientationBox->ChangeValue( wxString::Format( wxT( "%.2f"), self->m_pSliderWeightOrientation->GetValue() / 100.0 ) );

    value = std::min(value, MAX_WEIGHT_ORIENTATION);
    value = std::max(value, 0.0);

    self->m_pSliderWeightOrientation->SetValue(value * 100);
    self->m_pTxtWeightOrientationBox->ChangeValue( wxString::Format( wxT( "%.2f"), value ) );
}

void ClusteringWindow::OnTextBoxWeightSpatialFocusLost(wxFocusEvent& event)
{
    ClusteringWindow *self = (ClusteringWindow*)event.m_callbackUserData;
    wxString txt = self->m_pTxtWeightSpatialBox->GetValue();

    double value;

    if( !txt.ToDouble(&value) )
        self->m_pTxtWeightSpatialBox->ChangeValue( wxString::Format( wxT( "%.2f"), self->m_pSliderWeightSpatial->GetValue() / 100.0 ) );

    value = std::min(value, MAX_WEIGHT_SPATIAL);
    value = std::max(value, 0.0);

    self->m_pSliderWeightSpatial->SetValue(value * 100);
    self->m_pTxtWeightSpatialBox->ChangeValue( wxString::Format( wxT( "%.2f"), value ) );
}

void ClusteringWindow::OnTextBoxWeightLengthFocusLost(wxFocusEvent& event)
{
    ClusteringWindow *self = (ClusteringWindow*)event.m_callbackUserData;
    wxString txt = self->m_pTxtWeightLengthBox->GetValue();

    double value;

    if( !txt.ToDouble(&value) )
        self->m_pTxtWeightLengthBox->ChangeValue( wxString::Format( wxT( "%.2f"), self->m_pSliderWeightLength->GetValue() / 100.0 ) );

    value = std::min(value, MAX_WEIGHT_LENGTH);
    value = std::max(value, 0.0);

    self->m_pSliderWeightLength->SetValue(value * 100);
    self->m_pTxtWeightLengthBox->ChangeValue( wxString::Format( wxT( "%.2f"), value ) );
}

void ClusteringWindow::OnTextBoxThresholdFocusLost(wxFocusEvent& event)
{
    ClusteringWindow *self = (ClusteringWindow*)event.m_callbackUserData;
    wxString txt = self->m_pTxtThresholdBox->GetValue();

    double value;

    if( !txt.ToDouble(&value) )
        self->m_pTxtThresholdBox->ChangeValue( wxString::Format( wxT( "%.1f"), self->m_pSliderThreshold->GetValue() / 10.0 ) );

    value = std::min(value, MAX_THRESHOLD);
    value = std::max(value, 0.0);

    self->m_pSliderThreshold->SetValue(value * 10);
    self->m_pTxtThresholdBox->ChangeValue( wxString::Format( wxT( "%.1f"), value ) );
}

void ClusteringWindow::OnSliderThresholdMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderThreshold->GetValue() / 10.0;
    m_pTxtThresholdBox->ChangeValue( wxString::Format( wxT( "%.1f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeightMDFMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeightMDF->GetValue() / 100.0;
    m_pTxtWeightMDFBox->ChangeValue( wxString::Format( wxT( "%.2f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeightGeoMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeightGeo->GetValue() / 100.0;
    m_pTxtWeightGeoBox->ChangeValue( wxString::Format( wxT( "%.2f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeightOrientationMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeightOrientation->GetValue() / 100.0;
    m_pTxtWeightOrientationBox->ChangeValue( wxString::Format( wxT( "%.2f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeightSpatialMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeightSpatial->GetValue() / 100.0;
    m_pTxtWeightSpatialBox->ChangeValue( wxString::Format( wxT( "%.2f"), sliderValue ) );
}

void ClusteringWindow::OnSliderWeightLengthMoved(wxCommandEvent& WXUNUSED(event))
{
    float sliderValue = m_pSliderWeightLength->GetValue() / 100.0;
    m_pTxtWeightLengthBox->ChangeValue( wxString::Format( wxT( "%.2f"), sliderValue ) );
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