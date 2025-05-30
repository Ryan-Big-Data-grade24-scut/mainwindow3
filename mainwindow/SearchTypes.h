#pragma once
#include "QueryOperation.h"
#include "ContentOperation.h"
#include "GenerationOperation.h"
#include "UtilityOperation.h"
#include "FullSearchOperation.h"

using SearchOperationTypes = std::tuple<
    QueryOperation,
    ContentOperation,
    GenerationOperation,
    UtilityOperation,
    FullSearchOperation
>;