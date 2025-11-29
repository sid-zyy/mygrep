#include <stdio.h> //printf,fopen,getline
#include <stdlib.h>//exit malloc free
#include <string.h>//strlen strstr strcmp
#include <ctype.h>//tolower
#include <unistd.h>//getopt

int case_insensitive = 0;
int line_num = 0;

int match(const char* line,const char*pattern){
    return strstr(line,pattern)!= NULL;
}

int match_ci(const char*line, const char* pattern){
    int n = strlen(line);
    int m = strlen(pattern);

    for(int i = 0; i < n-m; i++){
        int flag = 1;
        for(int j = 0; j < m ; j++){
            if(tolower(line[j+i]) != pattern[j]){
                flag = 0;
                break;
            }
        }
        if(flag) return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
    int opt; //used for getopt() function;

    //parsinig the arguments
    while((opt = getopt(argc,argv,"in"))!= -1){
        //getopt() is used to get all the options used
        //it accepts argc(no. of arguments) argv(string of arguments) and string containing all special characters(i,n in this case)
        //it returns the string i or n in ASCII, when no more arguments it returns -1
        switch(opt){
            case 'i' : case_insensitive = 1; break;
            case 'n' : line_num = 1; break;//if -i or -n was entered set insensitive and num to 1
            default : printf("Incorrect argument"); exit(0);
        }
    }

    if(argc - optind > 2){//checks if total no. of arguments - no. of options is greater than 2 otherwise throws error 
        //as atleast two other arguments(file and pattern must be present)
        printf("Not enough arguments provided\n");
        exit(1);
    }
    char *pattern = argv[optind];
    char *filename = argv[optind+1];
    
    FILE *fp = fopen(filename,"r");

    char*line = NULL;
    int lineno = 1;
    int found = 0;

    size_t len = 0;//size_t is an unsigned integer type used to represent object sizes
    ssize_t nread;//ssize_t is the signed version of size-t in case getline() returns -1
    
    while((nread = getline(&line,&len,fp))!= -1){//keep going till out of lines
        //ssize_t getline(char **lineptr, size_t *n, FILE *stream);
        int matched;
        if(case_insensitive){
            matched = match_ci(line,pattern);
        }
        else matched = match(line,pattern);

        if(matched){
            found = 1;
            if(line_num){
                printf("%d :",lineno);
            }
            printf("%s",line);
        }
        lineno++;
    }
    free(line);//because getline() dynamically reallocates line
    fclose(fp);
    return found;
}