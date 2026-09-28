# graphics-3d-engine

A software 3D graphics engine built from scratch in C++ to develop a low-level understanding of how 3D rendering actually works. Vertices are transformed, projected, and rasterized by hand. SDL2 is used only to open a window and set pixels/lines on screen.

## Tech stack

- **Language:** C++17
- **Build system:** Make
- **Compiler:** g++
- **Windowing / input:** SDL2
- **Testing:** Google Test

## Getting started

```bash
make          # builds build/engine
make run      # builds and runs the demo
make test     # builds and runs the Google Test suite
make clean    # removes the build directory
```

**Controls** (during the demo):

| Key | Effect |
|---|---|
| `W` / `S` | Move camera forward / backward |
| `A` / `D` | Move camera left / right (strafe) |
| `Up` / `Down` | Move camera up / down |
| `Left` / `Right` | Yaw the camera (look left / right) |

## Project structure

```
include/            Public headers
  math/              Vec2, Vec3, Vec4, Matrix4x4
  Triangle.hpp        A single 3-point face + normal + color
  Mesh.hpp            A collection of triangles, .obj loading, depth sort
  Color.hpp           RGBA color + shading helpers
  Window.hpp          SDL2 window/renderer wrapper
src/                 Implementations mirroring include/
tests/               Google Test unit tests
objects/             .obj model files (e.g. cube.obj)
```

## The rendering pipeline (so far)

Everything happens in software, once per frame, inside the loop in `src/main.cpp`. Each triangle of the mesh flows through the following stages:

```
.obj file
   │  parse vertices & faces
   ▼
Model space (local mesh coordinates)
   │  rotation matrix
   ▼
World space (after rotation + translation)
   │  translate relative to camera
   ▼
Camera/view space
   │  back-face cull using the face normal
   │  clip against the near plane
   │  projection matrix
   ▼
Clip space (x, y, z, w)
   │  perspective divide (÷ w)
   ▼
Normalized device coordinates
   │  shift + scale to pixels
   ▼
Screen space
   │  painter's algorithm (depth sort)
   │  rasterize (wireframe / filled)
   ▼
Pixels on screen
```

### 1. Loading a mesh (`Mesh::loadFromObjectFile`)

A minimal Wavefront `.obj` parser reads:
- `v x y z` lines into vertex positions, stored as `Vec4(x, y, z, 1)`
- `f a b c` lines, which reference three vertices (1-indexed) and become a `Triangle`

Every point in the engine is a `Vec4` — a **homogeneous coordinate** `(x, y, z, w)` — rather than a plain 3-vector. This allows translation, rotation, and perspective projection all be expressed as a single 4×4 matrix multiplication:

- **Points** (positions) use `w = 1`
- **Directions** (like normals) use `w = 0`, so translations don't affect them

### 2. Rotation (model to world)

Each frame, `angle` is advanced slightly and a rotation matrix is rebuilt from it. The engine currently animates the cube by combining two elementary rotations: a full-rate spin around one axis and a half-rate tumble around a second axis. The elementary rotation matrices this is built from are the standard ones:

Rotation about the X axis by angle *θ*:

$$
R_x(\theta) =
\begin{bmatrix}
1 & 0 & 0 & 0 \\
0 & \cos\theta & -\sin\theta & 0 \\
0 & \sin\theta & \cos\theta & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

Rotation about the Y axis:

$$
R_y(\theta) =
\begin{bmatrix}
\cos\theta & 0 & \sin\theta & 0 \\
0 & 1 & 0 & 0 \\
-\sin\theta & 0 & \cos\theta & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

Rotation about the Z axis:

$$
R_z(\theta) =
\begin{bmatrix}
\cos\theta & -\sin\theta & 0 & 0 \\
\sin\theta & \cos\theta & 0 & 0 \\
0 & 0 & 1 & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

