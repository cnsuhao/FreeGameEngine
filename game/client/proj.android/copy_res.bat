
@echo off

set RES_PATH=res_android
echo res path: %RES_PATH%

if not exist %RES_PATH% (
	echo "the path 'res_android' doesn't exist"
	goto END
)

echo "update resource..."
svn up %RES_PATH%

echo "remove old assets..."
if exist assets (
	rmdir /S /Q assets
)

echo "copy resource...."
mkdir assets
xcopy %RES_PATH% assets\ /E /Y 
xcopy paths.xml assets /Y

echo "generate directory cache..."
python gen_dir_list.py assets

:END

pause
