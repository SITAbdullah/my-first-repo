# AndroidProgrammingQuiz-CrossPlatformCI

Cross-platform OpenGL ES 3.0 renderer displaying a rotating triangle and a yellow square.
Runs on Desktop (Windows), Android, and Web (WebGL).

## Build Instructions

### Desktop (Windows)
Run from project root:
```bat
script_build_and_run.bat
```
Press ESC to close.

### Android
Open the `android/` folder in Android Studio, sync, build, and run on an emulator or device.

### Web
Activate Emscripten first (adjust path to where you installed emsdk):
```bat
cd C:\emsdk
emsdk_env.bat
cd <back to project root>
```
Then build and serve:
```bat
build_web.bat
cd web && python -m http.server 8080
```
Open `http://localhost:8080/` in a browser.
