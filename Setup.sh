sudo rm -rf temp_build
mkdir temp_build
cd temp_build
sudo apt-get update -qq
sudo apt-get install -qq libglew-dev libopenal-dev

if [ -a "/usr/local/lib/libsndfile.so" ]
then
    echo "Lib sndfile found"
else
    wget http://www.mega-nerd.com/libsndfile/files/libsndfile-1.0.27.tar.gz
    tar zxf libsndfile-1.0.27.tar.gz
    cd libsndfile-1.0.27
    ./configure
    sudo make install
    cd ../
fi
if [ -a "/usr/local/lib/liblua.a" ]
then
    echo "Lib lua found"
else
    curl -R -O http://www.lua.org/ftp/lua-5.3.3.tar.gz
    tar zxf lua-5.3.3.tar.gz
    cd lua-5.3.3
    sudo make linux install MYCFLAGS='-fPIC'
    cd ../
fi
cd ../
echo $PWD
git submodule init && git submodule update
cd extlib/glfw
cmake . -DBUILD_SHARED_LIBS=ON
sudo make install
cd ../freetype2
mkdir build
cd build
cmake -D BUILD_SHARED_LIBS:BOOL=true .. && make
cd ../../Box2D/Box2D/Build
cmake .. -DCMAKE_CXX_FLAGS="-std=c++11" -DBOX2D_BUILD_SHARED=ON -DBOX2D_BUILD_EXAMPLES=OFF
sudo make install
cd ../../../ECS/
echo $PWD
mkdir build
cd build
cmake ..
make
cd ../../../../ 

