#include    "./include/app.h"
#include    <stdio.h>
#include    <stdbool.h>
#include    <sys/stat.h>

void sfinitialfileWR(void)
{
    const char *path = "./data";
    struct stat info;
    int status;

    if(stat(path, &info) != 0)
    {
        printf("Directory does not exist.\n");
        status = mkdir(path, 0755);

        if (status == 0)
        {
            printf("Directory created successfully.\n");
        }
    }
    else if(info.st_mode & S_IFDIR)
    {
        //printf("Directory exists.\n");
    }
    else
    {
        printf("Path exists, but it's not a directory.\n");
    }
}

void sffileWRTask(void)
{
    FILE *file;
    int wDataReadBusyflag = 0;

    if((sfGetTaskFlag() & (1 << cFileWRTask)) == (1 << cFileWRTask))
    {
        sfClearTaskFlag(cFileWRTask);
        file = fopen("./data/status.txt", "r");
        if(file == NULL)
        {
            printf("Can't Find status.txt\n");
            return;
        }
        fscanf(file, "%d", &wDataReadBusyflag);
        fclose(file);
        if(wDataReadBusyflag == true)
        {
            return;
        }
        file = fopen("./data/output.csv", "w");
        fprintf(file, "Acccle,Level\n");
        fprintf(file, "%f,%d\n", sffGetRmsTotalAcccle(), sfwGetItityLevel());
        fclose(file);
    }
}