SET ANDROID_HOME=F:/of/android-sdk
SET JAVA_HOME=F:\OF\JDK
SET OF_ROOT=F:/of/openframeworks/android
SET NDK_ROOT=F:/of/android-ndk
SET SDK_ROOT=F:/of/android-sdk
SET PATH=F:\OF\ANT\bin;F:\OF\android-sdk\platform-tools;F:\OF\MINGW\mingw64\bin;F:\of\mingw\msys\1.0\bin;F:\of\jdk\bin;F:\of\openframeworks\android\libs\openframeworkscompiled\project\android\windows;F:\windows\system32
make -j1 Release PLATFORM_OS=Android && if ERRORLEVEL 0 GOTO Deploy else GOTO End

:End
pause
GOTO:EOF

:Deploy
ant clean && ant debug && adb -s 02157df2a405393a install -r bin/COGenginelab.apk && adb shell am start -a android.intent.action.MAIN -n cc.openframeworks.COGenginelab/cc.openframeworks.COGenginelab.OFActivity  && adb logcat test:* *:S

pause