FROM ubuntu

COPY setup.sh /
RUN apt update
RUN bash setup.sh
COPY * srcs
CMD bash
