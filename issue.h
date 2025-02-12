#ifndef ISSUE_H
#define ISSUE_H

//Function declaration
void Add_Issue(char[],char [],char[],char[],char user_name[],char email[]);
void List_Issues(void);
void Edit_Issues(int);
void Search_Issues(int);
void Resolve_Issues(int,char[],char[]);
int get_last_id();
void update_last_id(int);
void get_current_datetime(char *, char *, size_t,size_t);

#endif