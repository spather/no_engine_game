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
