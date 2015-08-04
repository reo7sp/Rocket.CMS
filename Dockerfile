FROM debian:latest
MAINTAINER Oleg Morozenkov

RUN apt-get update && apt-get install -y g++ cmake make binutils npm wget sed tar bash jq
RUN ln -s /usr/bin/nodejs /usr/bin/node
RUN npm install -g gulp

WORKDIR /tmp/rocketcms
COPY install-poco.sh ./
RUN sed -i 's/\r$//' install-poco.sh && bash install-poco.sh
COPY web/package.json web/
RUN cd web && npm install
COPY cpp cpp
COPY web web
COPY CMakeLists.txt ./
COPY make-build.sh ./
RUN sed -i 's/\r$//' make-build.sh && bash make-build.sh
RUN mv rocketcms /usr/local/bin/
RUN mv *rcms.* /usr/local/lib/
RUN mkdir -p /usr/local/share/rocketcms
RUN mv webgui /usr/local/share/rocketcms/
RUN mv plugins /usr/local/share/rocketcms/
RUN mv translations /usr/local/share/rocketcms/
RUN rm -rf /tmp/rocketcms

RUN mkdir -p /data
WORKDIR /data
RUN /usr/local/bin/rocketcms --genconf | jq ".fs.cms.root = \"/usr/local/share/rocketcms\" | .fs.site.root = \"/data\"" > rocketcms.conf.json

VOLUME /data
EXPOSE 23307
CMD ["/usr/local/bin/rocketcms", "-c", "/data/rocketcms.conf.json"]
