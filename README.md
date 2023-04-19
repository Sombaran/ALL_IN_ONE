# Design_patterns


singleton_design.cpp --> Uses C++14
factory_design.cpp   --> Uses C++11


# docker CLI steps to pull, create, build, tag, push an image to dokcer hub 

--> pulling using simple pull command
    docker pull [OPTIONS] NAME[:TAG|@DIGEST]
    docker pull alpine:edge

--> create a Dockerfile and add contents into it

--> building the Dockerfile
    docker build [OPTIONS] PATH | URL | -
    docker build -t fedora:231702D .

--> login using access toker
    docker login -u <your_username>
    docker login -u usomgupta

--> tag an image before pushing
    docker tag [REPOSITORY] [TAG] given during docker build [USERNAME]/[SOME-NAME]:[VERSION]
    docker tag fedora:231702C usomgupta/fedora-github-design-pattern:v1

--> pushing an image
    docker push [USERNAME]/[SOME-NAME]:[VERSION]
    docker push usomgupta/fedora-github-design-pattern:v1