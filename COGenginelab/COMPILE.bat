SET ANDROID_HOME=c:/ofxp/android-sdk
SET JAVA_HOME=C:\OFXP\JDK
SET OF_ROOT=C:/ofxp/openframeworks/android
SET NDK_ROOT=C:/ofxp/android-ndk
SET SDK_ROOT=C:/ofxp/android-sdk
SET PATH=C:\OFXP\ANT\bin;C:\OFXP\android-sdk\platform-tools;C:\OFXP\MINGW\mingw64\bin;c:\ofxp\mingw\msys\1.0\bin;c:\ofxp\jdk\bin;C:\ofxp\openframeworks\android\libs\openframeworkscompiled\project\android\windows;c:\windows\system32
make -j8 clean PLATFORM_OS=Android
make -j8 Release PLATFORM_OS=Android