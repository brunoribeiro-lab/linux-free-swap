# linux-free-swap
This is a simple program for RAM optimization on Linux. With it, it is possible to free up swap memory to improve system performance.
# Compilation
To compile the program, follow these steps:
  ```shell
    $ sudo apt-get install libc6-dev
    $ git clone https://github.com/brunoribeiro-lab/linux-free-swap # (or use the ssh link)
    $ cd linux-free-swap/
    $ make clean && make
    $ sudo cp dropcaches /usr/local/bin/dropcaches
  ```
# Installation
To install the program, run the following command to open the crontab configuration file:
  ```shell
    $ sudo crontab -e
  ```
Add the following line at the end of the file:
  ```
  * * * * * /usr/local/bin/dropcaches
  ```
This will cause the program to run every minute, freeing up swap memory to improve system performance.
