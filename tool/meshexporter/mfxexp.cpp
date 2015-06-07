/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#pragma warning ( disable : 4530 )
#pragma warning ( disable : 4786 )

#include "pch.hpp"
#include "mfxexp.hpp"

#include "aboutbox.hpp"
#include "utility.hpp"

#undef IDS_LIBDESCRIPTION
#include <wm3.h>
#include <ieditnormals.h>
#include <iparamm2.h>
#include <iskin.h>

#ifndef CODE_INLINE
#include "mfxexp.ipp"
#endif

#include "string_tool.h"

// Defines
#define DEBUG_PRIMITIVE_FILE 0


/**
*	Constructor
*/
MFXExport::MFXExport()
    : nodeCount_ ( 0 ),
    animNodeCount_( 0 ),
    settings_( ExportSettings::instance() ),
    mfxRoot_(NULL)
{
}

/**
*	Destructor
*/
MFXExport::~MFXExport()
{
}


/**
* Number of extensions supported, one (mfx)
*/
int MFXExport::ExtCount()
{
    return 1;
}

/**
* Extension #n (only supports one, visual )
*/
const TCHAR* MFXExport::Ext(int n)
{
    static std::string desc;

    switch( n )
    {
    case 0:
        desc = GetString(IDS_EXTENSION1);
        break;

    default:
        desc.clear();
        break;
    }

    return desc.c_str();
}

/**
* Long ASCII description
*/
const TCHAR* MFXExport::LongDesc()
{
    static std::string desc = GetString(IDS_LONGDESC);
    return desc.c_str();
}

/**
* Short ASCII description (i.e. "3D Studio")
*/
const TCHAR* MFXExport::ShortDesc()
{
    static std::string desc;
    desc =  GetString(IDS_SHORTDESC);

#if defined BW_EXPORTER_DEBUG
    desc += _T(" Debug");
#endif

    return desc.c_str();
}

/**
* ASCII Author name
*/
const TCHAR* MFXExport::AuthorName()
{
    static std::string desc = GetString( IDS_AUTHORNAME );
    return desc.c_str();
}

/**
* ASCII Copyright message
*/
const TCHAR* MFXExport::CopyrightMessage()
{
    static std::string desc = GetString(IDS_COPYRIGHT);
    return desc.c_str();
}

/**
* Other message #1
*/
const TCHAR* MFXExport::OtherMessage1()
{
    return _T("");
}

/**
* Other message #2
*/
const TCHAR* MFXExport::OtherMessage2()
{
    return _T("");
}

/**
* Version number * 100 (i.e. v3.01 = 301)
*/
unsigned int MFXExport::Version()
{
    return MFX_EXPORT_VERSION;
}

/**
* Show DLL's "About..." box
*/
void MFXExport::ShowAbout(HWND hWnd)
{
    AboutBox ab;
    ab.display( hWnd );
}


/**
*	This method converts the filename into a resource name.  If the
*	resource is not in the BW_RESOURCE_TREE returns false.
*	Otherwise the method returns true and the proper resource name is
*	returned in the resName.
*/
bool validResource( const std::string& fileName, std::string& retResourceName )
{
    retResourceName = fileName;
    return true;
}


/**
*	Display a dialog explaining the resource is invalid.
*/
void invalidExportDlg( const std::string& fileName, Interface *i )
{
    std::string errorString;
    errorString = std::string( "The exported file \"" ) + fileName + std::string( "\" is not in a valid game path.\n" );
    errorString += "Need to export to a *subfolder* of:";

    MessageBox( i->GetMAXHWnd(), errorString.c_str(), "Visual Exporter Error", MB_OK | MB_ICONWARNING );
}


/**
*	Display a dialog explaining the resources is invalid.
*/
void invalidResourceDlg( const std::vector<std::string>& fileNames, Interface *i )
{
    std::string errorString( "The following resources are not in a valid game path:   \n\n" );

    for (uint32 i=0; i<fileNames.size(); i++)
    {
        errorString += fileNames[i];
        errorString += "\n";
    }		

    errorString += "\nAll resources must be from a *subfolder* of:    \n";
    uint32 pathIndex = 0;

    MessageBox( i->GetMAXHWnd(), errorString.c_str(), "Visual Exporter Error", MB_OK | MB_ICONWARNING );
}





