#include "./includes/minishell.h"
#include <dirent.h>

int cd(char *path)
{
    int     ret;
    char    s[1000];

    printf("%s\n", getcwd(s, 1000));
    ret = chdir(path);
    printf("%s\n", getcwd(s, 1000));
    printf("ret = %d\n", ret);
    return 0;
}

int main(int ac, char **av)
{
    if (ac == 1)
    {
        return 0;
    }
    
    struct dirent *de;  // Pointer for directory entry
  
    // opendir() returns a pointer of DIR type. 
    cd(av[1]);
    DIR *dr = opendir(".");

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }
    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL)
    {
            printf("%s\n", de->d_name);
            // if(ft_strcmp(de->d_name, av[1]))
            //     readdir(dr);
    }
  
    closedir(dr);    
    return 0;
}