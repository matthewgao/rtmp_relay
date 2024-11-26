FROM crpi-dq9yw3ziixu7q5k4.cn-shanghai.personal.cr.aliyuncs.com/ga0sh3n/ffmpeg61:v1.0.1

# EXPOSE 80

RUN apt install -y cmake

RUN make -p /home/admin/rtmp_relay
COPY alibabacloud-nls-cpp-sdk  /home/admin/rtmp_relay/alibabacloud-nls-cpp-sdk
COPY censor  /home/admin/rtmp_relay/censor
COPY processor  /home/admin/rtmp_relay/processor
COPY grpc  /home/admin/rtmp_relay/grpc
COPY utils  /home/admin/rtmp_relay/utils
COPY CMakeLists.txt  /home/admin/rtmp_relay/.
COPY dict.txt  /home/admin/rtmp_relay/.
COPY main.cpp  /home/admin/rtmp_relay/.

WORKDIR /home/admin/rtmp_relay
RUN mkdir -p /home/admin/rtmp_relay/build
RUN cd build && cmake ../. && make
ENV PATH="/home/admin/.local/bin:/usr/local/bin:$PATH"

WORKDIR /home/admin/rtmp_relay/build