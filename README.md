# VecTooDee
A single header vector math library designed to be used as a bridge between 2D vector implementations <br>
Goals of this library:
- lightweight
- easy to use
- minimal overhead
- seamless conversion between any 2D vector implementation

# Why does this library exist?
When I was trying out different game engines and corresponding physics engines in 
C++ I noticed that all of them had different implementations for the same thing:

    struct 2DVector
    {
        Foo x;
        Foo y;
        
        // and a bunch of member functions for vector math
    };

I was switching libraries quite a bit and always spent time 
writing some sort of conversion code between the multiple implementations of 2D vectors.
Long story short I was getting annoyed because I had to reimplement an adapter class every time. <br>
Because of that I made this library and implemented it in a single header file, 
so it is quickly and easily imported into any project. <br>
Some notable libraries whose coexistence in my code inspired me to create this and that follow the pattern:
- SFML (sf::Vector2) 
- Box2D (b2Vec2)
- Raylib (Vector2)
- Chipmunk2D (cpVect)

# Basic Usage
This library can convert any 2D vector implementation that follows the pattern shown above into another one.
```C++
    struct Some2DVector {int x, float y};    
    struct anotherVector {double x, long y};

    Some2DVector someVector;
    Another2DVector anotherVector;

    Some2DVector convertedVector = vectoo::convert<someVector>(anotherVector);
    Some2DVector combinedVector = vectoo::add(someVector, anotherVector);
```
Additionally, the "vectoo" namespace provides many basic and some more specialized math functions, 
that might be useful to someone (they were for me, which is why they're here). <br>
Please note that actually converting vectors of different types into one another does nothing more than cast the values. <br>
Therefore, the behaviour is the same as when casting the x and y components of one vector into those of the other. 

If for some reason two vector implementations aren't enough for you, I included my own in the library as well:
```C++
    // There are shortcuts for common vector types. vec2f -> float, vec2i -> int, vec2u -> unsigned int
    vec2f anotherFloatVector;

    // The template will accept any arithmetic type and even another vector implementation to match
    vec2<float> floatVector;

    // This will set x and y to match the ones from the provided vector implementation
    vec2<Some2DVector> inheritedTypeVector;
    
    // And it also has multiple constructors, that will take all expected arguments including a different vector
    vec2<Some2DVector> convertedVector = vec2(someVector);

    // If not disabled it will also inherit all functions from the vector provided via template or constructor
    convertedVector.some2DVectorFunction();

    // When the corresponding macro is defined, it will implicitly convert to any other vector and back easily
    someVector = vec2(anotherVector)

    // And of course it will provide the basic operators that you would expect (+,-,*,/,+=,-=,*=,/=)
    auto combinedVector = convertedVector + inheritedTypeVector;
    auto scaledVector = combinedVector * 2;
```
# Configuration options
While it will function in its entirety without configuration, 
the library does include some preprocessor macros that can be used,
either for compatibility or for tweaking its functionality:

| **Preprocessor Option**        | **Description**                                                                          |
|--------------------------------|------------------------------------------------------------------------------------------|
| **USE_DOUBLE_PRECISION**       | The floating point return values of functions will be double precision floats            |
| **WITHOUT_VECTOR_INHERITANCE** | The vec2 struct will no longer be a child class of the vector struct it was created from |
| **ALLOW_IMPLICIT_CONVERSION**  | The vec2 struct will convert to any other vector with implicit conversion.               |
| **WITHOUT_OPERATORS**          | The vec2 struct will no longer provide any operators                                     |
| **WITHOUT_MEMBER_FUNCTIONS**   | Removes all member functions from the vec2 struct (except for operators)                 |
| **WITHOUT_VECTOR_FUNCTIONS**   | This will remove the vectoo namespace and all functions within it                        |

These can be either passed as -D parameter during compilation or be '#define'-d before including the header file.

# TODOs
- Better support for integer vectors (both signed and unsigned)
- More options for defining the exact type of floats that are to be used
- More uniform naming scheme for functions
- Support for 3D vectors at some point (although then I'll have to think of a new name)
