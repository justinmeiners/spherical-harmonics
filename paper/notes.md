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


