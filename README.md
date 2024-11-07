# rtmp_relay
rtmp relay

```
git clone --depth 1 https://github.com/aliyun/alibabacloud-nls-cpp-sdk
cd alibabacloud-nls-cpp-sdk && cd scripts && ./build_liunx.sh

wget https://www.ffmpeg.org/releases/ffmpeg-6.1.tar.gz
tar -zxvf ffmpeg-6.1.tar.gz

apt install -y build-essential git yasm nasm libx264-dev libx265-dev libvpx-dev  libvorbis-dev libogg-dev  libfdk-aac-dev pkg-config libass-dev  libjpeg-dev libpng-dev zlib1g-dev

cd ffmpeg-6.1/
./configure --enable-gpl --enable-nonfree --enable-libfdk-aac --disable-shared  --enable-static
make
make install
```

sudo yum install libasan