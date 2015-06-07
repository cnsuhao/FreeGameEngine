/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#ifndef EXPSETS_HPP
#define EXPSETS_HPP

#pragma warning ( disable : 4530 )

class ExportSettings
{
public:
	~ExportSettings();

	bool prepareExport( HWND hWnd );

	enum ExportMode
	{
		NORMAL = 0,
		STATIC,
		STATIC_WITH_NODES,
		MESH_PARTICLES
	};

	ExportMode	exportMode( ) const { return exportMode_; };
	void		exportMode( ExportMode exportMode ) { exportMode_ = exportMode; };

	enum NodeFilter
	{
		ALL = 0,
		SELECTED,
		VISIBLE
	};

    void setStaticFrame( unsigned int i ) {  staticFrame_ = i; }
    unsigned int staticFrame( void ) const { return staticFrame_; }

	NodeFilter	nodeFilter( ) const { return nodeFilter_; };
	void		nodeFilter( NodeFilter nodeFilter ) { nodeFilter_ = nodeFilter; };

	bool		allowScale( ) const { return allowScale_; };
	void		allowScale( bool state ) { allowScale_ = state; };

	bool		copyTextures( ) const { return copyTextures_; }
	void		copyTextures( bool state ) { copyTextures_ = state; };

	bool		useLegacyOrientation( ) const { return useLegacyOrientation_; };
	void		useLegacyOrientation( bool state ) { useLegacyOrientation_ = state; };

	bool		snapVertices( ) const { return snapVertices_; };
	void		snapVertices( bool state ) { snapVertices_ = state; };

	float		unitScale( ) const;

	const std::string	& filePath() const { return filePath_; }
	void filePath( std::string state )  { filePath_ = state; }

    const std::string & resPath() const { return resPath_; }
    const std::string & defaultShader() const { return defaultShader_; }

	static ExportSettings& ExportSettings::instance();

private:
	ExportSettings();

	bool			allowScale_;
	bool			useLegacyOrientation_;
	bool			snapVertices_;
    bool			copyTextures_;
    
    //the static frame output
    unsigned int	staticFrame_;

	ExportMode		exportMode_;
	NodeFilter		nodeFilter_;

	std::string     filePath_;
    std::string     resPath_;
    std::string     defaultShader_;

    ExportSettings(const ExportSettings&);
    ExportSettings& operator=(const ExportSettings&);

	friend std::ostream& operator<<(std::ostream&, const ExportSettings&);
};

#endif
/*expsets.hpp*/