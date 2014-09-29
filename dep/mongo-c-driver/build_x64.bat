  cd src\libbson
  cmake -DCMAKE_INSTALL_PREFIX=C:\usr -G "Visual Studio 11 Win64" .
  msbuild.exe ALL_BUILD.vcxproj
  msbuild.exe INSTALL.vcxproj
  cd ..\..
  cmake -DCMAKE_INSTALL_PREFIX=C:\usr -DBSON_ROOT_DIR=C:\usr -G "Visual Studio 11 Win64" .
  msbuild.exe ALL_BUILD.vcxproj
  msbuild.exe INSTALL.vcxproj