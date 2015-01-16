#------------------------------------------------------------------------------
# Android build utilities
#------------------------------------------------------------------------------

# Locate ant

find_program(NX_ANT ant)

if(${NX_ANT} EQUAL NX_ANT-NOTFOUND)
    message(SEND_ERROR "Could not find ant executable")
else()
    message(STATUS "Found ant: ${NX_ANT}")
endif()

# Common variables
set(NX_ANDROID_BUILD_DIR_NAME android-build)
set(NX_ANDROID_PLATFORM 21)

if("${ANDROID_ABI}" STREQUAL "${ANDROID_ABI_ARM}")
    set(NX_ANDROID_ABI_MK "armeabi-v7a")
else()
    message(SEND_ERROR "Unknown Android ABI specified: ${ANDROID_ABI}")
endif()

function(NX_ANDROID_MKDIRS)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NX_ANDROID_BUILD_DIR_NAME})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NX_ANDROID_BUILD_DIR_NAME}/res/values)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${NX_ANDROID_BUILD_DIR_NAME}/jni)
endfunction()

# Generate the AndroidManifest.xml
function(NX_ANDROID_GEN_FILES TARGET APP_NAME)

    set(_android_dir ${CMAKE_CURRENT_BINARY_DIR}/${NX_ANDROID_BUILD_DIR_NAME})
    set(_android_manif ${_android_dir}/AndroidManifest.xml)
    set(_android_strings ${_android_dir}/res/values/strings.xml)
    set(_android_appmk ${_android_dir}/jni/Application.mk)
    set(_android_andmk ${_android_dir}/jni/Android.mk)

    # write manifest
    file(WRITE ${_android_manif}
"<?xml version=\"1.0\" encoding=\"utf-8\"?> \n"
"    <manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
"        package=\"nx.samples.${TARGET}\"\n"
"        android:versionCode=\"1\"\n"
"        android:versionName=\"1.0\">\n"
"    <uses-sdk android:minSdkVersion=\"${NX_ANDROID_PLATFORM}\" />\n"
"    <uses-permission android:name=\"android.permission.READ_EXTERNAL_STORAGE\" /> \n"
"    <application android:label=\"@string/app_name\" android:debuggable=\"true\" android:hasCode=\"false\">\n"
"    <activity android:name=\"android.app.NativeActivity\"\n"
"        android:label=\"@string/app_name\"\n"
"        android:configChanges=\"orientation|keyboardHidden\">\n"
"        <meta-data android:name=\"android.app.lib_name\"\n"
"             android:value=\"${TARGET}\" />\n"
"            <intent-filter>\n"
"                <action android:name=\"android.intent.action.MAIN\" />\n"
"                <category android:name=\"android.intent.category.LAUNCHER\" />\n"
"            </intent-filter>\n"
"        </activity>\n"
"    </application>\n"
"</manifest>"
    )

    # write strings
    file(WRITE ${_android_strings}
"<?xml version=\"1.0\" encoding=\"utf-8\"?> \n"
"<resources>\n"
"    <string name=\"app_name\">${APP_NAME}</string>\n"
"</resources>"
    )

    # write Application.mk
    file(WRITE ${_android_appmk}
"APP_ABI := ${NX_ANDROID_ABI_MK}\n"
"APP_PLATFORM := android-${NX_ANDROID_PLATFORM}\n"
    )

    get_property(_lib_location TARGET ${TARGET} PROPERTY LOCATION)

    # write Android.mk
    file(WRITE ${_android_andmk}
"LOCAL_PATH := $(call my-dir)\n"
"include $(CLEAR_VARS)\n"
"LOCAL_MODULE := ${TARGET}\n"
"LOCAL_SRC_FILES := ${_lib_location}\n"
"include $(PREBUILT_SHARED_LIBRARY)\n"
    )
    
    # generate ant project
    exec_program(${ANDROID_SDK_DIR}/tools/android
        ARGS update project --name ${APP_NAME} --path ${_android_dir}/ --target "android-${NX_ANDROID_PLATFORM}"
        RETURN_VALUE _ret_val
    )
    
    if (NOT ${_ret_val} EQUAL 0)
        message(SEND_ERROR "Failed to generate android project")
    endif()
    
    # add ndk_build & apk gen 
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ant debug
        WORKING_DIRECTORY ${_android_dir}
        COMMENT "Generating Android APK for ${TARGET}"
    )
        
     
endfunction()
