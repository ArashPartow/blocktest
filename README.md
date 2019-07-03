# 1. CI Status

[![Build Status (Linux) (master branch)](https://img.shields.io/travis/com/robotology/blocktest/master.svg?logo=travis&label=[master]%20build%20(Linux))](https://travis-ci.com/robotology/blocktest)
[![Build Status (Linux) (devel branch)](https://img.shields.io/travis/com/robotology/blocktest/devel.svg?logo=travis&label=[devel]%20build%20(Linux))](https://travis-ci.com/robotology/blocktest)

# 2. Block Test

- [1. CI Status](#1-CI-Status)
- [2. Block Test](#2-Block-Test)
- [3. Introduction](#3-Introduction)
- [4. Installation](#4-Installation)
  - [4.1. Prerequisite Linux](#41-Prerequisite-Linux)
  - [4.2. Prerequisite Windows](#42-Prerequisite-Windows)
  - [4.3. Installation and compilation](#43-Installation-and-compilation)
- [5. Test writing](#5-Test-writing)
  - [5.1. General Settings](#51-General-Settings)
  - [5.2. Library Settings](#52-Library-Settings)
  - [5.3. Prerequisites](#53-Prerequisites)
  - [5.4. Test list](#54-Test-list)
    - [5.4.1. Parallel or serie execution](#541-Parallel-or-serie-execution)
    - [5.4.2. Repetitions](#542-Repetitions)
  - [5.5. Finally the test](#55-Finally-the-test)
    - [5.5.1. Generic commands](#551-Generic-commands)
  - [5.6. Test writing with Scriptbuilder](#56-Test-writing-with-Scriptbuilder)
- [6. Parametric test](#6-Parametric-test)
  - [6.1. Table](#61-Table)
  - [6.2. Using the parameter](#62-Using-the-parameter)
  - [6.3. Examples](#63-Examples)
- [7. Test execution](#7-Test-execution)
- [8. Logging](#8-Logging)
  - [8.1. Application logging](#81-Application-logging)
  - [8.2. Sensors logging](#82-Sensors-logging)
  - [8.3. Plot logging](#83-Plot-logging)
  - [8.4. Report](#84-Report)
- [9. Specific plugin writing](#9-Specific-plugin-writing)
  - [9.1. Create new repository for plugin](#91-Create-new-repository-for-plugin)
  - [9.2. Create plugin initialization](#92-Create-plugin-initialization)
  - [9.3. Blocks writing](#93-Blocks-writing)
  - [9.4. XML files](#94-XML-files)
  - [9.5. Yarp BlockTest plugin](#95-Yarp-BlockTest-plugin)
- [10. Authors](#10-Authors)

# 3. Introduction

The application provides functionalities for developing and running not regression tests in a likely natural language as close as possible to test case writing.
The test philosophy is to divide a test into elementary blocks. The
blocks can be used to build different tests (See Figure below).
<br/><br/>


![alt text](img/img001.png "The tests are made by elementary blocks.")
<br/><br/>

# 4. Installation

Supported OS: Linux Ubuntu 18.10 and Windows 10.
Boost library version must be >1.64.


## 4.1. Prerequisite Linux

```bash
sudo apt-get install -y cmake libboost-all-dev libpugixml-dev qtbase5-dev qtdeclarative5-dev qtmultimedia5-dev libqt5opengl5-dev libqcustomplot-dev
```
## 4.2. Prerequisite Windows

```bash
vcpkg install boost:x86-windows
vcpkg install pugixml
```
Qt5 from binary https://doc.qt.io/qt-5/windows.html

## 4.3. Installation and compilation

In order to compile just execute the following commands in a bash
terminal.
```bash
git clone https://github.com/robotology/blocktest     
cd blocktest
mkdir build
cd build
cmake ..
make -j 4
```

Or it is possible to select the Scriptbuilder integration options. Note that it is active by default.

```bash
ccmake ..
<select options>
<type c>
<type g>
<type e>

```
![alt text](img/img003.png "Tarp and Scriptbuilder options.")

Select your preferred installation folder using:
CMAKE_INSTALL_PREFIX


# 5. Test writing

If you don't like work with a text editor you can skip directly to the section [Scriptbuilder](##4.7.-test-writing-with-scriptbuilder). <br>
Else the starting point for writing a test is the file ./test/test.xml

```xml
    <testlist repetitions="1">

   <settings  realrobot="false" onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce"  
            neverexecutecommands="" logseverity="debug" loggingtime="0.01"  tablename="test/tables/main.tab" 
            waitcommand="yarpwait" nowcommand="yarpnow" loggingcommand="infologgeryarp"/> <!--loggingtime in sec-->

    <prerequisite enabled="true" command="yarpserver" waitafter="5000" param="--silent" prefix="" kill="true"/>
    <prerequisite enabled="false" command="gzserver" waitafter="5000" param="--verbose -e ode --profile ode_default -slibgazebo_yarp_clock.so icub.world" prefix="" kill="true"/>
    <prerequisite enabled="true" command="gzserver" waitafter="5000" param="--verbose -e ode --profile ode_default -slibgazebo_yarp_clock.so icub_fixed.world" prefix="" kill="true"/>
    <prerequisite enabled="false" command="gzclient" waitafter="5000" param="" prefix="" kill="true"/>


    <!--Libraries-->
    <library enabled="true" path="genericactiondepotlib/libgenericactiondepot.so" name="genericactiondepot" note="System generic action library"/>
    <library enabled="true" path="yarpactiondepotlib/libyarpactiondepot.so" name="yarpactiondepot" note="Yarp action library" />

    <!--Libraries settings-->
    <librarysettings enabled="true" name="genericactiondepot"/>
    <librarysettings enabled="true" name="yarpactiondepot" wrappername="/right_leg /left_leg /torso /head /right_arm /left_arm" robotname="icubSim" netclock="true"/>
   
    
    <!--**************************-->
    <!--**********Tests***********-->
    <!--**************************-->

    <!--ICub pos && directpos-->
    <test file="test//0001.xml" repetitions="2" name="ICub right ankle roll move"  code="0001"  loggingtype="position" loggingwrappername="/right_leg" loggingpart="r_ankle_roll r_ankle_pitch"/>

    </testlist>   
```

This file contains:
 * the simulation general settings
 * the used libraries
 * the prerequisites
 * the tests link list

## 5.1. General Settings

```xml
<settings robotname="icubSim" realrobot="false" onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" netclock="true" 
            neverexecutecommands="" logseverity="debug" loggingtime="0.01"  tablename="test/tables/main.tab" 
            waitcommand="yarpwait" nowcommand="yarpnow" loggingcommand="infologgeryarp"/> <!--loggingtime in sec-->
```

| Param name           | Default value        | Comment                                                                                        |
| -------------------- | -------------------- | ---------------------------------------------------------------------------------------------- |
| realrobot            | false                | Indicates if it is a real robot in test, or it is a Gazebo simulation                          |
| onlysimcommands      | yarpreset applyForce | Indicates which are the commands to be executed only in simulation                             |
| neverexecutecommands | ---                  | Indicates which are the commands not to be executed.                                           |
| logseverity          | info                 | Indicates the severity to be logged in log.log                                                 |
| loggingtime          | 0.01                 | Indicates the time in seconds for logging joint info if are requested in test.                 |
| tablename            | test/tables/main.tab | Indicates the table name for parametric value                                                  |
| waitcommand          | yarpwait             | Indicates the command blocks to be used for wait                                               |
| nowcommand           | yaronow              | Indicates the command blocks to be used for now                                                |
| loggingcommand       | infologgeryarp       | Indicates the command blocks to be used for logging                                            |

## 5.2. Library Settings
In this section it is possible to specify the plugin library to be used.

```xml
<library enabled="true" path="genericactiondepotlib/libgenericactiondepot.so" name="genericactiondepot" note="System generic action library"/>
<library enabled="true" path="yarpactiondepotlib/libyarpactiondepot.so" name="yarpactiondepot" note="Yarp action library" />
```

| Param name | Default value | Comment                          |
| ---------- | ------------- | -------------------------------- |
| enabled    | true          | If these settings will be loaded |
| name       | ---           | library tag name                 |
| ...        | ...           | ...                              |

```xml
<!--Libraries settings-->
<librarysettings enabled="true" name="genericactiondepot"/>

<librarysettings enabled="true" name="yarpactiondepot" wrappername="/right_leg /left_leg /torso /head /right_arm /left_arm" robotname="icubSim" netclock="true"/>
```

| Param name | Default value | Comment                       |
| ---------- | ------------- | ----------------------------- |
| enabled    | true          | If the library will be loaded |
| path       | ---           | Librari .so file path         |
| name       | ---           | library tag name              |
| note       | ---           | Explanation notes             |
| robotname            | icubSim              | Robot name to be used
| simclock             | true                 | Indicates if the network clock should be used. In the case it is used the one on "/clock" port |
In this section it is also possible to specify the plugin library settings.



## 5.3. Prerequisites

The prerequisites are applications to be executed before the tests, if
necessary, and their parameters.

```xml
 <prerequisite enabled="true" command="gzserver" waitafter="5000" param="--verbose -e ode --profile ode_default -slibgazebo_yarp_clock.so icub_fixed.world" prefix="" kill="true"/>
```
| Param name | Default value | Comment                                                          |
| ---------- | ------------- | ---------------------------------------------------------------- |
| enabled    | true          | If the prerequisite will be loaded                               |
| command    | ---           | Command to be executed.                                          |
| waitafter  | 5000          | Time to wait after command execution.                            |
| param      | ---           | Command parameters.                                              |
| prefix     | ---           | Command prefixes.                                                |
| kill       | true          | The current prerequisite will be killed at the end of the tests. |


## 5.4. Test list

The test list includes all the tests written. The test list, basically,
list the tests together with the **file** in which the test has been written.

```xml
<test file="test//0001.xml" repetitions="2" repetitionsfortime="" name="ICub right ankle  roll move"  code="0001"  parallel="false"/>

<test file="test//0100.xml" repetitions="2" repetitionsfortime="" name="ICub right ankle roll pwm injection"  code="0100"  parallel="false"/>

<test file="test//0110.xml" repetitions="0" repetitionsfortime="" name="ICub right ankle roll pwmtrain injection"  code="0110" parallel="false"/>

```

| Param name         | Default value | Comment                                                                                          |
| ------------------ | ------------- | ------------------------------------------------------------------------------------------------ |
| file               | ---           | File in which the test is written.                                                               |
| repetitions        | 1             | How many times the test is repeated.                                                             |
| name               | ---           | Test name.                                                                                       |
| code               | ---           | Numeric code for identifying the test, could be related to test case.                            |
| parallel           | false         | Should the test be executed in paralell.                                                         |
| repetitionsfortime | 0             | If specified is the time to repeat the test.                                                     |

### 5.4.1. Parallel or serie execution
It is possibile to specify if some tests are executed in series or parallel.

```xml
<test file="test//0001.xml" repetitions="1" repetitionsfortime="" name="xxx"  code="0001"  parallel="false"/>
<test file="test//0002.xml" repetitions="1" repetitionsfortime="" name="xxx"  code="0002"  parallel="true"/>
<test file="test//0003.xml" repetitions="1" repetitionsfortime="" name="xxx"  code="0003"  parallel="true"/>
<test file="test//0004.xml" repetitions="1" repetitionsfortime="" name="xxx"  code="0004"  parallel="true"/>
<test file="test//0005.xml" repetitions="1" repetitionsfortime="" name="xxx"  code="0005"  parallel="false"/>
```
In the above example the tests will be executed in the following way:

**0001** start in serie

**0002-0003-0004** start in parallel when 0001 is finished

**0005** start when all 0002-0003-0004 are finished

In the case it is possibile to insert a dummy test in serie to  align the following tests executions.

### 5.4.2. Repetitions
Repetitions can be at action or test or test list level trought **"repetitions"** key (in action,test,test list).
At test level repetitions can be also used throught the key **"repetitionsfortime"**. In this case will be executed as many repetitions as the timer in seconds will allow. Inside of test is available 
the key **"wait"** that indicates the time between two tests executions.

## 5.5. Finally the test

The test is written in a separate file. Here is shown an example of a
simple test. Also take a look at the following section for graphical test creation.
```xml
<testbody>
    <info note="ICub right ankle roll move." shortnote="" version="1"/>
    <logging loggingtype="position" loggingpart="r_ankle_roll r_ankle_pitch" loggingwrappername="/right_leg" />
    <settings wait="10" />	
    
    <command name="yarpreset" repetitions="1" wait="0" reporterror="true"/>
    <command name="yarpsenddirectpos" wrappername="/right_leg" jointname="r_ankle_roll" degree="20" repetitions="1" wait="0" reporterror="true"/>    
</testbody>
```  

The test is composed by **commands** and each command has a parameters list.

The **common parameters** are shown in the following table.

| Param name  | Default | Comment                                                   |
| ----------- | ------- | --------------------------------------------------------- |
| name        | ---     | Command name.                                             |
| repetitions | 1       | How many times the command is repeated.                   |
| wait        | 0       | Wait time ad the end of the command execution in seconds. |
| reporterror | true    | If the command should report error if necessary.          |

The **info node** contains the following parameters:

| Param name | Default | Comment                 |
| ---------- | ------- | ----------------------- |
| note       | ---     | Test description.       |
| shortnote  | ---     | Short test description. |
| version    | ---     | Test version.           |

The **logging node** contains the following parameters:

| Param name | Default | Comment                 |
| ---------- | ------- | ----------------------- |
| loggingtype        | ---           | Indicates what kind of logging you need. For now it can be "position", "com" or both.            |
| loggingpart        | ---           | Joints name to be logged.                                                                        |
| loggingwrappername | ---           | The wrapper that controls the joints. Note that for now is possible to specify just one wrapper. |

The **settings node** contains the following parameters:

| Param name | Default | Comment                 |
| ---------- | ------- | ----------------------- |
| wait       | 0       | Wait after test completition.       |

### 5.5.1. Generic commands
These commands are contained in the generic command library plugin.

-   **wait**

    ```xml
        <command name="wait" seconds="5" repetitions="1" wait="0" reporterror="true"/>   
    ```

    The parameter in seconds, is the wait time in seconds. Could be a double 0.001 means 1 msec.


-   **nop**

    ```xml
    <command name="nop" fixvalue="" tables="kp" printerror="true" printtestinfo="true" justonetime="false" repetitions="1" wait="0" reporterror="true"/>
    ```

    The nop command is used to print log on the plot.log file.

-   **updatefile**     

    ```xml
    <command    name="updatefile" 
                sourcefile="./walkingV2PIDparameters.ini" 
                destinationfile="./test/walkingV2PIDparameters.ini" 
                repetitions="1" 
                wait="0"
                reporterror="true">
    </command>
    ```

    The command copy and update the specified file.

-   **execute**

    ```xml       
	<command    name="execute" 
                command="icubWalking" 
                param="--gazeboClock --MPC::solver_name mumps --IK::solver_name mumps" 
                prefix="" 
                waitafter="2"
                kill="false"
                repetitions="1" 
                wait="0"
                reporterror="true">
    </command>
    ```

    The command execute the specified application.

## 5.6. Test writing with Scriptbuilder
Script builder is a UI for test and test list writing.
See at https://github.com/robotology/blocktest/tree/master/scriptbuilder
for more informations.

![alt text](img/img002.png "Scriptbuilder.")


# 6. Parametric test

It is possible to execute the same test many times changing one or more parameters every execution. 

## 6.1. Table

We use a file, to specify the parameters value and parameters changing rules.

```xml
<settings robotname="icubSim" realrobot="false"  onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" simclock="true" neverexecutecommands="" logseverity="debug" loggingtime="0.008" tablename="test/tables/main.tab"/> 
```

In the settings section you can find the **tablename parameter** (**main.tab**) that is the file in which
all the tables are written.
Inside of the file  you can find the tables that contain the parameters:

```
(this is the first table)
[
kppos       (table name)
normal      (table type)
1           (increment by time)

40 
50
60
70
80
90
]

(this is the second table)
[
xvelocity   (table name)
increment   (table type)
1           (increment by time)

1           (increment unit)
1           (min)
10          (max)
]
```

Here we have two parameters, kppos and kp in two tables.
It is possible to specify how the parameters will change during the execution. There are four ways to do so, through the **table type** parameters. Four types of tables are available. All the tables have the following general parameters.

| Param position | Name | Default | Comment                 |
| ---------- | ------- | ------- | ----------------------- |
| row 1       | table name       | N/A       | The name for this table       |
| row 2       | table type       | N/A       | The type for this table       |
| row 3       | increment by type| N/A       | Every how many iterations a new value is taken        |


-   **normal:**
    in this case the parameter will have the value in the order the values are specified.
    In the above example the kppos parameter will have the value, in order, from
    the first to the last execution 40 50 60 70 80 90

| Param position | Name | Default | Comment                 |
| ---------- | ------- | ------- | ----------------------- |
| row 4...n       | value       | N/A       | Values to be taken       |

-   **increment:**
    in this case the parameter value will start from the min value to the max value using the given increment and then start again from the start min.

| Param position | Name | Default | Comment                 |
| ---------- | ------- | ------- | ----------------------- |
| row 4      | increment unit       | N/A       | Increment value every iteration       |
| row 5      | start value       | N/A       | Start value value every iteration       |
| row 6      | stop value       | N/A       | Stop value every iteration       |

-   **wave:**
    in this case the parameter value will start from the min value to the max value using the given increment and then invert the increment and continue.

| Param position | Name | Default | Comment                 |
| ---------- | ------- | ------- | ----------------------- |
| row 4      | increment unit       | N/A       | Increment value every iteration       |
| row 5      | start value       | N/A       | Start value value every iteration       |
| row 6      | stop value       | N/A       | Stop value every iteration       |

-   **expression:**
    in this case the parameter value will start from the min value to the max value using the given increment to calculate the expression.

| Param position | Name | Default | Comment                 |
| ---------- | ------- | ------- | ----------------------- |
| row 4      | increment unit       | N/A       | Increment value every iteration       |
| row 5      | start value       | N/A       | Start value value every iteration       |
| row 6      | stop value       | N/A       | Stop value every iteration       |
| row 7      | math expression       | N/A       | For full sintax look at https://github.com/ArashPartow/exprtk/blob/master/readme.txt       |

## 6.2. Using the parameter

In the test you can specify the table name you want to use, the paameter name is surround by 
$:

```xml
 <command name="setVelocity" xvelocity="$xvelocity$" yvelocity="0.5" repetitions="1" wait="0" reporterror="true"/>
```

In this case the xvelocity parameter will use the table xvelocity.

The test should be executed at least 10 times so:
```xml
    <test file="test//411.xml" repetitions="10" repetitionsfortime="" name="xxx"  code="0411"  />
```

## 6.3. Examples
Tests and example are present in folder test.

# 7. Test execution

To execute the test you should compile the test environment:

```bash
cd ./blocktest/build/bin
./blocktest
```

If you need to execute the test on the real robot use the appropriate settings:

```xml
    <settings robotname="icubSim" realrobot="true" onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" simclock="false" neverexecutecommands=""/>
```

No prerequisites are necessary in this case.

# 8. Logging

Various logging files are present in the system.

## 8.1. Application logging

Application logging is log/log.log You can read it by:\
*tail -f log.log*

It is possible to set the logs line to be shown using the attribute  logseverity="debug" inside of the

```xml
    <settings realrobot="false" onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" neverexecutecommands="" logseverity="debug" loggingtime="0.001" tablename="test/tables/main.tab" waitcommand="wait" nowcommand="now" loggingcommand="infologger" />
```

The awailable levels are:

    critical = 7
    exception = 6
    error = 5
    warning = 4
    info = 3
    trace = 2
    debug = 1
    none = 0

There are 10 logs used in a circular way.

## 8.2. Sensors logging

If sensor logging is setted (See section [Finally the test](#finally-the-test)) through
**position** parameter in loggingtype, the position log file has the format: \<test number>-\<joint name>-\<repetition number>.
If logging is setted through **com** parameter the position log file has the format: \<test number>-CoM-\<repetition number>.

```xml
<logging loggingtype="position" loggingpart="r_ankle_roll r_ankle_pitch" loggingwrappername="/right_leg" />
```  

## 8.3. Plot logging

The command **nop** write on the file log/plot.log.

## 8.4. Report

At the end of the test a report summary is been written:

(2019-05-21 11:43:24.490)(Info*****)====================================\
(2019-05-21 11:43:24.490)(Info*****)===============Report===============\
(2019-05-21 11:43:24.490)(Info*****)====================================\
(2019-05-21 11:43:24.490)(Info*****)Test code:0100 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0101 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0102 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0103 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0104 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0105 error number:0\
(2019-05-21 11:43:24.490)(Error****)Test code:0106 error number:2\
(2019-05-21 11:43:24.490)(Info*****)====================================\
(2019-05-21 11:43:24.490)(Info*****)====================================\
(2019-05-21 11:43:24.490)(Info*****)====================================\

# 9. Specific plugin writing

It is possibile to write your own plugin for blockTest. One plugin is already available
see section [Yarp BlockTest plugin](#yarp-blocktest-plugin). 


## 9.1. Create new repository for plugin
Create a new repository in github.

## 9.2. Create plugin initialization
Derive the class ActionDepotStart. The class will contains plugin initialization.

## 9.3. Blocks writing
Write your own blocks, each block is derived from CAction.

## 9.4. XML files
Write an xmk file for each blocks.

## 9.5. Yarp BlockTest plugin
An example is this plugin. It is available in https://github.com/robotology/blocktest-yarp-plugins.

# 10. Authors
* Luca Tricerri ([*personal page*](http://www.iit.it/en/people/Luca-tricerri.html))
