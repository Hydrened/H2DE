#pragma once

#include <H2DE/objects/H2DE_object.h>

class H2DE_DualSurfaceObject : public H2DE_Object {
public:
    /**
     * @brief Add a surface to the fill of the checkbox.
     * 
     * This function adds a surface (texture, sprite, or color) in fill of the checkbox,
     * which visually represents the actual filling part of the checkbox.
     * 
     * @tparam H2DE_Surface_T The type of the surface (e.g. Texture, Sprite, Color).
     * @param name The unique identifier for the surface.
     * @param surfaceData Common surface parameters (position, size, etc.).
     * @param specificData Additional data specific to the surface type.
     * @return A pointer to the created surface, or nullptr on failure.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurfaceToFill(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::_addSurface<H2DE_Surface_T>(_fillSurfaces, name, surfaceData, specificData);
    }
    /**
     * @brief Remove a fill surface from the checkbox.
     * 
     * This removes a surface that was previously added in fill of the checkbox
     * using its unique name.
     * 
     * @param name The name of the surface to remove.
     * @return true if the surface was removed successfully, false otherwise.
     */
    inline bool removeSurfaceFromFill(const std::string& name) {
        return H2DE_Object::_removeSurface(_fillSurfaces, name);
    }
    
    /**
     * @brief Add a surface to the background of the checkbox.
     * 
     * This adds a decorative or background surface behind the checkbox's fill.
     * 
     * @tparam H2DE_Surface_T The type of the surface (e.g. Texture, Sprite, Color).
     * @param name The unique identifier for the surface.
     * @param surfaceData Common surface parameters (position, size, etc.).
     * @param specificData Additional data specific to the surface type.
     * @return A pointer to the created surface, or nullptr on failure.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurfaceToBackground(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::_addSurface<H2DE_Surface_T>(_backgroundSurfaces, name, surfaceData, specificData);
    }
    /**
     * @brief Remove a background surface from the checkbox.
     * 
     * This removes a surface that was added behind the checkbox using its unique name.
     * 
     * @param name The name of the surface to remove.
     * @return true if the surface was removed successfully, false otherwise.
     */
    inline bool removeSurfaceFromBackground(const std::string& name) {
        return H2DE_Object::_removeSurface(_backgroundSurfaces, name);
    }

    /**
     * @brief Get all fill surfaces of the checkbox.
     * 
     * Returns a map containing all surfaces rendered in fill of the checkbox background.
     * These typically include the foreground checkbox fill or decorative elements.
     * 
     * @return A map of surface names to H2DE_Surface pointers.
     */
    inline H2DE_SurfaceMap getFillSurfaces() const noexcept {
        return _fillSurfaces;
    }
    /**
     * @brief Get all background surfaces of the checkbox.
     * 
     * Returns a map containing all surfaces rendered behind the checkbox foreground.
     * These typically include background textures or colored panels.
     * 
     * @return A map of surface names to H2DE_Surface pointers.
     */
    inline H2DE_SurfaceMap getBackgroundSurfaces() const noexcept {
        return _backgroundSurfaces;
    }
    /**
     * @brief Get a fill surface by name and type.
     * 
     * Retrieves a specific surface from the fill layer, cast to the requested surface type.
     * If the name does not exist or the type is incorrect, returns nullptr.
     * 
     * @tparam H2DE_Surface_T The expected surface type (Texture, Sprite, Color, etc.).
     * @param name The name of the surface to retrieve.
     * @return A pointer to the surface, or nullptr if not found or mismatched.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getFillSurface(const std::string& name) const {
        return H2DE_Object::_getSurface<H2DE_Surface_T>(_fillSurfaces, name);
    }
    /**
     * @brief Get a background surface by name and type.
     * 
     * Retrieves a specific surface from the background layer, cast to the requested surface type.
     * If the name does not exist or the type is incorrect, returns nullptr.
     * 
     * @tparam H2DE_Surface_T The expected surface type (Texture, Sprite, Color, etc.).
     * @param name The name of the surface to retrieve.
     * @return A pointer to the surface, or nullptr if not found or mismatched.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getBackgroundSurface(const std::string& name) const {
        return H2DE_Object::_getSurface<H2DE_Surface_T>(_backgroundSurfaces, name);
    }
    /**
     * @brief Checks whether a fill surface with the given name exists.
     * @param name Name of the surface to check.
     * @return true if the fill surface exists, false otherwise.
     */
    inline bool hasFillSurface(const std::string& name) const {
        return H2DE_Object::_hasSurface(_fillSurfaces, name); 
    }
    /**
     * @brief Checks whether a background surface with the given name exists.
     * @param name Name of the surface to check.
     * @return true if the background surface exists, false otherwise.
     */
    inline bool hasBackgroundSurface(const std::string& name) const {
        return H2DE_Object::_hasSurface(_backgroundSurfaces, name);
    }

protected:
    H2DE_SurfaceMap _backgroundSurfaces = {};
    H2DE_SurfaceMap _fillSurfaces = {};

    H2DE_DualSurfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData);
    ~H2DE_DualSurfaceObject() override;

private:
    void _refreshSurfaceBuffers() override;
    void _refreshMaxRadius() override;
};
