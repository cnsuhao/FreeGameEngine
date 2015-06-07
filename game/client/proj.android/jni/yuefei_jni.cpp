
#include <jni.h>
#include <android/asset_manager_jni.h>

#include <SDL/include/SDL_main.h>
#include <SDL/include/SDL_system.h>

#include "util/log_tool.h"
#include "util/string_tool.h"

#include "cstdmf/guard.hpp"
#include "resmgr/apk_file_system.hpp"

namespace
{
	std::string g_apkPath;
	std::string g_internalStoragePath;
}

extern "C"
{
	void Java_org_libsdl_app_SDLActivity_nativeSetAssetManager(
		JNIEnv* env, jclass jcls,
		jobject assetMgr)
	{
		ORA_STACK_TRACE;

		AAssetManager *pAssetMgr = AAssetManager_fromJava(env, assetMgr);
		ApkFileSystem::setAssetMgr(pAssetMgr);
	}

	void Java_org_libsdl_app_SDLActivity_nativeSetApkPath(
			JNIEnv*  env, jobject thiz,
			jstring apkPath)
	{
		ORA_STACK_TRACE;

		const char* chars = env->GetStringUTFChars(apkPath, NULL);
		g_apkPath = chars;
		env->ReleaseStringUTFChars(apkPath, chars);
	}


	/* Called before SDL_main() to initialize JNI bindings in SDL library */
	void SDL_Android_Init(JNIEnv* env, jclass cls);

	/* Start up the SDL app */
	void Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject obj)
	{
		BW_GUARD;
		ORA_STACK_TRACE;

		/* This interface could expand with ABI negotiation, calbacks, etc. */
		SDL_Android_Init(env, cls);

		SDL_SetMainReady();

		g_internalStoragePath = SDL_AndroidGetInternalStoragePath();
		ora::formatFilePath(g_internalStoragePath);

		/* Run the application code! */
		int status;
		char *argv[2];
		argv[0] = SDL_strdup("SDL_app");
		argv[1] = NULL;

		ORA_LOG_INFO("Start SDL_main...");
		status = SDL_main(1, argv);
		ORA_LOG_INFO("End SDL_main. status: %d", status);

		/* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
		/* exit(status); */
	}

}

bool onClientSetup()
{
	ORA_STACK_TRACE;

	return true;
}

namespace BWUtil
{
	std::string getAppDocDir()
	{
		return g_internalStoragePath;
	}

	// 获取程序app文件所在目录路径
	std::string getAppHomeDir()
	{
		return g_internalStoragePath;
	}

	// 获取程序tmp目录路径
	std::string getAppTmpDir()
	{
		return g_internalStoragePath;
	}

	// 获取程序应用包路径
	std::string getAppResPath()
	{
		return "/apk/";
	}

} // end namespace BWUtil
