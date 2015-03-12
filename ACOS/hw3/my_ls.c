#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>

char* string_cat(char* str1, char* str2)
{
    char* res = (char*) malloc((strlen(str1)+strlen(str2)+2)*sizeof(char));
    assert(res);
    strcpy(res, str1);
    strcat(res, "/");
    strcat(res, str2);
    return res;
}
void Print(const char* file_name)
{
    struct stat buf;
    char time[64];
    struct passwd *pas;
    if (stat(file_name, &buf))
    {
        fprintf(stderr,"file does not exist.\n");
        return;
    }  
    (S_ISDIR(buf.st_mode)) ? printf("d"): printf("-");
    (buf.st_mode & S_IRUSR) ? printf("r"): printf("-");
    (buf.st_mode & S_IWUSR) ? printf("w"): printf("-");
    (buf.st_mode & S_IXUSR) ? printf("x"): printf("-");
    (buf.st_mode & S_IRGRP) ? printf("r"): printf("-");
    (buf.st_mode & S_IWGRP) ? printf("w"): printf("-");
    (buf.st_mode & S_IXGRP) ? printf("x"): printf("-");
    (buf.st_mode & S_IROTH) ? printf("r"): printf("-");
    (buf.st_mode & S_IWOTH) ? printf("w"): printf("-");
    (buf.st_mode & S_IXOTH) ? printf("x"): printf("-");
    
   
    if ((pas = getpwuid(buf.st_uid))==NULL)
       fprintf(stderr,"Can't identify the user name");
    //pas = getpwuid(buf.st_uid);
    printf(" %s  ", pas->pw_name);                
    //printf("%d  ", pas->pw_gid);               
    printf("%7d ", buf.st_size);
    strftime(time, sizeof(time), "%Y-%m-%d %H:%M", localtime(&buf.st_mtime));
    printf("%s %s\n",time, file_name);
}

void ls_function(char* dir_name, struct dirent* d)
{
    	char* file = string_cat(dir_name, d->d_name);
    	struct stat buf;
    	if (lstat(file,&buf))
    	{
        	fprintf(stderr,"Error: path or file  does not exist.\n");
        	return;
    	}
	if (S_ISDIR(buf.st_mode))            
	{ 
		Print(file);
		recursion(file);
	}
	else                            
	{
		Print(file);
	}
	free((void*)file);
}

int recursion(char* dir_name)
{
    DIR* dir;                                
    struct dirent* d;                       
    if((dir = opendir(dir_name)) !=NULL) 
    {
    	while((d = readdir(dir)) != NULL)
        {
           if(d->d_name[0] != '.')  
           {
		ls_function(dir_name, d);
	   }
        }
        closedir (dir);
    }
    else                                                
    {
        printf("Can't open a directory: %s\n", dir_name);        
    }
    return 0;
}

int main(int argc, char* argv[])
{
    char* dir_name;
    if (argc < 2)
        dir_name = ".";
    else
        dir_name = argv[1];
    recursion(dir_name);
    return 0;
}
