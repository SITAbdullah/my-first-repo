# Android Programming Quiz - Cross Platform CI

Author: Muhammad Abdullah Bin Ahmad

This project builds the same shared OpenGL ES scene on Desktop, Android, and Web.

## Desktop

From the project root:

```bat
script_build_and_run.bat
```

Press ESC to close the window.

## Android

Open the `android/` folder in Android Studio, sync Gradle, then run the app on an emulator or device.

## Web

Activate Emscripten first, then from the project root run:

```bat
build_web.bat
```

Serve the generated files:

```bat
cd web
python -m http.server 8080
```

Open `http://localhost:8080/` in a browser.
