#include "wx/wx.h"
#include "wx/laywin.h"
#include "wx/filedlg.h"
#include "wx/statbmp.h"

#include "mainFrame.h"

int winNumber = 1;

DECLARE_EVENT_TYPE(wxEVT_MY_EVENT, -1)
   
BEGIN_EVENT_TABLE(MainFrame, wxMDIParentFrame)
    EVT_MENU(VIEWER_ABOUT, MainFrame::OnAbout)
    EVT_SIZE(MainFrame::OnSize)
    EVT_MENU(VIEWER_QUIT, MainFrame::OnQuit)
    EVT_MENU(VIEWER_LOAD, MainFrame::OnLoad)
    EVT_MOUSE_EVENTS(MainFrame::OnMouseEvent)
    EVT_COMMAND(ID_GL_NAV_X, wxEVT_MY_EVENT, MainFrame::OnGLEvent)
    EVT_COMMAND(ID_GL_NAV_Y, wxEVT_MY_EVENT, MainFrame::OnGLEvent)
	EVT_COMMAND(ID_GL_NAV_Z, wxEVT_MY_EVENT, MainFrame::OnGLEvent)
	EVT_SLIDER(ID_X_SLIDER, MainFrame::OnXSliderMoved)
	EVT_SLIDER(ID_Y_SLIDER, MainFrame::OnYSliderMoved)
	EVT_SLIDER(ID_Z_SLIDER, MainFrame::OnZSliderMoved)
END_EVENT_TABLE()


