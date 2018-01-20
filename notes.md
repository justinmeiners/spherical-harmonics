# Sphereical Harmonics


The irradiance is an integral over the upper hemisphere in the direction of the normal.

Irradiance environment map -> function of the normal. In normal, out irradiance.

Spherical harmonics are a set of orthogonal functions on the sphere. Also orthonormal.

They form a set of basis functions (like fourier series) which can be summed to approximate a function.

Spherical harmonics are analogous to Fourier transforms. But, they are defined on a sphere rather than a circle. Real and complex, but we only care about real.

Prove its rotationally invariant?

The spherical harmonics are defined in terms of legendre polynomials.


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



Sample Cube maps - http://www.humus.name/index.php?page=Textures


