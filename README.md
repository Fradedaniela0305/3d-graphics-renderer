# graphics-3d-renderer

An educational 3D renderer in C++. Mesh loading, transforms, culling, near-plane clipping, projection, and flat lighting are implemented on the CPU. SDL2 provides windowing, input, and triangle drawing through its accelerated renderer. The project does not implement a custom triangle rasterizer or GPU shaders.


## Utah Teapot

https://github.com/user-attachments/assets/edb9cf97-86b5-4140-b3ee-15ddb8a276f3

## Camera movement
https://github.com/user-attachments/assets/23e432a2-e8db-4388-93b1-ef98656cf995


## Tech stack

- **Language:** C++17
- **Build system:** Make
- **Compiler:** g++
- **Windowing / input / drawing:** SDL2
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
| `W` / `S` | Both currently move the camera forward (backward movement has a sign bug) |
| `A` / `D` | Move camera left / right (strafe) |
| `Up` / `Down` | Move camera up / down |
| `Left` / `Right` | Yaw the camera (look left / right) |

## Project structure

```
include/            Public headers
  math/              Vec4, Matrix4x4
  Triangle.hpp        A single 3-point face + normal + color
  Mesh.hpp            A collection of triangles, .obj loading, depth sort
  Color.hpp           RGBA color + shading helpers
  Window.hpp          SDL2 window/renderer lifecycle
  Renderer.hpp        SDL2 drawing wrapper
  Input.hpp           Keyboard and mouse state
  Clock.hpp           Frame timer (not yet used by the demo)
src/                 Implementations mirroring include/
tests/               Google Test unit tests
objects/             .obj model files (e.g. cube.obj)
```

## The rendering pipeline (so far)

The mesh is loaded before the frame loop. Each frame, `src/main.cpp` processes its triangles on the CPU and submits them to SDL for drawing:

```
.obj file
   │  parse vertices & faces
   ▼
Model space (local mesh coordinates)
   │  translation matrix
   ▼
World space
   │  translate relative to camera, then rotate into camera basis
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
   │  SDL triangle drawing (wireframe / filled)
   ▼
Pixels on screen
```

### 1. Loading a mesh (`Mesh::loadFromObjectFile`)

A minimal Wavefront `.obj` parser reads:
- `v x y z` lines into vertex positions, stored as `Vec4(x, y, z, 1)`
- `f a b c` lines, which reference three vertices (1-indexed) and become a `Triangle`

Face tokens may include texture/normal indices, but only the position index is used. The loader reads only the first three vertices of each face; it does not triangulate polygons or support negative indices, and malformed input is not robustly handled.

Mesh vertices use `Vec4`, a **homogeneous coordinate** `(x, y, z, w)`. Translation, rotation, and perspective projection can each be expressed as a 4×4 matrix multiplication. The usual convention is:

- **Points** (positions) use `w = 1`
- **Directions** (like normals) use `w = 0`, so translations don't affect them

### 2. Matrix transforms

`Matrix4x4::transformVector` applies a matrix to a vector, `v' = M·v`. `Triangle::transformedBy` applies that transform to all three vertices. The demo constructs a rotation matrix, but its application is currently commented out, so the model is only translated.

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

- With the current winding convention, a negative dot product indicates a front-facing triangle.
- A nonnegative result is culled.

`Vec4::dot` actually computes a normalized four-component dot product. Because the normal has `w = 0`, its sign matches the XYZ dot product used for this culling decision.

### 6. Near-plane clipping

Perspective divide breaks down for points behind (or exactly at) the camera, since dividing by a negative or zero `w` sends them to nonsensical or infinite screen coordinates. Before projecting, each surviving triangle is tested against the near plane (`Triangle::isBehindNearPlane`) and, if it straddles it, clipped so that everything passed downstream has `z ≥ near`:

