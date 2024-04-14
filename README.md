# 1. launch container
multipass launch 22.04 --name=nc-ass4
multipass shell nc-ass4

# 2. install compiler for C
sudo apt-get install -y gcc g++
git clone 

# 2.1. check if installed
gcc --version
g++ --version

# 3. build all needed files
g++ -std=c++11 -pthread /home/ubuntu/nc_assignment4/main.cpp /home/ubuntu/nc_assignment4/SafeFile.cpp -o /home/ubuntu/nc_assignment4/serverExecutable
chmod +x /home/ubuntu/nc_assignment4/serverExecutable
chmod +x /home/ubuntu/nc_assignment4/client.py

# 3. move .service to /etc/systemd/system
sudo mv /home/ubuntu/nc_assignment4/main-server.service /etc/systemd/system/main-server.service

# 4. start .service
sudo systemctl enable /path/to/main-server.service
sudo systemctl start main-server.service