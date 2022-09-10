/****************************************************************************
 * Copyright (C) 2020 Anthony Rabine
 * Contact: anthony@rabine.fr
 * License: MIT, see LICENSE file
 ****************************************************************************/

#include "GitRepo.h"

#include <string.h>

GitRepo::GitRepo(const std::string &path, bool valid)
    : mRepo(nullptr)
    , mPath(path)
    , mIsValid(valid)
    , mEnabled(true)
{

    int error = git_repository_open(&mRepo, path.data());

    mIsValid = (error == 0);
}

GitRepo::~GitRepo()
{
    git_repository_free(mRepo);
}

enum {
  FORMAT_DEFAULT   = 0,
  FORMAT_LONG      = 1,
  FORMAT_SHORT     = 2,
  FORMAT_PORCELAIN = 3,
};

#define MAX_PATHSPEC 8

struct status_opts {
  git_status_options statusopt;
  const char *repodir;
  char *pathspec[MAX_PATHSPEC];
  int npaths;
  int format;
  int zterm;
  int showbranch;
  int showsubmod;
  int repeat;
};


static void print_long(git_status_list *status)
{
  size_t i, maxi = git_status_list_entrycount(status);
  const git_status_entry *s;
  int header = 0, changes_in_index = 0;
  int changed_in_workdir = 0, rm_in_workdir = 0;
  const char *old_path, *new_path;

  for (i = 0; i < maxi; ++i) {
    const char *istatus = NULL;

    s = git_status_byindex(status, i);

    if (s->status == GIT_STATUS_CURRENT)
      continue;

    if (s->status & GIT_STATUS_WT_DELETED)
      rm_in_workdir = 1;

    if (s->status & GIT_STATUS_INDEX_NEW)
      istatus = "new file: ";
    if (s->status & GIT_STATUS_INDEX_MODIFIED)
      istatus = "modified: ";
    if (s->status & GIT_STATUS_INDEX_DELETED)
      istatus = "deleted:  ";
    if (s->status & GIT_STATUS_INDEX_RENAMED)
      istatus = "renamed:  ";
    if (s->status & GIT_STATUS_INDEX_TYPECHANGE)
      istatus = "typechange:";

    if (istatus == NULL)
      continue;

    if (!header) {
      printf("# Changes to be committed:\n");
      printf("#   (use \"git reset HEAD <file>...\" to unstage)\n");
      printf("#\n");
      header = 1;
    }

    old_path = s->head_to_index->old_file.path;
    new_path = s->head_to_index->new_file.path;

    if (old_path && new_path && strcmp(old_path, new_path))
      printf("#\t%s  %s -> %s\n", istatus, old_path, new_path);
    else
      printf("#\t%s  %s\n", istatus, old_path ? old_path : new_path);
  }

  if (header) {
    changes_in_index = 1;
    printf("#\n");
  }
  header = 0;


  for (i = 0; i < maxi; ++i) {
    const char *wstatus = NULL;

    s = git_status_byindex(status, i);

    if (s->status == GIT_STATUS_CURRENT || s->index_to_workdir == NULL)
      continue;


    if (s->status & GIT_STATUS_WT_MODIFIED)
      wstatus = "modified: ";
    if (s->status & GIT_STATUS_WT_DELETED)
      wstatus = "deleted:  ";
    if (s->status & GIT_STATUS_WT_RENAMED)
      wstatus = "renamed:  ";
    if (s->status & GIT_STATUS_WT_TYPECHANGE)
      wstatus = "typechange:";

    if (wstatus == NULL)
      continue;

    if (!header) {
      printf("# Changes not staged for commit:\n");
      printf("#   (use \"git add%s <file>...\" to update what will be committed)\n", rm_in_workdir ? "/rm" : "");
      printf("#   (use \"git checkout -- <file>...\" to discard changes in working directory)\n");
      printf("#\n");
      header = 1;
    }

    old_path = s->index_to_workdir->old_file.path;
    new_path = s->index_to_workdir->new_file.path;

    if (old_path && new_path && strcmp(old_path, new_path))
      printf("#\t%s  %s -> %s\n", wstatus, old_path, new_path);
    else
      printf("#\t%s  %s\n", wstatus, old_path ? old_path : new_path);
  }

  if (header) {
    changed_in_workdir = 1;
    printf("#\n");
  }


  header = 0;

  for (i = 0; i < maxi; ++i) {
    s = git_status_byindex(status, i);

    if (s->status == GIT_STATUS_WT_NEW) {

      if (!header) {
        printf("# Untracked files:\n");
        printf("#   (use \"git add <file>...\" to include in what will be committed)\n");
        printf("#\n");
        header = 1;
      }

      printf("#\t%s\n", s->index_to_workdir->old_file.path);
    }
  }

  header = 0;


  for (i = 0; i < maxi; ++i) {
    s = git_status_byindex(status, i);

    if (s->status == GIT_STATUS_IGNORED) {

      if (!header) {
        printf("# Ignored files:\n");
        printf("#   (use \"git add -f <file>...\" to include in what will be committed)\n");
        printf("#\n");
        header = 1;
      }

      printf("#\t%s\n", s->index_to_workdir->old_file.path);
    }
  }

  if (!changes_in_index && changed_in_workdir)
    printf("no changes added to commit (use \"git add\" and/or \"git commit -a\")\n");
}

static int print_submod(git_submodule *sm, const char *name, void *payload)
{
  int *count = reinterpret_cast<int *>(payload);
  (void)name;

  if (*count == 0)
    printf("# Submodules\n");
  (*count)++;

  printf("# - submodule '%s' at %s\n",
    git_submodule_name(sm), git_submodule_path(sm));

  return 0;
}

int GitRepo::Status()
{
    if (!mIsValid)
    {
        return -10;
    }

  git_status_list *status;
  struct status_opts o;

  git_status_options_init(&o.statusopt, GIT_STATUS_OPTIONS_VERSION);

  o.statusopt.show  = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
  o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
    GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
    GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;



  if (git_repository_is_bare(mRepo))
      return -1;
   // fatal("Cannot report status on bare repository", git_repository_path(mRepo));

show_status:
  if (o.repeat)
    printf("\033[H\033[2J");

  git_status_list_new(&status, mRepo, &o.statusopt);
 // check_lg2(git_status_list_new(&status, mRepo, &o.statusopt), "Could not get status", NULL);

//  if (o.showbranch)
//    show_branch(mRepo, o.format);

  if (o.showsubmod) {
    int submod_count = 0;
//    check_lg2(git_submodule_foreach(mRepo, print_submod, &submod_count), "Cannot iterate submodules", o.repodir);

    git_submodule_foreach(mRepo, print_submod, &submod_count);
  }

//  if (o.format == FORMAT_LONG)
    print_long(status);
//  else
//    print_short(mRepo, status);

  git_status_list_free(status);


  return 0;
}

bool GitRepo::IsEnabled() const
{
    return mEnabled;
}

void GitRepo::SetEnabled(bool enabled)
{
    mEnabled = enabled;
}

std::string GitRepo::GetPath() const
{
    return mPath;
}

bool GitRepo::IsValid() const
{
    return mIsValid;
}
