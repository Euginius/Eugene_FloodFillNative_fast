#include <jni.h>
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <android/bitmap.h>
#include <queue>

#include <unistd.h>

#define  LOG_TAG    "jnibitmap"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)


struct NodePoint {
    NodePoint *next;
    int x, y;
};

NodePoint *queueHead = nullptr;
NodePoint *queueTail = nullptr;

/* return newly created node point */
NodePoint *new_node_point(int x, int y) {
    auto *node = (struct NodePoint *) malloc(sizeof(struct NodePoint));
    node->x = x;
    node->y = y;
    node->next = nullptr;
    return node;
}

/* returns the new head of the stack after adding node to the queue */
void enqueue(NodePoint *node) {
    if (node) {
        if (queueHead == nullptr && queueTail == nullptr) {
            queueHead = node;
            queueTail = node;
        } else {
            queueTail->next = node;
            queueTail = node;
        }
    }
}

/* returns the new head of the stack after adding node to the queue */
NodePoint *dequeue() {
    NodePoint *retVal = nullptr;
    if (queueHead != nullptr) {
        retVal = queueHead;
        queueHead = queueHead->next;
        retVal->next = nullptr;
        if (queueHead == nullptr) {
            queueTail = nullptr;
        }
    }
    return retVal;
}

/* check if queue is empty */
bool is_empty_queue() {
    return queueHead == nullptr;
}

bool isPixelValid(int currentColor, int oldColor, int *startColor, int tolerance);

void
floodFillAlgo(uint32_t x, uint32_t y, uint32_t sourceColor,uint32_t targetColor, void *bitmapPixels, AndroidBitmapInfo *bitmapInfo,
              uint32_t tolerance);

extern "C"
JNIEXPORT void JNICALL
Java_com_crazyflystudio_floodfillnativelib_NativeLib_floodFill1(JNIEnv *env,
                                                                  jclass obj,
                                                                  jobject bitmap,
                                                                  jint x,
                                                                  jint y,
                                                                  jint targetColor,
                                                                  jint tolerance
) {
    AndroidBitmapInfo bitmapInfo;

    int ret;
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888!");
        return;
    }

    void *bitmapPixels;
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;

    }

    auto *pixels = (uint32_t *) bitmapPixels;
    int sourceColor = pixels[y * bitmapInfo.width + x];

    floodFillAlgo(x, y, sourceColor,targetColor, bitmapPixels, &bitmapInfo, tolerance);
    AndroidBitmap_unlockPixels(env, bitmap);
}



extern "C"
JNIEXPORT void JNICALL
Java_com_crazyflystudio_floodfillnativelib_NativeLib_floodFill2(JNIEnv *env,
                                                                  jclass obj,
                                                                  jobject bitmap,
                                                                  jint x,
                                                                  jint y,
                                                                  jint sourceColor,
                                                                  jint targetColor,
                                                                  jint tolerance
) {
    AndroidBitmapInfo bitmapInfo;

    int ret;
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888!");
        return;
    }

    void *bitmapPixels;
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;

    }

    floodFillAlgo(x, y, sourceColor,targetColor, bitmapPixels, &bitmapInfo, tolerance);
    AndroidBitmap_unlockPixels(env, bitmap);
}

bool isPixelValid(int currentColor, int oldColor, int *startColor, int tolerance) {

    if (tolerance != 0) {
        int alpha = ((currentColor & 0xFF000000) >> 24);
        int red = ((currentColor & 0xFF0000) >> 16) * alpha / 255; // red
        int green = ((currentColor & 0x00FF00) >> 8) * alpha / 255; // Green
        int blue = (currentColor & 0x0000FF) * alpha / 255; // Blue

        return (red >= (startColor[0] - tolerance)
                && red <= (startColor[0] + tolerance)
                && green >= (startColor[1] - tolerance)
                && green <= (startColor[1] + tolerance)
                && blue >= (startColor[2] - tolerance)
                && blue <= (startColor[2] + tolerance));
    } else {
        return currentColor == oldColor;
    }
}

void
floodFillAlgo(uint32_t x, uint32_t y,uint32_t sourceColor, uint32_t destColor, void *bitmapPixels, AndroidBitmapInfo *bitmapInfo,
              uint32_t tolerance) {

    int values[3] = {};

    if (x > bitmapInfo->width - 1)
        return;
    if (y > bitmapInfo->height - 1)
        return;
    if (x < 0)
        return;
    if (y < 0)
        return;

    auto *pixels = (uint32_t *) bitmapPixels;
    uint32_t oldColor;

    int red = 0;
    int blue = 0;
    int green = 0;
    int alpha = 0;
    oldColor = sourceColor;

// Get red,green and blue values of the old color we like to chnage
    alpha = (int) ((destColor & 0xFF000000) >> 24);
    values[0] = (int) ((oldColor & 0xFF0000) >> 16) * alpha / 255; // red
    values[1] = (int) ((oldColor & 0x00FF00) >> 8) * alpha / 255; // Green
    values[2] = (int) (oldColor & 0x0000FF) * alpha / 255; // Blue


    alpha = (int) ((destColor & 0xFF000000) >> 24);
    blue = (int) ((destColor & 0xFF0000) >> 16);
    green = (int) ((destColor & 0x00FF00) >> 8);
    red = (int) (destColor & 0x0000FF);
    blue = blue * alpha / 255;
    green = green * alpha / 255;
    red = red * alpha / 255;

    int tmp = 0;
    tmp = red;
    red = blue;
    blue = tmp;

    destColor = ((alpha << 24) & 0xFF000000) | ((blue << 16) & 0xFF0000) |
                ((green << 8) & 0x00FF00) |
                (red & 0x0000FF);

    int nx = 0;
    int ny = 0;
    int wx = 0;
    int wy = 0;
    int ex = 0;
    int ey = 0;

    // Add first pixel to queue
    enqueue(new_node_point(x, y));

    while (!is_empty_queue()) {

        NodePoint *p = dequeue();
        nx = p->x;
        ny = p->y;
        free(p);

        if (pixels[ny * bitmapInfo->width + nx] == destColor)
            continue;

        wx = nx;
        wy = ny;
        ex = wx + 1;
        ey = wy;

        while (wx >= 0 &&
               isPixelValid(pixels[wy * bitmapInfo->width + wx], oldColor, values, tolerance)) {
            pixels[wy * bitmapInfo->width + wx] = destColor;

            if (wy > 0 && pixels[(wy - 1) * bitmapInfo->width + wx] == oldColor) {
                enqueue(new_node_point(wx, wy - 1));
            }
            if (wy < bitmapInfo->height - 1 &&
                isPixelValid(pixels[(wy + 1) * bitmapInfo->width + wx], oldColor, values,
                             tolerance)) {
                enqueue(new_node_point(wx, wy + 1));
            }
            wx--;
        }

        while (ex < bitmapInfo->width &&
               isPixelValid(pixels[ey * bitmapInfo->width + ex], oldColor, values, tolerance)) {
            pixels[ey * bitmapInfo->width + ex] = destColor;
            if (ey > 0 && isPixelValid(pixels[(ey - 1) * bitmapInfo->width + ex], oldColor, values,
                                       tolerance)) {
                enqueue(new_node_point(ex, ey - 1));
            }
            if (ey < bitmapInfo->height - 1 &&
                isPixelValid(pixels[(ey + 1) * bitmapInfo->width + ex], oldColor, values,
                             tolerance)) {
                enqueue(new_node_point(ex, ey + 1));
            }
            ex++;
        }
    }
}
