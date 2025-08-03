#pragma once

#include <H2DE/utils/H2DE_utils.h>

/**
 * @brief Represents a 2D rectangle with position and size.
 * 
 * The rectangle is defined by its center position (x, y) and dimensions (w, h).
 * Template allows using int, float or any numeric type.
 * 
 * @tparam H2DE_Rect_T Numeric type of the rectangle's components.
 */
template<typename H2DE_Rect_T>
struct H2DE_Rect {  
    H2DE_Rect_T x = 0.0f;   /**< Center position on the X axis */
    H2DE_Rect_T y = 0.0f;   /**< Center position on the Y axis */
    H2DE_Rect_T w = 1.0f;   /**< Width of the rectangle */
    H2DE_Rect_T h = 1.0f;   /**< Height of the rectangle */

    /**
     * @brief Default constructor, initializes rect to (0,0,1,1).
     */
    constexpr H2DE_Rect() noexcept = default;
    /**
     * @brief Constructs a rectangle from a translation and a uniform scale.
     * 
     * @param translate Value used for both x and y position
     * @param scale Value used for both width and height
     */
    constexpr H2DE_Rect(H2DE_Rect_T translate, H2DE_Rect_T scale) noexcept : x(translate), y(translate), w(scale), h(scale) {}
    /**
     * @brief Constructs a rectangle with specified position and size.
     * 
     * @param x Center X position
     * @param y Center Y position
     * @param w Width
     * @param h Height
     */
    constexpr H2DE_Rect(H2DE_Rect_T x, H2DE_Rect_T y, H2DE_Rect_T w, H2DE_Rect_T h) noexcept : x(x), y(y), w(w), h(h) {}

    /**
     * @brief Adds two rectangles component-wise.
     * @return A new rectangle with added position and size.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> operator+(const H2DE_Rect<H2DE_Rect_T>& other) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{
            x + other.x,
            y + other.y,
            w + other.w,
            h + other.h
        };
    }
    /**
     * @brief Subtracts two rectangles component-wise.
     * @return A new rectangle with subtracted position and size.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> operator-(const H2DE_Rect<H2DE_Rect_T>& other) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{
            x - other.x,
            y - other.y,
            w - other.w,
            h - other.h
        };
    }
    /**
     * @brief Multiplies rectangle's position and size by a float multiplier.
     * @param multiplier The value to multiply by.
     * @return A new scaled rectangle.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> operator*(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{
            static_cast<H2DE_Rect_T>(x * multiplier),
            static_cast<H2DE_Rect_T>(y * multiplier),
            static_cast<H2DE_Rect_T>(w * multiplier),
            static_cast<H2DE_Rect_T>(h * multiplier)
        };
    }
    /**
     * @brief Divides rectangle's position and size by a float divider.
     * @param divider The value to divide by.
     * @return A new scaled rectangle.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> operator/(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{
            static_cast<H2DE_Rect_T>(x / divider),
            static_cast<H2DE_Rect_T>(y / divider),
            static_cast<H2DE_Rect_T>(w / divider),
            static_cast<H2DE_Rect_T>(h / divider)
        };
    }

    /**
     * @brief Adds the values of another rect to this rect (in-place).
     * @param other The rect to add.
     * @return Reference to this rect after addition.
     */
    H2DE_Rect<H2DE_Rect_T>& operator+=(const H2DE_Rect<H2DE_Rect_T>& other) noexcept;
    /**
     * @brief Subtracts the values of another rect from this rect (in-place).
     * @param other The rect to subtract.
     * @return Reference to this rect after subtraction.
     */
    H2DE_Rect<H2DE_Rect_T>& operator-=(const H2DE_Rect<H2DE_Rect_T>& other) noexcept;
    /**
     * @brief Multiplies all values of this rect by a scalar (in-place).
     * @param multiplier The scalar to multiply by.
     * @return Reference to this rect after multiplication.
     */
    H2DE_Rect<H2DE_Rect_T>& operator*=(float multiplier) noexcept;
    /**
     * @brief Divides all values of this rect by a scalar (in-place).
     * @param divider The scalar to divide by.
     * @return Reference to this rect after division.
     */
    H2DE_Rect<H2DE_Rect_T>& operator/=(float divider);

    /**
     * @brief Checks equality between two rectangles.
     * @return true if position and size are equal.
     */
    constexpr bool operator==(const H2DE_Rect<H2DE_Rect_T>& other) const noexcept {
        return (x == other.x && y == other.y && w == other.w && h == other.h);
    }
    /**
     * @brief Checks inequality between two rectangles.
     * @return true if position or size differ.
     */
    constexpr bool operator!=(const H2DE_Rect<H2DE_Rect_T>& other) const noexcept {
        return !(*this == other);
    }

