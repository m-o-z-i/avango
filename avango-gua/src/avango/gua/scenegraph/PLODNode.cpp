#include <avango/gua/scenegraph/PLODNode.hpp>
#include <avango/gua/network/NetTransform.h>
#include <avango/Base.h>
#include <boost/bind.hpp>

AV_FC_DEFINE(av::gua::PLODNode);

AV_FIELD_DEFINE(av::gua::SFPLODNode);
AV_FIELD_DEFINE(av::gua::MFPLODNode);

av::gua::PLODNode::PLODNode(std::shared_ptr< ::gua::node::PLODNode> guanode)
    : GeometryNode(guanode)
    , m_guaPLODNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        boost::bind(&PLODNode::getGeometryCB, this, _1),
                        boost::bind(&PLODNode::setGeometryCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      boost::bind(&PLODNode::getMaterialCB, this, _1),
                      boost::bind(&PLODNode::setMaterialCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(RadiusScale,
                      boost::bind(&PLODNode::getRadiusScaleCB, this, _1),
                      boost::bind(&PLODNode::setRadiusScaleCB, this, _1));
 
  AV_FC_ADD_ADAPTOR_FIELD(ErrorThreshold,
                      boost::bind(&PLODNode::getErrorThresholdCB, this, _1),
                      boost::bind(&PLODNode::setErrorThresholdCB, this, _1));
  
  AV_FC_ADD_ADAPTOR_FIELD(EnableBackfaceCulling,
                      boost::bind(&PLODNode::getEnableBackfaceCullingCB, this, _1),
                      boost::bind(&PLODNode::setEnableBackfaceCullingCB, this, _1));

  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }
}

//av::gua::PLODNode::~PLODNode()
//{}

void
av::gua::PLODNode::on_distribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_distribute(netNode);

  if (m_Material.isValid()) {
    m_Material->on_distribute(netNode);
  }
  netNode.distributeFieldContainer(m_Material);
}

void
av::gua::PLODNode::on_undistribute(av::gua::NetTransform& netNode) 
{
  GeometryNode::on_undistribute(netNode);

  if (m_Material.isValid()) {
    m_Material->on_undistribute(netNode);
  }
  netNode.undistributeFieldContainer(m_Material);
}

void
av::gua::PLODNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::PLODNode, true);

    SFPLODNode::initClass("av::gua::SFPLODNode", "av::Field");
    MFPLODNode::initClass("av::gua::MFPLODNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::PLODNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODNode->get_geometry_description();
}

void
av::gua::PLODNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaPLODNode->set_geometry_description(event.getValue());
}

void
av::gua::PLODNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::PLODNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaPLODNode->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::PLODNode::getRadiusScaleCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODNode->get_radius_scale();
}

void
av::gua::PLODNode::setRadiusScaleCB(const SFFloat::SetValueEvent& event)
{
  m_guaPLODNode->set_radius_scale(event.getValue());
}

void
av::gua::PLODNode::getErrorThresholdCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODNode->get_error_threshold();
}

void
av::gua::PLODNode::setErrorThresholdCB(const SFFloat::SetValueEvent& event)
{
  m_guaPLODNode->set_error_threshold(event.getValue());
}

void
av::gua::PLODNode::getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPLODNode->get_enable_backface_culling_by_normal();
}

void
av::gua::PLODNode::setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event)
{
  m_guaPLODNode->set_enable_backface_culling_by_normal(event.getValue());
}

std::shared_ptr< ::gua::node::PLODNode>
av::gua::PLODNode::getGuaPLODNode() const {
  return m_guaPLODNode;
}
