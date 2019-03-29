/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file txTestList.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#pragma once

#include "general.h"
#include "type.h"
#include "test.h"

class Test;

class TestsDepot: public std::enable_shared_from_this<TestsDepot>
{
public:
    static std::string robotStr_;
    static std::string waitcommand_;
    std::string loggingcommand_;
    double loggingTime_{0.010};

    explicit TestsDepot();
    ~TestsDepot();
    bool load(const std::string& path);
    bool execute() const;

    bool valid() const;
   
    std::set<std::string> commandOnlySim_;   

private:
    const std::string testName_{"./test/test.xml"};    

    unsigned int repetitions_{0};
    bool realRobot_{false};

    std::vector<Test_sptr> data_;

    pugi::xml_document doc_;

    void loadSimulationCommand();
};
