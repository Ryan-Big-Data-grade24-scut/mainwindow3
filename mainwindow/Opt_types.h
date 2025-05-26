#include "GaussianBlurOpt.h"
#include "GrayOpt.h"
#include "CannyOpt.h"

// 所有操作类型的类型列表
using OperationTypes = std::tuple<
    BlurOperation, 
    GrayOperation,
    CannyOperation
    /* 后续添加的新操作直接在这里扩展 */
>;
