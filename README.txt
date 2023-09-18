directory for glm.hpp needs to installed to this folder and be set for each user, line 9 of raycast.cc.

code works with earlier versions of input files, I made it so if only 3 colors are read in from materials they act as rgb and don't get effected by lights.

use "make" to prompt program and ./target "text-file" to run the program, a new .ppm file will be created that will resemble the conditions of the text file.

text file rules:
imsize: resolution of the outputted .ppm file.
eye: location of the camera/eye we are seeing from in 3d space.
viewdir: the viewing direction, the direction the camera is facing.
hfov: horizontal fov.
updir: the upwards direction of the eye/camera, can be used to rotate what is being viewed.
bkgcolor: the color to be rendered if a ray does not intersect an object, the background's color.
mtlcolor: properties for the coloring and shading of the objects referenced to after. Includes object's diffuse color, ambient coefficient, diffuse coefficient, specular coefficent and specular exponent.
sphere: the location and radius of a sphere in 3d space.
light: the location of the light in 3d space, the type of light it is (1 for point, 2 for directional), and the color of the light.
