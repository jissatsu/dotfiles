#!/bin/bash

export ROOT_ID=0
export E_NOTROOT=87
export BLOGO=/usr/share/plymouth/themes/mint-logo
export APACHEPC=/etc/apache2/ports.conf

export PACKS=(
	nikto
	sqlmap
	whatweb
	vim
	g++
	nasm
	wireshark-qt
	libpcap-dev
	libnet-dev
	nmap
	aircrack-ng
	p7zip-full
	git
	iptables
	tmux
	htop
	code
	npm
	nodejs-legacy
	mailutils
	binwalk
)

#
# terminate script execution if we are not root
abort_root(){
	echo -e "\e[33mMust be root to run this script!\e[0m"
	exit $E_NOTROOT
}

#
# set the apache listening port in `/etc/apache2/ports.conf`
apache_set_listen_port(){
	local pconf=$(cat ${APACHEPC})
	echo "$pconf" | sed -i "s/Listen 80/Listen $1/g" ${APACHEPC}
}

#
# install apache2 web server
apache2_install(){
	read -p "$(echo -e '\n\e[33mApache port\e[0m -> ')" port
	sudo apt update
	sudo apt install apache2
	
	if [ $port -gt 1 -a $port -le 65535 ]; then
		apache_set_listen_port $port
		sudo iptables -I INPUT -p tcp -m tcp --dport $port -j ACCEPT
		sudo systemctl restart apache2.service
	fi
}

#
# install MySql database
mysql_db_install(){
	sudo apt update
	sudo apt install mysql-server
	sudo mysql_secure_installation
	read -p "$(echo -e '\e[33mSet password for mysql root user -> \e[0m')" pass
	
	local plen=${#pass}
	if [ $plen -le 5 ]; then
		echo -e "\e[31m\nMySql root password si too weak!\n\e[0m"
		exit 1
	fi
	sudo mysql --execute="ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '$pass';"
	sudo mysql --execute="FLUSH PRIVILEGES"

	read -p "$(echo -e '\e[33mCreate a new MySql user -> \e[0m')" user
	read -p "$(echo -e '\e[33mPassword for the new user -> \e[0m')" pass

	local ulen=${#user}
	local plen=${#pass}
	if [ $plen -gt 5 -a $ulen -gt 1 ]; then
		sudo mysql -u root -p --execute="CREATE USER '$user'@'localhost' IDENTIFIED BY '$pass';"
		sudo mysql -u root -p --execute="GRANT ALL PRIVILEGES ON *.* TO '$user'@'localhost' WITH GRANT OPTION;"
	fi
	sudo systemctl restart mysql.service
}

#
# install PHP 7.0, 7.2, 7.3
php_install(){
	sudo add-apt-repository ppa:ondrej/php
	sudo apt update
	sudo apt install php7.0 php7.0-cli php7.0-json php7.0-pdo php7.0-mysql php7.0-zip php7.0-gd  php7.0-mbstring php7.0-curl php7.0-xml php7.0-bcmath php7.0-json
	sudo apt install php7.2 php7.2-cli php7.2-json php7.2-pdo php7.2-mysql php7.2-zip php7.2-gd  php7.2-mbstring php7.2-curl php7.2-xml php7.2-bcmath php7.2-json
	sudo apt install php7.3 php7.3-cli php7.3-json php7.3-pdo php7.3-mysql php7.3-zip php7.3-gd  php7.3-mbstring php7.3-curl php7.3-xml php7.3-bcmath php7.3-json
	sudo apt install php7.0-intl php7.2-intl php7.3-intl
	echo -e "\n\e[33mPHP installation complete!\e[0m\n";
}

#
# change the booting logo (mint only)
set_boot_logo(){
	[ -d $BLOGO ] || return 1
	cd $BLOGO
	test $(find . -type f -name mint-logo.png) && mv mint-logo.png old-mint-logo.png;\
	cp $1 . ; \
	mv ./`basename $1` ./mint-logo.png; \
	update-initramfs -u -k all
	return 0
}


[ $UID -ne $ROOT_ID ] && abort_root
sudo apt update && sudo chmod +x /etc/rc.local

echo -e "\n\e[33mInstalling ${PACKS[*]}\e[0m\n"
echo "y" | sudo apt install ${PACKS[*]}

source ./themes.sh

# APACHE, PHP && MYSQL
#############################
read -p "$(echo -e '\n\e[33mApache2 web server is about to be installed!\e[0m y/n ')" cf
case "$cf" in
	"y") apache2_install;;
	*  ) echo -e "\n\e[31mSkipping apache installation!\e[0m";;
esac

# PHP
read -p "$(echo -e '\e[33m\nPHP is about to be installed!\e[0m y/n ')" cf
case "$cf" in
	"y") php_install;;
	*  ) echo -e "\n\e[31mSkipping PHP installation!\e[0m\n";;
esac

# MYSQL
read -p "$(echo -e '\n\e[33mMySql is about to be installed!\e[0m y/n ')" cf
case "$cf" in
	"y") mysql_db_install;;
	*  ) echo -e "\n\e[31mSkipping MySql installation!\e[0m";;
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
		echo -e "\n\e[31mChrome installation aborted!\e[0m\n" ;;
esac

if [ $cstat -eq 1 ]; then
	echo -e "\n\e[33mChrome installation complete!\e[0m\n"
fi
########################################################################

read -p "$(echo -e '\e[33m\nInstall skypeforlinux? y/n \e[0m')" sk
case "$sk" in
	"y") wget https://go.skype.com/skypeforlinux-64.deb
		 sudo dpkg -i skypeforlinux-64.deb
		 skypeforlinux
		;;
	* ) echo -e "\n\e[31mSkipping skypeforlinux installation!\e[0m\n";;
esac

read -p "$(echo -e '\n\e[33mUpdate boot logo?\e[0m y/n')" cf
case "$cf" in
	"y")
		read -p "Absolute path to your image -> " ipath
		[ -f "$ipath" ] || echo "Failed" && exit 1
		set_boot_logo $ipath && echo "Done" || echo "Failed" && exit 1
		;;
	*)
		echo "" ;;
esac
