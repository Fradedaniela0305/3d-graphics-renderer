#include "Window.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "math/Vec4.hpp"
#include "math/Matrix4x4.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include "Clock.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

int main()
{

    Window window("3D Graphics Engine", 800, 600);
    if (!window.init())
    {
        return 1;
    }
    Input input;
    Renderer renderer(window.getRenderer());

    int width = 800;
    int height = 600;

    // TODO: refactor — projection-matrix math belongs in a Camera/Projection class, not loose in main()
    float aspectRatio = (float)width / height;
    float fView = 90.0f;
    float fFar = 1000.0f;
    float fNear = 0.1f;

    float fViewRad = fView * (float)M_PI / 180.0f;
    float f = 1.0f / std::tan(fViewRad / 2.0f);
    float q = fFar / (fFar - fNear);

    float yawAngle = 0;

    Vec4 worldUp{0, 1, 0, 0};
    Vec4 worldRight{1, 0, 0, 0};
    Vec4 worldForward{0, 0, 1, 0};

    Vec4 cameraForward{0, 0, 1, 0};
    Vec4 cameraRight{1, 0, 0, 0};
    Vec4 cameraUp{0, 1, 0, 0};

    Vec4 camera{0.0, 0.0, -1.0, 0.0};

    Vec4 r1{(float)f / aspectRatio, 0, 0, 0};
    Vec4 r2{0, f, 0, 0};
    Vec4 r3{0, 0, q, -fNear * q};
    Vec4 r4{0, 0, 1, 0};

    Matrix4x4 projectionMat{r1, r2, r3, r4};

    Matrix4x4 translationMat{
        Vec4{1, 0, 0, 0},
        Vec4{0, 1, 0, 0},
        Vec4{0, 0, 1, 4},
        Vec4{0, 0, 0, 1}};

    Vec4 lightVec{0.0, 0.0, -1.0, 0.0};

    Mesh shipMesh;
    bool loaded = shipMesh.loadFromObjectFile("objects/teapot.obj");
    Mesh sortedShipMesh;
    Mesh clippedTrianglesMesh;


    if (!loaded)
    {
        std::cerr << "Failed to load objects/cube.obj. Run from the project root.\n";
        return 1;
    }

    float angle = 45.0f * M_PI / 180.0f;
    Matrix4x4 rotationMat;
    float factor = .01;

    Clock phaseClock;
    float inputTimeAccumSec = 0.0f;
    float cameraTimeAccumSec = 0.0f;
    float clearTimeAccumSec = 0.0f;
    float transformClipTimeAccumSec = 0.0f;
    float projectShadeTimeAccumSec = 0.0f;
    float sortTimeAccumSec = 0.0f;
    float drawTimeAccumSec = 0.0f;
    int frameCount = 0;

    while (!window.shouldClose())
    {
        phaseClock.tick();

        window.pollEvents();
        input.poll();

        if (input.isKeyDown(SDL_SCANCODE_W))
        {
            camera = camera + cameraForward * (1*factor);
        }
        if (input.isKeyDown(SDL_SCANCODE_A))
        {
            camera = camera + cameraRight * (-1*factor);
        }
        if (input.isKeyDown(SDL_SCANCODE_S))
        {
            camera = camera + cameraForward * (1*factor);
        }
        if (input.isKeyDown(SDL_SCANCODE_D))
        {
            camera = camera + cameraRight * (1*factor);
        }
        if (input.isKeyDown(SDL_SCANCODE_UP))
        {
            camera = camera + cameraUp * (1*factor);
        }
        if (input.isKeyDown(SDL_SCANCODE_LEFT))
        {
            yawAngle -= .1*factor;
        }
        if (input.isKeyDown(SDL_SCANCODE_RIGHT))
        {
            yawAngle += .1*factor;
        }
        if (input.isKeyDown(SDL_SCANCODE_DOWN))
        {
            camera = camera + cameraUp * (-1*factor);
        }

        inputTimeAccumSec += phaseClock.tick();

        // TODO: refactor — camera orientation/basis update belongs in a Camera class, not loose in main()
        cameraForward.setX(sinf(yawAngle));
        cameraForward.setY(0.0f);
        cameraForward.setZ(cosf(yawAngle));

        cameraRight = Vec4::cross(worldUp, cameraForward);
        cameraUp = Vec4::cross(cameraForward, cameraRight);

        Matrix4x4 cameraRotationMatrix{cameraRight, cameraUp, cameraForward, Vec4{0, 0, 0, 1}};

        Vec4 viewLight = cameraRotationMatrix.transformVector(lightVec);

        cameraTimeAccumSec += phaseClock.tick();

        renderer.clear({20, 20, 20});

        clearTimeAccumSec += phaseClock.tick();

        angle += 0.01;
        sortedShipMesh.clearMesh();
        clippedTrianglesMesh.clearMesh();
        // TODO: refactor — this entire per-triangle pipeline (model rotation, view transform,
        // back-face culling, near-plane clipping) belongs in a render-pipeline/clipper class, not loose in main()
        Triangle transformedTriangle;
        Triangle translatedTriangle;
        Triangle cameraViewTriangle;
        for (Triangle &t : shipMesh.getMesh())
        {

            // TODO: refactor — this model-rotation state/math belongs on the model/entity itself, not loose in main()
            float c = cosf(angle);
            float s = sinf(angle);
            float ch = cosf(angle * 0.5f);
            float sh = sinf(angle * 0.5f);

            rotationMat = Matrix4x4{
                Vec4{c, s * ch, s * sh, 0},
                Vec4{-s, c * ch, c * sh, 0},
                Vec4{0, -sh, ch, 0},
                Vec4{0, 0, 0, 1}};

            t.transformedBy(rotationMat, transformedTriangle);
            transformedTriangle.transformedBy(translationMat, translatedTriangle);

            Triangle viewedTriangle{translatedTriangle.getP1() - camera,
                                    translatedTriangle.getP2() - camera,
                                    translatedTriangle.getP3() - camera};

            viewedTriangle.transformedBy(cameraRotationMatrix, cameraViewTriangle);

            Vec4 normal = cameraViewTriangle.getNormal();
            Vec4 cameraToPlaneVec = cameraViewTriangle.getP1();

            if (Vec4::dot(normal, cameraToPlaneVec) >= 0.0f)
            {
                continue;
            }

            if (cameraViewTriangle.isBehindNearPlane(fNear))
            {

                std::vector<Vec4> inside;
                std::vector<Vec4> outside;

                for (Vec4 vec : cameraViewTriangle.getPoints())
                {

                    if (vec.getZ() >= fNear)
                    {
                        inside.push_back(vec);
                    }
                    else
                    {
                        outside.push_back(vec);
                    }
                }

                if (inside.size() == 1)
                {

                    Vec4 A = inside[0];

                    Vec4 B = outside[0];
                    Vec4 C = outside[1];

                    Vec4 directionAB = B - A;
                    Vec4 directionAC = C - A;

                    float tAB =
                        (fNear - A.getZ()) /
                        directionAB.getZ();

                    float tAC =
                        (fNear - A.getZ()) /
                        directionAC.getZ();

                    Vec4 intersectionAB = Vec4{A.getX() + (directionAB.getX()) * tAB,
                                               A.getY() + (directionAB.getY()) * tAB,
                                               A.getZ() + (directionAB.getZ()) * tAB,
                                               1};

                    Vec4 intersectionAC = Vec4{A.getX() + (directionAC.getX()) * tAC,
                                               A.getY() + (directionAC.getY()) * tAC,
                                               A.getZ() + (directionAC.getZ()) * tAC,
                                               1};

                    Triangle newTriangle = Triangle(A, intersectionAB, intersectionAC);
                    clippedTrianglesMesh.addTriangle(newTriangle);
                }
                else if (inside.size() == 2)
                {
                    Vec4 A = inside[0];
                    Vec4 B = inside[1];

                    Vec4 C = outside[0];

                    Vec4 directionAC = C - A;
                    Vec4 directionBC = C - B;

                    float tAC =
                        (fNear - A.getZ()) /
                        directionAC.getZ();

                    float tBC =
                        (fNear - B.getZ()) /
                        directionBC.getZ();

                    Vec4 intersectionAC = Vec4{A.getX() + (directionAC.getX()) * tAC,
                                               A.getY() + (directionAC.getY()) * tAC,
                                               A.getZ() + (directionAC.getZ()) * tAC,
                                               1};

                    Vec4 intersectionBC = Vec4{B.getX() + (directionBC.getX()) * tBC,
                                               B.getY() + (directionBC.getY()) * tBC,
                                               B.getZ() + (directionBC.getZ()) * tBC,
                                               1};

                    Triangle newTriangleLeft = Triangle(A, intersectionBC, intersectionAC);
                    Triangle newTriangleRight = Triangle(A, B, intersectionBC);
                    clippedTrianglesMesh.addTriangle(newTriangleLeft);
                    clippedTrianglesMesh.addTriangle(newTriangleRight);
                }
                else
                {
                    continue;
                }
            }
            else
            {

                clippedTrianglesMesh.addTriangle(cameraViewTriangle);
            }
        }

        transformClipTimeAccumSec += phaseClock.tick();

        // TODO: refactor — projecting to screen space and shading by light belongs in a
        // Rasterizer/Projector + Shader class, not loose in main()
        Triangle projectedTriangle;
        for (Triangle &t : clippedTrianglesMesh.getMesh()) {

            t.transformedBy(projectionMat, projectedTriangle);

            Vec4 p1 = (projectedTriangle.getP1().perspectiveDivide() * Vec4(1, -1, 1, 1) + Vec4(1.0f, 1.0f, 0, 0)) * Vec4(0.5f * width, 0.5f * height, 1, 1);
            Vec4 p2 = (projectedTriangle.getP2().perspectiveDivide() * Vec4(1, -1, 1, 1) + Vec4(1.0f, 1.0f, 0, 0)) * Vec4(0.5f * width, 0.5f * height, 1, 1);
            Vec4 p3 = (projectedTriangle.getP3().perspectiveDivide() * Vec4(1, -1, 1, 1) + Vec4(1.0f, 1.0f, 0, 0)) * Vec4(0.5f * width, 0.5f * height, 1, 1);

            Triangle drawnTriangle{p1, p2, p3};
            Color triangleColor = Color::getColor(std::clamp(Vec4::dot(t.getNormal(), viewLight), 0.0f, 1.0f));

            drawnTriangle.setColor(triangleColor); 
            sortedShipMesh.addTriangle(drawnTriangle);
        }

        projectShadeTimeAccumSec += phaseClock.tick();

        sortedShipMesh.sortMesh();

        sortTimeAccumSec += phaseClock.tick();

        for (Triangle &drawnTriangle : sortedShipMesh.getMesh())
        {

            renderer.drawFilledTriangle(drawnTriangle, drawnTriangle.getColor());
            // renderer.drawTriangle(drawnTriangle, {0, 0, 0});
        }

        renderer.present();

        drawTimeAccumSec += phaseClock.tick();

        frameCount++;

        if (inputTimeAccumSec + cameraTimeAccumSec + clearTimeAccumSec +
                transformClipTimeAccumSec + projectShadeTimeAccumSec +
                sortTimeAccumSec + drawTimeAccumSec >=
            1.0f)
        {
            float total = inputTimeAccumSec + cameraTimeAccumSec + clearTimeAccumSec +
                           transformClipTimeAccumSec + projectShadeTimeAccumSec +
                           sortTimeAccumSec + drawTimeAccumSec;

            std::cout << "--- avg ms over " << frameCount << " frames (" << frameCount / total << " FPS) ---\n"
                       << "  input:          " << (inputTimeAccumSec / frameCount) * 1000.0f << " ms\n"
                       << "  camera update:   " << (cameraTimeAccumSec / frameCount) * 1000.0f << " ms\n"
                       << "  clear:           " << (clearTimeAccumSec / frameCount) * 1000.0f << " ms\n"
                       << "  transform+clip:  " << (transformClipTimeAccumSec / frameCount) * 1000.0f << " ms\n"
                       << "  project+shade:   " << (projectShadeTimeAccumSec / frameCount) * 1000.0f << " ms\n"
                       << "  sort:            " << (sortTimeAccumSec / frameCount) * 1000.0f << " ms\n"
                       << "  draw+present:    " << (drawTimeAccumSec / frameCount) * 1000.0f << " ms\n"
                       << "  total:           " << (total / frameCount) * 1000.0f << " ms\n";

            inputTimeAccumSec = 0.0f;
            cameraTimeAccumSec = 0.0f;
            clearTimeAccumSec = 0.0f;
            transformClipTimeAccumSec = 0.0f;
            projectShadeTimeAccumSec = 0.0f;
            sortTimeAccumSec = 0.0f;
            drawTimeAccumSec = 0.0f;
            frameCount = 0;
        }
    }

    return 0;
}
