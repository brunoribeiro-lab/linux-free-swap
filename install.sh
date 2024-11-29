#!/bin/bash

GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"

PROJECT_DIR="$(pwd)"
SERVICE_FILE="/etc/systemd/system/dropcaches.service"

echo "Cleaning and compiling..."
make clean
make
find ./src -type f -name "*.o" -exec rm -f {} \;

echo "Creating systemd service..."

cat <<EOL | sudo tee $SERVICE_FILE > /dev/null
[Unit]
Description=Drop Caches Service
After=network.target

[Service]
Type=simple
ExecStart=$PROJECT_DIR/bin/dropcaches
WorkingDirectory=$PROJECT_DIR
Restart=always
User=$(whoami)

[Install]
WantedBy=multi-user.target
EOL

echo "Configuring the service..."
sudo chmod 644 $SERVICE_FILE
sudo systemctl daemon-reload
sudo systemctl enable dropcaches.service

echo "Starting the service..."
sudo systemctl start dropcaches.service

echo "Service status:"
STATUS_OUTPUT=$(sudo systemctl status dropcaches.service)
if [[ "$STATUS_OUTPUT" == *"active (running)"* ]]; then
    echo -e "${GREEN}The dropcaches service has been successfully installed and is running.${NC}"
else
    echo -e "${RED}There was an issue starting the dropcaches service.${NC}"
fi