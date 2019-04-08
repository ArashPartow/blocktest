/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionCheckRobot.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionCheckRobot.h"
#include "test.h"
#include "logger.h"
#include "report.h"
#include "testsDepot.h"
#include "yarpActionDepotStart.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>

ACTIONREGISTER_DEF_TYPE(ActionCheckRobot,"checkrobot");

ActionCheckRobot::ActionCheckRobot(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{
    wrapperPrefix_=nodeCommand.attribute("wrappername").value();
}     

bool ActionCheckRobot::execute(unsigned int testrepetition)
{
    yarp::dev::IEncoders       *iencs;
    int nj;

    if(!YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(iencs))
    {
        TXLOG(Severity::critical)<<"Unable to view IEncoder interface"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Unable to view IEncoder interface");
    }
    if(!iencs->getAxes(&nj))
    {
        TXLOG(Severity::critical)<<"getAxes failed"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"getAxes failed");
    }        
    return true;
}
