FROM ghcr.io/doganulus/devel:latest

# Add a container user (good practice)
RUN groupadd reelay -g 1000 \
    && useradd -ms /bin/bash reelay -g 1000 -u 1000 \
    && printf "reelay:reelay" | chpasswd \
    && printf "reelay ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

USER reelay

ENV PATH "/home/reelay/.local/bin:${PATH}"

RUN python -m pip install black gcovr