/**
* Main entrypoint to exporter.
* Do export.
*/
int MFXExport::DoExport(const TCHAR *nameFromMax,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
    ip_ = i;

    if (mfxRoot_)
    {
        delete mfxRoot_;
        mfxRoot_ = nullptr;
    }

    meshNodes_.clear();
    visualMeshes_.clear();


    // This has to be done for edit normals modifier to work properly, don't know quite why ;)
    i->SetCommandPanelTaskMode( TASK_MODE_MODIFY );

    errors_ = false;


    // Force the extension to lowercase
    std::string fileName = nameFromMax;
    removeFileExtension(fileName);
    fileName += ".mesh";

    std::string exportPath = getFilePath( fileName );
    settings_.filePath(exportPath);

    Interval inter = ip_->GetAnimRange();
    settings_.setStaticFrame( ip_->GetTime() / GetTicksPerFrame() );


    // Check if the filename is in the right res path.
    std::string resName;
    if (!validResource( fileName, resName ))
    {
        invalidExportDlg( fileName, ip_ );
        errors_ = true;
        return 1;
    }

    std::string resNameCamelCase;
    if (!validResource( fileName, resNameCamelCase ))
    {
        invalidExportDlg( fileName, ip_ );
        errors_ = true;
        return 1;
    }


    if( !settings_.prepareExport( ip_->GetMAXHWnd() ) )	
    {
        DEBUG_MSG("Canceled\n");
        return 1;
    }

    preProcess( ip_->GetRootNode() );

    // Snapping is now a flagable option for static objects
    this->exportMeshes(settings_.snapVertices() );



    //Check all resources used by all VisualMeshes.
    std::vector<std::string> allResources;
    for (uint32 i = 0; i < visualMeshes_.size(); i++)
    {
        visualMeshes_[i]->resources(allResources);		
    }


    MeshExporter::CopyList copyList;
    for (uint32 i = 0; i < visualMeshes_.size(); i++)
    {
        visualMeshes_[i]->resourcesToCopy(copyList);		
    }

    std::vector<std::string> invalidResources;
    for (uint32 i = 0; i < allResources.size(); i++)
    {
        std::string temp;
        if (!validResource( allResources[i], temp ))
        {
            invalidResources.push_back(allResources[i]);
        }
    }

    if (!invalidResources.empty())
    {
        invalidResourceDlg( invalidResources, ip_ );
        errors_ = true;
        return 1;
    }

    //Ö´ÐÐµ¼³ö
    BoundingBox bb;
    MeshExporter::VisualMeshPtr accMesh = new MeshExporter::VisualMesh();
    std::vector<MeshExporter::VisualMeshPtr> origVisualMeshes = visualMeshes_;

    bool bLargeIndices = false;
    uint32 meshCount = visualMeshes_.size();
    int count = 0;
    while (visualMeshes_.size())
    {
        accMesh->dualUV( accMesh->dualUV() || visualMeshes_.back()->dualUV() );
        accMesh->vertexColours( accMesh->vertexColours() || visualMeshes_.back()->vertexColours() );

        bool worldSpaceAcc = settings_.exportMode() == ExportSettings::MESH_PARTICLES ?  false : true;
        visualMeshes_.back()->add( *accMesh, count++, worldSpaceAcc );

        bb.addBounds( visualMeshes_.back()->bb() );

        visualMeshes_.pop_back();
    }

    float unitScale = ExportSettings::instance().unitScale();
    if (ExportSettings::instance().useLegacyOrientation())
    {
        bb.setBounds(Vector3(-bb.max_.x, bb.min_.z, -bb.max_.y ) * unitScale,
            Vector3(-bb.min_.x, bb.max_.z, -bb.min_.y ) * unitScale);
    }
    else
    {
        bb.setBounds(Vector3(bb.min_.x, bb.min_.z, bb.min_.y ) * unitScale,
            Vector3(bb.max_.x, bb.max_.z, bb.max_.y ) * unitScale);
    }

    if (count)
    {
        if(!accMesh->save(fileName, bb))
            return IMPEXP_FAIL;

        bLargeIndices = (bLargeIndices || accMesh->largeIndices());
    }
    accMesh = NULL;

    if (settings_.copyTextures())
    {
        DEBUG_MSG("Num textures %d", copyList.size());

        for (MeshExporter::CopyList::iterator iter = copyList.begin(); iter != copyList.end(); iter++)
        {
            std::string from = iter->first;
            std::string to = exportPath + getFileName(iter->second);

            DWORD error = 0;
            if (CopyFile( from.c_str(), to.c_str(), FALSE ) == 0)
            {
                error = GetLastError();
                if (error == ERROR_ACCESS_DENIED)
                {
                    SetFileAttributes( to.c_str(), FILE_ATTRIBUTE_NORMAL );
                    if (CopyFile( from.c_str(), to.c_str(), FALSE ) == 0)
                    {
                        error = GetLastError();
                    }
                    else
                    {
                        error = 0;
                    }
                }

            }

            if (error)
            {
                const size_t bufferLen = 1024;
                char buffer[ bufferLen ];
                FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, buffer, bufferLen, NULL);
                MessageBox( ip_->GetMAXHWnd(), buffer, "MFExporter Copy Texture Error", MB_OK | MB_ICONERROR );
            }
            else
            {
                DEBUG_MSG("Copy texture from '%s' to '%s'", from.c_str(), to.c_str());
            }
        }
    }

    return IMPEXP_SUCCESS;
}


