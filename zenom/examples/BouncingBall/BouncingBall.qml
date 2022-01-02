import QtQuick 2.2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Scene3D 2.0

Scene3D {
    objectName: "BouncingBall Scene"
    id: scene3d
    focus: true
    aspects: ["input", "logic"]
    cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

    Entity {
        objectName: "Scene Entity"
        id: sceneRoot

        Camera {
            objectName: "Camera"
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 45
            nearPlane : 0.1
            farPlane : 1000.0
            position: Qt.vector3d( 0.0, 8.0, 26.0 )
            upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
            viewCenter: Qt.vector3d( -1, 7, -1 )
        }

        FirstPersonCameraController
        {
            objectName: "FPS Camera Controller"
            camera: camera
        }

        components: [
            RenderSettings {
                objectName: "Render Settings"
                activeFrameGraph: ForwardRenderer {
                    objectName: "Forward Renderer"
                    camera: camera
                    clearColor: Qt.rgba(0, 0.5, 1, 1)
                }
            },
            InputSettings { }
        ]

        PhongMaterial {
            objectName: "Phong Material"
            id: material
        }

        PlaneMesh {
            objectName: "Plane Mesh"
            id: planeMesh
            height: 10
            width: 10
        }

        Transform {
            objectName: "Plane Transform"
            id: planeTransform
        }

        Entity {
            objectName: "Plane Entity"
            id: planeEntity
            components: [ planeMesh, material, planeTransform ]
        }

        SphereMesh {
            objectName: "Sphere Mesh"
            id: sphereMesh
            radius: 1
        }

        Transform {
            objectName: "Sphere Transform"
            id: sphereTransform
            translation: Qt.vector3d(0, 15, 0)
        }

        Entity {
            objectName: "Sphere Entity"
            id: sphereEntity
            components: [ sphereMesh, material, sphereTransform ]
        }
    }
}
