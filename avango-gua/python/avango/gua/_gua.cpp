#include <boost/python.hpp>

#include <avango/Init.h>
#include <avango/Config.h>

#include <avango/gua/Init.hpp>
#include <avango/python/register_field.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/Init.h>
#endif

#include "math/math.hpp"

#include "scenegraph/Node.hpp"
#include "scenegraph/GeometryNode.hpp"
#include "scenegraph/TransformNode.hpp"
#include "scenegraph/ClippingPlaneNode.hpp"
#include "scenegraph/LODNode.hpp"
#include "scenegraph/TriMeshNode.hpp"
#include "scenegraph/Video3DNode.hpp"
#if defined(AVANGO_PBR_SUPPORT)
#include "scenegraph/PBRNode.hpp"
#include "scenegraph/PLODNode.hpp"
#endif
#include "scenegraph/NURBSNode.hpp"
#include "scenegraph/ScreenNode.hpp"
#include "scenegraph/LightNode.hpp"
#include "scenegraph/RayNode.hpp"
#include "scenegraph/TexturedQuadNode.hpp"
#include "scenegraph/TexturedScreenSpaceQuadNode.hpp"
#include "scenegraph/SceneGraph.hpp"
#include "scenegraph/CameraNode.hpp"
#include "scenegraph/PickResult.hpp"

#include "math/BoundingBox.hpp"
#include "math/Frustum.hpp"
#include "math/BoundingSphere.hpp"

#if defined(AVANGO_PHYSICS_SUPPORT)
#include "physics/RigidBodyNode.hpp"
#include "physics/CollisionShapeNode.hpp"
#include "physics/CollisionShape.hpp"
#include "physics/BoxShape.hpp"
#include "physics/ConvexHullShape.hpp"
#include "physics/CylinderShape.hpp"
#include "physics/PlaneShape.hpp"
#include "physics/SphereShape.hpp"
#include "physics/TriangleMeshShape.hpp"
#include "physics/Constraint.hpp"
#include "physics/FixedConstraint.hpp"
#include "physics/HingeConstraint.hpp"
#include "physics/Point2PointConstraint.hpp"
#include "physics/SliderConstraint.hpp"
#include "physics/PhysicsDatabase.hpp"
#include "physics/Physics.hpp"
#endif

#include "renderer/WindowBase.hpp"
#include "renderer/Window.hpp"
#include "renderer/HeadlessSurface.hpp"
#include "renderer/GlfwWindow.hpp"
#include "renderer/MaterialShaderMethod.hpp"
#include "renderer/MaterialShaderDescription.hpp"
#include "renderer/PBSMaterialFactory.hpp"
#include "renderer/Material.hpp"
#include "renderer/PipelineDescription.hpp"
#include "renderer/PipelinePassDescription.hpp"
#include "renderer/StencilPassDescription.hpp"
#include "renderer/TriMeshPassDescription.hpp"
#include "renderer/TexturedQuadPassDescription.hpp"
#include "renderer/DebugViewPassDescription.hpp"
#include "renderer/BackgroundPassDescription.hpp"
#include "renderer/SkyMapPassDescription.hpp"
#include "renderer/BBoxPassDescription.hpp"
#include "renderer/EmissivePassDescription.hpp"
#include "renderer/PhysicallyBasedShadingPassDescription.hpp"
#include "renderer/TexturedScreenSpaceQuadPassDescription.hpp"
#include "renderer/FullscreenPassDescription.hpp"
#include "renderer/SSAOPassDescription.hpp"
#include "renderer/ResolvePassDescription.hpp"
#include "renderer/LightVisibilityPassDescription.hpp"
#include "renderer/SSAAPassDescription.hpp"
#include "renderer/Databases.hpp"
#include "renderer/TriMeshLoader.hpp"
#include "renderer/Video3DLoader.hpp"
#if defined(AVANGO_PBR_SUPPORT)
#include "renderer/PBRLoader.hpp"
#include "renderer/PLODLoader.hpp"
#include "renderer/PLODPassDescription.hpp"
#endif
#include "renderer/NURBSLoader.hpp"
#include "renderer/Texture.hpp"

#include "viewer/Viewer.hpp"
#include "viewer/BlenderViewer.hpp"

#include "utils/Color.hpp"
#include "utils/Logger.hpp"
#include "utils/Ray.hpp"

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include "network/NetTransform.h"
#endif

using namespace boost::python;
using namespace av::python;


namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }

BOOST_PYTHON_MODULE(_gua)
{
    PyEval_InitThreads();


    av::gua::Init::initClass();

    init_Vec2();
    init_Vec2ui();
    init_Vec3();
    init_Vec4();
    init_Quat();

    init_Mat3();
    init_Mat4();

    init_Color();

    init_Node();
    init_GeometryNode();
    init_TransformNode();
    init_ClippingPlaneNode();
    init_CameraNode();
    init_LODNode();
#if defined(AVANGO_DISTRIBUTION_SUPPORT)
    init_NetTransform();
    av::gua::network::Init::initClass();
#endif
    init_TriMeshNode();
    // init_Video3DNode();
#if defined(AVANGO_PBR_SUPPORT)
    init_PLODNode();
    // init_PBRNode();
#endif
    // init_NURBSNode();
    init_ScreenNode();
    init_LightNode();
    init_RayNode();
    init_TexturedQuadNode();
    init_TexturedScreenSpaceQuadNode();
    init_SceneGraph();
    init_PickResult();

    init_BoundingBox();
    init_Frustum();
    init_BoundingSphere();

#if defined(AVANGO_PHYSICS_SUPPORT)
    init_RigidBodyNode();
    init_CollisionShapeNode();
    init_CollisionShape();
    init_BoxShape();
    init_ConvexHullShape();
    init_CylinderShape();
    init_PlaneShape();
    init_SphereShape();
    init_TriangleMeshShape();
    init_Constraint();
    init_FixedConstraint();
    init_HingeConstraint();
    init_Point2PointConstraint();
    init_SliderConstraint();
    init_PhysicsDatabase();
    init_Physics();
#endif

    init_WindowBase();
    init_Window();
    init_HeadlessSurface();
    init_GlfwWindow();
    init_MaterialShaderMethod();
    init_MaterialShaderDescription();
    init_PBSMaterialFactory();
    init_Material();
    init_PipelineDescription();
    init_PipelinePassDescription();
    init_StencilPassDescription();
    init_TriMeshPassDescription();
    init_TexturedQuadPassDescription();
    init_DebugViewPassDescription();
    init_BackgroundPassDescription();
    init_SkyMapPassDescription();
    init_BBoxPassDescription();
    init_EmissivePassDescription();
    init_PhysicallyBasedShadingPassDescription();
    init_TexturedScreenSpaceQuadPassDescription();
    init_FullscreenPassDescription();
    init_SSAOPassDescription();
    init_SSAAPassDescription();
    init_ResolvePassDescription();
    init_LightVisibilityPassDescription();
    init_Databases();
    init_TriMeshLoader();
    // init_Video3DLoader();
#if defined(AVANGO_PBR_SUPPORT)
    init_PLODLoader();
    init_PLODPassDescription();
    // init_PBRLoader();
#endif
    // init_NURBSLoader();
    init_Texture();

    init_Viewer();
    init_BlenderViewer();

    init_Logger();
    init_Ray();
}
