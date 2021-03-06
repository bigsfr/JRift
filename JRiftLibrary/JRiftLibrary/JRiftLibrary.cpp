#include "de_fruitfly_ovr_OculusRift.h"
#include "OVR.h"	

// @void256 I had to comment out this include when I tried to compile it with
// the latest Oculus SDK v0.2.2. Maybe this file existed in older versions of the SDK?
//
// it complained with this error otherwise:
//  JRiftLibrary.cpp:9:37: error: Util/Util_Render_Stereo.h: No such file or directory
//
//#include "Util/Util_Render_Stereo.h"

#include <cstring>
#include <iostream>

// @void256 conio.h does not exist on OS X as far as I know
#ifdef __WIN64__
#include <conio.h>
#elif __WIN32__
#include <conio.h>
#endif

using namespace OVR;

Ptr<DeviceManager>	pManager;
Ptr<HMDDevice>		pHMD;
Ptr<SensorDevice>	pSensor;
SensorFusion		FusionResult;
HMDInfo			Info;
bool			InfoLoaded;
bool			Initialized = false;
Quatf quaternion;
float yaw, pitch, roll;

JNIEXPORT jboolean JNICALL Java_de_fruitfly_ovr_OculusRift_initSubsystem(JNIEnv *env, jobject jobj) {
	System::Init();

	pManager = *DeviceManager::Create();

	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
	if (pHMD) {
		printf("Oculus Rift Device Interface created.\n");
		InfoLoaded = pHMD->GetDeviceInfo(&Info);
		pSensor = *pHMD->GetSensor();
		FusionResult.AttachToSensor(pSensor);
		Initialized = InfoLoaded && pSensor;
		printf("Oculus Rift Device Interface initialized.\n");
	}
	else {
		printf("Unable to create Oculus Rift device interface.\n");
	}

	if (InfoLoaded) {
		printf(" DisplayDeviceName: %s\n", Info.DisplayDeviceName);
		printf(" ProductName: %s\n", Info.ProductName);
		printf(" Manufacturer: %s\n", Info.Manufacturer);
		printf(" Version: %i\n", Info.Version); // seems to be an int now with 0.2.2
		printf(" HResolution: %i\n", Info.HResolution);
		printf(" VResolution: %i\n", Info.VResolution);
		printf(" HScreenSize: %f\n", Info.HScreenSize);
		printf(" VScreenSize: %f\n", Info.VScreenSize);
		printf(" VScreenCenter: %f\n", Info.VScreenCenter);
		printf(" EyeToScreenDistance: %f\n", Info.EyeToScreenDistance);
		printf(" LensSeparationDistance: %f\n", Info.LensSeparationDistance);
        printf(" InterpupillaryDistance: %f\n", Info.InterpupillaryDistance);
        printf(" DistortionK[0]: %f\n", Info.DistortionK[0]);
        printf(" DistortionK[1]: %f\n", Info.DistortionK[1]);
		printf(" DistortionK[2]: %f\n", Info.DistortionK[2]);
	}

	return Initialized;
}

JNIEXPORT void JNICALL Java_de_fruitfly_ovr_OculusRift_destroySubsystem(JNIEnv *env, jobject jobj) {
	printf("Destroying Oculus Rift device interface.\n");
	pSensor.Clear();
	pManager.Clear();

	System::Destroy();
}

JNIEXPORT void JNICALL Java_de_fruitfly_ovr_OculusRift_pollSubsystem(JNIEnv *, jobject) {
	if (!Initialized) return;
	if (!pSensor) return;

	quaternion = FusionResult.GetOrientation();
	quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);
}

JNIEXPORT jint JNICALL Java_de_fruitfly_ovr_OculusRift__1getHResolution(JNIEnv *, jobject) {
	if (!Initialized) return 0;
	return Info.HResolution;
}

JNIEXPORT jint JNICALL Java_de_fruitfly_ovr_OculusRift__1getVResolution(JNIEnv *, jobject){
	if (!Initialized) return 0;
	return Info.VResolution;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getHScreenSize(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.HScreenSize;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getVScreenSize(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.VScreenSize;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getVScreenCenter(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.VScreenCenter;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getEyeToScreenDistance(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.EyeToScreenDistance;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getLensSeparationDistance(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.LensSeparationDistance;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getInterpupillaryDistance(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.InterpupillaryDistance;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getDistortionK0(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.DistortionK[0];
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getDistortionK1(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.DistortionK[1];
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getDistortionK2(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.DistortionK[2];
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getDistortionK3(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return Info.DistortionK[3];
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getYaw(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return yaw;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getPitch(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return pitch;
}

JNIEXPORT jfloat JNICALL Java_de_fruitfly_ovr_OculusRift__1getRoll(JNIEnv *, jobject){
	if (!Initialized) return 0.0f;
	return roll;
}
