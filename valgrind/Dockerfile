FROM debian

RUN apt update && apt upgrade -y
RUN apt install -y \
	valgrind \
	clang \
	make \
	kcachegrind \
	graphviz

RUN apt install -y x11-apps