#include "H2DE/H2DE_utils.h"

float H2DE_Lerp(float min, float max, float blend) {
    return min + blend * (max - min);
}
