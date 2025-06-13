FROM alpine:3.21.3

RUN apk add make gcc libc-dev python3-dev openjdk17-jdk && \
  python3 -m venv . &&                                     \
  pip install --upgrade pip &&                             \
  pip install cdm-devkit

WORKDIR /root/cdm-devkit