/*
*	List of superclass id's that are ignored when exporting
*/
static uint32 s_nonExportableSuperClass[] =
{
    CAMERA_CLASS_ID,
    LIGHT_CLASS_ID,
    SHAPE_CLASS_ID
};
const uint32 NUM_NONEXPORTABLE_SUPER = sizeof( s_nonExportableSuperClass ) / sizeof( uint32 );

/*
*	List of class id's that are ignored when exporting
*/
static Class_ID s_nonExportableClass[] =
{
    Class_ID(TARGET_CLASS_ID, 0),
    Class_ID(0x74f93b07, 0x1eb34300) // Particle view class id
};
const uint32 NUM_NONEXPORTABLE_CLASS = sizeof( s_nonExportableClass ) / sizeof( Class_ID );


/**
* Preprocess all the nodes, counts the total number of nodes.
* Adds all used materials to the material list etc.
*/
void MFXExport::preProcess( INode * node, MFXNode *mfxParent )
{
    nodeCount_++;

    bool includeNode = false;

    if (settings_.nodeFilter() == ExportSettings::SELECTED )
    {
        if (node->Selected() && !node->IsHidden())
            includeNode = true;
    }
    else if (settings_.nodeFilter() == ExportSettings::VISIBLE)
    {
        if (!node->IsHidden())
            includeNode = true;
    }

    if (toLower(node->GetName()).find("_bsp") != std::string::npos &&
        !node->IsHidden())
    {
        if (settings_.nodeFilter() == ExportSettings::SELECTED )
        {
            if (node->Selected())
            {
                meshNodes_.push_back( node );
                return;
            }
        }
        else
        {
            meshNodes_.push_back( node );
            return;
        }
    }

    ObjectState os = node->EvalWorldState( staticFrame() );

    if( os.obj )
    {
        Class_ID* foundClass = std::find( s_nonExportableClass, s_nonExportableClass +
            NUM_NONEXPORTABLE_CLASS, os.obj->ClassID() );

        if (foundClass != s_nonExportableClass + NUM_NONEXPORTABLE_CLASS)
            includeNode = false;

        uint32* found =  std::find( s_nonExportableSuperClass, s_nonExportableSuperClass + 
            NUM_NONEXPORTABLE_SUPER, os.obj->SuperClassID() );
        if (found != s_nonExportableSuperClass + NUM_NONEXPORTABLE_SUPER)
        {
            includeNode = false;
        }


        /*		std::string nodeName = node->GetName();
        INFO_MSG( "--- Object: %s\n", nodeName.c_str() );
        INFO_MSG( "******* Superclass 0x%08x\n", os.obj->SuperClassID() );
        Class_ID cid = os.obj->ClassID();
        INFO_MSG( "******* Class ID 0x%08x 0x%08x\n", cid.PartA(), cid.PartB() );*/
    }


    if (includeNode && node->Renderable())
    {
        BOOL isPortal = false;
        node->GetUserPropBool( "portal", isPortal );

        Modifier *pPhyMod = findPhysiqueModifier( node );
        Modifier* pSkinMod = findSkinMod( node );

        std::string nodeName = node->GetName();
        std::string nodePrefix;
        if (nodeName.length() > 3)
        {
            nodePrefix = toLower(nodeName.substr( 0, 3 ));
        }

        if (isPortal)
        {
            includeNode = false;
        }
        else if( (pPhyMod || pSkinMod) && settings_.exportMode() == ExportSettings::NORMAL )
        {
            includeNode = false;
        }
        else if (nodePrefix != "hp_")
        {
            meshNodes_.push_back( node );
        }
    }

    MFXNode* thisNode = mfxParent;

    thisNode = new MFXNode( node );
    thisNode->include( includeNode );
    if (mfxParent)
        mfxParent->addChild( thisNode );
    else
        mfxRoot_ = thisNode;

    if (includeNode)
        thisNode->includeAncestors();

    if (includeNode && trailingLeadingWhitespaces(thisNode->getIdentifier()))
    {
        char s[1024];
        sprintf_s( s, sizeof(s), "Node \"%s\" has an illegal name. Remove any spaces at the beginning and end of the name.", thisNode->getIdentifier().c_str() );
        MessageBox( ip_->GetMAXHWnd(), s, "MFExporter Error", MB_OK | MB_ICONWARNING );
    }

    for( int index = 0; index < node->NumberOfChildren(); index++ )
        preProcess( node->GetChildNode( index ), thisNode );

}


