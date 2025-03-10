/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file tablenormal.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "api.h"
#include "general.h"
#include "table.h"

namespace BlockTestCore
{

class TableNormal: public Table
{ 
    public:
        bool Init(std::vector<std::string> tableValue) override;

        std::string get() override;
        std::string fetch() override;
        std::string get(unsigned int position) const override;

};
}