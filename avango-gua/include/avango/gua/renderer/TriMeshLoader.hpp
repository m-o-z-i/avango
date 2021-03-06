#ifndef AVANGO_GUA_TRI_MESH_LOADER_HPP
#define AVANGO_GUA_TRI_MESH_LOADER_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/TriMeshLoader.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/scenegraph/TriMeshNode.hpp>
#include <avango/FieldContainer.h>

#include <avango/gua/windows_specific_gua.hpp>


namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TriMeshLoader
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TriMeshLoader : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Flags {
        DEFAULTS = ::gua::TriMeshLoader::DEFAULTS,
        LOAD_MATERIALS = ::gua::TriMeshLoader::LOAD_MATERIALS,
        OPTIMIZE_GEOMETRY = ::gua::TriMeshLoader::OPTIMIZE_GEOMETRY,
        MAKE_PICKABLE = ::gua::TriMeshLoader::MAKE_PICKABLE,
        NORMALIZE_SCALE = ::gua::TriMeshLoader::NORMALIZE_SCALE,
        NORMALIZE_POSITION = ::gua::TriMeshLoader::NORMALIZE_POSITION
      };

      /**
       * Constructor. When called without arguments, a new ::gua::TriMeshLoader is created.
       * Otherwise, the given ::gua::TriMeshLoader is used.
       */
      TriMeshLoader(::gua::TriMeshLoader* guaTriMeshLoader = new ::gua::TriMeshLoader());

      av::Link<av::gua::Node> createGeometryFromFile(std::string const& nodeName,
                                                             std::string const& fileName,
                                                             std::string const& fallbackMaterial,
                                                             Flags flags = DEFAULTS) const;

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
//      virtual ~TriMeshLoader();

    public:

      /**
       * Get the wrapped ::gua::TriMeshLoader.
       */
      ::gua::TriMeshLoader* getGuaTriMeshLoader() const;


    private:

      ::gua::TriMeshLoader *m_guaTriMeshLoader;

      av::gua::Node* createChildren(std::shared_ptr< ::gua::node::Node> root) const;

      TriMeshLoader(const TriMeshLoader&);
      TriMeshLoader& operator=(const TriMeshLoader&);
    };

    typedef SingleField<Link<TriMeshLoader> > SFTriMeshLoader;
    typedef MultiField<Link<TriMeshLoader> > MFTriMeshLoader;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TriMeshLoader> >;
  template class AV_GUA_DLL MultiField<Link<gua::TriMeshLoader> >;
#endif

}

#endif //AVANGO_GUA_TRI_MESH_LOADER_HPP
