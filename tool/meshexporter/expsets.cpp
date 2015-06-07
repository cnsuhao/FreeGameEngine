/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#pragma warning ( disable: 4503 )
#pragma warning ( disable: 4786 )

#include "pch.hpp"
#include "expsets.hpp"

// max includes
#include "max.h"

// exporter resources
#include "resource.h"
#include "file_stream.h"
#include "string_tool.h"

static const std::string ConfigFile = "meshexporter.config";
static const std::string DefaultShader = "shaders/light_texture.shader";
static const std::string DefaultResPath = "";

//the instance
extern HINSTANCE hInstance;

ExportSettings::ExportSettings()
: exportMode_( NORMAL ),
  nodeFilter_( SELECTED ),
  allowScale_( true ),
  useLegacyOrientation_( true ),
  snapVertices_(false),
  copyTextures_(true),
  staticFrame_( 0 ),
  defaultShader_(DefaultShader),
  resPath_(DefaultResPath)
{
}

ExportSettings::~ExportSettings()
{
}

bool ExportSettings::prepareExport( HWND hWnd )
{
    this->exportMode_ = STATIC;//目前只允许导出静态mesh
    this->nodeFilter_ = ExportSettings::VISIBLE;
    
    std::string configFileName = getModuleFilePath(hInstance) + ConfigFile;
    FileSreamPtr config = FileSream::openStream(configFileName);
    if(config)
    {
        config->readV("shader", defaultShader_, DefaultShader);
        config->readV("resPath", resPath_, DefaultResPath);

        formatPathName(resPath_);
    }

	return true;
}


ExportSettings& ExportSettings::instance()
{
	static ExportSettings expsets;
	return expsets;
}


std::ostream& operator<<(std::ostream& o, const ExportSettings& t)
{
	o << "ExportSettings\n";
	return o;
}

float ExportSettings::unitScale( ) const
{
	return (float)GetMasterScale( UNITS_CENTIMETERS ) / 100;
}
