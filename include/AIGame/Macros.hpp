#ifndef AI_MACROS_HPP
#define AI_MACROS_HPP

// PI, 'tis pretty important for stuffs.
#ifndef M_PI
	#define M_PI 3.14159265359f
#endif

// A macro for making classe uncopyable
// http://stackoverflow.com/questions/2173746/how-do-i-make-this-c-object-non-copyable
#define UNCOPYABLE(TypeName) \
	TypeName(const TypeName&); \
	void operator=(const TypeName&);

// Stolen from here https://gist.github.com/dbrockman/4773781
// DEG to RAD
#define DEGTORAD(deg) ((float)(deg * M_PI / 180.0f))

// RAD to DEG
#define RADTODEG(rad) ((float)(rad * 180.0f / M_PI))

#endif