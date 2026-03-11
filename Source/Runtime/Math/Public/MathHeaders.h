#pragma once

#include <cassert>

#include <math.h>

#if defined(_MSC_VER)
#include <intrin.h>
#endif
#include "Platform.h"

#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <algorithm>

#include "MathUtil.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

#include "ScreenPoint.h"

#include "Color32.h"
#include "LinearColor.h"
#include "HSVColor.h"

#include "Rotator.h"

#include "Quaternion.h"

#include "Transform.h"

#include "Plane.h"

#include "Circle.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "Box.h"

#include "Frustum.h"

using namespace CK;
