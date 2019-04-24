/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file testsdepotmodel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "pugixml.hpp"
#include "testsdepotmodel.h"
#include <experimental/filesystem>
#include <QKeyEvent>

namespace fs = std::experimental::filesystem;

TestsDepotModel::TestsDepotModel()
{
    bool ok=QObject::connect(this,&QStandardItemModel::itemChanged, this, &TestsDepotModel::onChanged);
    Q_ASSERT(ok);
}

void TestsDepotModel::load(const std::string& fileName)
{
    size_t pos = fileName.find_last_of("/");
    testPath_=fileName.substr(0,pos);
    pugi::xml_parse_result result = doc_.load_file(fileName.c_str());
    redraw();
}

void TestsDepotModel::redraw()
{
    clear();
    pugi::xpath_node_set commands = doc_.select_nodes("/testlist/test");
    QStandardItem *item = invisibleRootItem();
    for (pugi::xpath_node current:commands)
    {
        std::string code=current.node().attribute("code").value();
        std::string repetitions=current.node().attribute("repetitions").value();
        std::string file=current.node().attribute("file").value();
        bool missingFile{false};
        size_t pos=testPath_.find_last_of("/");
        std::string smallpath=testPath_.substr(0,pos);
        if(!fs::exists(smallpath+"/"+file))
        {
            missingFile=true;
        }

        QStandardItem* nameItem = new QStandardItem(code.c_str());
        nameItem->setIcon(QIcon(":/icons/envelope.png"));

        QStandardItem* repetitionItem = new QStandardItem(repetitions.c_str());
        QStandardItem* fileItem = new QStandardItem(file.c_str());

        QList<QStandardItem*> itemList;
        itemList<<nameItem;
        itemList<<repetitionItem;
        itemList<<fileItem;
        if(missingFile)
        {
            fileItem->setBackground(Qt::red);
        }

        QStringList list{"","",""};
        list[URFfile]=file.c_str();
        list[URFcode]=code.c_str();
        nameItem->setData(list,Qt::UserRole);
        repetitionItem->setData(list,Qt::UserRole);
        fileItem->setData(list,Qt::UserRole);

        item->appendRow(itemList);
    }

    setHeaderData(0,Qt::Horizontal,"Code");
    setHeaderData(1,Qt::Horizontal,"Repetitions");
    setHeaderData(2,Qt::Horizontal,"File");
}

void TestsDepotModel::save(const std::string& fileName,const pugi::xml_document& prerequisiteDoc)
{
    //Delete old prerequisite
    pugi::xpath_node_set prerequisiteOldNodes = doc_.select_nodes("/testlist/prerequisite");
    for(const pugi::xpath_node& current:prerequisiteOldNodes)
    {
        current.parent().remove_child(current.node());
    }

    //Add new prerequisite
    pugi::xpath_node testListRoot = doc_.select_node("/testlist");
    pugi::xpath_node_set prerequisiteNewNodes = prerequisiteDoc.select_nodes("/testlist/prerequisite");
    for(const pugi::xpath_node& current:prerequisiteNewNodes)
    {
        testListRoot.node().append_copy(current.node());
    }

    doc_.save_file((fileName).c_str());
}

void TestsDepotModel::onChanged(QStandardItem * item)
{
    std::string newValue=item->text().toStdString();

    std::string toModify;
    int column=item->column();
    int row=item->row();
    if(column==0)
        toModify="code";
    else if(column==1)
        toModify="repetitions";
    else if(column==2)
        toModify="file";

    QStringList itemList;
    itemList=item->data(Qt::UserRole).toStringList();
    QString code;
    code=itemList[URFcode];

    auto commands = doc_.select_nodes("/testlist/test");
    for (auto &current:commands)
    {
        std::string currentCode=current.node().attribute("code").value();
        if(code.toStdString()==currentCode)
        {
            current.node().attribute(toModify.c_str())=newValue.c_str();
        }
    }

    redraw();
}

void TestsDepotModel::deleteTest(const QModelIndex& index)
{
    QStandardItem* item;
    item=itemFromIndex(index);
    std::string newValue=item->text().toStdString();

    QStringList itemList;
    itemList=item->data(Qt::UserRole).toStringList();
    QString code;
    code=itemList[URFcode];

    auto commands = doc_.select_nodes("/testlist/test");
    for (auto &current:commands)
    {
        std::string currentCode=current.node().attribute("code").value();
        if(code.toStdString()==currentCode)
        {
            current.node().parent().remove_child(current.node());
        }
    }
    removeRow(index.row(),index.parent());
}

void TestsDepotModel::newTest(const QModelIndex& index)
{
    pugi::xpath_node rootPath = doc_.select_node("/testlist");
    pugi::xml_node root=rootPath.node();
    pugi::xml_node added=root.append_child("test");
    added.append_attribute("code").set_value("9999");
    added.append_attribute("file").set_value("xxx");
    added.append_attribute("repetitions").set_value(0);
    added.append_attribute("loggingtype");
    added.append_attribute("loggingpart");
    added.append_attribute("loggingwrappername");

    redraw();
}


void TestsDepotModel::keypressed(QEvent* e,const QModelIndex &index)
{
    QString pressedK=((QKeyEvent*)e)->text();
    keypressed(pressedK,index);
}

void TestsDepotModel::keypressed(const QString& pressedK,const QModelIndex& index)
{
    QStandardItem *root = invisibleRootItem();

    if(pressedK=="\177") //DEL
    {
        deleteTest(index);
    }
}