// Define my frame constructor
MainFrame::MainFrame(wxWindow *parent, const wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
    const long style):
  wxMDIParentFrame(parent, id, title, pos, size, style)
{
	NAV_SIZE = wxMin(255,size.y/4);
	NAV_GL_SIZE = NAV_SIZE - 4;
	
	// A window to the left of the client window
	wxSashLayoutWindow* win = new wxSashLayoutWindow(this, ID_WINDOW_LEFT,
                               wxDefaultPosition, wxSize(200, 30),
                               wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
	win->SetDefaultSize(wxSize(200, 1020));
	win->SetOrientation(wxLAYOUT_VERTICAL);
	win->SetAlignment(wxLAYOUT_LEFT);
	win->SetBackgroundColour(wxColour(0, 255, 0));
	win->SetSashVisible(wxSASH_RIGHT, false);
	
	win->SetExtraBorderSize(10);
	
	m_textWindow = new wxTextCtrl(win, wxID_ANY, wxEmptyString, 
			  wxDefaultPosition, wxDefaultSize,
			  wxTE_MULTILINE|wxSUNKEN_BORDER);
	m_textWindow->SetValue(_T("Info"));
	m_leftWindow = win;
	
	// navigation window with three sub windows for gl widgets 
    win = new wxSashLayoutWindow(this, ID_WINDOW_NAV_MAIN, 
  		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE*4),
  		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE*4));
    win->SetOrientation(wxLAYOUT_VERTICAL);
    win->SetAlignment(wxLAYOUT_LEFT);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_navWindow = win;

    // main window right side, holds the big gl widget
    win = new wxSashLayoutWindow(this, ID_WINDOW_RIGHT_HOLDER, 
  		  wxDefaultPosition, wxSize(200, 30),
  		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(765, 765));
    win->SetMinSize(wxSize(100,100));
    win->SetMaxSize(wxSize(2000,2000));
    win->SetOrientation(wxLAYOUT_VERTICAL);
    win->SetAlignment(wxLAYOUT_LEFT);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_rightWindowHolder = win;
    
    // main window right side, holds the big gl widget
    win = new wxSashLayoutWindow(m_rightWindowHolder, ID_WINDOW_RIGHT, 
  		  wxDefaultPosition, wxSize(200, 30),
  		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(700, 700));
    win->SetMinSize(wxSize(100,100));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(180, 180, 180));
    m_rightWindow = win;
    
    // extra window right side, prevent scaling of the main gl widget
    win = new wxSashLayoutWindow(m_rightWindowHolder, ID_WINDOW_EXTRA_RIGHT, 
  		  wxDefaultPosition, wxSize(765, 765),
  		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(765, 1));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_BOTTOM);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_extraRightWindow = win;

    win = new wxSashLayoutWindow(m_navWindow, ID_WINDOW_NAV_X, 
  		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
  		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_topNavWindow = win;
    
    win = new wxSashLayoutWindow(m_navWindow, ID_WINDOW_NAV_Y, 
  		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
  		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
    win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
    win->SetOrientation(wxLAYOUT_HORIZONTAL);
    win->SetAlignment(wxLAYOUT_TOP);
    win->SetBackgroundColour(wxColour(0, 0, 0));
    m_middleNavWindow = win;

    win = new wxSashLayoutWindow(m_navWindow, ID_WINDOW_NAV_Z, 
     		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
     		  wxRAISED_BORDER | wxSW_3D | wxCLIP_CHILDREN);
       win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
       win->SetOrientation(wxLAYOUT_HORIZONTAL);
       win->SetAlignment(wxLAYOUT_TOP);
       win->SetBackgroundColour(wxColour(0, 0, 0));
    m_bottomNavWindow = win;
      
    // extra window to avoid scaling of the bottom gl widget when resizing
    win = new wxSashLayoutWindow(m_navWindow, ID_WINDOW_NAV3, 
      		  wxDefaultPosition, wxSize(NAV_SIZE, NAV_SIZE),
       		  wxNO_BORDER | wxSW_3D | wxCLIP_CHILDREN);
          win->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
          win->SetOrientation(wxLAYOUT_HORIZONTAL);
          win->SetAlignment(wxLAYOUT_TOP);
          win->SetBackgroundColour(wxColour(255, 255, 255));
    m_extraNavWindow = win;
   

    m_xSlider = new wxSlider(m_extraNavWindow, ID_X_SLIDER, 50, 0, 100, wxPoint(0,0), wxSize(NAV_SIZE, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
    
    m_ySlider = new wxSlider(m_extraNavWindow, ID_Y_SLIDER, 50, 0, 100, wxPoint(0,m_xSlider->GetSize().y), wxSize(NAV_SIZE, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
    m_zSlider = new wxSlider(m_extraNavWindow, ID_Z_SLIDER, 50, 0, 100, wxPoint(0,m_xSlider->GetSize().y + m_ySlider->GetSize().y), wxSize(NAV_SIZE, -1), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
   
    m_xSlider->SetMinSize(wxSize(1, -1));
    m_ySlider->SetMinSize(wxSize(1, -1));
    m_zSlider->SetMinSize(wxSize(1, -1));
    
    GLboolean doubleBuffer = GL_TRUE;
    
	#ifdef __WXMSW__
    	int *gl_attrib = NULL;
	#else
    int gl_attrib[20] = { WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
        WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
        WX_GL_DOUBLEBUFFER,
	#if defined(__WXMAC__) || defined(__WXCOCOA__)
        GL_NONE };
	#else
        None };
	#endif
	#endif

    if(!doubleBuffer)
    {
        printf("don't have double buffer, disabling\n");
	#ifdef __WXGTK__
        gl_attrib[9] = None;
	#endif
        doubleBuffer = GL_FALSE;
    }
    
    m_scene = new TheScene();

    m_mainGL = new MainCanvas(m_scene, m_rightWindow, ID_GL_MAIN, wxDefaultPosition,
        			wxDefaultSize, 0, _T("MainGLCanvas"), gl_attrib);
    m_gl0 = new NavigationCanvas(m_scene, 0, m_topNavWindow, ID_GL_NAV_X, wxDefaultPosition,
    	        wxDefaultSize, 0, _T("NavGLCanvasX"), gl_attrib);
    m_gl1 = new NavigationCanvas(m_scene, 1, m_middleNavWindow, ID_GL_NAV_Y, wxDefaultPosition,
        	        wxDefaultSize, 0, _T("NavGLCanvasY"), gl_attrib);
    m_gl2 = new NavigationCanvas(m_scene, 2, m_bottomNavWindow, ID_GL_NAV_Z, wxDefaultPosition,
       	        wxDefaultSize, 0, _T("NavGLCanvasZ"), gl_attrib);
    
    
    m_xclick = 0;
    m_yclick = 0;
    m_zclick = 0;
    
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainFrame::OnLoad(wxCommandEvent& WXUNUSED(event))
{
	wxString caption = wxT("Choose a file");
	wxString wildcard = wxT("Header files (*.hea)|*.hea|*.*|*.*");
	wxString defaultDir = wxEmptyString;
	wxString defaultFilename = wxEmptyString;
	wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard, wxOPEN);
	if (dialog.ShowModal() == wxID_OK)
	{
		wxString path = dialog.GetPath();
		m_dataset = new TheDataset();
		
		
		if (!m_dataset->load(path)) 
		{
			wxMessageBox(wxT("Fehler"),  wxT(""), wxOK|wxICON_INFORMATION, NULL);
		}
		else 
		{ 
			m_scene->setDataset(m_dataset);
		}
		m_textWindow->SetValue(m_dataset->getInfoString());
		m_xclick = m_dataset->getColumns()/2;
		m_yclick = m_dataset->getRows()/2;
		m_zclick = m_dataset->getFrames()/2;
		m_xSlider->SetMax(m_dataset->getColumns());
		m_xSlider->SetValue(m_xclick);
		m_ySlider->SetMax(m_dataset->getRows());
		m_ySlider->SetValue(m_yclick);
		m_zSlider->SetMax(m_dataset->getFrames());
		m_zSlider->SetValue(m_zclick);
		m_scene->updateView(m_xclick, m_yclick, m_zclick);
		refreshAllGLWidgets();
	}
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
      (void)wxMessageBox(_T("wxWidgets 2.0 Sash Demo\nAuthor: Julian Smart (c) 1998"), _T("About Sash Demo"));
}

void MainFrame::OnSashDrag(wxSashEvent& event)
{
#if wxUSE_MDI_ARCHITECTURE
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
#endif // wxUSE_MDI_ARCHITECTURE

    // Leaves bits of itself behind sometimes
    GetClientWindow()->Refresh();
}

void MainFrame::OnGLEvent( wxCommandEvent &event )
{
	wxPoint pos, newpos;
	
	switch (event.GetInt())
	{
	case 0:
		pos = m_gl0->getMousePos();
		m_xclick = (int)(((float)pos.x/NAV_GL_SIZE)*m_dataset->getColumns());
		m_yclick = (int)(((float)pos.y/NAV_GL_SIZE)*m_dataset->getRows());
		m_xSlider->SetValue(m_xclick);
		m_ySlider->SetValue(m_yclick);
		m_scene->updateView(m_xclick, m_yclick, m_zclick);
		break;
	case 1:
		pos = m_gl1->getMousePos();
		m_xclick = (int)(((float)pos.x/NAV_GL_SIZE)*m_dataset->getColumns());
		m_zclick = (int)(((float)pos.y/NAV_GL_SIZE)*m_dataset->getFrames());
		m_xSlider->SetValue(m_xclick);
		m_zSlider->SetValue(m_zclick);
		m_scene->updateView(m_xclick, m_yclick, m_zclick);
		break;
	case 2:
		pos = m_gl2->getMousePos();
		m_yclick = (int)(((float)pos.x/NAV_GL_SIZE)*m_dataset->getRows());
		m_zclick = (int)(((float)pos.y/NAV_GL_SIZE)*m_dataset->getFrames());
		m_ySlider->SetValue(m_yclick);
		m_zSlider->SetValue(m_zclick);
		m_scene->updateView(m_xclick, m_yclick, m_zclick);
		break;
	}
	refreshAllGLWidgets();
}

void MainFrame::OnMouseEvent(wxMouseEvent& event)
{
		
	this->Refresh();
}

void MainFrame::OnSize(wxSizeEvent& WXUNUSED(event))
{
	/* resize the navigation widgets */
	int height = this->GetClientSize().y;
	NAV_SIZE = wxMin(255, height/4);
	NAV_GL_SIZE = NAV_SIZE-4;
	m_navWindow->SetDefaultSize(wxSize(NAV_SIZE, height));
	m_topNavWindow->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
	m_middleNavWindow->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
	m_bottomNavWindow->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
	m_extraNavWindow->SetDefaultSize(wxSize(NAV_SIZE, NAV_SIZE));
	
	/* resize sliders */
	m_xSlider->SetSize(wxSize(NAV_GL_SIZE, -1));
	m_ySlider->SetSize(wxSize(NAV_GL_SIZE, -1));
	m_zSlider->SetSize(wxSize(NAV_GL_SIZE, -1));
	
	/* resize main gl window */
	int mainSize = wxMin((this->GetClientSize().x - m_leftWindow->GetSize().x - m_navWindow->GetSize().x), 
			this->GetClientSize().y);
	m_rightWindow->SetSize(wxSize(mainSize, mainSize));
	
	m_scene->updateView(m_xclick, m_yclick, m_zclick);
	 
	
#if wxUSE_MDI_ARCHITECTURE
    wxLayoutAlgorithm layout;
    layout.LayoutMDIFrame(this);
#endif // wxUSE_MDI_ARCHITECTURE

    GetClientWindow()->Refresh();
    this->Refresh();
}

void MainFrame::OnXSliderMoved(wxCommandEvent& event)
{
	if (!m_dataset) return;
	 m_xclick = m_xSlider->GetValue();
	 m_scene->updateView(m_xclick, m_yclick, m_zclick);
	 refreshAllGLWidgets();
}

void MainFrame::OnYSliderMoved(wxCommandEvent& event)
{
	if (!m_dataset) return;
	m_yclick = m_ySlider->GetValue();
	m_scene->updateView(m_xclick, m_yclick, m_zclick);
	refreshAllGLWidgets();
}

void MainFrame::OnZSliderMoved(wxCommandEvent& event)
{
	if (!m_dataset) return;
	m_zclick = m_zSlider->GetValue();
	m_scene->updateView(m_xclick, m_yclick, m_zclick);
	refreshAllGLWidgets();
}

void MainFrame::refreshAllGLWidgets()
{
	m_gl0->render();
	m_gl1->render();
	m_gl2->render();
	m_mainGL->render();
}