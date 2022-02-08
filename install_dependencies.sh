set -e
# OpenCV dependencies - Details available at: http://docs.opencv.org/trunk/doc/tutorials/introduction/linux_install/linux_install.html
sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev

# Download v3.4.11 .zip file and extract.
curl -sL https://github.com/Itseez/opencv/archive/3.4.11.zip > opencv.zip
unzip opencv.zip
cd opencv-3.4.11
