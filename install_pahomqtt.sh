##### BUILD FROM SOURCE #####
# Clone MQTT using release v1.5.3
git clone https://github.com/eclipse/paho.mqtt.cpp ../paho.mqtt.cpp
cd ../paho.mqtt.cpp
git checkout v1.5.3

git submodule init
git submodule update

# Creates build folder and create build files with CMAKE
mkdir build
cd build
cmake -D PAHO_WITH_MQTT_C=ON -D PAHO_BUILD_EXAMPLES=ON -D PAHO_BUILD_DOCUMENTATION=ON -D CMAKE_POLICY_VERSION_MINIMUM=3.5 -D CMAKE_INSTALL_PREFIX=$HOME/opt/pahomqtt ..

# Install MQTT make files to system
cmake --build . --target install