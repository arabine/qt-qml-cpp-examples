/****************************************************************************
 * Copyright (C) 2020 Anthony Rabine
 * Contact: anthony@rabine.fr
 * License: MIT, see LICENSE file
 ****************************************************************************/

#ifndef GITREPO_H
#define GITREPO_H

#include <string>
#include "git2.h"

class GitRepo
{
public:
    GitRepo(const std::string &path, bool valid);
    ~GitRepo();

    int Status();
    bool IsEnabled() const;
    void SetEnabled(bool enabled);
    std::string GetPath() const;
    bool IsValid() const;

private:
    git_repository *mRepo;
    std::string mPath;
    bool mIsValid;
    bool mEnabled;
};

#endif // GITREPO_H
