/////////////////////////////////////////////////////////////////////////////
// Name:            ClusteringWindow.h
// Author:          ---
// Creation Date:   ---
//
// Description: This is the implementation file for the Clustering window.
/////////////////////////////////////////////////////////////////////////////


#ifndef CLUSTERINGWINDOW_H_
#define CLUSTERINGWINDOW_H_

#include "MainFrame.h"
#include "MainCanvas.h"
#include "MyListCtrl.h"

//#include "../mis/Algorithms/Helper.h"

#include <wx/scrolwin.h>

//class MainFrame;
class wxToggleButton;

class ClusteringWindow: public wxScrolledWindow
{
public:
    ClusteringWindow(){};
    ClusteringWindow( wxWindow *pParent, MainFrame *pMf, wxWindowID id, const wxPoint &pos, const wxSize &size );

    ~ClusteringWindow(){};
    void OnPaint( wxPaintEvent &event );
    void OnSize( wxSizeEvent &event );
    wxSizer* getWindowSizer();


    void OnDoClustering                        ( wxCommandEvent& event );
    //void OnSelectShell                         ( wxCommandEvent& event );
    //void OnClearBox                            ( wxTreeEvent&    event );
    void OnSliderThresholdMoved                ( wxCommandEvent& event );
    void OnSliderWeigthMDFMoved                ( wxCommandEvent& event );
    void OnSliderWeigthGeoMoved                ( wxCommandEvent& event );
    void OnSliderWeigthOrientationMoved        ( wxCommandEvent& event );
    void OnSliderWeigthSpatialMoved            ( wxCommandEvent& event );
    void OnSliderWeigthLengthMoved             ( wxCommandEvent& event );
    //void OnSliderWgMoved                       ( wxCommandEvent& event );
    //void OnSliderWoMoved                       ( wxCommandEvent& event );
    //void OnSliderWsMoved                       ( wxCommandEvent& event );
    //void OnSliderWlMoved                       ( wxCommandEvent& event );
    //void OnSelectFileDTI                       ( wxCommandEvent& event );
    //void OnSelectMask                          ( wxCommandEvent& event );
    
    //void OnPlay                                ( wxCommandEvent& event );
    //void OnStop                                ( wxCommandEvent& event );

public:
    wxButton      *m_pBtnStart;
    //wxTextCtrl          *m_pTxtTotalSeedNbBox;


private:
    MainFrame           *m_pMainFrame;
    wxSlider            *m_pSliderThreshold;
    wxTextCtrl          *m_pTxtThresholdBox;
    wxStaticText        *m_pTextThreshold;

    wxSlider            *m_pSliderWeigthMDF;
    wxTextCtrl          *m_pTxtWeigthMDFBox;
    wxStaticText        *m_pTextWeigthMDF;

    wxSlider            *m_pSliderWeigthGeo;
    wxTextCtrl          *m_pTxtWeigthGeoBox;
    wxStaticText        *m_pTextWeigthGeo;

    wxSlider            *m_pSliderWeigthOrientation;
    wxTextCtrl          *m_pTxtWeigthOrientationBox;
    wxStaticText        *m_pTextWeigthOrientation;

    wxSlider            *m_pSliderWeigthSpatial;
    wxTextCtrl          *m_pTxtWeigthSpatialBox;
    wxStaticText        *m_pTextWeigthSpatial;

    wxSlider            *m_pSliderWeigthLength;
    wxTextCtrl          *m_pTxtWeigthLengthBox;
    wxStaticText        *m_pTextWeigthLength;
    //wxButton            *m_pBtnSelectFile;
    //wxButton            *m_pBtnSelectShell;
    //wxToggleButton      *m_pToggleShell;
    //wxButton            *m_pBtnSelectMap;
    //wxButton            *m_pBtnConvert;
    //wxBitmapButton      *m_pPlayPause;
    //wxBitmapButton      *m_pBtnStop;
    //wxImage             m_bmpPause;
    //wxImage             m_bmpPlay;
    


private:
    wxSizer *m_pClusteringSizer;
    ClusteringWindow( wxWindow *pParent, wxWindowID id, const wxPoint &pos, const wxSize &size );
    DECLARE_DYNAMIC_CLASS( ClusteringWindow )
    DECLARE_EVENT_TABLE()
};

#endif /*CLUSTERINGWINDOW_H_*/