void MFXExport::exportMeshes( bool snapVertices )
{
    for( int i = 0; i < meshNodes_.size(); i++ )
    {
        exportMesh( meshNodes_[ i ], snapVertices );
    }
}

void MFXExport::exportMesh( INode *node, bool snapVertices )
{

    bool deleteIt = false;

    bool bspNode = (toLower(node->GetName()).find("_bsp") != std::string::npos);
    bool hullNode = (toLower(node->GetName()).find("_hull") != std::string::npos);

    MeshExporter::VisualMeshPtr spVisualMesh = new MeshExporter::VisualMesh;
    spVisualMesh->snapVertices( snapVertices );

    MSTR path = ip_->GetCurFilePath();
    std::string exportFromPath = getFilePath(std::string(path.data(), path.length()));
    if (spVisualMesh->init( node, exportFromPath, settings_.filePath() ) )
    {
        if (bspNode)
        {
        }
        else if (hullNode)
        {
        }
        else
        {
            visualMeshes_.push_back( spVisualMesh );
        }
    }
}

TriObject* MFXExport::getTriObject( INode *node, TimeValue t, bool &needDelete )
{
    needDelete = false;
    ObjectState os = node->EvalWorldState( t );

    if( os.obj && ( os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ) )
    {
        Object *obj = os.obj;
        if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
        { 
            TriObject *tri = (TriObject *) obj->ConvertToType( t, Class_ID(TRIOBJ_CLASS_ID, 0) );
            if ( obj != tri ) 
                needDelete = true;

            return tri;
        }
    }
    return NULL;
}


Modifier* MFXExport::findPhysiqueModifier( INode* node )
{
    // Get object from node. Abort if no object.
    Object* object = node->GetObjectRef();

    if( !object ) 
        return NULL;

    // Is derived object ?
    if( object->SuperClassID() == GEN_DERIVOB_CLASS_ID )
    {
        // Yes -> Cast.
        IDerivedObject* derivedObject = static_cast< IDerivedObject* >( object );

        // Iterate over all entries of the modifier stack.
        int modStackIndex = 0;
        while( modStackIndex < derivedObject->NumModifiers() )
        {
            // Get current modifier.
            Modifier* modifier = derivedObject->GetModifier( modStackIndex );

            // Is this Physique ?
            if( modifier->ClassID() == Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B ) )
            {
                // Yes -> Exit.
                return modifier;
            }

            // Next modifier stack entry.
            modStackIndex++;
        }
    }

    // Not found.
    return NULL;
}

IEditNormalsMod* MFXExport::findEditNormalsMod( INode* node )
{
    // Get object from node. Abort if no object.
    Object* object = node->GetObjectRef();

    if( !object ) 
        return NULL;

    // Is derived object ?
    if( object->SuperClassID() == GEN_DERIVOB_CLASS_ID )
    {
        // Yes -> Cast.
        IDerivedObject* derivedObject = static_cast< IDerivedObject* >( object );

        // Iterate over all entries of the modifier stack.
        int modStackIndex = 0;
        while( modStackIndex < derivedObject->NumModifiers() )
        {
            // Get current modifier.
            Modifier* modifier = derivedObject->GetModifier( modStackIndex );

            // Does this modifier have the editnormals interface?
            IEditNormalsMod* mod = (IEditNormalsMod*)modifier->GetInterface( EDIT_NORMALS_MOD_INTERFACE );
            if (mod)
                return mod;

            // Next modifier stack entry.
            modStackIndex++;
        }
    }

    // Not found.
    return NULL;
}

Modifier* MFXExport::findSkinMod( INode* node )
{
    // Get object from node. Abort if no object.
    Object* object = node->GetObjectRef();

    if( !object ) 
        return NULL;

    // Is derived object ?
    if( object->SuperClassID() == GEN_DERIVOB_CLASS_ID )
    {
        // Yes -> Cast.
        IDerivedObject* derivedObject = static_cast< IDerivedObject* >( object );

        // Iterate over all entries of the modifier stack.
        int modStackIndex = 0;
        while( modStackIndex < derivedObject->NumModifiers() )
        {
            // Get current modifier.
            Modifier* modifier = derivedObject->GetModifier( modStackIndex );

            // Is this Physique ?
            if( modifier->ClassID() == SKIN_CLASSID )
            {
                // Yes -> Exit.
                return modifier;
            }

            // Next modifier stack entry.
            modStackIndex++;
        }
    }

    // Not found.
    return NULL;
}


std::ostream& operator<<(std::ostream& o, const MFXExport& t)
{
    o << "MFXExport\n";
    return o;
}
