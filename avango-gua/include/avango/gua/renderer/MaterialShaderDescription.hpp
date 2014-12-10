#ifndef AVANGO_GUA_MATERIAL_SHADER_DESCRIPTION_HPP
#define AVANGO_GUA_MATERIAL_SHADER_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/MaterialShaderDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/MaterialShaderMethod.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::MaterialShaderDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL MaterialShaderDescription : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::MaterialShaderDescription is created.
       * Otherwise, the given ::gua::MaterialShaderDescription is used.
       */
      MaterialShaderDescription(::gua::MaterialShaderDescription const& guaMaterialShaderDescription =
                                ::gua::MaterialShaderDescription());



    public:

      SFString FileName;
      MFMaterialShaderMethod VertexMethods;
      MFMaterialShaderMethod FragmentMethods;

      virtual void getFileNameCB(const SFString::GetValueEvent& event);
      virtual void setFileNameCB(const SFString::SetValueEvent& event);

      virtual void getVertexMethodsCB(const MFMaterialShaderMethod::GetValueEvent& event);
      virtual void setVertexMethodsCB(const MFMaterialShaderMethod::SetValueEvent& event);

      virtual void getFragmentMethodsCB(const MFMaterialShaderMethod::GetValueEvent& event);
      virtual void setFragmentMethodsCB(const MFMaterialShaderMethod::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::MaterialShaderDescription.
       */
      ::gua::MaterialShaderDescription const& getGuaMaterialShaderDescription() const;

    private:

      ::gua::MaterialShaderDescription m_guaMaterialShaderDescription;

      MFMaterialShaderMethod::ContainerType m_vertexMethods;
      MFMaterialShaderMethod::ContainerType m_fragmentMethods;


      MaterialShaderDescription(const MaterialShaderDescription&);
      MaterialShaderDescription& operator=(const MaterialShaderDescription&);
    };

    typedef SingleField<Link<MaterialShaderDescription> > SFMaterialShaderDescription;
    typedef MultiField<Link<MaterialShaderDescription> > MFMaterialShaderDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::MaterialShaderDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::MaterialShaderDescription> >;
#endif

}

#endif //AVANGO_GUA_MATERIAL_SHADER_DESCRIPTION_HPP