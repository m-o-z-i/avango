#ifndef AVANGO_GUA_PHYSICALLY_BASED_SHADING_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_PHYSICALLY_BASED_SHADING_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/PhysicallyBasedShadingPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PhysicallyBasedShadingPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PhysicallyBasedShadingPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::PhysicallyBasedShadingPassDescription is created.
       * Otherwise, the given ::gua::PhysicallyBasedShadingPassDescription is used.
       */
      PhysicallyBasedShadingPassDescription(std::shared_ptr< ::gua::PhysicallyBasedShadingPassDescription> const& PhysicallyBasedShadingPassDescription =
                                std::shared_ptr< ::gua::PhysicallyBasedShadingPassDescription>(new ::gua::PhysicallyBasedShadingPassDescription()) );


    public:

      /**
       * Get the wrapped ::gua::PhysicallyBasedShadingPassDescription.
       */
      std::shared_ptr< ::gua::PhysicallyBasedShadingPassDescription> const& getGuaPhysicallyBasedShadingPassDescription() const;

    private:

      std::shared_ptr< ::gua::PhysicallyBasedShadingPassDescription> m_guaPhysicallyBasedShadingPassDescription;


      PhysicallyBasedShadingPassDescription(const PhysicallyBasedShadingPassDescription&);
      PhysicallyBasedShadingPassDescription& operator=(const PhysicallyBasedShadingPassDescription&);
    };

    typedef SingleField<Link<PhysicallyBasedShadingPassDescription> > SFPhysicallyBasedShadingPassDescription;
    typedef MultiField<Link<PhysicallyBasedShadingPassDescription> > MFPhysicallyBasedShadingPassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PhysicallyBasedShadingPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::PhysicallyBasedShadingPassDescription> >;
#endif

}

#endif //AVANGO_GUA_PHYSICALLY_BASED_SHADING_PASS_DESCRIPTION_HPP
