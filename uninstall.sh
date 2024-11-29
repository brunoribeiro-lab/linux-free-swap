#!/bin/bash

GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"

PROJECT_DIR="$(pwd)"
SERVICE_FILE="/etc/systemd/system/dropcaches.service"

echo "Stopping the service..."
sudo systemctl stop dropcaches.service

echo "Disabling the service..."
sudo systemctl disable dropcaches.service

echo "Removing the systemd service file..."
sudo rm -f $SERVICE_FILE

echo "Cleaning up binary and object files..."
rm -f $PROJECT_DIR/bin/dropcaches
find ./src -type f -name "*.o" -exec rm -f {} \;

echo "Reloading systemd..."
sudo systemctl daemon-reload

echo "Checking service status..."
STATUS_OUTPUT=$(sudo systemctl status dropcaches.service 2>&1)

if [[ "$STATUS_OUTPUT" == *"/etc/systemd/system/dropcaches.service"* ]]; then
    echo -e "${RED}The dropcaches service has NOT been uninstalled. The service file is still present.${NC}"
else
    echo -e "${GREEN}The dropcaches service has been successfully uninstalled.${NC}"
fi