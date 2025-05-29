#pragma once
#pragma once
#include "SendPromptOperation.h"
#include "AddFavoriteOperation.h"
#include "RemoveFavoriteOperation.h"
#include "ShowFavoriteOperation.h"
#include "ShowHistoryOperation.h"
//#include "ShowImageOnPage4Operation.h"

using OllamaOperationTypes = std::tuple<
    SendPromptOperation,
    AddFavoriteOperation,
    RemoveFavoriteOperation,
    ShowFavoriteOperation,
    ShowHistoryOperation
    /*ShowImageOnPage4Operation*/
>;