- **All 3 points behind the near plane** → the triangle is discarded entirely.
- **1 point inside, 2 outside** → the two edges leaving the inside point are intersected with the near plane, producing one smaller triangle.
- **2 points inside, 1 outside** → the two edges crossing the near plane are intersected with it, producing a quad (re-triangulated as two triangles) that covers the surviving inside region.

Each intersection point is found by walking the edge from the inside point `A` toward the outside point `B` and solving for the parameter `t` at which `z` reaches `near`:

$$
t = \frac{\text{near} - A_z}{B_z - A_z}, \qquad \text{intersection} = A + t \cdot (B - A)
$$


Clipping can reverse vertex winding in some cases, affecting the recomputed normals and lighting. Far-plane and side-plane clipping are not implemented.

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

- `x' = f/aspect · x`, `y' = f · y` —> scales by field of view and corrects for the window's aspect ratio
- `z' = q·z − near·q` — produces clip-space depth; after division by `w`, depths between near and far map to `[0, 1]`
- `w' = z`-> the **output w becomes the input's camera-space depth**.

### 8. Perspective divide

Dividing XYZ by `w` (`Vec4::perspectiveDivide`) produces **normalized device coordinates** for those components. The implementation retains the original `w` rather than setting it to 1. Since `w = z_camera`, this division is what makes distant geometry shrink toward the center of the screen:

$$
(x, y, z, w) \rightarrow \left(\frac{x}{w}, \frac{y}{w}, \frac{z}{w}, w\right)
$$

### 9. Viewport transform

The visible NDC X/Y range is `[-1, 1]`. The viewport transform maps it to pixels and flips Y for screen coordinates; offscreen vertices can lie outside that range:

```
screen_x = (ndc_x + 1) * 0.5 * width
screen_y = (1 - ndc_y) * 0.5 * height
```

### 10. Depth sorting — the painter's algorithm

Before rasterizing, `Mesh::sortMesh` sorts all triangles back-to-front by the average of their post-perspective-divide Z coordinates, and they're drawn in that order:

$$
z_{\text{mid}} = \frac{z_1 + z_2 + z_3}{3}
$$

This is the **painter's algorithm**: triangles with larger average depth are drawn first. This approximation can produce incorrect visibility for intersecting triangles or cyclic overlap. There is no per-pixel depth buffer.

### 11. Shading

A directional light is rotated into view space to match the face normals. The normalized dot product of the normal and light direction is clamped to `[0, 1]` and converted to grayscale by `Color::getColor`. Each triangle receives one flat color; lighting is computed on the CPU.

### 12. SDL drawing

`Renderer` provides two triangle drawing methods:
- `drawFilledTriangle` — fills the triangle using SDL's `SDL_RenderGeometry`, shaded per-triangle. This is what the demo currently uses.
- `drawTriangle` — draws the triangle's three edges as lines (wireframe).

## What's implemented

- Limited triangular `.obj` mesh loading
- Homogeneous-coordinate vector/matrix math (`Vec4`, `Matrix4x4`)
- `Vec4` math operations — `+`, `-`, and `*` (scalar and component-wise) operator overloads, plus static `Vec4::cross(a, b)` / `Vec4::dot(a, b)` functions
- World translation and camera-relative view transform; model rotation code is currently inactive
- Camera translation and yaw via a rotating camera basis, with the backward-movement bug noted above
- Back-face culling
- Near-plane clipping (1-in/2-out and 2-in/1-out cases), with winding limitations noted above
- Perspective projection + perspective divide + viewport mapping
- Normal vector calculation per triangle
- Painter's-algorithm depth sorting
- Basic per-triangle grayscale shading
- SDL filled-triangle drawing (used by the demo) and wireframe drawing

Movement uses fixed per-frame increments, so its speed depends on frame rate.


## Testing

Google Test tests exercise `Vec4`, `Matrix4x4`, `Triangle`, `Mesh`, and `Color`. Run them with `make test`. The suite does not test the full rendering pipeline or clipping output. The depth-sort test currently expects ascending order, which conflicts with the renderer's descending painter's-algorithm order.
