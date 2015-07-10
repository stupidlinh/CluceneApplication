#ifndef LINH_HELPER_CPP
#define LINH_HELPER_CPP

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include <iostream>
#include <string>
#include <stack>

using namespace std;

void ProcessDirectory(std::string, stack<string>& );
void ProcessFile(std::string);
void ProcessEntity(struct dirent*, stack<string>& );

void LINH_LIST_DIRECTORY(std::string directory, stack<string>& output)
{
    ProcessDirectory(directory, output);
}

string system__root_path = "";
void ProcessDirectory(std::string directory, stack<string>& output)
{
    std::string dirToOpen = system__root_path + directory;
    DIR *dir = opendir(dirToOpen.c_str());

    //set the new path for the content of the directory
    system__root_path = dirToOpen + "/";

    //Output dirToOpen.c_str();
    std::cout << "Process directory: " << dirToOpen.c_str() << std::endl;
    output.push(dirToOpen);

    if(NULL == dir)
    {
        std::cout << "could not open directory: " << dirToOpen.c_str() << std::endl;
        return;
    }

    struct dirent *entity = readdir(dir);

    while(entity != NULL)
    {
        ProcessEntity(entity,output);
        entity = readdir(dir);
    }

    //we finished with the directory so remove it from the path
    system__root_path.resize(system__root_path.length() - 1 - directory.length());
    closedir(dir);
}

void ProcessEntity(struct dirent* entity, stack<string>& output)
{
    //find entity type
    if(entity->d_type == DT_DIR)
    {//it's an direcotry
        //don't process the  '..' and the '.' directories
        if(entity->d_name[0] == '.')
        {
            return;
        }

        //it's an directory so process it
        ProcessDirectory(std::string(entity->d_name),output);
        return;
    }

    if(entity->d_type == DT_REG)
    {//regular file
        ProcessFile(std::string(entity->d_name));
        return;
    }

    //there are some other types
    //read here http://linux.die.net/man/3/readdir
    std::cout << "Not a file or directory: " << entity->d_name << std::endl;
}


void ProcessFile(std::string file)
{
    std::cout << "Process file     : " << file.c_str() << std::endl;
    //if you want to do something with the file add your code here
}

#endif
