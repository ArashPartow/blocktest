/******************************************************************************
 *                                                                            *
 * Copyright (C) 2021 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionFindInFile.h
 * @author Luca Tricerri
 */

#pragma once

#include <string>

#include "action.h"

using namespace BlockTestCore;

namespace GenericActions
{

/**
 * @class ActionWait
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionFindInFile : public Action
{
    public:
        ActionFindInFile(const CommandAttributes& commandAttributes,const std::string& testCode);    
        execution execute(const TestRepetitions& testrepetition) override;
        void beforeExecute() override;

    private:
        std::string string_;
        std::string filename_;
        bool errorOnFind_{true};
        bool bckIfError_{false};

    ACTIONREGISTER_DEC_TYPE(ActionFindInFile)        
};

}
