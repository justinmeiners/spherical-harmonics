# Sphereical Harmonics


The irradiance is an integral over the upper hemisphere in the direction of the normal.

Irradiance environment map -> function of the normal. In normal, out irradiance.

Spherical harmonics are a set of orthogonal functions on the sphere. Also orthonormal.

They form a set of basis functions (like fourier series) which can be summed to approximate a function.

Spherical harmonics are analogous to Fourier transforms. But, they are defined on a sphere rather than a circle. Real and complex, but we only care about real.

Prove its rotationally invariant?

The spherical harmonics are defined in terms of legendre polynomials.

### Solving Laplacian in Spherical coordinates

https://math.stackexchange.com/questions/1325594/converting-a-differential-equation-to-polar-coordinates

http://mathworld.wolfram.com/LaplacesEquation.html
> A solution to Laplace's equation has the property that the average value over a spherical surface is equal to the value at the center of the sphere


### Encoding

(Ramaoorthi)
To find the 9 lighting coeffecients you integrate the irradiance with the basis functions.

The integration is sort of like a continuous rather than discrete dot product.

### Decoding

Scale the basis functions by the coeffecients and add them together.



### Process

1. Generate a uniform distribution of random points on the sphere. 
    - use spherical coordinates
2. Monte carlo integration.
    For each point:
    1. calculate the SH value at that point
    2. calculate the incoming light value at that point
    3. multiply together
    Divide by number of pointers
3. We now have the coeffecient for that SH function.


### Cube maps

Sample Cube maps - http://www.humus.name/index.php?page=Textures

```
float2 sampleCube(
    const float3 v,
    out float faceIndex)
{
	float3 vAbs = abs(v);
	float ma;
	float2 uv;
	if(vAbs.z >= vAbs.x && vAbs.z >= vAbs.y)
	{
		faceIndex = v.z < 0.0 ? 5.0 : 4.0;
		ma = 0.5 / vAbs.z;
		uv = float2(v.z < 0.0 ? -v.x : v.x, -v.y);
	}
	else if(vAbs.y >= vAbs.x)
	{
		faceIndex = v.y < 0.0 ? 3.0 : 2.0;
		ma = 0.5 / vAbs.y;
		uv = float2(v.x, v.y < 0.0 ? -v.z : v.z);
	}
	else
	{
		faceIndex = v.x < 0.0 ? 1.0 : 0.0;
		ma = 0.5 / vAbs.x;
		uv = float2(v.x < 0.0 ? v.z : -v.z, -v.y);
	}
	return uv * ma + 0.5;
}
```

enum 
{
    CUBE_FACE_RIGHT = 0, // x+
    CUBE_FACE_LEFT,  // x-
    CUBE_FACE_TOP,   // y+
    CUBE_FACE_BOTTOM,// y-
    CUBE_FACE_BACK,  // z+
    CUBE_FACE_FRONT, // z-
    CUBE_FACE_COUNT
} CubeFace;

http://www.rorydriscoll.com/2012/01/15/cubemap-texel-solid-angle/



## Previous Intro

In realtime computer graphics, objects are usually lit by both diffuse and ambient lighting. The diffuse lighting simulates the direct radiance from nearby lights, while the ambient simulates light bounced from the surrounding scene. Since computing bounced lighting is usually too slow for realtime rendering an approximation method must be used, with the most basic being a constant color (usually a dark gray) added to the diffuse light.

![diffuse term](img/teapot_diffuse.png)
![ambient term](img/teapot_ambient.png)
![combined](img/teapot_combined.png)

