#include <jni.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <android/bitmap.h>

using namespace cv;

extern "C" {
JNIEXPORT void JNICALL
Java_eu_sisik_opencvsample_MainActivity_canny(
        JNIEnv *env,
        jobject /* this */,
        jobject bitmap,
        jstring destination) {

    // Get information about format and size
    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    // Get pointer to pixel buffer
    void *pixels = 0;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    // I create separate scope for input Mat here
    // to make sure it is destroyed before unlocking
    // pixels
    {
        // Check the format info before you pick the CV_ type
        // for OpenCV Mat
        // info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 -> CV_8UC4

        // Now create the mat
        Mat input(info.height, info.width, CV_8UC4, pixels);

        // Perform canny edge detection
        Mat edges;
        Canny(input, edges, 200.0, 600.0);

        // Save to destination
        const char *dest = env->GetStringUTFChars(destination, 0);
        imwrite(dest, edges);
        env->ReleaseStringUTFChars(destination, dest);
    }

    // Release the Bitmap buffer once we have it inside out Mat
    AndroidBitmap_unlockPixels(env, bitmap);
}

}
