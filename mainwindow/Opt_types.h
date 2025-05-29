#pragma once
#include "GaussianBlurOpt.h"
#include "GrayOpt.h"
#include "cannyOpt.h"
#include "effectAOpt.h"
#include "effectBOpt.h"
#include "effectCOpt.h"
#include "effectDOpt.h"
#include "effectEOpt.h"


// 所有操作类型的类型列表
using OperationTypes = std::tuple<
    BlurOperation,
    GrayOperation,
    CannyOperation,
    effectAOperation,
    effectBOperation,
    effectCOperation,
    effectDOperation,
    effectEOperation
    /* 后续添加的新操作直接在这里扩展 */
>;