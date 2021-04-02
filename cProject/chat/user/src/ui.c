#include "ui.h"
#include <stdio.h>

void MenuWelcome()
{
    printf("hello user,please choose option below:\n");
    printf("1)registration\n");
    printf("2)login\n");
    printf("3)exit\n");
}

void MenuStart()
{
    printf("hello user,please choose option below:\n");
    printf("1)create group\n");
    printf("2)join group\n");
    printf("3)leave group\n");
    printf("4)logout\n");
/*     printf("5)exit\n"); */
}

void getUsernamePassword(char username[], char password[])
{
    printf("please enter your username(max 20 chars):\n");
    scanf("%s", username);
    getchar();
    printf("please enter your password(max 10 chars):\n");
    scanf("%s", password);
    getchar();
}

void GetNameOfGroup(char groupname[])
{
    printf("please enter your group name:\n");
    scanf("%s", groupname);
    getchar();
}

void GroupNameAlreadyExist()
{
    printf("group name already exists\n");
}

void RegReplyFail()
{
    printf("username already exists\n");
}

void LogginReplyFail()
{
    printf("username not found \n");
}

void UiShowGroups(char _groupname[], size_t _index)
{
    printf("%ld) %s  ", _index + 1, _groupname);
}

void GroupNameNotExist()
{
    printf("group name not exists\n");
}

void WrongGroupName()
{
    printf("wrong group name, please enter group name again:\n");
}

void WrongGroupNameLeave()
{
    printf("wrong group name\n");
}

void DontHaveGroupsYet()
{
    printf("you dont have groups\n");
}

void LogOutReplyError()
{
    printf("error log out from server\n");
}

void PressZeroToGoBack()
{
    printf(" press 0 to go back\n");
}

void DontHaveGroupsYetServer()
{
    printf("server dont have groups to join\n");
}

void SuccessToLeaveGroup()
{
    printf("successfully left group\n");
}

void AlreadyInGroup()
{
    printf("you are already in this group\n");
}

void TooBigName()
{
    printf("too big name\n");
}

void TooBigPass()
{
    printf("too big pass\n");
}


void getName(char _name[])
{
    printf("please enter your  name:\n");
    scanf("%s", _name);
    getchar();
}

void getPassword(char _pass[])
{
    printf("please enter your  password:\n");
    scanf("%s", _pass);
    getchar();
}