    /**
     * @brief Converts this rectangle to SDL_Rect for SDL rendering.
     * 
     * The rectangle is converted from center-position + size to SDL's top-left + size.
     */
    inline operator SDL_Rect() const noexcept {
        return SDL_Rect{
            static_cast<int>(H2DE::round(x - w * 0.5f)),
            static_cast<int>(H2DE::round(y - h * 0.5f)),
            static_cast<int>(w),
            static_cast<int>(h)
        };
    }

    friend inline std::ostream& operator<<(std::ostream& os, const H2DE_Rect<H2DE_Rect_T>& rect) {
        os << "[x: " << rect.x << ", y: " << rect.y << ", w: " << rect.w << ", h: " << rect.h << "]";
        return os;
    }

    /**
     * @brief Returns a new rectangle with position translated by given vector.
     * @param translate Vector to add to position.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addTranslate(const H2DE_Vector2D<H2DE_Rect_T>& translate) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x + translate.x, y + translate.y, w, h };
    }
    /**
     * @brief Returns a new rectangle with size scaled by given vector.
     * @param scale Vector to add to size.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addScale(const H2DE_Vector2D<H2DE_Rect_T>& scale) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w + scale.x, h + scale.y };
    }
    /**
     * @brief Returns a new rectangle with position translated negatively by given vector.
     * @param translate Vector to subtract from position.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractTranslate(const H2DE_Vector2D<H2DE_Rect_T>& translate) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x - translate.x, y - translate.y, w, h };
    }
    /**
     * @brief Returns a new rectangle with size scaled down by given vector.
     * @param scale Vector to subtract from size.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractScale(const H2DE_Vector2D<H2DE_Rect_T>& scale) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w - scale.x, h - scale.y };
    }
    /**
     * @brief Returns a new rectangle with position multiplied by multiplier, size unchanged.
     * @param multiplier Value to multiply position by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyTranslate(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x * multiplier), static_cast<H2DE_Rect_T>(y * multiplier), w, h };
    }
    /**
     * @brief Returns a new rectangle with size multiplied by multiplier, position unchanged.
     * @param multiplier Value to multiply size by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyScale(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w * multiplier), static_cast<H2DE_Rect_T>(h * multiplier) };
    }
    /**
     * @brief Returns a new rectangle with position divided by divider, size unchanged.
     * @param divider Value to divide position by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideTranslate(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x / divider), static_cast<H2DE_Rect_T>(y / divider), w, h };
    }
    /**
     * @brief Returns a new rectangle with size divided by divider, position unchanged.
     * @param divider Value to divide size by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideScale(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w / divider), static_cast<H2DE_Rect_T>(h / divider) };
    }

    /**
     * @brief Returns a new rectangle with the X component increased by a given value.
     * @param value Value to add to the X component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addX(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x + value, y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component increased by a given value.
     * @param value Value to add to the Y component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addY(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y + value, w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component increased by a given value.
     * @param value Value to add to the W component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addW(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w + value, h };
    }
    /**
     * @brief Returns a new rectangle with the H component increased by a given value.
     * @param value Value to add to the H component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> addH(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, h + value };
    }

    /**
     * @brief Returns a new rectangle with the X component decreased by a given value.
     * @param value Value to subtract from the X component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractX(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x - value, y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component decreased by a given value.
     * @param value Value to subtract from the Y component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractY(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y - value, w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component decreased by a given value.
     * @param value Value to subtract from the W component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractW(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w - value, h };
    }
    /**
     * @brief Returns a new rectangle with the H component decreased by a given value.
     * @param value Value to subtract from the H component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> subtractH(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, h - value };
    }

    /**
     * @brief Returns a new rectangle with the X component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the X component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyX(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x * multiplier), y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the Y component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyY(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, static_cast<H2DE_Rect_T>(y * multiplier), w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the W component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyW(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w * multiplier), h };
    }
    /**
     * @brief Returns a new rectangle with the H component multiplied by a given multiplier.
     * @param multiplier Multiplier to multiply the H component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> multiplyH(float multiplier) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, static_cast<H2DE_Rect_T>(h * multiplier) };
    }

    /**
     * @brief Returns a new rectangle with the X component divided by a given divider.
     * @param divider Divider to divide the X component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideX(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{ static_cast<H2DE_Rect_T>(x / divider), y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component divided by a given divider.
     * @param divider Divider to divide the Y component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideY(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{ x, static_cast<H2DE_Rect_T>(y / divider), w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component divided by a given divider.
     * @param divider Divider to divide the W component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideW(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, static_cast<H2DE_Rect_T>(w / divider), h };
    }
    /**
     * @brief Returns a new rectangle with the H component divided by a given divider.
     * @param divider Divider to divide the H component by.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> divideH(float divider) const {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, static_cast<H2DE_Rect_T>(h / divider) };
    }

    /**
     * @brief Returns position vector of the rectangle center.
     */
    constexpr H2DE_Vector2D<H2DE_Rect_T> getTranslate() const noexcept {
        return H2DE_Vector2D<H2DE_Rect_T>{ x, y };
    }
    /**
     * @brief Returns size vector of the rectangle.
     */
    constexpr H2DE_Vector2D<H2DE_Rect_T> getScale() const noexcept {
        return H2DE_Vector2D<H2DE_Rect_T>{ w, h };
    }

