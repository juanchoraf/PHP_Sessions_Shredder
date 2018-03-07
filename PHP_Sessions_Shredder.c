#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
    #define OS_SLASH "\\"
    #define OS_SEP "\n\r"
#else
    #define OS_SLASH "/"
    #define OS_SEP "\n"
#endif

#define BUF_MAX 4000


static char * GetCurrentDir( void ){
    char *result;
    result = getcwd( 0, 0 );
    return result;
}

static time_t CurrentTime( char *measure ){
    time_t seconds, result;
    seconds = time(NULL);

    if( strcmp(measure, "hours") == 0 ){
        result = seconds / 3600;
    }else if( strcmp(measure, "mins") == 0 ){
        result = seconds / 60;
    }else if( strcmp(measure, "secs") == 0 ){
        result = seconds;
    }else if( strcmp(measure, "msecs") == 0 ){
        result = seconds * 1000;
    }else{
        result = seconds;
    }

    return result;
}

static int TimeDiff(time_t startime, time_t endtime){
    int result;
    result = (int) difftime(endtime, startime);

    return result;
}

static int ConvertTime( int timeinsecs, const char *measure ){
    int result;

    if( strcmp(measure, "hours") == 0 ){
        result = timeinsecs / 3600;
    }else if( strcmp(measure, "mins") == 0 ){
        result = timeinsecs / 60;
    }else if( strcmp(measure, "secs") == 0 ){
        result = timeinsecs;
    }else if( strcmp(measure, "msecs") == 0 ){
        result = timeinsecs * 1000;
    }else{
        result = timeinsecs;
    }

    return result;
}

static int LogChanges( const char *logmsg ){
    static FILE *fp;
    static char logflbuf[BUF_MAX];

    snprintf( logflbuf, BUF_MAX, "%s%s%s", GetCurrentDir(), OS_SLASH, "Log_PHP_Sessions_Shredder.log" );
    fp = fopen( logflbuf, "a" );

    if( fp == NULL ){
        fclose(fp);
        perror("[AnonB0T]-> fopen Error");
        exit(EXIT_FAILURE);
    }else{
        size_t msglen = strlen(logmsg);
        size_t result = fwrite( logmsg, msglen, 1, fp );
        (result != 0) ?: printf("[AnonB0T]-> Error writing on Log file !\n");
        fclose(fp);
    }

    return 0;
}

static int TriturateSessions( char *SESS_DIR, int EXPIRY, char *MEASURE, char *LOGCHG ){
    DIR *dirp = opendir(SESS_DIR);

    if( dirp == NULL ){
        closedir(dirp);
        perror("[AnonB0T]-> opendir Error");
        exit(EXIT_FAILURE);
    }else{
        struct dirent *dirobj;
        struct stat sb;
        static char filebuf[BUF_MAX];
        static char fullpathbuf[BUF_MAX];
        static char logbuf[BUF_MAX];
        time_t endtime = CurrentTime("secs");
        static char *flstat, *cleantime;

        while( (dirobj = readdir(dirp)) != NULL ){
            char *fname = dirobj->d_name;
            snprintf( filebuf, BUF_MAX, "%.5s", fname );

            if( strcmp(filebuf, "sess_") == 0 ){
                snprintf( fullpathbuf, BUF_MAX, "%s%s%s", SESS_DIR, OS_SLASH, fname );

                if( stat(fullpathbuf, &sb) == -1 ){
                    perror("[AnonB0T]-> stat Error");
                    exit(EXIT_FAILURE);
                }else if( (sb.st_mode & S_IFMT) == S_IFREG ){
                    int startime = (int) sb.st_mtime;
                    int timediff = TimeDiff(startime, endtime);
                    int modtime = ConvertTime(timediff, MEASURE);

                    if( modtime >= EXPIRY ){
                        flstat = ( remove(fullpathbuf) == 0 ) ? "[DELETED]" : "[ERROR] Can\'t delete:";
                        cleantime = strtok( ctime(&sb.st_ctime), "\n");
                        snprintf( logbuf, BUF_MAX, "[AnonB0T]-> %s %s %s%s", cleantime, flstat, fullpathbuf, OS_SEP );
                        ( strcmp(LOGCHG, "log") != 0 ) ?: LogChanges( logbuf );
                    }
                }
            }
        }
        closedir(dirp);
    }

    return 0;
}

int main( int argc, char **argv ){
    static char *SESS_DIR, *MEASURE, *LOGCHG;
    static int EXPIRED;
    SESS_DIR = ( argc > 1 && argv[1] != NULL) ? argv[1] : GetCurrentDir() ;
    EXPIRED = ( argc > 2 && argv[2] != NULL ) ? atoi( argv[2] ) : 30;
    LOGCHG = ( argc > 3 && argv[3] != NULL ) ? argv[3] : "NONE";
    MEASURE = "mins";

    TriturateSessions(SESS_DIR, EXPIRED, MEASURE, LOGCHG);

    exit(EXIT_SUCCESS);
}
