#!/bin/bash

export ROOT_ID=0
export E_NOTROOT=87
export BLOGO=/usr/share/plymouth/themes/mint-logo
export APACHEPC=/etc/apache2/ports.conf

#
# terminate script execution if we are not root
#
abort_root(){
	echo -e "\e[33mMust be root to run this script!\e[0m"
	exit $E_NOTROOT
}

#
# set the apache listening port in `/etc/apache2/ports.conf`
#
apache_set_listen_port(){
	pconf=$(cat ${APACHEPC})
	echo "$pconf" | sed -i "s/Listen 80/Listen $1/g" ${APACHEPC}
}

#
# change the booting logo (mint only)
#
set_boot_logo(){
	[ -d $BLOGO ] || return 1
	cd $BLOGO
	test $(find . -type f -name mint-logo.png) && mv mint-logo.png old-mint-logo.png;\
	cp $1 . ; cp -f ./`basename $1` ./mint-logo.png; \
	update-initramfs -u -k all
	return 0
}


if [ $UID -ne $ROOT_ID ]; then
	abort_root
fi

cd /tmp && sudo apt update

# First install git
#############################
if [ ! $(which git) ]; then
	echo -e "\n\e[33mI'm about to install GIT!\e[0m\n"
	sleep 1
	sudo apt install git
else
	echo -e "\n\e[33mGit si already installed!\e[0m\n"
fi

# Next install iptables
#############################
if [ ! $(which iptables) ]; then
	echo -e "\n\e[33mIptables package is about to be installed!\e[0m\n"
	sleep 1
	sudo apt install iptables
else
	echo -e "\n\e[33mIptables package is already installed!\e[0m\n"
fi

#INSTALL NIKTO AND OTHER PENTEST STUFF
########################################
echo -e "\n\e[33mInstalling nikto, whatweb, sqlmap, vim, g++, nmap, wireshark, 7zip\e[0m\n"
echo "y" | sudo apt install nikto
echo "y" | sudo apt install whatweb
echo "y" | sudo apt install sqlmap
echo "y" | sudo apt install vim
echo "y" | sudo apt install g++
echo "y" | sudo apt install nmap
echo "y" | sudo apt install wireshark-qt
echo "y" | sudo apt install p7zip-full


# APACHE, PHP && MYSQL
#############################
read -p "$(echo -e '\n\e[33mApache2 web server is about to be installed!\e[0m y/n ')" cf
cstat=0
case "$cf" in
	"y")
		read -p "$(echo -e '\n\e[33mApache port\e[0m -> ')" port
		sudo apt update
		sudo apt install apache2
		cstat=1
		;;
	"n")
		echo -e "\n\e[33mSkipping apache installation!\e[0m\n";
		;;
	*)
		echo -e "\n\e[33mSkipping apache installation!\e[0m\n";
		;;
esac

if [ $cstat -eq 1 ]; then
	if [ $port -gt 1 -a $port -le 65535 ]; then
		sudo iptables -I INPUT -p tcp -m tcp --dport $port -j ACCEPT
		apache_set_listen_port $port
	fi
fi

# PHP
read -p "$(echo -e '\e[33mPHP is about to be installed!\e[0m y/n ')" cf
case "$cf" in
	"y")
		sudo add-apt-repository ppa:ondrej/php
		sudo apt update
		sudo apt install php7.0 php7.0-cli php7.0-json php7.0-pdo php7.0-mysql php7.0-zip php7.0-gd  php7.0-mbstring php7.0-curl php7.0-xml php7.0-bcmath php7.0-json
		sudo apt install php7.2 php7.2-cli php7.2-json php7.2-pdo php7.2-mysql php7.2-zip php7.2-gd  php7.2-mbstring php7.2-curl php7.2-xml php7.2-bcmath php7.2-json
		sudo apt install php7.3 php7.3-cli php7.3-json php7.3-pdo php7.3-mysql php7.3-zip php7.3-gd  php7.3-mbstring php7.3-curl php7.3-xml php7.3-bcmath php7.3-json
		sudo apt install php7.0-intl php7.2-intl php7.3-intl
		echo -e "\n\e[33mPHP installation complete!\e[0m\n"
		;;
	"n")
		echo -e "\n\e[33mSkipping PHP installation!\e[0m\n";
		;;
	*)
		echo -e "\n\e[33mSkipping PHP installation!\e[0m\n";
		;;
esac
########################################################################

#GOOGLE CHROME INSTALLATION
#############################
echo -e "\n\e[33mgoogle-chrome is about to be installed!\e[0m\n"
read -p "Choose chrome version -> 32 or 64bit " ver
cstat=0
case "$ver" in
	"32")
		wget https://archive.org/download/google-chrome-stable_48.0.2564.116-1_i386/google-chrome-stable_48.0.2564.116-1_i386.deb
		sudo dpkg -i google-chrome-stable_48.0.2564.116-1_i386.deb
		cstat=1
		;;
	"64")
		wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
		sudo dpkg -i google-chrome-stable_current_amd64.deb
		cstat=1
		;;
	*)
		echo -e "\n\e[33mChrome installation aborted!\e[0m\n"
		;;
esac

if [ $cstat -eq 1 ]; then
	echo -e "\n\e[33mChrome installation complete!\e[0m\n"
fi
########################################################################


read -p "$(echo -e '\n\e[33mUpdate boot logo?\e[0m y/n')" cf
case "$cf" in
	"y")
		read -p "Absolute path to your image -> " ipath
		set_boot_logo $ipath && echo "Done" || echo "Failed" && exit 1
		;;
	*)
		echo "" ;;
esac