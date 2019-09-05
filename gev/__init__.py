from .pygev import Camera
Camera.initialize()

import atexit
atexit.register(Camera.uninitialize)
