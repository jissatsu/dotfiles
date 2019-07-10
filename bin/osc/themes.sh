#!/bin/bash

cd /tmp
git clone https://github.com/erikdubois/Arc-Theme-Colora-Collection.git

cd Arc-Theme-Colora-Collection && chmod +x theme-arc-colora-collection-3.2-v1.sh
./theme-arc-colora-collection-3.2-v1.sh

cd ..
git clone https://github.com/erikdubois/Themes-Icons-Pack.git
cd Themes-Icons-Pack

chmod +x icons-sardi-v2.sh
./icons-sardi-v2.sh

chmod +x icons-sardi-extra-v2.sh
./icons-sardi-extra-v2.sh