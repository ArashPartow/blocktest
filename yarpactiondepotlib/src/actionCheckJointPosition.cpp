/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionCheckJointPosition.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionCheckJointPosition.h"
#include "test.h"
#include "logger.h"
#include "report.h"
#include "testsDepot.h"
#include "yarpActionDepotStart.h"

#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>


ACTIONREGISTER_DEF_TYPE(ActionCheckJointPosition,"yarpcheckjointposition");

ActionCheckJointPosition::ActionCheckJointPosition(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{
    wrapperPrefix_=nodeCommand.attribute("wrappername").value();
    jointname_ =nodeCommand.attribute("jointname").value();
    expectedValue_=nodeCommand.attribute("expectedvalue").as_double();
    tolerance_=nodeCommand.attribute("tolerance").as_double();
}     

bool ActionCheckJointPosition::execute(unsigned int testrepetition)
{
    yarp::dev::IEncoders *iencoders=nullptr;
    
    if(!YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(iencoders))
    {
        TXLOG(Severity::error)<<"Unable to open encoder control mode interface 2"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Unable to open encoder control mode interface 2");        
        return false;
    }

    int nj{0};
    if(!iencoders->getAxes(&nj))
    {
        TXLOG(Severity::error)<<"Unable to open encoder control mode interface 3"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Unable to open encoder control mode interface 3");
        return false;    
    }       
         
    std::map<std::string, int> jointNames;
    getJointNames(*YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_],jointNames);         

    auto it=jointNames.find(jointname_);
    if(it==jointNames.end())
    {
        TXLOG(Severity::error)<<"Joint not found:"<<jointname_<<std::endl;   
        addProblem(test_->code_,testrepetition,Severity::critical,"Joint not found:");
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    std::vector<double> positions(nj,0.0);
    while(!iencoders->getEncoders(positions.data()))
    {
        TXLOG(Severity::debug)<<"Unable to open encoder control mode interface 4:"<<positions[5]<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));    
    }
    
    double ref=positions[it->second];
    if(expectedValue_>ref+tolerance_ || expectedValue_<ref-tolerance_)
    {
        TXLOG(Severity::error)<<"Joint position check value:"<<ref<<" expected:" <<expectedValue_<<" tolerance:"<<tolerance_ <<" name:"<<jointname_<<std::endl;   
        addProblem(test_->code_,testrepetition,Severity::error,"Joint position check value");
    }
    else
    {
        TXLOG(Severity::debug)<<"Joint position check value ok:"<<ref<<" expected:" <<expectedValue_<<" tolerance:"<<tolerance_ <<" name:"<<jointname_<<std::endl;   
    }
    return true;
}
