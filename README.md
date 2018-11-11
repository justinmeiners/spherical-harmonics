# Spherical Harmonics

This project shows how to implement spherical harmonics for precalculating lighting in 3D environments. Spherical harmonics are primarily useful in graphics for compactly storing a rough approximation of irradiance. This method can approximate a cube map with just 9 floating point numbers. 

I creates this project for my mathematics capstone course. But, I ended up doing more programming than math. 

The demo is a [Literate Program](https://en.wikipedia.org/wiki/Literate_programming). Thanks to the creator of [Literate](https://github.com/zyedidia/Literate)

## Includes

- `demo/` - Contains the generated C source code for the actual demo program. Requires SDL2 to build.

- `literate/` - Commentary on the source code and a practical overview of spherical harmonics. I started this for my paper, but it was too much of an engineering projects, and not enough math. I then switched to the approach in the papers folder. The commentary here is very incomplete and unpolished, but they are still useful.

- `paper/functions.tex` - This paper introduces the theory of orthogonal systems and generalized fourier series from a mathematical point of view. There is no original research here. I wrote it for the purpose of understanding how Spherical Harmonics work. 

- `paper/laplace.tex` - This paper presents an organized way to transform Laplace's equation into spherical coordinates. Many differential equations textbooks mention that this can be done, but none I have seen actually show how to do it.


## Screenshots

Perfect reflection of cubemap.
![cubemap reflection](screenshots/cubemap.png)

Spherical harmonics approximation.
![spherical harmonics approximation](screenshots/sh.png)

## Further Reading

1. [An Effecient Representation for Irradiance Environment Maps](https://graphics.stanford.edu/papers/envmap/)
2. [Shading in Valve's Source Engine](http://www.valvesoftware.com/publications/2006/SIGGRAPH06_Course_ShadingInValvesSourceEngine.pdf)

## Project License

MIT License

Copyright (c) 2017 Justin Meiners

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
