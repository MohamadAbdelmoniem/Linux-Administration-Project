#!/bin/bash

# Assign arguments to variables
USERNAME=$1
USERPASS=$2
GROUPNAME=$3
GROUPID=200

# Check if script is run with sudo permissions
if [ "$USER" != "root" ]; then
    echo "Error: This script must be run as root (use sudo)."
    exit 1
fi
# Check The number of Arguments send to the script
if [ $# -ne 3 ];
then
   echo "Error: This script must be run by passing 3 arguments"
   exit 1
fi

# Print the Values Passed
echo "USERNAME = "$USERNAME""
echo "USERPASS = "$USERPASS""
echo "GROUPNAME = "$GROUPNAME""

# Create new User name should be taken from the argument
# Create a home directory for this user
# Assign the default shell as “Bash”
# Do not create a group with the same name
# Check if the command is executed successfuly
useradd -m -N -s /bin/bash $USERNAME
if [ $? -eq 0 ]; then
    echo "User '$USERNAME' created successfully."
else
    echo "Error: Failed to create user '$USERNAME'."
    exit 1
fi

# Set the password for the new user
# Check if the command is executed successfuly
echo "$USERNAME:$USERPASS" | chpasswd
if [ $? -eq 0 ]; then
    echo "Password for user '$USERNAME' set successfully."
else
    echo "Error: Failed to set password for user '$USERNAME'."
    exit 1
fi

# Display the user and group information about this user
id $USERNAME

# Create a new group with ID=200 and the name should be taken from the argument
# Check if the command is executed successfuly
groupadd -f -g 200 $GROUPNAME
if [ $? -eq 0 ]; then
    echo "Group was created with name '$GROUPNAME' and GID '$GROUPID' successfully."
else
    echo "Error: Failed to create the group named '$GROUPNAME'."
    exit 1
fi

# Add the user to the new created group
usermod -aG $GROUPNAME $USERNAME
if [ $? -eq 0 ]; then
    echo "User '$USERNAME' Added to group '$GROUPNAME' successfully."
else
    echo "Error: Failed to Add user '$USERNAME' to group '$GROUPNAME'"
    exit 1
fi

# Display the info Again
id $USERNAME

#Change the UID for the user and make the primary group of the user the group that just createt
usermod -u 1600 -g $GROUPNAME $USERNAME
if [ $? -eq 0 ]; then
    echo "User '$USERNAME' UID is Updated to 1600 successfully and Primary group changed to '$GROUPNAME'."
else
    echo "Error: Failed to chnaged the UID for '$USERNAME'"
    exit 1
fi

# Display the info Again
id $USERNAME
echo "Script execution completed successfully."
