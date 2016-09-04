FROM aerath/osdev
MAINTAINER Aerath Aiskurimu

WORKDIR home
ADD . .
WORKDIR toolchain
RUN wget -c ftp://ftp.gnu.org/gnu/binutils/binutils-2.27.tar.gz ftp://ftp.gnu.org/gnu/gcc/gcc-6.1.0/gcc-6.1.0.tar.gz \
&& make binutils-install-deb \
&& make gcc-install-deb \
&& rm -rf binutils-2.27* gcc-6.1.0* \
&& apt-get -y remove --purge bzip2 automake1.11 autoconf2.64 wget gcc texinfo libmpfr-dev libgmp3-dev libisl-dev libcloog-isl-dev libmpc-dev \
&& apt-get -y autoremove \
&& apt-get -y autoclean \
&& rm -rf /var/cache/debconf/*-old \
&& rm -rf /var/lib/apt/lists/* \
&& rm -rf /usr/share/man/?? \
&& rm -rf /usr/share/man/??_* \
&& rm -rf $(find /usr/share/locale -maxdepth 1 -type d ! -name 'en*' ! -name locale) \
&& mv dist / \
&& cd / \
&& rm -rf /home/*

CMD /bin/bash