`Matrix4x4::transformVector` applies a matrix to a vector: each output component is the dot product of a matrix row with the input vector, `v' = M·v`. `Matrix4x4::transformTriangle` just applies this to all three points of a triangle.

### 3. Translation (world space)

A translation matrix pushes the mesh away from the origin so it's in front of the camera:

$$
T =
\begin{bmatrix}
1 & 0 & 0 & t_x \\
0 & 1 & 0 & t_y \\
0 & 0 & 1 & t_z \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

Because points carry `w = 1`, the last column of the matrix adds `(t_x, t_y, t_z)` straight into the result.

### 4. Camera / view space

The camera's position is tracked as a `Vec4` and moved by WASD/arrow input. Each triangle's world-space points are first shifted into camera-relative space by simple vector subtraction, `p_view = p_world - camera`.

The camera also has an orientation, built each frame from a yaw angle controlled by the left/right arrow keys:

- `cameraForward` is derived directly from `yawAngle`: `(sin(yaw), 0, cos(yaw))`
- `cameraRight = worldUp × cameraForward`
- `cameraUp = cameraForward × cameraRight`

These three vectors form a rotation basis. Packing them as the rows of a matrix and multiplying it by `p_view` rotates points into the camera's local axes, so that "forward" always corresponds to `+Z` regardless of which way the camera is currently looking:

$$
R_{\text{camera}} =
\begin{bmatrix}
\text{right}_x & \text{right}_y & \text{right}_z & 0 \\
\text{up}_x & \text{up}_y & \text{up}_z & 0 \\
\text{forward}_x & \text{forward}_y & \text{forward}_z & 0 \\
0 & 0 & 0 & 1
\end{bmatrix}
$$

### 5. Back-face culling

A triangle's **normal** is the unit vector perpendicular to its surface, computed from the cross product of two of its edges:

$$
\vec{n} = \frac{(\vec{p_2} - \vec{p_1}) \times (\vec{p_3} - \vec{p_1})}{\lVert (\vec{p_2} - \vec{p_1}) \times (\vec{p_3} - \vec{p_1}) \rVert}
$$

The cross product of two vectors $\vec{a} = (a_x,a_y,a_z)$ and $\vec{b} = (b_x,b_y,b_z)$:

$$
\vec{a} \times \vec{b} = \left(a_y b_z - a_z b_y,\ \ a_z b_x - a_x b_z,\ \ a_x b_y - a_y b_x\right)
$$

To decide whether a face is pointing toward or away from the camera, the engine takes the **dot product** of the normal with the vector from the camera to the triangle. The dot product of two vectors relates to the cosine of the angle between them:

$$
\vec{a} \cdot \vec{b} = a_x b_x + a_y b_y + a_z b_z = \lVert \vec{a} \rVert \lVert \vec{b} \rVert \cos\theta
$$

- If the angle is less than 90° (dot product positive), the face points toward the camera → visible.
- If the angle is 90° or more (dot product ≤ 0), the face points away, thus it can be skipped, since we'd otherwise be looking at the inside of the mesh.

### 6. Near-plane clipping

Perspective divide breaks down for points behind (or exactly at) the camera, since dividing by a negative or zero `w` sends them to nonsensical or infinite screen coordinates. Before projecting, each surviving triangle is tested against the near plane (`Triangle::isBehindNearPlane`) and, if it straddles it, clipped so that everything passed downstream has `z ≥ near`:

- **All 3 points behind the near plane** → the triangle is discarded entirely.
- **1 point inside, 2 outside** → the two edges leaving the inside point are intersected with the near plane, producing one smaller triangle.
- **2 points inside, 1 outside** → the two edges crossing the near plane are intersected with it, producing a quad (re-triangulated as two triangles) that covers the surviving inside region.

Each intersection point is found by walking the edge from the inside point `A` toward the outside point `B` and solving for the parameter `t` at which `z` reaches `near`:

