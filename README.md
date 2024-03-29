# 473 Final Project
High Resolution Raytraced Animations.

## Build Instructions
I've only tested building on Linux machines lately. The provided CMakeLists.txt file should work sufficiently.

1. Create a Build folder in the root of the project
2. Navigate to that Build directory
3. Build with Cmake (cmake ..)
4. Run make (make)
5. Execute the executable "raytrace" (./raytrace ...)

### Program Arguments
There are several arguments you can use while running raytrace that effect the program

1. Mode: Select "render" for normal still image rendering, or "scene_1", "scene_2", or "scene_3" for rendering animated pre-defined scenes
2. File location: Input the relative file path for your desired input .pov file (relative to the executable, i.e "../resources/simple.pov")
3. Width: The width in pixels of the desired output image
4. Height: The height in pixels of the desired output image
5. Flags: There are several optional flags to choose from when rendering an image
    1. -ss=n: Super-sampling, where n is the number of anti-aliasing samples you would like to be rendered for each pixel
    2. -fresnel: A toggle for fresnel effect
    3. -beers: A toggle for beers effect
    4. -sds: A toggle for using spatial data structure representation - good for .pov files with lots of spread out objects
    5. -threads: Use this if you would like to enable multi-threading ("-threads 12" for 12 threads)
    6. -gi: A toggle for global illumination - NOTE: do not use this with -threads, as it causes slower than expected performance
    7. -p: A toggle which prints theh progress of the render in the console (current row the raytracer is rendering in pixels)


### Why I Chose my Final Project Idea
I chose High Resolution Raytraced Animations because I wanted to
1. Explore the limits of my computer's hardware
2. Produce something that visually highlights raytracing's benefits over traditional rendering techniques

Going into this final project, I knew most of my time spent was going to be getting the scenes looking right, and waiting for renders to complete. To speed up this process, I knew I needed to implement multi-threading support into my raytracer. Originally I split the threads into rendering different pixels for the same frame of the animation, but I found that having each thread rendering a seperate frame decreased overall completion time for a scene.

### Resources Used
To implement my final project, I didn't really use any external resources. 

I modified Professor Ian Dunn's multi-threaded code he provided to the class to have each thread render a seperate frame instead of a seperate pixel of the same frame.

The physical animations were based off of simple sin and cos functions (scene 1), simple kinematics equations I learned from 474 (Computer Animation taught by Christian Eckhardt) (scene 2), and simply moving the camera in the negative z direction (scene 3).

To convert the output .png files into a video, I used [Blender's](https://www.blender.org/) video sequence editor and h.264 encoder, set to a bitrate of 65000kbps (needs to be high for 4k 60fps). I chose my settings by just messing around in Blender until the output video looked about right.

### Software Design Implementation
For my software design implementation, I backtraced through my code and removed redundant functions, unused variables, and commented-out print statements that haven't been used since the early stages of developing this program. Additionally, I consider the multi-threaded approach a new Software Design Implementation, as it's not necessary for my final project, but extremely useful to speed up rendering times. I also implemented copy constructors for all objects, and for scenes (which hold the objects and other scene informaiton).

### Other Thoughts and Comments
Since I had different frames rendering on seperate threads when the -threads flag is set, the frames of my animation are no longer rendered sequentially. Therefore, I couldn't simply modify objects in a scene based on their previous position. Instead, the position for each object has to be calculated from the initial scene positions, based on the current frame. This means (for some scenes) I needed to loop through all previous frames until the current frame, and increment the objects position per frame. Then I store these positions in a new copy of the original scene, and render a frame based on the copy. For the three scenes I rendered for this final project, this process isn't overly computationally expensive, although for longer and more complex scenes, I could see this being an issue.

## Scene 1
Scene 1 involves a light source orbiting around a sphere in the center of a room. I thought the shadows produced look interesting, and this scene can be looped infinitely since the ending position is 1 frame away from the initial position.

I rendered this scene with the command "./raytrace scene_1 ../resources/final_scene_1.pov 3840 2160 -threads 12"

![Screenshot](/resources/scene_1_frame_0.png)

[Link to video](https://youtu.be/Zv_WpVO17zw)

## Scene 2
Scene 2 involves a red sphere being affected by gravity, and having a slight negative x initial velocity. When the sphere hits the floor, it bounces back upwards with a slightly lower velocity, causing it to not bounce as high.

I rendered this scene with the command "./raytrace scene_2 ../resources/final_scene_2.pov 3840 2160 -threads 12 -ss=2"

![Screenshot](/resources/scene_2_frame_0.png)

[Link to video](https://youtu.be/r8AeDB4mtH0)

## Scene 3
Scene 3 depicts a camera moving forwards in a hallway with blue and red lights. These lights are generated in my code, instead of being present in the .pov file. There is a variable in Globals.h which can be edited to change the number of lights in the scene (more lights = slower rendering).

I rendered this scene with the command "./raytrace scene_3 ../resources/final_scene_3.pov 3840 2160 -threads 12"

![Screenshot](/resources/scene_3_frame_0.png)

[Link to video](https://youtu.be/xOq6kjXYFsg)