FROM debian:latest
MAINTAINER Oleg Morozenkov

RUN apt-get update && apt-get install -y g++ cmake make binutils libpoco-dev npm wget bash jq
RUN ln -s /usr/bin/nodejs /usr/bin/node
RUN npm install -g gulp

WORKDIR /usr/local/rocketcms

COPY install-poco.sh make-build.sh ./
RUN find -type f -name "*.sh" -print0 | xargs -0 sed -i 's/\r$//' # Removes Windows line endings
RUN bash install-poco.sh
COPY web/package.json web/package.json
RUN cd web && npm install

COPY cpp cpp
COPY web web
COPY CMakeLists.txt ./
RUN cd web && bash download-libs.sh
RUN bash make-build.sh
RUN rm -rf cpp web *.sh cmake* CMake*

RUN mkdir -p /data
WORKDIR /data
RUN /usr/local/rocketcms/rocketcms --genconf > rocketcms.conf.json
RUN jq ".fs.cms.root = \"/usr/local/rocketcms\"" rocketcms.conf.json > rocketcms.conf.json
RUN jq ".fs.site.root = \"/data\"" rocketcms.conf.json > rocketcms.conf.json

VOLUME /data
EXPOSE 23307

CMD ["/usr/local/rocketcms/rocketcms", "-c", "rocketcms.conf.json"]
