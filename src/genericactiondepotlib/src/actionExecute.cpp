/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionExecute.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionExecute.h"
#include "logger.h"
#include "report.h"
#include "testsDepot.h"

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionExecute,"execute");

namespace GenericActions
{
std::map<std::string,std::shared_ptr<boost::process::child>> ActionExecute::processes_;

ActionExecute::ActionExecute(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
}   

void ActionExecute::beforeExecute()
{
    getCommandAttribute("tag",tag_);
    getCommandAttribute("command",commandName_);
    getCommandAttribute("param",commandParam_);
    getCommandAttribute("prefix",prefix_);
    getCommandAttribute("waitafter",waitafter_);
    getCommandAttribute("kill",kill_); 
    getCommandAttribute("nobackground",nobackground_);
    getCommandAttribute("usetestpath",useTestPath_);
}

execution ActionExecute::execute(const TestRepetitions& testrepetition)
{
    std::string tagTmp=normalizeSingle(tag_,false);
    if(kill_)
    {
        if(processes_.find(tagTmp)==processes_.end())
        {
            std::stringstream ss;
            ss<<"tag "<<tagTmp <<" not found for kill."<<std::endl;
            addProblem(testrepetition,Severity::critical,ss.str(),true);
            return execution::stopexecution;
        }

        auto process=processes_[tagTmp];
        process->terminate();
        processes_.erase(tagTmp);
        std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
        return execution::continueexecution;
    }
    std::stringstream ss;
    if(useTestPath_)
       ss<<TestsDepot::path_<<"/";
    if(!prefix_.empty())
        ss<<prefix_<<" ";

    ss<<commandName_<<" "<<normalize(commandParam_,false);

    if(!nobackground_)
        ss<<" &";

    if(processes_.find(tagTmp)!=processes_.end())
    {
        TXLOG(Severity::warning)<<"Repetition "<<testrepetition.testRepetitions_<<": duplicate execute command tag "<<tagTmp<<std::endl;
        //return execution::stopexecution;
    }

    TXLOG(Severity::debug)<<"Executing:"<<ss.str()<<std::endl;
    try
    {
        auto process=std::make_shared<boost::process::child>(ss.str());
        processes_[tagTmp]=process;
    }
    catch(const std::exception& e)
    {
        TXLOG(Severity::error)<<"Execution "<<e.what()<<std::endl;
        return execution::continueexecution;
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
    return execution::continueexecution;
}

}