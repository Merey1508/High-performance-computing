# Mini-Rt manual

## Starting using the library 

    #include "minirt/minirt.h"
    using namespace minirt;

## Creating materials

A material contains diffuse and specular colors for Phong shading.
Specular color is used in computing reflection and highlights, 
shininess parameter controls highlights' size.

The same color for diffuse and specular:

    Color color {1, 0, 0};
    Material m1 {color};

Different colors for diffuse and specular, with a shininess parameter:

    Color diffuse {0.2, 0.2, 0};
    Color specular {0.8, 0.5, 0.5};
    double shininess = 50;
    Material m2 {diffuse, specular, shininess};

A material can be made transparent by setting refraction amount (from 0 to 1; 0 - no transparency, max reflection, 1 - max transparency, no reflection) and refraction index 
(setting it above or below 1 can lead to different effects):

    Material m3 {Color {0.2}};
    double refractionAmount = 0.9;
    double refractionIndex = 1.33;
    m3.makeTransparent(refractionAmount, refractionIndex);

## Creating objects

    Sphere sphere {position, radius, material};

## Creating lights

Point light has a position in space and a color.

    Point3D position {10, 0, 10};
    Color color {0.9, 0, 0.9};
    PointLight light {position, color};

## Setting up a scene

Scene is a collection of objects and lights:

    Scene scene;

Add objects:

    scene.addObject(sphere);

Add lights:

    scene.addLight(light);

You can also set background color:

    scene.setBackground(Color {0.1, 0.1, 0.2});

And ambient light:

    scene.setAmbient(Color {0.1, 0.1, 0.1});

Setting maximum depth of recursion for computing reflection and refraction:

    scene.setRecursionLimit(10);

### Scene files

To load scene from a file:
    
    scene.loadFromFile("scene.txt");

[Scene file format documentation](Script.md).

## Setting up a camera

A scene is rendered as if looked on from a camera.

    Camera camera;

The camera has position and orientation in the world (scene) space.
By default the camera is located at (0, 0, -20) and looks at (0, 0, 0), 
the camera's 'up' direction is (0, 1, 0) in world coordinates.
You can set camera's position and orientation during creation 
(the third optional parameter is 'up' direction in the world and is (0, 1, 0) by default):

    Point3D viewPoint {10, 0, -10};
    Point3D target {0, 0, 1};
    Camera camera {viewPoint, target};

You can also change position and orientation later:
    
    camera.set(viewPoint, target);

Or change position only 
(orientation doesn't change, but target changes accordingly):
    
    camera.setViewPoint(viewPoint);

To set camera for the scene:

    scene.setCamera(camera);

## Setting up a view (image) plane

View (image) plane is what a camera 'sees' and what will be rendered. 
To create a view plane:

    ViewPlane plane {resolutionX, resolutionY, sizeX, sizeY, distance};

Here `resolutionX` and `resolutionY` are resolution in pixels by X (width) and Y (height) respectively, for example, 1920x1080.
`sizeX` and `sizeY` are plane width and height in world coordinates, `distance` - distance from a camera in world coordinates.

## Rendering a pixel

    Color color = plane.computePixel(scene, x, y, numberOfSamples);

This computes color for the pixel with an index (x, y), counting from the bottom left corner of the image.
`numberOfSamples` parameter specifies how many samples to do for the pixel.
With default value 1 center of the pixel will be used to trace a ray, 
otherwise specified number of random positions within the pixel will be used.
More samples generally produces a better result (anti-aliasing).

## Rendering an image

To store a rendered image let's create it first:

    Image image {width, height};

Here `width` is image size by X (number of columns), `height` - image size by Y (number of rows).

Here is the main loop for rendering the image:

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            image.set(x, y, plane.computePixel(scene, x, y, numberOfSamples));

Image can be set or get pixel by pixel:

    image.set(x, y, color);
    Color pixelColor = image.get(x, y);

Here `x` is a pixel's coordinate by X (width), `y` - by Y (height). 
These coordinates are measured from the bottom left corner of the image (to match view plane's coordinate system).

To access the whole image data as array of pixels:

    auto *data = image.getData();

Pixels are stored by columns, starting from the bottom left corner of the image.

## Saving a result

A rendered image can be saved as a JPEG file:

    image.saveJPEG("result.jpg", quality);

`quality` is a value from 1 to 100 which controls the amount of compression and may affect image's quality. By default `quality` = 90.
