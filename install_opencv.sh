set -e
# OpenCV dependencies - Details available at: http://docs.opencv.org/trunk/doc/tutorials/introduction/linux_install/linux_install.html
sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev

cd ~

# Download v3.4.11 .zip file and extract.
curl -sL https://github.com/Itseez/opencv/archive/3.4.11.zip > opencv.zip
unzip opencv.zip

cd opencv-3.4.11

  # Create a new 'build' folder.
mkdir build_opencv
cd build_opencv

  # Set build instructions for Ubuntu distro.
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_NEW_PYTHON_SUPPORT=OFF -D INSTALL_C_EXAMPLES=OFF -D INSTALL_PYTHON_EXAMPLES=OFF -D BUILD_EXAMPLES=OFF -D ENABLE_PRECOMPILED_HEADERS=OFF ..

  # Run 'make' with four threads.
make -j4

  # Install to OS.
sudo make install

  # Add configuration to OpenCV to tell it where the library files are located on the file system (/usr/local/lib)
sudo sh -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'

sudo ldconfig
echo "OpenCV installed."
