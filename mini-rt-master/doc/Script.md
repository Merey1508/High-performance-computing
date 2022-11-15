# Scene file format

A scene file is a text file, describing a scene.
It contains statements for materials, objects, lights, camera, etc.

## Materials

A material is described with:

    material materialName
        parameters
    end

Parameters are:

- `color r g b`: color for diffuse and specular
- `diffuse r g b`: diffuse color
- `specular r g b`: specular color
- `shininess value`: value for shininess
- `transparent value value`: values for refraction amount and refraction index (to make material transparent)

Example:

    material shinyRed
        diffuse 0.5 0.1 0.1
        specular 0.5 0.5 0.5
        shininess 250
    end

## Objects

A sphere is added with:

    sphere
        parameters
    end

Parameters are:

- `position x y z`: sphere's position
- `radius value`: sphere's radius (1 by default)
- `material materialName`: sphere's material

Example:

    sphere
        position 10 -2 0
        radius 0.5
        material shinyRed
    end

You can also specify the same parameters as for material right away. In that case you may not specify material separately:

    sphere 
        position 5 5 3
        radius 1
        diffuse 0.5 0 0.2
        specular 0.5 0.5 0.4
    end

## Lights

A point light is added with:

    light
        parameters
    end

Parameters are:

- `position x y z`: light's position
- `color r g b`: light's color

Example:

    light
        position -10 0 -10
        color 0.8 0.8 0.8
    end

## Camera

A camera is set with:

    camera
        parameters
    end

Parameters are:

- `position x y z`: camera's position (view point)
- `target x y z`: camera's target (a point which camera looks at)
- `up x y z`: 'up' direction in the scene, (0, 1, 0) by default

Example:

    camera
        position 0 -10 -10
        target 0 0 1
    end

## Other parameters

- `background r g b`: background color
- `ambient r g b`: ambient light
- `recursion value`: max recursion depth

Example:

    background 0.1 0.1 0.2
    ambient 0.05 0.05 0.05
    recursion 10

## [Scene file example](../scenes/default.txt)