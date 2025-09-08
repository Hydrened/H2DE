#pragma once

#include <SDL2/SDL_video.h>
#include <H2DE/utils/H2DE_utils.h>

#define H2DE_VERSION_MAJOR 4
#define H2DE_VERSION_MINOR 3
#define H2DE_VERSION_PATCH 5
#define H2DE_VERSION_BETA 0

#define H2DE_STR_HELPER(x) #x
#define H2DE_STR(x) H2DE_STR_HELPER(x)

#if H2DE_VERSION_BETA > 0
    #define H2DE_VERSION \
        H2DE_STR(H2DE_VERSION_MAJOR) "." \
        H2DE_STR(H2DE_VERSION_MINOR) "." \
        H2DE_STR(H2DE_VERSION_PATCH) "-b" \
        H2DE_STR(H2DE_VERSION_BETA)
#else
    #define H2DE_VERSION \
        H2DE_STR(H2DE_VERSION_MAJOR) "." \
        H2DE_STR(H2DE_VERSION_MINOR) "." \
        H2DE_STR(H2DE_VERSION_PATCH)
#endif

#define H2DE_VERSION_NUM (H2DE_VERSION_MAJOR * 10000 + H2DE_VERSION_MINOR * 100 + H2DE_VERSION_PATCH)



/** @brief Minimum value for unsigned 8-bit integer. */
#define H2DE_UINT8_MIN 0u
/** @brief Maximum value for unsigned 8-bit integer. */
#define H2DE_UINT8_MAX 255u

/** @brief Minimum value for opacity (fully transparent). */
#define H2DE_OPACITY_MIN H2DE_UINT8_MIN
/** @brief Maximum value for opacity (fully opaque). */
#define H2DE_OPACITY_MAX H2DE_UINT8_MAX

/** @brief Minimum value for 32-bit signed index. */
#define H2DE_INDEX_MIN (-2147483648)
/** @brief Maximum value for 32-bit signed index. */
#define H2DE_INDEX_MAX 2147483647

/** @brief Minimum volume value. */
#define H2DE_VOLUME_MIN 0
/** @brief Maximum volume value. */
#define H2DE_VOLUME_MAX 100

/** @brief Constant used to define the maximum length of an input. */
#define H2DE_INPUT_MAX_LENGTH 65535u



/** @brief Special value representing an invalid delay ID. */
#define H2DE_INVALID_DELAY_ID 4294967295u
/** @brief Special value representing an invalid timeline ID. */
#define H2DE_INVALID_TIMELINE_ID 4294967295u

/** @brief Special value representing an invalid channel ID. */
#define H2DE_INVALID_CHANNEL_ID 255u



/** @brief Null pointer used to represent a non-existent object. */
#define H2DE_NULL_OBJECT nullptr
/** @brief Null pointer used to represent a non-existent surface. */
#define H2DE_NULL_SURFACE nullptr

/** @brief Constant used to represent an infinite loop in timelines or animations. */
#define H2DE_INFINITE_LOOP 4294967295u

/** @brief Constant used to center a window on the screen when creating it. */
#define H2DE_WINDOW_POS_CENTERED SDL_WINDOWPOS_CENTERED
