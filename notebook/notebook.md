20210919:

Implemented the first part of the [transformations tutorial](https://learnopengl.com/Getting-started/Transformations) which introduces [`glm`](https://github.com/g-truc/glm) into the project (via vcpkg) and simply scales and rotates the object. I introduced a method on `ShaderProgram` to set a `mat4` uniform. I had to give the function a dedicated name (vs just overloading `setUniform`) because all the `glUniformMatrix*()` methods take the same value type, so it's impossible to write unique overloads for each of these.


20210918:

Made the texture unit a parameter to loadTexture().

Finished implementing the last part of the [textures tutorial](https://learnopengl.com/Getting-started/Textures) which involves loading a second texture and mixing it with the first. I adapted the Texture class to encapsulate texture units and be able to load different pixel formats. I also created an abstraction on `ShaderProgram` to assign the texture unit from a `Texture` directly into a uniform. I'm trying to hide the apparent implementation detail that texture units are set with `GL_TEXTURE0`, `GL_TEXTURE1` etc enum values when creating the textures but set in the uniforms as just `0`, `1`, etc. Some context on this in this [khronos forum post](https://community.khronos.org/t/why-use-or-define-texture-units-and-what-is-the-mapping-with-uniforms-and-texture-units/106265).

20210914:

Started the rest of the textures tutorial. Did a bit of refactoring in prep for using multiple textures with texture units. This is the start of the parameterization of the texture library that I was expecting.

20210911:

Implemented the new texture classes in the library and made the app use them. For now, a lot of the texture parameters are hardcoded in the library. Depending on what I end up needing to vary, I'll make some of this parameterized. So at this point I feel good enough about the texture
tutorial implementation so far to commit.

20210910:

Haven't had much time to work on this project this week. In writing the new textures code, I found myself needing to create a representation for texture load errors. But then I realized I'm not yet convinced I need to programmatically do anything with errors besides just printing them. So for now, I'm just going to create one Error class to use for everything and I'll do something more complex when I need it. To accomplish this, I first created the new Error class and then made the existing ShaderProgram library class use it.

20210905:

Read another good article about passing things by value in C++:
* http://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/

Finished the last part of the "Hello Triangle" tutorial, which draws a rectangle and uses an EBO to manage the indices into the vertex array.

Added the code to draw in wireframe mode but leaving it commented out.

Read through the [shaders tutorial](https://learnopengl.com/Getting-started/Shaders). It all seemed pretty straightforward and I didn't feel the need to implement it.

Moved on to the [textures tutorial](https://learnopengl.com/Getting-started/Textures) and implemented that. I was happy about how easy it was to integrate the [stb image library](https://github.com/nothings/stb/blob/master/stb_image.h) via vcpkg and to get CMake to create the textures directory in the build output. Got it working but I need to clean up a few things in the code before committing.

20210904:

Started working on [vcpkg](https://github.com/microsoft/vcpkg) integration. Kind of confused by the docs because it seems like most of the instructions set up vcpkg as a system-wide thing vs a per-project thing. Which seems baffling - surely I'd want to have distinct installs of packages per project. But it seems like the solution is to clone vcpkg as a [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules). I found this kind of buried in the docs:
>"First, download and bootstrap vcpkg itself; it can be installed anywhere, but generally we recommend using vcpkg as a submodule for CMake projects, and installing it globally for Visual Studio projects."

Set it up this way and added an instruction to the readme to init/update submodules after cloning the repo.

It was also a bit of a mystery to figure out how to record dependencies in a manifest. But it turns out vcpkg has a [manifest mode](https://vcpkg.readthedocs.io/en/latest/users/manifests/) just for that.

Got glfw installed via vcpkg now:
* Uninstalled the homebrew version I had installed
* Created a vcpkg.json manifest specifying glfw3 as a dependency
* Changed the `find_package()` call to find glfw3 the way vcpkg suggests

At first, it just wouldn't find the glfw package. I messed around trying to re-read the docs and debug but in the end, it turns out that just deleting the CMakeCache.txt file and running CMake again made it work. So now everything is good: I fixed up the instructions in the readme to remove the step to install glfw manually as this is no longer required! Now, when you build, it installs the dependencies listed in the manifest. Awesome!

...

Now that I have vcpkg working, I used it to install [tl-expected](https://github.com/TartanLlama/expected), an implementation of the `std::expected` proposal. I used this to handle errors in the shader program code without throwing exceptions. I first encountered this pattern when learning to use functional patterns in a web app and coming across [`Either<E,A> in fp-ts`](https://gcanti.github.io/fp-ts/modules/Either.ts.html). I liked it then and want to try it here. So far, I'm pretty happy with how the shader program error handling turned out.

...

Put things in appropriate namespaces.

...

Read some articles to refresh my memory on rvalue references and move semantics:
* https://stackoverflow.com/questions/270408/is-it-better-in-c-to-pass-by-value-or-pass-by-constant-reference
* https://stackoverflow.com/questions/3106110/what-is-move-semantics
* https://www.cprogramming.com/c++11/rvalue-references-and-move-semantics-in-c++11.html


20210903:

The last several days have been an adventure in getting the project to build with modern CMake. I went down this road because I anticipate needing to install some additional libraries and it seemed like doing so with [vcpkg](https://github.com/microsoft/vcpkg) would be a good way to do it (vs checking in third-party code). Vcpkg integrates well with CMake so I started looking into it and liked what I saw. Particularly, I liked that modern CMake helps enforce good modularity and keeps things working cross-platform. It also seemed like a good, widely applicable tool to have in my toolbelt.

The most useful resource I found was [this Modern CMake slide deck](https://github.com/toeb/moderncmake/blob/master/Modern%20CMake.pdf) via [this brief introduction and list of resources](https://cliutils.gitlab.io/modern-cmake/).

The point about CMake encouraging modularity really seems true. To do this right, I had to separate the app code from re-usable library code (just the shader program helpers, for now). I also had to create libraries for the generated code and the one third-party library I have checked in ([whereami](https://github.com/gpakosz/whereami)). This is all goodness, relative to what I had before.

I was able to get 99% of the way really quickly. But I ran into linking problems: it wouldn't automatically link against the system libraries GLFW requires (Cocoa, IOKit, CoreFoundation). I managed to figure out how to explicitly link those in CMake but I didn't want to hardcode knowledge of those dependencies into my CMakeLists.txt (rather, I wanted them to flow from GLFW).

I struggled with this, almost obsessively, the last few days. I ended up [posting on StackOverflow](https://stackoverflow.com/questions/69038471/whats-the-right-way-to-link-against-glfw-in-a-macos-app-with-cmake) (that post is a good writeup of what went wrong and my attempts to fix it). I learned a lot more about CMake along the way. As part of this, I ended up uninstalling the version of glfw I had built from source and instead installed via homebrew (hoping this might help).

In the end, I was able to get it to work by abandoning `pkg-config` and just relying on the built in `find_package()`. I posted this resolution as [an answer to my own question on StackOverflow](https://stackoverflow.com/a/69051875/490982).

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