$$
t = \frac{\text{near} - A_z}{B_z - A_z}, \qquad \text{intersection} = A + t \cdot (B - A)
$$


### 7. Projection (view to clip space)

A **perspective projection matrix** converts camera-space coordinates into clip space, so that farther-away objects appear smaller. It's built from the field of view and the near/far clip planes:

$$
f = \frac{1}{\tan(\text{fov}/2)} \qquad q = \frac{\text{far}}{\text{far} - \text{near}}
$$

$$
P =
\begin{bmatrix}
f / \text{aspect} & 0 & 0 & 0 \\
0 & f & 0 & 0 \\
0 & 0 & q & -\text{near} \cdot q \\
0 & 0 & 1 & 0
\end{bmatrix}
$$

Applying `P` to a camera-space point `(x, y, z, 1)` gives:

- `x' = f/aspect · x`, `y' = f · y` — scales by field of view and corrects for the window's aspect ratio
- `z' = q·z − near·q` — remaps depth into the `[0, 1]` range used later for depth sorting
- `w' = z` — critically, the **output w becomes the input's camera-space depth**. This is what makes perspective (rather than orthographic) projection work.

### 8. Perspective divide

Dividing every component by `w` (`Vec4::perspectiveDivide`) turns clip space into **normalized device coordinates**. Since `w = z_camera`, this division is what makes distant geometry shrink toward the center of the screen — the defining property of perspective:

$$
(x, y, z, w) \rightarrow \left(\frac{x}{w}, \frac{y}{w}, \frac{z}{w}, 1\right)
$$

### 9. Viewport transform

NDC coordinates are roughly in `[-1, 1]`. To draw them, they're shifted into `[0, 2]` and scaled by half the screen dimensions to land in pixel space:

```
screen_x = (ndc_x + 1) * 0.5 * width
screen_y = (ndc_y + 1) * 0.5 * height
```

### 10. Depth sorting — the painter's algorithm

Before rasterizing, `Mesh::sortMesh` sorts all triangles back-to-front by the average (midpoint) of their Z coordinates, and they're drawn in that order:

$$
z_{\text{mid}} = \frac{z_1 + z_2 + z_3}{3}
$$

This is the **painter's algorithm**: like a painter working back-to-front, triangles farther from the camera are drawn first so nearer triangles naturally overwrite them. 

### 11. Shading

Currently a simple grayscale shade is derived per-triangle from `dot(normal, camera)` (`Color::getColor`), giving faces facing the camera a brighter value than faces at a grazing angle — a first step toward real lighting. 

### 12. Rasterization

Two rasterizers exist in `Window`:
- `drawFilledTriangle` — fills the triangle using SDL's `SDL_RenderGeometry`, shaded per-triangle. This is what the demo currently uses.
- `drawTriangle` — draws the triangle's three edges as lines (wireframe).

## What's implemented 

- `.obj` mesh loading
- Homogeneous-coordinate vector/matrix math (`Vec4`, `Matrix4x4`)
- Clean `Vec4` math API — `+`, `-`, and `*` (scalar and component-wise) operator overloads, plus static `Vec4::cross(a, b)` / `Vec4::dot(a, b)` functions
- Model rotation, world translation, camera-relative view transform
- Camera movement (WASD + up/down) and yaw look-around (left/right arrows) via a rotating camera basis
- Back-face culling
- Near-plane clipping (1-in/2-out and 2-in/1-out cases), though currently with 2 known bugs
- Perspective projection + perspective divide + viewport mapping
- Normal vector calculation per triangle
- Painter's-algorithm depth sorting
- Basic per-triangle grayscale shading
- Filled-triangle rasterization (currently used by the demo); wireframe rasterizer also available


## Testing

Unit tests (Google Test) cover the math and model layers: `Vec2`, `Vec3`, `Vec4`, `Matrix4x4`, `Triangle`, `Mesh`, and `Color`. Run them with `make test`.
