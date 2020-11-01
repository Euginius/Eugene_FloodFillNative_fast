# 🎨 Fast Flood and Fill Native library for Android
Fast native implementation of Flood fill algorithm for Android

This is fast native implementation based on original "threekkapps_library" but works 3 to 4 times faster and takes 10-20% less memory on execution

   Sample app Screenshot     
:-------------------------:|
![](https://github.com/Euginius/Eugene_FloodFillNative_fast/blob/main/screenshot.png)

Usage:

	```kotlin
     NativeLib.floodFill1(bitmap, event.x.toInt(), event.y.toInt(), selectedColor,tolerance)
    ```

Credits:
</br>
Based on "jnibitmap.cpp" file from 
<a href="https://github.com/mar3kk/threekkapps_library/">https://github.com/mar3kk/threekkapps_library/</a>
<a href="https://github.com/Euginius/threekkapps_library/">https://github.com/Euginius/threekkapps_library/</a>
