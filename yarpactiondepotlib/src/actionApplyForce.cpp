/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionApplyForce.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionApplyForce.h"
#include "test.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>

#include "logger.h"
#include "report.h"

ACTIONREGISTER_DEF_TYPE(ActionApplyForce,"applyforce");

ActionApplyForce::ActionApplyForce(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{
    force_ =nodeCommand.attribute("force").value();
}     

bool ActionApplyForce::execute(unsigned int testrepetition)
{
    std::string localExtWrenchPort = "/myPortForExternalWrench:o";
    std::string remoteExtWrenchPort = "/isaac-next-proto-yarp-modelica-fixed/applyExternalWrench/rpc:i";
    yarp::os::RpcClient extWrenchPort;
    bool ok=extWrenchPort.open(localExtWrenchPort);
   if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to open ports applyforce"<<std::endl;        
        addProblem(test_->code_,testrepetition,Severity::critical,"Unable to open ports applyforce");
        return false;
    }

    yarp::os::Network::connect(localExtWrenchPort,remoteExtWrenchPort);

    std::istringstream iss(force_);
    std::vector<std::string> tokenized(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>());

    if(tokenized.size()!=8)
    {
        TXLOG(Severity::error)<<"Error in parameter number for applyForce"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Error in parameter number for applyForce");
        return false;
    }

    yarp::os::Bottle cmd;
    cmd.addString(tokenized[0]);
    cmd.addDouble(std::atof(tokenized[1].c_str()));
    cmd.addDouble(std::atof(tokenized[2].c_str()));
    cmd.addDouble(std::atof(tokenized[3].c_str()));
    cmd.addDouble(std::atof(tokenized[4].c_str()));
    cmd.addDouble(std::atof(tokenized[5].c_str()));
    cmd.addDouble(std::atof(tokenized[6].c_str()));
    cmd.addDouble(std::atof(tokenized[7].c_str()));

    yarp::os::Bottle response;
    extWrenchPort.write(cmd,response);
  
    if(response.toString().empty())
    {
          TXLOG(Severity::critical)<<"No response from ExternalWrench plugin"<<std::endl;        
          addProblem(test_->code_,testrepetition,Severity::critical,"No response from ExternalWrench plugin");
    }
      
    extWrenchPort.interrupt();
    extWrenchPort.close();    
    return true;
}
