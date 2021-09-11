// By defining STB_IMAGE_IMPLEMENTATION and then including stb_image.h
// this ends up becoming the implementation of stb. Doing it once here
// in this dedicated cpp file so that we can be sure it's linked only
// once (vs doing this in other random .cpp files and potentially doing
// it more than once).
//
// Defining STBI_FAILURE_USERMSG results in user-friendlier messages
// from stbi_failure_reason(). E.g. without defining this, if it
// can't open a file, stbi_failure_reason() returns "can't fopen".
// With it defined you get "Unable to open file".

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>