Rendering algorithms which simulate some form of light bouncing from the scene are called [global illumination](https://en.wikipedia.org/wiki/Global_illumination) methods. In this paper I will explain how to implement a fast global illumination method using a mathematical tool called **spherical harmonics**. Spherical harmonics are a special set of functions that will be used to store a more complex ambient lighting signal with only 9 floats per color channel. The bounced lighting signal will the be recreated approximately in a vertex or fragment shader, using only these 9 floats.

This method was originally described by Ramaoorthi. [2] This paper is a more in depth explanation of his work.

@s Background

Global illumination is often implemented by rendering environment maps which capture light reflected from the scene.

Typically reflection probes are placed throughout the scene. At each probe, images of the surrounding enviornment are rendered to create a cubemap. At runtime, other scene objects search for the nearest probes and use its cubemaps to calculate reflections and ambient lighting.

Unfortuantly, cubemaps require a significant amount of memory. A very low resolution RGB cubemap uses $64\times64\times6\times3\approx73kb$ and a high resolution RGB cubemap uses $1024\times1024\times6\times3\approx18mb$! Even though smooth reflections, such as glass or mirrors, require such high resolution, most enviornmental lighting is low frequency, meaning changes between shades are gradual.

Since ambient light is low frequency, we can aim for a rough approximation of the overall signal, instead of crisp images. Valve's source engine accomplishes this by averaging the cubemap to a single color for each face. [1] Each of these colors is passed to the vertex shader and the surface normal is used to interpolate between values.

Valve's Ambient Cube is effecient and provides a richer ambient than a constant term, but only provides a rough approximation of ambient lighting. To store more complex ambient lighting we can use **spherical harmonics**. As we will explore, a good spherical harmonic approximation requires only 9 coeffecients to be stored, and is much richer than Valve's Ambient Cube.


@s Overview

1. Render a cubemap of the surrounding scene.
I will assume that this step is taken care of. Usually cubemaps are rendered offline, or when the scnee is loaded.
2. Calculate spherical harmonic coeffecients from the cubemap.

3. 

@s Spherical Harmonics

What are spherical harmonics?

**Definition:** Laplace's equation states that the divergence of the gradient of a function is 0. It can be written as: $$\nabla\cdot\nabla f=0$$.

In three variables Laplace's equation can be written as:

$$\frac{\partial^{2}}{\partial x^{2}}+\frac{\partial^{2}}{\partial y^{2}}+\frac{\partial^{2}}{\partial z^{2}}=0$$

**Definition:** Equations which satisfy Laplace's equation are called **harmonics**. [3]

Our objective will be to find equations which are defined on the sphere, which satisfy laplace's equation.

Laplace's equation in spherical coordinates can be written as:

$$\frac{\partial^{2}}{\partial r^{2}}+\frac{2}{r}\frac{\partial}{\partial r} + \frac{1}{r^{2}\sin^{2}(\phi)}\frac{\partial^{2}}{\partial \theta^{2}}+\frac{1}{r^{2}}\frac{\partial^{2}}{\partial \phi^{2}}+\frac{\cot(\phi)}{r^{2}}\frac{\partial}{\partial \phi}=0$$

Here is what some of these solutions look like:

![spherical harmonics](img/sh.jpg)

You may recognize these from chemistry. There spherical harmonics also describe the probablity functions of [Atomic Orbitals](https://en.wikipedia.org/wiki/Atomic_orbital).

The big idea is that we can take any function on a sphere (such as a light signal) and approximate it using sphereical harmonics. We simply calculate how much each term contributes to the overall signal and save those coeffecients. To recreate the signal we add up individual terms multiplied by the weighting coeffecients. This idea is very similar to fourier series in two dimensions.

How do we calculate how much each term contributes? The inner product gives us the component of a vector along another vector. 

let $l(x)$ be the function on the sphere and $h(x)$ be the spherical harmonic, and $S$ be the sphere surface.

$$ \int_{S} h(x)l(x) dv$$

For computational purposes we will do a discrete integral.

$$ \frac{1}{N} \sum_{i=0}^{N} h(x_{i})l(x_{i}) $$

@s Cubemap Approximation

The following structure stores the 9 spherical harmonics coeffecients for each of the red, green, and blue color channels. The total number of coeffecients is 27.

This function calculates one of the first 9 spherical harmonics at a particular point on the unit sphere. Even though they are derived from a complex formula, then end results are very simple calculations. The most complex is the 9th harmonic which is only a quadratic computation. A table of spherical harmonics can be found [https://en.wikipedia.org/wiki/Table_of_spherical_harmonics](https://en.wikipedia.org/wiki/Table_of_spherical_harmonics).

--- spherical harmonics
static inline double sh_eval_9(int i, double x, double y, double z)
{
    switch (i)
    {
        case 0:
            return 0.5 * sqrt(1.0 / M_PI);
        case 1:
            return x * 0.5 * sqrt(3.0 / M_PI);
        case 2:
            return z * 0.5 * sqrt(3.0 / M_PI);
        case 3:
            return y * 0.5 * sqrt(3.0 / M_PI);
        case 4:
            return x * z * 0.5 * sqrt(15.0 / M_PI);
        case 5:
            return y * z * 0.5 * sqrt(15.0 / M_PI);
        case 6:
            return x * y * 0.5 * sqrt(15.0 / M_PI);
        case 7:
            return (3.0 * z*z - 1.0) * 0.25 * sqrt(5.0 / M_PI);
        case 8:
            return (x*x - y*y) * 0.25 * sqrt(15.0 / M_PI);
        default:
            assert(0);
            return 0;
    }
}
--- 

For each spherical harmonic we need to calculute a coeffecient which describes how much a particular harmonic function contributes to the overall light signal. Each coeffecient is found by calculating the inner product of each function and the harmonic function.

This calculation is a surface integral over the unit sphere of the product of the light signal and the harmonic function.

$$ \int_{S} h(x)l(x) dv$$

To calculate this integral we will use a discrete summation which approximates the analytic integral. This also makes sense because the cubemap is a discrete signal as well. There are only a finite number of pixels to include.

$$ \frac{1}{N} \sum_{i=0}^{N} h(x_{i})l(x_{i}) $$

In this formula $N$, the total number of pieces to sum, is the number of pixels on the cubemap.

$$N=6w^{2}h^{2}$$
