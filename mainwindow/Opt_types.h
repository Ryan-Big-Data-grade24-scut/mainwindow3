#include "GaussianBlurOpt.h"
#include "GrayOpt.h"
#include "CannyOpt.h"

// ���в������͵������б�
using OperationTypes = std::tuple<
    BlurOperation, 
    GrayOperation,
    CannyOperation
    /* ������ӵ��²���ֱ����������չ */
>;
