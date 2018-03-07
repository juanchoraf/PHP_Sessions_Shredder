# PHP Sessions Shredder

This is a Ultra fast garbage collector for PHP Sessions.

## Getting Started

These instructions will let you have this project up and running in seconds on your Server or local machine for testing purposes. We assume that you are on a Linux system.

## Support

This project has support for POSIX systems (Linux, Windows, MacOS, etc.)

#### Tested in:
* Linux
* Windows
  
## Download

If you want the precompiled files just go to the [Releases](https://github.com/juanchoraf/PHP_Sessions_Shredder/releases) and download the file you wish according with your Server platform.

you also can clone this repository with the [Clone a Repository](https://help.github.com/articles/cloning-a-repository/#platform-windows) instructions (e.g.): 

```
git clone https://github.com/juanchoraf/PHP_Sessions_Shredder
```

or just get the precompiled file on your Linux with:

```
cd /usr/local/src/

wget https://github.com/juanchoraf/PHP_Sessions_Shredder/releases/download/1.0.0/PHP_Sessions_Shredder.raf
```

## Installation

you just need to give permissions to the downloaded file (e.g. 777):

```
chmod 777 PHP_Sessions_Shredder.raf
```

## Use

You can execute the program like this:

```
[EXECUTABLE] [PATH_TO_PHP_SESSIONS] [INACTIVITY_TIME] [LOG]
```

For example, this will look for all the sessions with an **INACTIVITY** of 30 minutes or more

* Linux:
```
./PHP_Sessions_Shredder.raf /var/lib/php/sessions/ 30
```
* Windows:
```
PHP_Sessions_Shredder.exe /var/lib/php/sessions/ 30
```

#### Log

You also can log the activity of the Triturated Sessions:

* Linux:
```
./PHP_Sessions_Shredder.raf /var/lib/php/sessions/ 30 log
```
* Windows:
```
PHP_Sessions_Shredder.exe /var/lib/php/sessions/ 30 log
```
**NOTE:** be aware that this program could be executed 24/7 on your Server, so, the log file could take a lot of disk space (maybe you don't need to use the log option on Production)

* **Log file name:**  Log_PHP_Sessions_Shredder.log
* **Log location:**  The same of the PHP_Sessions_Shredder executable

### Add PHP_Sessions_Shredder to Cron (for 24/7 purposes) ONLY Linux:

* Add a new file to cron:
```
nano /etc/cron.d/PHP_Sessions_Shredder
```
* Paste this line in that file and save it with Ctrl O :
```
*/30 * * * *    root    SHELL=/bin/bash    /usr/local/src/PHP_Sessions_Shredder.raf    /var/lib/php/sessions/  30
```
* Set permissions to that file:
```
chmod 644 /etc/cron.d/PHP_Sessions_Shredder

chown root:root /etc/cron.d/PHP_Sessions_Shredder
```

## License

Check the [LICENSE](https://github.com/juanchoraf/PHP_Sessions_Shredder/blob/master/LICENSE) file for more information.
