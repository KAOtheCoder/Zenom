#!/bin/sh

#qt4 and boost should be present in your system
#pacaur -S qt4
#pacaur -S boost

#build osg
cd OpenSceneGraph
git apply ../patches/osg-gcc-flag-fix.patch
sudo rm -fr build
sudo mkdir build 
cd build
cmake -DDESIRED_QT_VERSION=4 ../
make osgQt osgSim osgdb_osg osgdb_deprecated_osg -j4
cd ../../

#install osg
sudo rm -fr /usr/local/osg
sudo mkdir /usr/local/osg/
sudo cp -ra OpenSceneGraph/include /usr/local/osg/
sudo cp -ra OpenSceneGraph/build/include /usr/local/osg/
sudo cp -ra OpenSceneGraph/build/lib /usr/local/osg


#build & install qwt
cd qwt
qmake-qt4 .
make -j4
sudo make install
cd ../

#build & install qtserialport
cd qtserialport
qmake-qt4 .
make -j4
sudo make install
cd ../

#build & install zenom
cd zenom
qmake-qt4 .
make -j4
sudo make install

#add ld library path to bashrc
#echo 'export LD_LIBRARY_PATH=/usr/local/qwt-6.0.1-svn/lib/:/usr/local/lib64:/usr/local/osg/lib' >> ~/.bashrc

