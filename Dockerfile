
#https://docs.docker.com/config/containers/multi-service_container/
FROM ubuntu:jammy

# hadolint ignore=DL3008
RUN apt-get update && apt-get upgrade -y && \
apt-get install --no-install-recommends \
ca-certificates \
cmake \
curl \
gdb \
git \
g++ \
libspdlog-dev \
make \
neovim \
systemctl \
telnet \
tree -y \
ufw \
xinetd \
&& apt-get clean \
&& rm -rf /var/lib/apt/lists/*

COPY .envfiles/telnet /etc/xinetd.d/telnet
RUN systemctl start xinetd.service && ufw allow 23 && rm -rf /socket-t3
COPY . /socket-t3/
WORKDIR /socket-t3
