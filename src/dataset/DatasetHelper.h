/*
 * DatasetHelper.h
 *
 *  Created on: 27.07.2008
 *      Author: ralph
 */
#ifndef DATASETHELPER_H_
#define DATASETHELPER_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/textfile.h"
#include "wx/file.h"
#include "wx/image.h"
#include "wx/wfstream.h"
#include "wx/datstrm.h"
#include "wx/txtstrm.h"
#include "wx/xml/xml.h"

#include <vector>

#include "DatasetInfo.h"

#include "../gui/SelectionObject.h"

#include "SplinePoint.h"

#include "AnatomyHelper.h"
#include "../gfx/ShaderHelper.h"

#include "../misc/lic/TensorField.h"
#include "../misc/Fantom/FMatrix.h"

class DatasetInfo;
class AnatomyHelper;
class ShaderHelper;
class SplinePoint;
class SelectionObject;
class Fibers;
class FibersGroup;
class TensorField;
class Surface;

class DatasetHelper 
{
public:
    // Constructor/destructor
    DatasetHelper();
    virtual ~DatasetHelper();

    // Functions
    void   deleteAllPoints();
    Vector mapMouse2World( const int i_x, const int i_y,GLdouble i_projection[16], GLint i_viewport[4], GLdouble i_modelview[16]);
    Vector mapMouse2WorldBack( const int i_x, const int i_y,GLdouble i_projection[16], GLint i_viewport[4], GLdouble i_modelview[16]);   

    /*
     * Called from MainFrame when a kdTree thread signals it's finished
     */
    void treeFinished();

	bool getFibersGroupDataset( FibersGroup* &i_fiberGroup );
	bool getSelectedFiberDataset ( Fibers* &i_fiber );
    bool getSurfaceDataset( Surface* &i_surface );
    bool getTextureDataset( std::vector< DatasetInfo* > &o_types ); 
    std::vector< float >* getVectorDataset();
    TensorField* getTensorField();

    void toggleShowAllSelectionObjects() { m_showObjects = ! m_showObjects; };
    void toggleActivateAllSelectionObjects() { m_activateObjects = ! m_activateObjects; };

    void doLicMovie       ( int i_mode );
    void createLicSliceSag( int i_slize );
    void createLicSliceCor( int i_slize );
    void createLicSliceAxi( int i_slize );
    void licMovieHelper();

    /////////////////////////////////////////////////////////////////////////////////
    // general info about the datasets
    /////////////////////////////////////////////////////////////////////////////////
    std::vector<Vector>   m_rulerPts;
    bool                  m_isRulerToolActive;
    double                m_rulerFullLength;
    double                m_rulerPartialLength;

    FMatrix m_niftiTransform;

    unsigned int m_countFibers;

    bool m_scnFileLoaded;
    bool m_surfaceIsDirty;

    /////////////////////////////////////////////////////////////////////////////////
    // state variables for rendering
    /////////////////////////////////////////////////////////////////////////////////
    //! if set the shaders will be reloaded during next render() call
    bool      m_scheduledReloadShaders;
    // the screenshot button has been pressed, next render pass it will be executed
    bool      m_scheduledScreenshot;

    bool      m_showObjects;
    bool      m_activateObjects;

    float     m_frustum[6][4]; // Contains the information of the planes forming the frustum.
    /////////////////////////////////////////////////////////////////////////////////
    // state variables for menu entries
    /////////////////////////////////////////////////////////////////////////////////
    bool  m_clearToBlack;
    bool  m_filterIsoSurf;
    int   m_colorMap;
    bool  m_showColorMapLegend;

    //////////////////////////////////////////////////////////////////////////
    // MOVE TO MAINFRAME
    //////////////////////////////////////////////////////////////////////////
	bool  m_isDrawerToolActive;
	enum  DrawMode
	{
		DRAWMODE_PEN = 0,
		DRAWMODE_ERASER = 1,
        DRAWMODE_INVALID
	};
	DrawMode m_drawMode;
	int     m_drawSize;
	bool    m_drawRound;
	bool    m_draw3d;
    bool    m_canUseColorPicker;
	wxColor m_drawColor;
	wxImage m_drawColorIcon;
    //////////////////////////////////////////////////////////////////////////

    bool  m_boxLockIsOn;
    int   m_threadsActive;

    /////////////////////////////////////////////////////////////////////////////////
    // MOVE TO MAINCANVAS
    /////////////////////////////////////////////////////////////////////////////////
    bool  m_isDragging;
    bool  m_isrDragging;
    bool  m_ismDragging;
    //////////////////////////////////////////////////////////////////////////

    bool  m_texAssigned;
    bool  m_selBoxChanged;

    int   m_geforceLevel;

    wxString m_scenePath;
    wxString m_scnFileName;
    wxString m_screenshotPath;
    wxString m_screenshotName;

    /////////////////////////////////////////////////////////////////////////////////
    // pointers to often used objects
    /////////////////////////////////////////////////////////////////////////////////
    //AnatomyHelper*   m_anatomyHelper;
    SelectionObject* m_boxAtCrosshair;
    SplinePoint*     m_lastSelectedPoint;
    SelectionObject* m_lastSelectedObject;
};

#define ID_KDTREE_FINISHED    50

#endif /* DATASETHELPER_H_ */
