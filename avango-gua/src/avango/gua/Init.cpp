#include <avango/gua/Init.hpp>

#include <avango/Logger.h>

#include <avango/gua/scenegraph/Node.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/LODNode.hpp>
#include <avango/gua/scenegraph/TriMeshNode.hpp>
#include <avango/gua/scenegraph/Video3DNode.hpp>
#include <avango/gua/scenegraph/VolumeNode.hpp>
#if defined(AVANGO_PBR_SUPPORT)
#include <avango/gua/scenegraph/PBRNode.hpp>
#include <avango/gua/scenegraph/PLODNode.hpp>
#endif
#include <avango/gua/scenegraph/NURBSNode.hpp>
#include <avango/gua/scenegraph/VolumeNode.hpp>
#include <avango/gua/scenegraph/ScreenNode.hpp>
#include <avango/gua/scenegraph/PointLightNode.hpp>
#include <avango/gua/scenegraph/SpotLightNode.hpp>
#include <avango/gua/scenegraph/SunLightNode.hpp>
#include <avango/gua/scenegraph/RayNode.hpp>
#include <avango/gua/scenegraph/TexturedQuadNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>
#include <avango/gua/scenegraph/PickResult.hpp>

#include <avango/gua/math/BoundingBox.hpp>
#include <avango/gua/math/BoundingSphere.hpp>

#if defined(AVANGO_PHYSICS_SUPPORT)
#include <avango/gua/physics/RigidBodyNode.hpp>
#include <avango/gua/physics/CollisionShapeNode.hpp>
#include <avango/gua/physics/CollisionShape.hpp>
#include <avango/gua/physics/BoxShape.hpp>
#include <avango/gua/physics/ConvexHullShape.hpp>
#include <avango/gua/physics/CylinderShape.hpp>
#include <avango/gua/physics/PlaneShape.hpp>
#include <avango/gua/physics/SphereShape.hpp>
#include <avango/gua/physics/TriangleMeshShape.hpp>
#include <avango/gua/physics/Constraint.hpp>
#include <avango/gua/physics/FixedConstraint.hpp>
#include <avango/gua/physics/HingeConstraint.hpp>
#include <avango/gua/physics/Point2PointConstraint.hpp>
#include <avango/gua/physics/SliderConstraint.hpp>
#include <avango/gua/physics/Physics.hpp>
#endif

#include <avango/gua/renderer/Renderer.hpp>
#include <avango/gua/renderer/TriMeshLoader.hpp>
#include <avango/gua/renderer/Video3DLoader.hpp>
#include <avango/gua/renderer/VolumeLoader.hpp>
#if defined(AVANGO_PBR_SUPPORT)
#include <avango/gua/renderer/PBRLoader.hpp>
#include <avango/gua/renderer/PLODLoader.hpp>
#endif
#include <avango/gua/renderer/NURBSLoader.hpp>
#include <avango/gua/renderer/Window.hpp>
#include <avango/gua/renderer/Pipeline.hpp>
#include <avango/gua/renderer/Camera.hpp>

#include <avango/gua/viewer/Viewer.hpp>

#include <avango/gua/utils/Logger.hpp>

#include <avango/gua/Fields.hpp>
#include <gua/guacamole.hpp>

namespace
{
    av::Logger& logger(av::getLogger("av::gua::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::gua::Init);

/* static */ void
av::gua::Init::initClass()
{
    if (!isTypeInitialized())
    {
        char** argv;
        ::gua::init(0, argv);

        av::gua::initFields();

        av::gua::Node::initClass();
        av::gua::GeometryNode::initClass();
        av::gua::TransformNode::initClass();
        av::gua::LODNode::initClass();
        av::gua::TriMeshNode::initClass();
        av::gua::Video3DNode::initClass();
        av::gua::VolumeNode::initClass();
#if defined(AVANGO_PBR_SUPPORT)
        av::gua::PBRNode::initClass();
        av::gua::PLODNode::initClass();
#endif
        av::gua::NURBSNode::initClass();
        av::gua::VolumeNode::initClass();
        av::gua::ScreenNode::initClass();
        av::gua::PointLightNode::initClass();
        av::gua::SpotLightNode::initClass();
        av::gua::SunLightNode::initClass();
        av::gua::RayNode::initClass();
        av::gua::TexturedQuadNode::initClass();
        av::gua::SceneGraph::initClass();
        av::gua::PickResult::initClass();

        av::gua::BoundingBox::initClass();
        av::gua::BoundingSphere::initClass();

#if defined(AVANGO_PHYSICS_SUPPORT)
        av::gua::RigidBodyNode::initClass();
        av::gua::CollisionShapeNode::initClass();
        av::gua::CollisionShape::initClass();
        av::gua::BoxShape::initClass();
        av::gua::ConvexHullShape::initClass();
        av::gua::CylinderShape::initClass();
        av::gua::PlaneShape::initClass();
        av::gua::SphereShape::initClass();
        av::gua::TriangleMeshShape::initClass();
        av::gua::Constraint::initClass();
        av::gua::FixedConstraint::initClass();
        av::gua::HingeConstraint::initClass();
        av::gua::Point2PointConstraint::initClass();
        av::gua::SliderConstraint::initClass();
        av::gua::Physics::initClass();
#endif

        av::gua::Pipeline::initClass();
        av::gua::Renderer::initClass();
        av::gua::Window::initClass();
        av::gua::Camera::initClass();
        av::gua::TriMeshLoader::initClass();
        av::gua::Video3DLoader::initClass();
        av::gua::VolumeLoader::initClass();
#if defined(AVANGO_PBR_SUPPORT)
        av::gua::PBRLoader::initClass();
        av::gua::PLODLoader::initClass();
#endif
        av::gua::NURBSLoader::initClass();

        av::gua::Viewer::initClass();

        av::gua::Logger::initClass();

        AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::gua::Init", true);
    }
}
