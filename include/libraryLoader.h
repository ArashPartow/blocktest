#pragma once

/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file libraryLoader.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */
#include "general.h"

class LibraryLoader
{
    public:
        LibraryLoader();
        bool load(const std::string& path);

    private:
        const std::string testName_{"./test/test.xml"};
};