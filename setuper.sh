#!/bin/bash

# 1. Launch container and access it
# multipass launch 22.04 --name=nc-ass4
# multipass exec nc-ass4 -- bash -c 

# 2. Install compilers and other necessary packages
sudo apt-get update
sudo apt-get install -y gcc g++ python3

# Clone the repository
git clone https://github.com/Gooowan/nc_assignment4 /home/ubuntu/nc_assignment4

# 3. Build all needed files
g++ -std=c++11 -pthread /home/ubuntu/nc_assignment4/main.cpp /home/ubuntu/nc_assignment4/SafeFile.cpp -o /home/ubuntu/nc_assignment4/serverExecutable

# Make executables
chmod +x /home/ubuntu/nc_assignment4/serverExecutable
chmod +x /home/ubuntu/nc_assignment4/client.py

# 3.1. Move the .service file to /etc/systemd/system
sudo mv /home/ubuntu/nc_assignment4/main-server.service /etc/systemd/system/

# 4. Start the service
sudo systemctl enable main-server.service
sudo systemctl start main-server.service

# Print completion message
echo 'Setup complete. nc-ass4 is configured and running.'