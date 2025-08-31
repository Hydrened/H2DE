#pragma once

#include <H2DE/objects/H2DE_object.h>

class H2DE_SingleSurfaceObject : public H2DE_Object {
public:
    /**
     * @brief Add a surface to the object.
     * 
     * Adds a new surface (texture, sprite, or color) to the object.
     * The surface is stored and identified by a unique name.
     * 
     * @tparam H2DE_Surface_T The type of surface to add (e.g. Texture, Sprite, Color).
     * @param name The unique name to associate with the surface.
     * @param surfaceData Common parameters such as position, size, etc.
     * @param specificData Specific data for the surface type.
     * @return A pointer to the created surface, or nullptr if it failed.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurface(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::_addSurface<H2DE_Surface_T>(_surfaces, name, surfaceData, specificData);
    }
    /**
     * @brief Remove a surface from the object.
     * 
     * Removes a surface previously added by its unique name.
     * 
     * @param name The name of the surface to remove.
     * @return true if the surface was found and removed, false otherwise.
     */
    inline bool removeSurface(const std::string& name) {
        return H2DE_Object::_removeSurface(_surfaces, name);
    }

    /**
     * @brief Get all surfaces of the object.
     * 
     * Returns a map of all currently attached surfaces. Each entry maps the surface name
     * to its base `H2DE_Surface` pointer.
     * 
     * @return A map of surface names to their respective surface pointers.
     */
    inline H2DE_SurfaceMap getSurfaces() const noexcept {
        return _surfaces; 
    }
    /**
     * @brief Get a surface by name and cast it to a specific type.
     * 
     * Retrieves a surface of a specific type from the object. If the name is not found or
     * the type is incorrect, returns nullptr.
     * 
     * @tparam H2DE_Surface_T The expected surface type (Texture, Sprite, Color, etc.).
     * @param name The name of the surface to retrieve.
     * @return A pointer to the surface cast to the specified type, or nullptr.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getSurface(const std::string& name) const {
        return H2DE_Object::_getSurface<H2DE_Surface_T>(_surfaces, name); 
    }
    /**
     * @brief Checks whether a surface with the given name exists.
     * @param name Name of the surface to check.
     * @return true if the surface exists, false otherwise.
     */
    inline bool hasSurface(const std::string& name) const {
        return H2DE_Object::_hasSurface(_surfaces, name);
    }

protected:
    H2DE_SurfaceMap _surfaces = {};

    H2DE_SingleSurfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData);
    ~H2DE_SingleSurfaceObject() override;

private:
    void _refreshSurfaceBuffers() override;
    void _refreshMaxRadius() override;
};
