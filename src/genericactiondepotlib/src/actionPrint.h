/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionPrint.h
 * @author davide.tome@iit.it
 */

#pragma once

#include "action.h"

using namespace BlockTestCore;

namespace GenericActions
{

/**
 * @class ActionWait
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionPrint : public Action
{
    public:
        ActionPrint(const CommandAttributes& commandAttributes,const std::string& testCode);    
        execution execute(const TestRepetitions& testrepetition) override;
        void beforeExecute() override;

    private:
        std::string target_;
        std::string message_;

    ACTIONREGISTER_DEC_TYPE(ActionPrint)        
};

}
