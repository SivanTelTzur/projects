#ifndef __UI_H__
#define __UI_H__
#include <stddef.h>  /* size_t */

void MenuWelcome();
void MenuStart();
void getUsernamePassword(char username[],char password[]);
void GetNameOfGroup(char groupname[]);
void GroupNameAlreadyExist();
void RegReplyFail();
void LogginReplyFail();
void UiShowGroups(char _groupname[], size_t _index);
void GroupNameNotExist();
void WrongGroupName();
void WrongGroupNameLeave();
void DontHaveGroupsYet();
void LogOutReplyError();
void PressZeroToGoBack();
void DontHaveGroupsYetServer();
void SuccessToLeaveGroup();
void AlreadyInGroup();
void TooBigName();
void TooBigPass();
void getName(char _groupname[]);
void getPassword(char _pass[]);
#endif