    /**
     * @brief Returns minimal X coordinate of the rectangle (left edge).
     */
    constexpr H2DE_Rect_T getMinX() const noexcept {
        return (x - static_cast<H2DE_Rect_T>(w * 0.5f));
    }
    /**
     * @brief Returns maximal X coordinate of the rectangle (right edge).
     */
    constexpr H2DE_Rect_T getMaxX() const noexcept {
        return (x + static_cast<H2DE_Rect_T>(w * 0.5f));
    }
    /**
     * @brief Returns minimal Y coordinate of the rectangle (top edge).
     */
    constexpr H2DE_Rect_T getMinY() const noexcept {
        return (y - static_cast<H2DE_Rect_T>(h * 0.5f));
    }
    /**
     * @brief Returns maximal Y coordinate of the rectangle (bottom edge).
     */
    constexpr H2DE_Rect_T getMaxY() const noexcept {
        return (y + static_cast<H2DE_Rect_T>(h * 0.5f));
    }

    /**
     * @brief Returns a new rectangle with the X component set to a given value.
     * @param value New value for the X component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> setX(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ value, y, w, h };
    }
    /**
     * @brief Returns a new rectangle with the Y component set to a given value.
     * @param value New value for the Y component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> setY(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, value, w, h };
    }
    /**
     * @brief Returns a new rectangle with the W component set to a given value.
     * @param value New value for the W component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> setW(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, value, h };
    }
    /**
     * @brief Returns a new rectangle with the H component set to a given value.
     * @param value New value for the H component.
     */
    constexpr H2DE_Rect<H2DE_Rect_T> setH(H2DE_Rect_T value) const noexcept {
        return H2DE_Rect<H2DE_Rect_T>{ x, y, w, value };
    }

    /**
     * @brief Checks if this rectangle collides with another rectangle.
     * @param rect Rectangle to test collision against.
     * @return true if rectangles overlap.
     */
    constexpr bool collides(const H2DE_Rect<H2DE_Rect_T>& rect) const noexcept {
        return (
            (H2DE::abs(x - rect.x) < ((w + rect.w) * 0.5f)) &&
            (H2DE::abs(y - rect.y) < ((h + rect.h) * 0.5f))
        );
    }
    /**
     * @brief Checks if a point is inside this rectangle.
     * @param translate Point position vector.
     * @return true if point lies within rectangle bounds.
     */
    constexpr bool collides(const H2DE_Vector2D<H2DE_Rect_T>& translate) const noexcept {
        return (
            (translate.x >= x - w * 0.5f) &&
            (translate.x <= x + w * 0.5f) &&
            (translate.y >= y - h * 0.5f) &&
            (translate.y <= y + h * 0.5f)
        );
    }
    /**
     * @brief Checks collision with a circle (point + radius).
     * @param translate Circle center point.
     * @param radius Circle radius.
     * @return true if circle intersects rectangle.
     */
    bool collides(const H2DE_Vector2D<H2DE_Rect_T>& translate, float radius) const noexcept;
    /**
     * @brief Gets the face of this rectangle collided with another.
     * @param rect Rectangle to test against.
     * @return Optional face enum representing the side collided, or nullopt if none.
     */
    const std::optional<H2DE_Face> getCollidedFace(const H2DE_Rect<H2DE_Rect_T>& rect) const noexcept;
    /**
     * @brief Snaps this rectangle to another rectangle along the specified face.
     * @param rect Rectangle to snap to.
     * @param face Face of the rectangle to align.
     */
    void snap(const H2DE_Rect<H2DE_Rect_T>& rect, H2DE_Face face) noexcept;

    /**
     * @brief Returns an array of the 4 corners of the rectangle.
     * @return std::array with corners ordered (top-left, top-right, bottom-right, bottom-left).
     */
    std::array<H2DE_Vector2D<H2DE_Rect_T>, 4> getCorners() const noexcept;
};

/** @brief Alias for rectangle with integer components (pixels). */
using H2DE_PixelRect = H2DE_Rect<int>;
/** @brief Alias for rectangle used in level coordinates (float). */
using H2DE_LevelRect = H2DE_Rect<float>;
