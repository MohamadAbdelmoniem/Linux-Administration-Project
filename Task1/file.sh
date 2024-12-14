#!/bin/bash

# Assign arguments to variables
USERNAME=$1
USER_HOME_DIR="/home/$USERNAME"
FILEPATH="/home/moniem/Desktop/Linux_Administration_Project/TestFolder"
TAR_FILE="/home/moniem/Desktop/Linux_Administration_Project/TestFolder.tar"

# Check if script is run with sudo permissions
if [ "$USER" != "root" ]; then
    echo "Error: This script must be run as root (use sudo)."
    exit 1
fi
# Check if the directory exists, and remove it if it does
if [ -d "$FILEPATH" ]; then
   rm -rf "$FILEPATH"
fi

# Create the directory and the 4 files
mkdir -p "$FILEPATH" && touch "$FILEPATH/main.c" "$FILEPATH/main.h" "$FILEPATH/hello.c" "$FILEPATH/hello.h"

# Loop through each file and write the filename content
for FILE in "$FILEPATH"/*; do
    echo "This file is named $(basename "$FILE")"
done

# Compress the directory into a tar file
tar -cvf "$TAR_FILE" -C "$(dirname "$FILEPATH")" "$(basename "$FILEPATH")"
if [ $? -eq 0 ]; then
    echo "Directory compressed into tar file '$TAR_FILE' successfully."
else
    echo "Error: Failed to create tar file."
    exit 1
fi

# Change directory to the home directory of the new user
if [ -d "$USER_HOME_DIR" ]; then
    echo "Home directory for user '$USERNAME' exists."
else
    echo "Error: Home directory for user '$USERNAME' does not exist."
    exit 1
fi

# Copy the tar file to the new user's home directory and extract it
sudo cp "$TAR_FILE" "$USER_HOME_DIR"
if [ $? -eq 0 ]; then
    echo "Tar file copied to '$USER_HOME_DIR'."
    cd "$USER_HOME_DIR" || { echo "Error: Cannot change directory to '$USER_HOME_DIR'."; exit 1; }
    tar -xvf "$(basename "$TAR_FILE")"
    if [ $? -eq 0 ]; then
        echo "Tar file extracted successfully."
    else
        echo "Error: Failed to extract tar file."
        exit 1
    fi
else
    echo "Error: Failed to copy tar file to '$USER_HOME_DIR'."
    exit 1
fi

echo "Script execution completed successfully."

