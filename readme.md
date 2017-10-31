# ofxGrabCam

<img src="https://raw.github.com/elliotwoods/ofxGrabCam/master/ofxaddons_thumbnail.png" />

Camera for browsing your 3D world in openFrameworks, which 'picks' the xyz position under the cursor, so all your mouse actions are performed relative to that point in 3D space.

* Rotations and zoom are then performed with respect to that xyz position.
* Rotations are performed using a trackball model.


Inspired by Google SketchUp (http://sketchup.google.com/)

Elliot Woods
http://www.kimchiandchips.com

# Usage

```c++
//in your app .h
ofxGrabCam camera
```

```c++
//in your .cpp

void ofApp::draw() {
  camera.begin();
  {
    //do your 3d drawing
  }
  camera.end();
}
```



# Controls
## ofxGrabCam (general)
Left click drag = rotate

Right click drag = zoom

hold 'h' whilst left click drag = pan

'r' key = reset

## Example project
'c' = toggle draw cursor

'u' = toggle fix up vector

# License
MIT License
http://en.wikipedia.org/wiki/MIT_License

# Notes

* We use glReadPixels to read back the depth buffer around the mouse cursor to find the XYZ position
* Reading back the depth buffer happens only when required (e.g. mouse down/dragged, or when you enable the 3D cursor)
* Reading back all the time may reduce your frame rate, so make sure to leave the cursor disabled (default) if you want a higher frame rate.

# Testimonials

"this is basically the best thing ever." - kyle mcdonald

"this is absolute crap" - golan levin
