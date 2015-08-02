FROM debian:latest
MAINTAINER Oleg Morozenkov

RUN apt-get update && apt-get install -y g++ cmake make binutils libpoco-dev npm wget bash jq
RUN ln -s /usr/bin/nodejs /usr/bin/node
RUN npm install -g gulp

WORKDIR /usr/local/rocketcms

COPY install-poco.sh ./
RUN sed -i 's/\r$//' install-poco.sh && bash install-poco.sh
COPY web/package.json web/package.json
RUN cd web && npm install
COPY web/download-libs.sh web/download-libs.sh
RUN cd web && sed -i 's/\r$//' download-libs.sh && bash download-libs.sh

COPY cpp cpp
COPY web web
COPY CMakeLists.txt ./
COPY make-build.sh ./
RUN sed -i 's/\r$//' make-build.sh && bash make-build.sh
RUN rm -rf cpp web *.sh cmake* CMake*

RUN mkdir -p /data
WORKDIR /data
RUN /usr/local/rocketcms/rocketcms --genconf | jq ".fs.cms.root = \"/usr/local/rocketcms\" | .fs.site.root = \"/data\"" > rocketcms.conf.json

VOLUME /data
EXPOSE 23307
CMD ["/usr/local/rocketcms/rocketcms", "-c", "/data/rocketcms.conf.json"]
