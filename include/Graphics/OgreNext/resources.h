//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_RESOURCES_H
#define AMORPHOUS_ENGINE_RESOURCES_H

#include <string>

#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreResourceManager.h>

void loadResources(const std::string &filepath) {
    // Loading resources
    Ogre::ConfigFile cf;
    cf.load(filepath);

    Ogre::ConfigFile::SectionIterator it = cf.getSectionIterator();
    Ogre::String section, type, arch;
    while (it.hasMoreElements()) {
        section = it.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = it.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            type = i->first;
            arch = i->second;
            if (type == "DoNotUseAsResource")
                continue;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, section);
        }
    }

    // Initialise resources groups
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);
}

void loadResourcesForProject(const std::string &projectRoot) {
    Ogre::ResourceGroupManager::getSingleton()
            .addResourceLocation(
                    projectRoot,
                    "FileSystem",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::ResourceGroupManager::getSingleton()
            .addResourceLocation(
                    projectRoot + "/Models",
                    "FileSystem",
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    // Initialise resources groups
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);
}

#endif //AMORPHOUS_ENGINE_RESOURCES_H
