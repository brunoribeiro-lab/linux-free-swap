# linux-free-swap
This is a simple program for RAM optimization on Linux. With it, it is possible to free up swap memory to improve system performance.

# Preparing the Environment
To prepare the environment and compile the program, follow these steps:

  ```shell
    $ sudo apt-get install libc6-dev
    $ git clone https://github.com/brunoribeiro-lab/linux-free-swap # (or use the ssh link)
    $ cd linux-free-swap/
    $ sudo chmod +x install.sh && sudo chmod +x uninstall.sh
  ```

# Installation
To install the program, simply run the `install.sh` script:

  ```shell
    $ sudo ./install.sh
  ```

This will:
- Clean, compile, and build the program.
- Set up a systemd service for dropcaches.
- Enable and start the service automatically.

The program will run as a background service, regularly freeing up swap memory to help optimize your system.

# Configuration
Once installed, you can modify the program’s behavior by editing its configuration file located at `/etc/dropcache.conf`

  ```shell
[Settings]
debug = 0
cache_ram_threshold = 50
sleep_time = 5
  ```
## Configuration Options:
- **debug**: Set to `1` to enable debug logs (default is `0`).
- **cache_ram_threshold**: The percentage of RAM (including cache and buffers) to be cleared before triggering swap freeing  (default is `50`).
- **sleep_time**: The time (in seconds) the program waits before checking swap usage again (default is `5`).

Modify these values based on your system's needs.

# Uninstallation
To uninstall the program and remove the service, run the following command:

  ```shell
    $ sudo ./uninstall.sh
  ```

This will:
- Stop and disable the service.
- Remove the systemd service file.
- Clean up any leftover binaries and object files.

# Service Status
After installation, you can check the status of the service with:

  ```shell
    $ sudo systemctl status dropcaches.service
  ```

This will show whether the service is active and running.