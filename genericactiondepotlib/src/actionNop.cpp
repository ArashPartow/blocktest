/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionNop.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "actionNop.h"
#include "logger.h"
#include "tables.h"
#include "report.h"

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionNop,"nop");

namespace GenericActions
{

ActionNop::ActionNop(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
    getCommandAttribute(commandAttributes,"fixvalue",fixvalue_);
    getCommandAttribute(commandAttributes,"tables",tables_);
    getCommandAttribute(commandAttributes,"printerror",printerror_);
    getCommandAttribute(commandAttributes,"justonetime",justOneTime_);
    getCommandAttribute(commandAttributes,"printtestinfo",printtestinfo_); 
}     

bool ActionNop::execute(unsigned int testrepetition)
{
    if(justOneTime_ && testrepetition!=0)
        return true;

    std::vector<std::string> tablesDepot;
    tokenize<std::string>(tables_,tablesDepot);
   
    std::stringstream ss;
    if(!fixvalue_.empty())
        ss<<fixvalue_<<'\t';
    
    if(printtestinfo_)
        ss<<testCode_<<'\t'<<testrepetition<<'\t'; 
    
    for(std::string current:tablesDepot)
    {
        ss<<Tables::instance().fetch(current)<<'\t';
    }

    if(printerror_)
    {
        ss<<Report::instance().get(Severity::critical,std::make_pair(testCode_,testrepetition))+Report::instance().get(Severity::error,std::make_pair(testCode_,testrepetition));
    }
        
    TXLOG(Severity::plot)<<ss.str()<<std::endl;
    return true;
}

}
