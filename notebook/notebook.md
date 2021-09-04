20210903:

The last several days have been an adventure in getting the project to build with modern CMake. I went down this road because I anticipate needing to install some additional libraries and it seemed like doing so with [vcpkg](https://github.com/microsoft/vcpkg) would be a good way to do it (vs checking in third-party code). Vcpkg integrates well with CMake so I started looking into it and liked what I saw. Particularly, I liked that modern CMake helps enforce good modularity and keeps things working cross-platform. It also seemed like a good, widely applicable tool to have in my toolbelt.

The most useful resource I found was [this Modern CMake slide deck](https://github.com/toeb/moderncmake/blob/master/Modern%20CMake.pdf) via [this brief introduction and list of resources](https://cliutils.gitlab.io/modern-cmake/).

The point about CMake encouraging modularity really seems true. To do this right, I had to separate the app code from re-usable library code (just the shader program helpers, for now). I also had to create libraries for the generated code and the one third-party library I have checked in ([whereami](https://github.com/gpakosz/whereami)). This is all goodness, relative to what I had before.

I was able to get 99% of the way really quickly. But I ran into linking problems: it wouldn't automatically link against the system libraries GLFW requires (Cocoa, IOKit, CoreFoundation). I managed to figure out how to explicitly link those in CMake but I didn't want to hardcode knowledge of those dependencies into my CMakeLists.txt (rather, I wanted them to flow from GLFW).

I struggled with this, almost obsessively, the last few days. I ended up [posting on StackOverflow](https://stackoverflow.com/questions/69038471/whats-the-right-way-to-link-against-glfw-in-a-macos-app-with-cmake) (that post is a good writeup of what went wrong and my attempts to fix it). I learned a lot more about CMake along the way. In the end, I was able to get it to work by abandoning `pkg-config` and just relying on the built in `find_package()`. I posted this resolution as [an answer to my own question on StackOverflow](https://stackoverflow.com/a/69051875/490982).

After figuring it out in my simple repro, I ported the fix back here and got everything cleaned up. I also installed the CMake and CMake Tools VSCode extensions and got the build and debug tasks set up.

20210831:

Got the basic ShaderProgram class written and the main program using it. Also made it so that it can find the shader files in the same directory as the executable, regardless of where you run it from (previously, you had to run it from the bin directory for it to find the shader files).

To make this work, I had to use the [whereami](https://github.com/gpakosz/whereami) library to find the path of the current executable (seemingly, this is hard to do in C++ in a platform neutral way without this library). This is my first third-party library and I'm just checking it in because there appears to be no package manager support for it.

I also discovered [how to get VSCode to understand C++17 syntax](https://stackoverflow.com/a/66251689). I needed this to get the `std::filesystem` references to work but also now I can use `auto`.

20210830:

Started writing the ShaderProgram class. Basing it on the implementation from https://learnopengl.com/Getting-started/Shaders but trying to improve the code.

20210829:

Yesterday I noticed that though the triangle is originally drawn in the wrong position, as soon as you move the window it fixes itself.

<p align="center">
  <img alt="Video showing the triangle in the wrong position and centering itself when the window is moved." src="assets/triangle-wrong-size-until-moved.gif" />
</p>

I had vaguely remembered reading something about problems on MacOS until you move the window, and that's what inspired me to try this.

I then spent a lot of time googling for solutions. Some posts that looked promising:

* https://stackoverflow.com/questions/59405118/triangle-draws-in-wrong-place-before-window-resize
* https://bleepcoder.com/glfw/364324977/cannot-draw-any-content-initially-on-macos-10-14-until

One of the comments pointed to [this commit](https://github.com/kovidgoyal/kitty/commit/b82e74f99ae30599cd88ef6a1216625b57a0414a) which fixes it by calling update on the underlying `NSOpenGLContext` a few times. I started down the road of trying to do that, by integrating one Objective-C compilation unit that exposes a C function to do the `[NSOpenGLContext update]`. I only briefly tried but ran into some build problems that I didn't have time to dig into last night.

It did occur to me that some of the posts I found mentioned that the problems were happening on Mojave. I was on Catalina but was overdue for an upgrade to Big Sur, so tried that in the hopes it would fix it. Nope.

But, I did notice that when you start the app on the external monitor i.e. not the Mac's retina display, the problem doesn't happen. That clued me into the idea that it's a problem with the retina display doing something funky with the coordinates. Googling yielded [this Stack Overflow post](https://stackoverflow.com/questions/36672935/why-retina-screen-coordinate-value-is-twice-the-value-of-pixel-value) which, I think, describes the solution I need.

...

Tried it and it works! The fix basically came down to this change, in the place where I initially call `glViewport()`:

```cpp
-  glViewport(0, 0, 1024, 768);
+  GLsizei width;
+  GLsizei height;
+
+  glfwGetFramebufferSize(window, &width, &height);
+  glViewport(0, 0, width, height);
```
With this, the app starts and draws the triangle in the right place, on both the external monitor and the retina display. And you can move the window between them and things stay fine.

I was wondering whether I also needed to call `glfwGetFramebufferSize()` in the framebuffer size callback, or whether the `width` and `height` params there are correct. Checked this by adding a debug print statement in the callback and moving the window between the retina screen and the external monitor:

```cpp
 void framebuffer_size_callback(
     __attribute__((unused)) GLFWwindow* _window, int width, int height) {
+  cout << "in framebuffer_size_callback: width is " << width << " height is " << height << endl;
   glViewport(0, 0, width, height);
 }
```

Ran it starting on the retina screen, then moved it to the external monitor and saw this output:

```
in framebuffer_size_callback: width is 1024 height is 768
```

Moved it back to the retina and saw:

```
in framebuffer_size_callback: width is 2048 height is 1536
```

OK, so I think the framebuffer size callback is good and overall this problem is fixed!

20210827:

Meta: created this notebook file and added the notes from yesterday and the day before.

Implemented the code from the [Hello Triangle tutorial](https://learnopengl.com/Getting-started/Hello-Triangle) that gets a triangle to draw on the screen. It works but:
* The triangle isn't centered, like shown in the tutorial screenshot.
* It only works if you run the binary from the bin/ directory because it only knows how to load the shaders from the cwd.
* There are lots of TODO comments for things missing.

20210826:

All the work yesterday was not in a git repo. I created this repo, moved the code in, created a .gitignore, LICENSE, and README file.

Added the dev setup instructions to the README.

20210825:

Started the project, loosely inspired by [this video in which a guy created a Minecraft "clone" in C in 48 hours](https://youtu.be/4O0_-1NaWnY).

Goal for day 1 was just to get a window on the screen. Got the initial code set up following a mix of:
* [The GLFW Getting Started tutorial](https://www.glfw.org/docs/latest/quick.html)
* [Anton Gerdelan's "Hello Triangle" tutorial](https://antongerdelan.net/opengl/hellotriangle.html)
* [The GitHub repo of the Minecraft in C project](https://github.com/jdah/minecraft-weekend)

Got GLFW building and installed (see directions in the project's [README](README.md)).

Generated GLAD files using the [web interface](https://glad.dav1d.de/#profile=core&language=c&specification=gl&loader=on&api=gl%3D3.3), specifying:
* Language = C/C++
* Specification = OpenGL
* API gl = 3.3
* Profile = Core
* "Generate a loader" option checked

Played around with how to get things set up to use VSCode as my IDE. Started by trying to define a build task in [tasks.json](.vscode/tasks.json) but found this was pretty limiting (I couldn't e.g. get the command to run `pkg-config` in a sub-shell to get the include/lib paths for GLFW dynamically). So based on what I saw in [this template for a VSCode project that works with GLFW on GitHub](https://github.com/markfaction/VSCode-CPlusGLFW/blob/master/.vscode/tasks.json), I just created a regular old Makefile and made VSCode invoke that.

Took some tinkering and muscle memory to get the makefile figured out. One issue I ran into is that the generated glad source file is a .c file and compiling it with the C++ compiler generated a warning. So I had to split out the compiler for the C file as `clang` and for C++ files as `clang++` in the makefile. Not sure if there's a more elegant way to do this.

After getting it to build clean, it would run and a beachball would spin, but the window wouldn't appear on the screen. By going back to some old OpenGL code I'd written (back in 2016) that did work, by trial and error I found that the thing that makes it work is to add the [`glfwPollEvents()`](https://github.com/spather/no_engine_game/commit/cdedc1541f1a88661aad9cf9a5523f91eb338144#diff-34d21af3c614ea3cee120df276c9c4ae95053830d7f1d3deaf009a4625409ad2R40) call in the main loop.

With that, I now have the code to open a window!