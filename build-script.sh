#!/bin/bash


# the next example of handling flags and arguments from command line has been brought from here:
# https://pretzelhands.com/posts/command-line-flags 


HELP='Usage: ./build-script.h [OPTIONS]\n\n'
HELP+='A script for building image from dockerfiles\n\n'
HELP+='Options:\n\n'
HELP+='    -n, --name    Image name'
HELP+='    -t, --tag     In case of production, tag the commit the image is built on'
HELP+='\n\n'

# Default values of arguments
IMAGE_NAME=''
TAG=''

# Loop through arguments and process them
for arg in "$@"
do
    case $arg in
        -n|--name)
        IMAGE_NAME="$2"
        shift # Remove argument name from processing
        shift # Remove argument value from processing
        ;;
        -t|--tag)
        TAG="$2"
        shift # Remove argument name from processing
        shift # Remove argument value from processing
        ;;
    esac
done

if [ "$IMAGE_NAME" = '' ]; then 
    echo "Warning - No image name was given"
    echo "# image name: Will be randomized"
else; then
    echo "# image name: $IMAGE_NAME"
fi

if [ ! "$TAG" = '' ]; then 
    echo "# tag: $TAG"
fi

echo "Do you want to continue? y/n"
read continue_script

if [ "$continue_script" = "n" ]; then 
    exit 1
fi

echo $(git rev-parse HEAD) >> sourceHash.txt
docker build --tag $IMAGE_NAME --file dockerfiles/build-dlep-enviroment-dockerfile .

git tag -a $TAG $(git rev-parse HEAD)
git push origin $TAG

