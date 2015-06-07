/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

#include "pch.hpp"
#include "mfxexp.hpp"
#include "string_tool.h"

//Global variables.

HINSTANCE hInstance;
int controlsInit = FALSE;

//Global functions
extern ClassDesc* GetMFXExpDesc();


/*
 * The dll entrypoint function
 */
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	hInstance = hinstDLL;

	// Initialise the custom controls. This should be done only once.
	if (!controlsInit)
	{
		controlsInit = TRUE;
#ifndef MAX_RELEASE_R10 // GetEffectFilename was deprecated in max 2008
		InitCustomControls(hInstance);
#endif
		InitCommonControls();

        DEBUG_MSG("Mesh Exporter initialized. 3ds max version %d", MAX_RELEASE);
    }

	return TRUE;
}

/*
 * Returns the description of the library (stored in the string table)
 */
__declspec( dllexport ) const TCHAR* LibDescription() 
{
	return GetString(IDS_LIBDESCRIPTION);
}

/*
 * Returns the number of classes in the Library.
 */
__declspec( dllexport ) int LibNumberClasses() 
{
	return 1;
}

/*
 * Returns the class description for the requested class.
 */
__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i) 
	{
	case 0: 
		return GetMFXExpDesc();
	default: 
		return 0;
	}
}

/*
 * Returns the library version. (defined in mfxexp.hpp)
 */
__declspec( dllexport ) ULONG LibVersion() 
{
	return VERSION_3DSMAX;
}

/*
 * The class description class.
 */
class MFXExpClassDesc:public ClassDesc {
public:
	int				IsPublic() { return 1; }
	void*			Create(BOOL loading = FALSE) { return new MFXExport; } 
	const TCHAR*	ClassName() { return GetString(IDS_MFEXP); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; } 
	Class_ID		ClassID() { return MFEXP_CLASS_ID; }
	const TCHAR*	Category() { return GetString(IDS_CATEGORY); }
};

/*
 * The class description instance.
 */
static MFXExpClassDesc MFXExpDesc;


/*
 * Returns the class description class for the mfx exporter.
 */
ClassDesc* GetMFXExpDesc()
{
	return &MFXExpDesc;
}

/*
 * Gets a string from the resource string table.
 */
TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;

	return NULL;
}

// expmain.cpp
