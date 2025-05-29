#pragma once
#include "GaussianBlurOpt.h"
#include "GrayOpt.h"
#include "cannyOpt.h"
#include "effectAOpt.h"
#include "effectBOpt.h"
#include "effectCOpt.h"
#include "effectDOpt.h"
#include "effectEOpt.h"


// ���в������͵������б�
using OperationTypes = std::tuple<
    BlurOperation,
    GrayOperation,
    CannyOperation,
    effectAOperation,
    effectBOperation,
    effectCOperation,
    effectDOperation,
    effectEOperation
    /* ������ӵ��²���ֱ����������չ */
>;